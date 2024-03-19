#include "Return.h"
#include "ReturnItem.h"
#include "Order.h"
#include <iostream>
#include "sqlite\sqlite3.h"

using namespace std;

Return::Return(int returnId, int customerId, const vector<int> &itemsReturned, const string &returnReason,
               const string &returnDate, int orderId)
    : ReturnId(returnId), CustomerId(customerId), ItemsReturned(itemsReturned), ReturnReason(returnReason),
      ReturnDate(returnDate), OrderId(orderId) {}

void Return::ProcessReturn(int returnId)
{
    cout << "Processing return with Id: " << ReturnId << "\n";
    RefundCustomer(returnId);

    UpdateInventory(returnId);
}

// Function to refund customer for a return
void Return::RefundCustomer(int returnId)
{
    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return;
    }

    // Retrieve product price for the return
    double productPrice = 0.0;
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "SELECT Price FROM Products WHERE ProductId = (SELECT ProductId FROM Returns WHERE ReturnId = ?)", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare select statement\n";
        sqlite3_close(db);
        return;
    }

    // Bind parameter
    sqlite3_bind_int(statement, 1, returnId);

    // Execute statement
    if (sqlite3_step(statement) == SQLITE_ROW)
    {
        productPrice = sqlite3_column_double(statement, 0);
    }
    else
    {
        cerr << "Product price not found for the return\n";
        sqlite3_finalize(statement);
        sqlite3_close(db);
        return;
    }

    // Finalize statement
    sqlite3_finalize(statement);

    // Display refund amount
    cout << "Refund amount for return with Id " << returnId << ": $" << productPrice << "\n";

    // Close the database
    sqlite3_close(db);
}

// Function to update inventory after return
void Return::UpdateInventory(int returnId)
{
    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return;
    }

    // Prepare SQL statement for selecting return details
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "SELECT ProductId FROM Returns WHERE ReturnId = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare select statement\n";
        sqlite3_close(db);
        return;
    }

    // Bind parameter
    sqlite3_bind_int(statement, 1, returnId);

    // Execute statement
    if (sqlite3_step(statement) != SQLITE_ROW)
    {
        cerr << "Return not found\n";
        sqlite3_finalize(statement);
        sqlite3_close(db);
        return;
    }

    // Retrieve productId
    int productId = sqlite3_column_int(statement, 0);

    // Finalize statement
    sqlite3_finalize(statement);

    // Prepare SQL statement for updating inventory
    if (sqlite3_prepare_v2(db, "UPDATE Inventories SET ProductAvailability = ProductAvailability + 1 WHERE ProductId = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare update statement\n";
        sqlite3_close(db);
        return;
    }

    // Bind parameter
    sqlite3_bind_int(statement, 1, productId);

    // Execute statement
    if (sqlite3_step(statement) != SQLITE_DONE)
    {
        cerr << "Failed to update inventory\n";
        sqlite3_finalize(statement);
        sqlite3_close(db);
        return;
    }

    cout << "Inventory updated successfully for return with Id: " << returnId << "\n";

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);
}

#include "Return.h"
#include "sqlite3.h"
#include <iostream>

using namespace std;

void Return::CreateReturn(int customerId, const vector<int> &itemsReturned, const string &returnReason, const string &returnDate, int orderId)
{
    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return;
    }

    // Begin transaction
    if (sqlite3_exec(db, "BEGIN TRANSACTION", nullptr, nullptr, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to begin transaction\n";
        sqlite3_close(db);
        return;
    }

    // Prepare SQL statement for inserting return
    sqlite3_stmt *returnStatement;
    if (sqlite3_prepare_v2(db, "INSERT INTO Returns (CustomerId, OrderId, ReturnDate, ReturnReason) VALUES (?, ?, ?, ?)", -1, &returnStatement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare return statement\n";
        sqlite3_close(db);
        return;
    }

    // Bind parameters for return insertion
    sqlite3_bind_int(returnStatement, 1, customerId);
    sqlite3_bind_int(returnStatement, 2, orderId);
    sqlite3_bind_text(returnStatement, 3, returnDate.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(returnStatement, 4, returnReason.c_str(), -1, SQLITE_STATIC);

    // Execute return insertion statement
    if (sqlite3_step(returnStatement) != SQLITE_DONE)
    {
        cerr << "Failed to execute return statement\n";
        sqlite3_finalize(returnStatement);
        sqlite3_close(db);
        return;
    }

    // Get the ReturnId of the newly inserted return
    int returnId = sqlite3_last_insert_rowid(db);

    // Finalize return statement
    sqlite3_finalize(returnStatement);

    // Prepare SQL statement for inserting return items
    sqlite3_stmt *returnItemStatement;
    if (sqlite3_prepare_v2(db, "INSERT INTO ReturnItems (ReturnId, ProductId) VALUES (?, ?)", -1, &returnItemStatement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare return item statement\n";
        sqlite3_close(db);
        return;
    }

    // Insert each returned item into the ReturnItems table
    for (int productId : itemsReturned)
    {
        // Bind parameters for return item insertion
        sqlite3_bind_int(returnItemStatement, 1, returnId);
        sqlite3_bind_int(returnItemStatement, 2, productId);

        // Execute return item insertion statement
        if (sqlite3_step(returnItemStatement) != SQLITE_DONE)
        {
            cerr << "Failed to execute return item statement\n";
            sqlite3_finalize(returnItemStatement);
            sqlite3_close(db);
            return;
        }

        // Reset the statement for the next iteration
        sqlite3_reset(returnItemStatement);
    }

    // Finalize return item statement
    sqlite3_finalize(returnItemStatement);

    // Commit transaction
    if (sqlite3_exec(db, "COMMIT", nullptr, nullptr, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to commit transaction\n";
        sqlite3_close(db);
        return;
    }

    // Close database
    sqlite3_close(db);

    cout << "Return created successfully with id: " << returnId << "\n";
}
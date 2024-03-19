#include "Inventory.h"
#include "Product.h"
#include <iostream>
#include "sqlite\sqlite3.h"

using namespace std;

Inventory::Inventory(int productId, int productAvailability, int reorderThreshold)
    : ProductId(productId), ProductAvailability(productAvailability), ReorderThreshold(reorderThreshold) {}

void Inventory::AddItemToInventory(int productId, int quantity)
{
    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return;
    }

    // Prepare SQL statement for updating inventory
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "UPDATE Inventories SET ProductAvailability = ProductAvailability + ? WHERE ProductId = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare update statement\n";
        sqlite3_close(db);
        return;
    }

    // Bind parameters
    sqlite3_bind_int(statement, 1, quantity);
    sqlite3_bind_int(statement, 2, productId);

    // Execute statement
    if (sqlite3_step(statement) != SQLITE_DONE)
    {
        cerr << "Failed to execute update statement\n";
        sqlite3_finalize(statement);
        sqlite3_close(db);
        return;
    }

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    cout << "Item added to inventory successfully\n";
}

void Inventory::RemoveItemFromInventory(int productId, int quantity)
{
    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return;
    }

    // Prepare SQL statement for updating inventory
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "UPDATE Inventories SET ProductAvailability = ProductAvailability - ? WHERE ProductId = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare update statement\n";
        sqlite3_close(db);
        return;
    }

    // Bind parameters
    sqlite3_bind_int(statement, 1, quantity);
    sqlite3_bind_int(statement, 2, productId);

    // Execute statement
    if (sqlite3_step(statement) != SQLITE_DONE)
    {
        cerr << "Failed to execute update statement\n";
        sqlite3_finalize(statement);
        sqlite3_close(db);
        return;
    }

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    cout << "Item removed from inventory successfully\n";
}

void Inventory::UpdateStockLevels(int productId, int newAvailability, int newReorderThreshold)
{
    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return;
    }

    // Prepare SQL statement for updating inventory
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "UPDATE Inventories SET ProductAvailability = ?, ReorderThreshold = ? WHERE ProductId = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare update statement\n";
        sqlite3_close(db);
        return;
    }

    // Bind parameters
    sqlite3_bind_int(statement, 1, newAvailability);
    sqlite3_bind_int(statement, 2, newReorderThreshold);
    sqlite3_bind_int(statement, 3, productId);

    // Execute statement
    if (sqlite3_step(statement) != SQLITE_DONE)
    {
        cerr << "Failed to execute update statement\n";
        sqlite3_finalize(statement);
        sqlite3_close(db);
        return;
    }

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    cout << "Stock levels updated successfully\n";
}

void Inventory::UpdateInventory(int productId, int difference)
{
    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return;
    }

    // Prepare SQL statement for updating inventory
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "SELECT ProductAvailability FROM Inventories WHERE ProductId = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare select statement\n";
        sqlite3_close(db);
        return;
    }

    // Bind parameter
    sqlite3_bind_int(statement, 1, productId);

    // Execute statement
    if (sqlite3_step(statement) == SQLITE_ROW)
    {
        int currentAvailability = sqlite3_column_int(statement, 0);

        // Finalize select statement
        sqlite3_finalize(statement);

        // Prepare SQL statement for updating inventory
        if (sqlite3_prepare_v2(db, "UPDATE Inventories SET ProductAvailability = ? WHERE ProductId = ?", -1, &statement, nullptr) != SQLITE_OK)
        {
            cerr << "Failed to prepare update statement\n";
            sqlite3_close(db);
            return;
        }

        // Bind parameters
        sqlite3_bind_int(statement, 1, currentAvailability + difference);
        sqlite3_bind_int(statement, 2, productId);

        // Execute statement
        if (sqlite3_step(statement) != SQLITE_DONE)
        {
            cerr << "Failed to execute update statement\n";
            sqlite3_finalize(statement);
            sqlite3_close(db);
            return;
        }

        // Finalize statement
        sqlite3_finalize(statement);

        cout << "Inventory updated successfully\n";
    }
    else
    {
        cerr << "Product not found\n";
    }

    // Close database
    sqlite3_close(db);
}

void Inventory::ViewInventory()
{
    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return;
    }

    // Prepare SQL statement for selecting all inventory items
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "SELECT * FROM Inventories", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare select statement\n";
        sqlite3_close(db);
        return;
    }

    // Execute statement and fetch rows
    cout << "Inventory:\n";
    while (sqlite3_step(statement) == SQLITE_ROW)
    {
        int productId = sqlite3_column_int(statement, 0);
        int productAvailability = sqlite3_column_int(statement, 1);
        int reorderThreshold = sqlite3_column_int(statement, 2);

        cout << "Product Id: " << productId << endl;
        cout << "Product Availability: " << productAvailability << endl;
        cout << "Reorder Quantity: " << reorderThreshold << endl;
        cout << endl;
    }

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);
}

int Inventory::GetProductAvailability(int productId)
{
    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return -1; // Return -1 to indicate error
    }

    // Prepare SQL statement for selecting product availability
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "SELECT ProductAvailability FROM Inventories WHERE ProductId = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare select statement\n";
        sqlite3_close(db);
        return -1; // Return -1 to indicate error
    }

    // Bind parameter
    sqlite3_bind_int(statement, 1, productId);

    // Execute statement
    if (sqlite3_step(statement) == SQLITE_ROW)
    {
        int productAvailability = sqlite3_column_int(statement, 0);
        sqlite3_finalize(statement);
        sqlite3_close(db);
        return productAvailability;
    }
    else
    {
        cerr << "Product not found\n";
        sqlite3_finalize(statement);
        sqlite3_close(db);
        return -1; // Return -1 to indicate error
    }
}

int Inventory::GetReorderThreshold(int productId)
{
    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return -1; // Return -1 to indicate error
    }

    // Prepare SQL statement for selecting reorder threshold
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "SELECT ReorderThreshold FROM Inventories WHERE ProductId = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare select statement\n";
        sqlite3_close(db);
        return -1; // Return -1 to indicate error
    }

    // Bind parameter
    sqlite3_bind_int(statement, 1, productId);

    // Execute statement
    if (sqlite3_step(statement) == SQLITE_ROW)
    {
        int reorderThreshold = sqlite3_column_int(statement, 0);
        sqlite3_finalize(statement);
        sqlite3_close(db);
        return reorderThreshold;
    }
    else
    {
        cerr << "Product not found\n";
        sqlite3_finalize(statement);
        sqlite3_close(db);
        return -1; // Return -1 to indicate error
    }
}

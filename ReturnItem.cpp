#include "ReturnItem.h"
#include <iostream>

using namespace std;

ReturnItem::ReturnItem(int returnItemId, int returnId, int productId, int quantity)
    : ReturnItemId(returnItemId), ReturnId(returnId), ProductId(productId), Quantity(quantity) {}

vector<ReturnItem> ReturnItem::GetReturnItemsByReturnId(int returnId)
{
    vector<ReturnItem> returnItems;

    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return returnItems;
    }

    // Prepare SQL statement for selecting returned items by return id
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "SELECT * FROM ReturnItems WHERE ReturnId = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare select statement\n";
        sqlite3_close(db);
        return returnItems;
    }

    // Bind parameters
    sqlite3_bind_int(statement, 1, returnId);

    // Execute statement
    while (sqlite3_step(statement) == SQLITE_ROW)
    {
        int returnItemId = sqlite3_column_int(statement, 0);
        int productId = sqlite3_column_int(statement, 2);
        int quantity = sqlite3_column_int(statement, 3);

        returnItems.emplace_back(returnItemId, returnId, productId, quantity);
    }

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    return returnItems;
}

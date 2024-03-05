#include "OrderItem.h"
#include <iostream>
#include <vector>

using namespace std;

OrderItem::OrderItem(int orderItemId, int orderId, int productId, int quantity)
    : OrderItemId(orderItemId), OrderId(orderId), ProductId(productId), Quantity(quantity) {}

vector<OrderItem> OrderItem::GetOrderItemsByOrderId(int orderId)
{
    vector<OrderItem> orderItems;

    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return orderItems;
    }

    // Prepare SQL statement for selecting order items by order id
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "SELECT * FROM OrderItems WHERE OrderId = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare select statement\n";
        sqlite3_close(db);
        return orderItems;
    }

    // Bind parameters
    sqlite3_bind_int(statement, 1, orderId);

    // Execute statement
    while (sqlite3_step(statement) == SQLITE_ROW)
    {
        int orderItemId = sqlite3_column_int(statement, 0);
        int productId = sqlite3_column_int(statement, 2);
        int quantity = sqlite3_column_int(statement, 3);

        orderItems.emplace_back(orderItemId, orderId, productId, quantity);
    }

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    return orderItems;
}

string OrderItem::GetProductName() const
{
    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return ""; // Return empty string indicating failure
    }

    // Prepare SQL statement for selecting product name by ProductId
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "SELECT Name FROM Products WHERE ProductId = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare select statement\n";
        sqlite3_close(db);
        return ""; // Return empty string indicating failure
    }

    // Bind parameter
    sqlite3_bind_int(statement, 1, ProductId);

    // Execute statement
    if (sqlite3_step(statement) == SQLITE_ROW)
    {
        const unsigned char *productName = sqlite3_column_text(statement, 0);

        // Finalize statement and close database
        sqlite3_finalize(statement);
        sqlite3_close(db);

        return reinterpret_cast<const char *>(productName);
    }

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    return ""; // Return empty string indicating failure
}

int OrderItem::GetProductId() const
{
    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return -1; // Return empty string indicating failure
    }

    // Prepare SQL statement for selecting product name by ProductId
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "SELECT Id FROM Products WHERE ProductId = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare select statement\n";
        sqlite3_close(db);
        return -1; // Return empty string indicating failure
    }

    // Bind parameter
    sqlite3_bind_int(statement, 1, ProductId);

    // Execute statement
    if (sqlite3_step(statement) == SQLITE_ROW)
    {
        const int productId = sqlite3_column_int(statement, 0);

        // Finalize statement and close database
        sqlite3_finalize(statement);
        sqlite3_close(db);

        return productId;
    }

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    return -1; // Return empty string indicating failure
}

#include "Order.h"
#include "OrderItem.h"
#include <iostream>
#include <sstream>
#include "sqlite\sqlite3.h"

using namespace std;

Order::Order(int orderId, int customerId, const vector<int> &itemsOrdered, const string &orderDate, const OrderStatus &status)
    : OrderId(orderId), CustomerId(customerId), ItemsOrdered(itemsOrdered), OrderDate(orderDate), Status(status) {}

void Order::CreateOrder(int customerId, const string &orderDate, const OrderStatus &status, const vector<int> itemsOrdered)
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

    // Prepare SQL statement for inserting order
    sqlite3_stmt *orderStatement;
    if (sqlite3_prepare_v2(db, "INSERT INTO Orders (CustomerId, OrderDate, Status) VALUES (?, ?, ?)", -1, &orderStatement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare order statement\n";
        sqlite3_close(db);
        return;
    }

    // Bind parameters for order insertion
    sqlite3_bind_int(orderStatement, 1, customerId);
    sqlite3_bind_text(orderStatement, 2, orderDate.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(orderStatement, 3, to_string(static_cast<int>(status)).c_str(), -1, SQLITE_STATIC);

    // Execute order insertion statement
    if (sqlite3_step(orderStatement) != SQLITE_DONE)
    {
        cerr << "Failed to execute order statement\n";
        sqlite3_finalize(orderStatement);
        sqlite3_close(db);
        return;
    }

    // Get the OrderId of the newly inserted order
    int orderId = sqlite3_last_insert_rowid(db);

    // Finalize order statement
    sqlite3_finalize(orderStatement);

    // Prepare SQL statement for inserting order items
    sqlite3_stmt *orderItemStatement;
    if (sqlite3_prepare_v2(db, "INSERT INTO OrderItems (OrderId, ProductId, Quantity) VALUES (?, ?, ?)", -1, &orderItemStatement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare order item statement\n";
        sqlite3_close(db);
        return;
    }

    // Bind parameters for order item insertion
    for (int productId : itemsOrdered)
    {
        sqlite3_bind_int(orderItemStatement, 1, orderId);
        sqlite3_bind_int(orderItemStatement, 2, productId);
        sqlite3_bind_int(orderItemStatement, 3, 1);

        // Execute order item insertion statement
        if (sqlite3_step(orderItemStatement) != SQLITE_DONE)
        {
            cerr << "Failed to execute order item statement\n";
            sqlite3_finalize(orderItemStatement);
            sqlite3_close(db);
            return;
        }

        // Reset statement for next iteration
        sqlite3_reset(orderItemStatement);
    }

    // Finalize order item statement
    sqlite3_finalize(orderItemStatement);

    // Commit transaction
    if (sqlite3_exec(db, "COMMIT", nullptr, nullptr, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to commit transaction\n";
        sqlite3_close(db);
        return;
    }

    // Close database
    sqlite3_close(db);

    cout << "Order created successfully\n";
}

Order Order::GetOrderById(int orderId)
{
    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return Order(-1, -1, {}, "", OrderStatus::Cancelled);
    }

    // Prepare SQL statement for selecting an order by Id
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "SELECT * FROM Orders WHERE OrderId = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare select statement\n";
        sqlite3_close(db);
        return Order(-1, -1, {}, "", OrderStatus::Cancelled);
    }

    // Bind parameter
    sqlite3_bind_int(statement, 1, orderId);

    // Execute statement
    if (sqlite3_step(statement) == SQLITE_ROW)
    {
        int customerId = sqlite3_column_int(statement, 1);
        const unsigned char *itemsOrderedStr = sqlite3_column_text(statement, 3);
        const unsigned char *orderDate = sqlite3_column_text(statement, 4);
        const unsigned char *statusStr = sqlite3_column_text(statement, 5);

        // Convert itemsOrdered from string to vector<int>
        vector<int> itemsOrdered;
        string itemsStr(reinterpret_cast<const char *>(itemsOrderedStr));
        size_t pos = 0;
        while ((pos = itemsStr.find(',')) != string::npos)
        {
            itemsOrdered.push_back(stoi(itemsStr.substr(0, pos)));
            itemsStr.erase(0, pos + 1);
        }
        itemsOrdered.push_back(stoi(itemsStr));

        // Convert status from string to OrderStatus enum
        OrderStatus status;
        string statusString(reinterpret_cast<const char *>(statusStr));
        if (statusString == "Placed")
            status = OrderStatus::Placed;
        else if (statusString == "Processing")
            status = OrderStatus::Processing;
        else if (statusString == "Shipped")
            status = OrderStatus::Shipped;
        else if (statusString == "Delivered")
            status = OrderStatus::Delivered;
        else if (statusString == "Cancelled")
            status = OrderStatus::Cancelled;
        else
        {
            cerr << "Unknown order status\n";
            sqlite3_finalize(statement);
            sqlite3_close(db);
            return Order(-1, -1, {}, "", OrderStatus::Cancelled);
        }

        // Finalize statement and close database
        sqlite3_finalize(statement);
        sqlite3_close(db);

        return Order(orderId, customerId, itemsOrdered, reinterpret_cast<const char *>(orderDate), status);
    }

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    return Order(-1, -1, {}, "", OrderStatus::Cancelled);
}

#include "Order.h"
#include "sqlite3.h"

void Order::CancelOrder(int orderId)
{
    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return;
    }

    // Prepare SQL statement for updating order status to Cancelled
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "UPDATE Orders SET Status = 'Cancelled' WHERE OrderId = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare update statement\n";
        sqlite3_close(db);
        return;
    }

    // Bind parameters
    sqlite3_bind_int(statement, 1, orderId);

    // Execute statement
    if (sqlite3_step(statement) != SQLITE_DONE)
    {
        cerr << "Failed to update order status\n";
    }

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);
}

void Order::TrackOrder(int orderId)
{
    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return;
    }

    // Prepare SQL statement for selecting order status
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "SELECT Status FROM Orders WHERE OrderId = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare select statement\n";
        sqlite3_close(db);
        return;
    }

    // Bind parameter
    sqlite3_bind_int(statement, 1, orderId);

    // Execute statement
    if (sqlite3_step(statement) == SQLITE_ROW)
    {
        const unsigned char *statusStr = sqlite3_column_text(statement, 0);
        cout << "Order Status: " << statusStr << endl;
    }
    else
    {
        cerr << "Order not found\n";
    }

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);
}

void Order::UpdateStatus(OrderStatus status)
{
    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return;
    }

    // Prepare SQL statement for updating order status
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "UPDATE Orders SET Status = ? WHERE OrderId = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare update statement\n";
        sqlite3_close(db);
        return;
    }

    // Bind parameters
    sqlite3_bind_text(statement, 1, to_string(static_cast<int>(status)).c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(statement, 2, OrderId);

    // Execute statement
    if (sqlite3_step(statement) != SQLITE_DONE)
    {
        cerr << "Failed to update order status\n";
    }

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    // Update the status in the object
    Status = status;
}

vector<Order> Order::GetAllOrders()
{
    vector<Order> orders;

    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return orders;
    }

    // Prepare SQL statement for selecting all orders
    sqlite3_stmt *statement;
    const char *sqlQuery = "SELECT * FROM Orders";
    if (sqlite3_prepare_v2(db, sqlQuery, -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare select statement\n";
        sqlite3_close(db);
        return orders;
    }

    // Execute statement and fetch rows
    while (sqlite3_step(statement) == SQLITE_ROW)
    {
        int orderId = sqlite3_column_int(statement, 0);
        int customerId = sqlite3_column_int(statement, 1);
        const unsigned char *itemsOrderedStr = sqlite3_column_text(statement, 2);
        string orderDate = reinterpret_cast<const char *>(sqlite3_column_text(statement, 3));
        const unsigned char *statusStr = sqlite3_column_text(statement, 4);

        // Convert comma-separated items ordered string to vector<int>
        vector<int> itemsOrdered;
        string itemsStr(reinterpret_cast<const char *>(itemsOrderedStr));
        istringstream iss(itemsStr);
        string item;
        while (getline(iss, item, ','))
        {
            itemsOrdered.push_back(stoi(item));
        }

        // Convert status string to OrderStatus enum
        OrderStatus status;
        string statusString(reinterpret_cast<const char *>(statusStr));
        if (statusString == "Placed")
        {
            status = OrderStatus::Placed;
        }
        else if (statusString == "Processing")
        {
            status = OrderStatus::Processing;
        }
        else if (statusString == "Shipped")
        {
            status = OrderStatus::Shipped;
        }
        else if (statusString == "Delivered")
        {
            status = OrderStatus::Delivered;
        }
        else if (statusString == "Cancelled")
        {
            status = OrderStatus::Cancelled;
        }
        else
        {
            // Handle unknown status string
            continue; // Skip this row
        }

        Order order(orderId, customerId, itemsOrdered, orderDate, status);
        orders.push_back(order);
    }

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    return orders;
}

int Order::GetOrderId() const
{
    return OrderId;
}

int Order::GetCustomerId() const
{
    return CustomerId;
}

string Order::GetStatus() const
{
    string statusString;
    switch (Status)
    {
    case OrderStatus::Placed:
        statusString = "Placed";
        break;
    case OrderStatus::Processing:
        statusString = "Processing";
        break;
    case OrderStatus::Shipped:
        statusString = "Shipped";
        break;
    case OrderStatus::Delivered:
        statusString = "Delivered";
        break;
    case OrderStatus::Cancelled:
        statusString = "Cancelled";
        break;
    default:
        statusString = "Unknown";
        break;
    }
    return statusString;
}

vector<OrderItem> Order::GetOrderItems() const
{
    vector<OrderItem> orderItems;

    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return orderItems;
    }

    // Prepare SQL statement for selecting order items by OrderId
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "SELECT * FROM OrderItems WHERE OrderId = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare select statement\n";
        sqlite3_close(db);
        return orderItems;
    }

    // Bind parameter
    sqlite3_bind_int(statement, 1, OrderId);

    // Execute statement
    while (sqlite3_step(statement) == SQLITE_ROW)
    {
        int productId = sqlite3_column_int(statement, 1);
        double price = sqlite3_column_double(statement, 2);
        int quantity = sqlite3_column_int(statement, 3);
        int orderId = sqlite3_column_int(statement, 4);

        // Create OrderItem object and add it to the vector
        orderItems.emplace_back(productId, price, quantity, orderId);
    }

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    return orderItems;
}

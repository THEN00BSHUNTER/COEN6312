#include "Customer.h"
#include <iostream>
#include "sqlite\sqlite3.h"

using namespace std;

Customer::Customer(int id, const string &username, const string &password, const string &email,
                   const string &phone, const string &address, const string &role,
                   const string &shippingAddress, const string &paymentMethod)
    : User(id, username, password, email, phone, address, role), ShippingAddress(shippingAddress), PaymentMethod(paymentMethod) {}

Customer *Customer::RegisterCustomer(const string &username, const string &password, const string &email,
                                     const string &phone, const string &address,
                                     const string &shippingAddress, const string &paymentMethod)
{
    // Check if username already exists
    if (!User::IsUsernameUnique(username))
    {
        return nullptr;
    }

    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return nullptr;
    }

    // Begin transaction
    if (sqlite3_exec(db, "BEGIN TRANSACTION", nullptr, nullptr, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to begin transaction\n";
        sqlite3_close(db);
        return nullptr;
    }

    // Prepare SQL statement for inserting user
    sqlite3_stmt *userStatement;
    if (sqlite3_prepare_v2(db, "INSERT INTO Users (Username, Password, Email, Phone, Address, Role) VALUES (?, ?, ?, ?, ?, ?)", -1, &userStatement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare user statement\n";
        sqlite3_close(db);
        return nullptr;
    }

    // Bind parameters for user insertion
    sqlite3_bind_text(userStatement, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(userStatement, 2, password.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(userStatement, 3, email.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(userStatement, 4, phone.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(userStatement, 5, address.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(userStatement, 6, "customer", -1, SQLITE_STATIC);

    // Execute user insertion statement
    if (sqlite3_step(userStatement) != SQLITE_DONE)
    {
        cerr << "Failed to execute user statement\n";
        sqlite3_finalize(userStatement);
        sqlite3_close(db);
        return nullptr;
    }

    // Get the UserId of the newly inserted user
    int userId = sqlite3_last_insert_rowid(db);

    // Finalize user statement
    sqlite3_finalize(userStatement);

    // Prepare SQL statement for inserting customer
    sqlite3_stmt *customerStatement;
    if (sqlite3_prepare_v2(db, "INSERT INTO Customers (Id, ShippingAddress, PaymentMethod) VALUES (?, ?, ?)", -1, &customerStatement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare customer statement\n";
        sqlite3_close(db);
        return nullptr;
    }

    // Bind parameters for customer insertion
    sqlite3_bind_int(customerStatement, 1, userId);
    sqlite3_bind_text(customerStatement, 2, shippingAddress.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(customerStatement, 3, paymentMethod.c_str(), -1, SQLITE_STATIC);

    // Execute customer insertion statement
    if (sqlite3_step(customerStatement) != SQLITE_DONE)
    {
        cerr << "Failed to execute customer statement\n";
        sqlite3_finalize(customerStatement);
        sqlite3_close(db);
        return nullptr;
    }

    // Finalize customer statement
    sqlite3_finalize(customerStatement);

    // Commit transaction
    if (sqlite3_exec(db, "COMMIT", nullptr, nullptr, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to commit transaction\n";
        sqlite3_close(db);
        return nullptr;
    }

    // Close database
    sqlite3_close(db);

    cout << "Customer registered successfully with id: " << userId << "\n";
    return new Customer(userId, username, password, email, phone, address, "customer", shippingAddress, paymentMethod);
}

void Customer::PlaceOrder()
{
    // Check if any user is logged in
    if (!User::IsAnyUserLoggedIn())
    {
        cout << "Please log in to place an order.\n";
        return;
    }

    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return;
    }

    // Prepare SQL statement for inserting the order
    sqlite3_stmt *orderStatement;
    if (sqlite3_prepare_v2(db, "INSERT INTO Orders (UserId, OrderDate, Status) VALUES (?, CURRENT_TIMESTAMP, 'Pending')", -1, &orderStatement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare order statement\n";
        sqlite3_close(db);
        return;
    }

    // Bind parameters for order insertion
    sqlite3_bind_int(orderStatement, 1, Id);

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
    sqlite3_close(db);

    cout << "Order placed successfully with id: " << orderId << endl;
}

void Customer::ViewOrderHistory()
{
    // Check if any user is logged in
    if (!User::IsAnyUserLoggedIn())
    {
        cout << "Please log in to view your order history.\n";
        return;
    }

    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return;
    }

    // Prepare SQL statement for selecting order history
    sqlite3_stmt *historyStatement;
    if (sqlite3_prepare_v2(db, "SELECT OrderId, OrderDate, Status FROM Orders WHERE CustomerId = ?", -1, &historyStatement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare order history statement\n";
        sqlite3_close(db);
        return;
    }

    // Bind parameters for order history selection
    sqlite3_bind_int(historyStatement, 1, Id);

    // Execute order history selection statement
    cout << "Order history:\n";
    while (sqlite3_step(historyStatement) == SQLITE_ROW)
    {
        int orderId = sqlite3_column_int(historyStatement, 0);
        string orderDate = reinterpret_cast<const char *>(sqlite3_column_text(historyStatement, 1));
        string status = reinterpret_cast<const char *>(sqlite3_column_text(historyStatement, 2));

        cout << "OrderId: " << orderId << " | Date: " << orderDate << " | Status: " << status << endl;
    }

    // Finalize order history statement and close database
    sqlite3_finalize(historyStatement);
    sqlite3_close(db);
}

Customer Customer::GetCustomerById(int userId)
{
    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        // Return a default-initialized Customer object
        return Customer(-1, "", "", "", "", "", "", "", "");
    }

    // Prepare SQL statement for selecting customer by user Id
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "SELECT * FROM Customers WHERE Id = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare select statement\n";
        sqlite3_close(db);
        // Return a default-initialized Customer object
        return Customer(-1, "", "", "", "", "", "", "", "");
    }

    // Bind parameter (user Id)
    sqlite3_bind_int(statement, 1, userId);

    // Execute statement
    if (sqlite3_step(statement) == SQLITE_ROW)
    {
        // Retrieve customer details from the database
        int id = sqlite3_column_int(statement, 0);
        string shippingAddress = reinterpret_cast<const char *>(sqlite3_column_text(statement, 1));
        string paymentMethod = reinterpret_cast<const char *>(sqlite3_column_text(statement, 2));

        // Finalize statement and close database
        sqlite3_finalize(statement);
        sqlite3_close(db);

        // Return the Customer object
        return Customer(id, "", "", "", "", "", "", shippingAddress, paymentMethod);
    }

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    // Return a default-initialized Customer object if not found
    return Customer(-1, "", "", "", "", "", "", "", "");
}

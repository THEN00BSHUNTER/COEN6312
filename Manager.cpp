#include "Manager.h"
#include "Product.h"
#include "Transaction.h"
#include "Order.h"
#include <iostream>
#include "sqlite\sqlite3.h"

using namespace std;

Manager::Manager(int id, const string &username, const string &password, const string &email,
                 const string &phone, const string &address, const string &role, const string &department)
    : User(id, username, password, email, phone, address, role), Department(department) {}

Manager *Manager::RegisterManager(const string &username, const string &password, const string &email,
                                  const string &phone, const string &address, const string &department)
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
    sqlite3_bind_text(userStatement, 6, "manager", -1, SQLITE_STATIC);

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

    // Prepare SQL statement for inserting manager
    sqlite3_stmt *managerStatement;
    if (sqlite3_prepare_v2(db, "INSERT INTO Managers (Id, Department) VALUES (?, ?)", -1, &managerStatement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare manager statement\n";
        sqlite3_close(db);
        return nullptr;
    }

    // Bind parameters for manager insertion
    sqlite3_bind_int(managerStatement, 1, userId);
    sqlite3_bind_text(managerStatement, 2, department.c_str(), -1, SQLITE_STATIC);

    // Execute manager insertion statement
    if (sqlite3_step(managerStatement) != SQLITE_DONE)
    {
        cerr << "Failed to execute manager statement\n";
        sqlite3_finalize(managerStatement);
        sqlite3_close(db);
        return nullptr;
    }

    // Finalize manager statement
    sqlite3_finalize(managerStatement);

    // Commit transaction
    if (sqlite3_exec(db, "COMMIT", nullptr, nullptr, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to commit transaction\n";
        sqlite3_close(db);
        return nullptr;
    }

    // Close database
    sqlite3_close(db);

    cout << "Manager registered successfully\n";
    // Return the newly created Manager object
    return new Manager(userId, username, password, email, phone, address, "manager", department);
}

void Manager::ApproveOrder()
{
    int orderId;
    OrderStatus newStatus;

    // Ask for OrderId
    cout << "Enter the OrderId: ";
    cin >> orderId;

    // Ask for new OrderStatus
    int statusChoice;
    cout << "Choose the new Order Status:\n";
    cout << "1. Processing\n";
    cout << "2. Shipped\n";
    cout << "3. Delivered\n";
    cout << "4. Cancelled\n";
    cout << "Enter your choice: ";
    cin >> statusChoice;

    // Convert statusChoice to OrderStatus enum
    switch (statusChoice)
    {
    case 1:
        newStatus = OrderStatus::Processing;
        break;
    case 2:
        newStatus = OrderStatus::Shipped;
        break;
    case 3:
        newStatus = OrderStatus::Delivered;
        break;
    case 4:
        newStatus = OrderStatus::Cancelled;
        break;
    default:
        cout << "Invalid choice. Order status remains unchanged.\n";
        return;
    }

    // Update the order object
    Order order = Order::GetOrderById(orderId);
    order.UpdateStatus(newStatus);
}

void Manager::GenerateReport()
{
    // Read and print orders
    cout << "Orders:\n";
    vector<Order> orders = Order::GetAllOrders();
    for (const auto &order : orders)
    {
        cout << "Order Id: " << order.GetOrderId() << endl;
        cout << "Customer Id: " << order.GetCustomerId() << endl;
        cout << "Status: " << order.GetStatus() << endl;
        cout << endl;
    }

    // Read and print products
    cout << "Products:\n";
    vector<Product> products = Product::GetAllProducts();
    Product::DisplayAllProducts(products);
}

void Manager::MonitorSales()
{
    // Read and print transactions
    cout << "Transactions:\n";
    vector<Transaction> transactions = Transaction::GetAllTransactions();
    for (const auto &transaction : transactions)
    {
        transaction.RetrieveTransactionDetails();
        cout << endl;
    }

    // Read and print orders
    cout << "Orders:\n";
    vector<Order> orders = Order::GetAllOrders();
    for (const auto &order : orders)
    {
        cout << "Order Id: " << order.GetOrderId() << endl;
        cout << "Customer Id: " << order.GetCustomerId() << endl;
        cout << "Status: " << order.GetStatus() << endl;
        cout << endl;
    }
}

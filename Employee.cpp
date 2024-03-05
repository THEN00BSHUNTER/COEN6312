#include "Database.h"
#include "Order.h"
#include "Inventory.h"
#include "Employee.h"
#include <iostream>
#include "sqlite\sqlite3.h"

using namespace std;

Employee::Employee(int id, const string &username, const string &password, const string &email,
                   const string &phone, const string &address, const string &role, int managerId, int branchId, const string &position, const string &tasks)
    : User(id, username, password, email, phone, address, role), ManagerId(managerId), BranchId(branchId), Position(position), Tasks(tasks) {}

Employee *Employee::RegisterEmployee(const string &username, const string &password, const string &email,
                                     const string &phone, const string &address,
                                     int managerId, int branchId, const string &position, const string &tasks)
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
    sqlite3_bind_text(userStatement, 6, "employee", -1, SQLITE_STATIC);

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

    // Prepare SQL statement for inserting employee
    sqlite3_stmt *employeeStatement;
    if (sqlite3_prepare_v2(db, "INSERT INTO Employees (Id, ManagerId, BranchId, Position, Tasks) VALUES (?, ?, ?, ?, ?)", -1, &employeeStatement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare employee statement\n";
        sqlite3_close(db);
        return nullptr;
    }

    // Bind parameters for employee insertion
    sqlite3_bind_int(employeeStatement, 1, userId);
    sqlite3_bind_int(employeeStatement, 2, managerId);
    sqlite3_bind_int(employeeStatement, 3, branchId);
    sqlite3_bind_text(employeeStatement, 4, position.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(employeeStatement, 5, tasks.c_str(), -1, SQLITE_STATIC);

    // Execute employee insertion statement
    if (sqlite3_step(employeeStatement) != SQLITE_DONE)
    {
        cerr << "Failed to execute employee statement\n";
        sqlite3_finalize(employeeStatement);
        sqlite3_close(db);
        return nullptr;
    }

    // Get the last inserted row id (EmployeeId)
    int newEmployeeId = sqlite3_last_insert_rowid(db);

    // Finalize employee statement
    sqlite3_finalize(employeeStatement);

    // Commit transaction
    if (sqlite3_exec(db, "COMMIT", nullptr, nullptr, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to commit transaction\n";
        sqlite3_close(db);
        return nullptr;
    }

    // Close database
    sqlite3_close(db);

    cout << "Employee registered successfully\n";
    // Return the newly created Employee object
    return new Employee(userId, username, password, email, phone, address, "employee", managerId, branchId, position, tasks);
}

bool Employee::UpdateEmployeeData(Employee employee)
{
    return false;
}

void Employee::ProcessOrder()
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

void Employee::UpdateInventory(int productId, int difference)
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

string Employee::GetUsername() const
{
    return Username;
}

#include "Branch.h"
#include "Employee.h"
#include <iostream>
#include <vector>

Branch::Branch(int branchId, const string &location, int managerId)
    : BranchId(branchId), Location(location), ManagerId(managerId) {}

int Branch::GetBranchId() const
{
    return BranchId;
}

void Branch::ManageEmployees()
{
    // List the branches
    cout << "Branches:\n";
    vector<Branch> branches = GetAllBranches();
    for (const auto &branch : branches)
    {
        cout << "Branch: " << branch.BranchId << ", Location: " << branch.Location << endl;
    }

    // Take input for the branchId
    int branchId;
    cout << "Enter Branch Id: ";
    cin >> branchId;

    // Fetch employees based on the branchId
    vector<Employee> employees = GetAllEmployeesByBranchId(branchId);

    // Display employees
    cout << "Employees in Branch " << branchId << ":\n";
    for (const auto &employee : employees)
    {
        cout << "Employee Id: " << employee.GetId() << ", Username: " << employee.GetUsername() << endl;
    }

    // Take input for the employeeId
    int employeeId;
    cout << "Enter Employee Id to update: ";
    cin >> employeeId;

    // Find the employee in the list
    Employee *employeeToUpdate = nullptr;
    for (auto &employee : employees)
    {
        if (employee.GetId() == employeeId)
        {
            employeeToUpdate = &employee;
            break;
        }
    }

    // If employee found, update data fields
    if (employeeToUpdate)
    {
        string username, password, email, phone, address;
        cout << "Enter new data for the employee (press Enter to keep current values):\n";
        cout << "Username: ";
        getline(cin, username); // getline to handle spaces in input
        if (!username.empty())
            employeeToUpdate->SetUsername(username);

        cout << "Password: ";
        getline(cin, password);
        if (!password.empty())
            employeeToUpdate->SetPassword(password);

        cout << "Email: ";
        getline(cin, email);
        if (!email.empty())
            employeeToUpdate->SetEmail(email);

        cout << "Phone: ";
        getline(cin, phone);
        if (!phone.empty())
            employeeToUpdate->SetPhone(phone);

        cout << "Address: ";
        getline(cin, address);
        if (!address.empty())
            employeeToUpdate->SetAddress(address);

        // Update the employee in the database
        if (Employee::UpdateEmployeeData(*employeeToUpdate))
        {
            cout << "Employee data updated successfully.\n";
        }
        else
        {
            cout << "Failed to update employee data.\n";
        }
    }
    else
    {
        cout << "Employee not found in this branch.\n";
    }
}

void Branch::HandleCustomerInquiries()
{
    // It's the Return class functions
}

vector<Branch> Branch::GetAllBranches()
{
    vector<Branch> branches;

    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return branches;
    }

    // Prepare SQL statement for selecting all branches
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "SELECT * FROM Branches", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare select statement\n";
        sqlite3_close(db);
        return branches;
    }

    // Execute statement
    while (sqlite3_step(statement) == SQLITE_ROW)
    {
        int branchId = sqlite3_column_int(statement, 0);
        const unsigned char *location = sqlite3_column_text(statement, 1);
        int managerId = sqlite3_column_int(statement, 2);

        branches.emplace_back(branchId, reinterpret_cast<const char *>(location), managerId);
    }

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    return branches;
}

void Branch::DisplayAllBranches(vector<Branch> branches)
{
    for (const Branch &branch : branches)
    {
        cout << "Branch Id: " << branch.BranchId << ", Location: " << branch.Location << ", Manager Id: " << branch.ManagerId << endl;
    }
}

vector<Employee> Branch::GetAllEmployeesByBranchId(int branchId)
{
    vector<Employee> employees;

    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return employees;
    }

    // Prepare SQL statement for selecting employees by branchId
    sqlite3_stmt *statement;
    string query = "SELECT * FROM Employees WHERE BranchId = ?";
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare statement\n";
        sqlite3_close(db);
        return employees;
    }

    // Bind branchId parameter
    if (sqlite3_bind_int(statement, 1, branchId) != SQLITE_OK)
    {
        cerr << "Failed to bind branchId\n";
        sqlite3_finalize(statement);
        sqlite3_close(db);
        return employees;
    }

    // Execute the statement
    while (sqlite3_step(statement) == SQLITE_ROW)
    {
        int userId = sqlite3_column_int(statement, 1);
        int managerId = sqlite3_column_int(statement, 2);
        string position = reinterpret_cast<const char *>(sqlite3_column_text(statement, 3));
        string tasks = reinterpret_cast<const char *>(sqlite3_column_text(statement, 4));

        User user = User::GetUserById(userId);
        employees.push_back(Employee(user.GetId(), user.GetUsername(), user.GetPassword(), user.GetEmail(), user.GetPhone(), user.GetAddress(), user.GetRole(), managerId, branchId, position, tasks));
    }

    // Finalize the statement
    sqlite3_finalize(statement);

    // Close the database
    sqlite3_close(db);

    return employees;
}
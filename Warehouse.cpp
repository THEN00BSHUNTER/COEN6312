#include "Branch.h"
#include "Warehouse.h"
#include <iostream>
#include "sqlite\sqlite3.h"

Warehouse::Warehouse(int warehouseId, const string &location, int inventory, int manager)
    : WarehouseId(warehouseId), Location(location), Inventory(inventory), Manager(manager) {}

bool Warehouse::CanManageWarehouse(int managerId)
{
    // Check if the manager is already assigned as a manager to any branch
    for (const auto &branch : Branch::GetAllBranches())
    {
        if (branch.GetManagerId() == managerId)
        {
            cerr << "Manager is already assigned as a manager to a branch\n";
            return false;
        }
    }

    // count the number of warehouses managed by the manager
    int warehouseCount = 0;
    for (const auto &warehouse : GetAllWarehouses())
    {
        if (warehouse.Manager == managerId)
        {
            warehouseCount++;
        }
    }

    // Check if the manager can manage another warehouse based on the total products
    if (warehouseCount >= 3)
    {
        int totalProducts = 0;
        for (const auto &warehouse : GetAllWarehouses())
        {
            totalProducts += warehouse.Inventory; // Assuming Inventory represents the total products in the warehouse
        }
        if (totalProducts > 500)
        {
            return false;
        }
    }

    return true;
}

void Warehouse::AddWarehouse(const string &location, int manager)
{
    // check warehouse considering manager's constraint
    if (!CanManageWarehouse(manager))
    {
        cerr << "Manager cannot manage more warehouses due to product constraint\n";
        return;
    }

    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return;
    }

    // Prepare SQL statement for insertion
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "INSERT INTO Warehouses (Location, ManagerId) VALUES (?, ?)", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare insertion statement\n";
        sqlite3_close(db);
        return;
    }

    // Bind parameters
    sqlite3_bind_text(statement, 1, location.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(statement, 2, manager);

    // Execute statement
    if (sqlite3_step(statement) != SQLITE_DONE)
    {
        cerr << "Failed to execute insertion statement\n";
        sqlite3_finalize(statement);
        sqlite3_close(db);
        return;
    }

    // Get the warehouseId of the newly inserted warehouse
    int warehouseId = sqlite3_last_insert_rowid(db);

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    WarehouseId = warehouseId;
    Location = location;
    Manager = manager;

    cout << "Warehouse added successfully with id: " << warehouseId << "\n";
}

void Warehouse::UpdateWarehouseDetails(int warehouseId, const string &location, int manager)
{
    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return;
    }

    // Prepare SQL statement for updating
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "UPDATE Warehouses SET Location = ?, ManagerId = ? WHERE WarehouseId = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare update statement\n";
        sqlite3_close(db);
        return;
    }

    // Bind parameters
    sqlite3_bind_text(statement, 1, location.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(statement, 2, manager);
    sqlite3_bind_int(statement, 3, warehouseId);

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

    Location = location;
    Manager = manager;

    cout << "Warehouse details updated successfully\n";
}

void Warehouse::DeleteWarehouse(int warehouseId)
{
    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return;
    }

    // Prepare SQL statement for deletion
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "DELETE FROM Warehouses WHERE WarehouseId = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare deletion statement\n";
        sqlite3_close(db);
        return;
    }

    // Bind parameters
    sqlite3_bind_int(statement, 1, warehouseId);

    // Execute statement
    if (sqlite3_step(statement) != SQLITE_DONE)
    {
        cerr << "Failed to execute deletion statement\n";
        sqlite3_finalize(statement);
        sqlite3_close(db);
        return;
    }

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    cout << "Warehouse deleted successfully\n";
}

vector<Warehouse> Warehouse::GetAllWarehouses()
{
    vector<Warehouse> warehouses;

    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return warehouses;
    }

    // Prepare SQL statement for selecting all warehouses
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "SELECT * FROM Warehouses", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare select statement\n";
        sqlite3_close(db);
        return warehouses;
    }

    // Execute statement
    while (sqlite3_step(statement) == SQLITE_ROW)
    {
        int warehouseId = sqlite3_column_int(statement, 0);
        const unsigned char *location = sqlite3_column_text(statement, 1);
        int managerId = sqlite3_column_int(statement, 2);

        warehouses.emplace_back(warehouseId, reinterpret_cast<const char *>(location), 0, managerId);
    }

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    return warehouses;
}

Warehouse Warehouse::GetWarehouseById(int warehouseId)
{
    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return Warehouse(-1, "", 0, -1);
    }

    // Prepare SQL statement for selecting a warehouse by Id
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "SELECT * FROM Warehouses WHERE WarehouseId = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare select statement\n";
        sqlite3_close(db);
        return Warehouse(-1, "", 0, -1);
    }

    // Bind parameter
    sqlite3_bind_int(statement, 1, warehouseId);

    // Execute statement
    if (sqlite3_step(statement) == SQLITE_ROW)
    {
        const unsigned char *location = sqlite3_column_text(statement, 1);
        int managerId = sqlite3_column_int(statement, 2);

        // Finalize statement and close database
        sqlite3_finalize(statement);
        sqlite3_close(db);

        return Warehouse(warehouseId, reinterpret_cast<const char *>(location), 0, managerId);
    }

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    return Warehouse(-1, "", 0, -1);
}

bool Warehouse::DoesWarehouseExist(int warehouseId)
{
    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return false;
    }

    // Prepare SQL statement for checking if warehouse exists
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM Warehouses WHERE WarehouseId = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare statement\n";
        sqlite3_close(db);
        return false;
    }

    // Bind parameter
    sqlite3_bind_int(statement, 1, warehouseId);

    // Execute statement
    if (sqlite3_step(statement) == SQLITE_ROW)
    {
        int count = sqlite3_column_int(statement, 0);

        // Finalize statement and close database
        sqlite3_finalize(statement);
        sqlite3_close(db);

        return count > 0;
    }

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    return false;
}

void Warehouse::DisplayWarehouseDetails() const
{
    cout << "Warehouse Id: " << WarehouseId << endl;
    cout << "Location: " << Location << endl;
    cout << "Manager Id: " << Manager << endl;
}

void Warehouse::DisplayAllWarehouses(const vector<Warehouse> &warehouses)
{
    cout << "------------------------------------------------\n";
    cout << "All Warehouses:\n";
    for (const auto &warehouse : warehouses)
    {
        warehouse.DisplayWarehouseDetails();
        cout << "------------------------------------------------\n";
    }
}

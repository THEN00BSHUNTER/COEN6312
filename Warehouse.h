#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include <string>
#include <vector>

using namespace std;

class Warehouse
{
private:
    int WarehouseId;
    string Location;
    int Inventory;
    int Manager;

public:
    Warehouse(int warehouseId, const string &location, int inventory, int manager);

    // Functions
    void AddWarehouse(const string &location, int manager);
    void UpdateWarehouseDetails(int warehouseId, const string &location, int manager);
    void DeleteWarehouse(int warehouseId);

    static vector<Warehouse> GetAllWarehouses();
    static Warehouse GetWarehouseById(int warehouseId);
    static bool DoesWarehouseExist(int warehouseId);

    void DisplayWarehouseDetails() const;
    static void DisplayAllWarehouses(const vector<Warehouse> &warehouses);
};

#endif

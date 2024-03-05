#ifndef INVENTORY_H
#define INVENTORY_H

class Inventory
{
private:
    int ProductId;
    int ProductAvailability;
    int ReorderQuantity;

public:
    Inventory(int productId, int productAvailability, int reorderQuantity);

    // Functions
    void AddItemToInventory(int productId, int quantity);
    void RemoveItemFromInventory(int productId, int quantity);
    void UpdateStockLevels(int productId, int newAvailability, int newReorderQuantity);
    void UpdateInventory(int productId, int difference);
    static void ViewInventory();
};

#endif

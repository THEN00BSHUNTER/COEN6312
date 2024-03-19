#ifndef INVENTORY_H
#define INVENTORY_H

const int DISCOUNT_THRESHOLD = 10;

class Inventory
{
private:
    int ProductId;
    int ProductAvailability;
    int ReorderThreshold;

public:
    Inventory(int productId, int productAvailability, int reorderThreshold);

    // Functions
    void AddItemToInventory(int productId, int quantity);
    void RemoveItemFromInventory(int productId, int quantity);
    void UpdateStockLevels(int productId, int newAvailability, int newReorderThreshold);
    void UpdateInventory(int productId, int difference);
    static void ViewInventory();
    static int GetProductAvailability(int productId);
    static int GetReorderThreshold(int productId);
};

#endif

#ifndef ORDERITEM_H
#define ORDERITEM_H

#include <vector>
#include <string>
#include "sqlite\sqlite3.h"

using namespace std;

class OrderItem
{
private:
    int OrderItemId;
    int OrderId;
    int ProductId;
    int Quantity;

public:
    OrderItem(int orderItemId, int orderId, int productId, int quantity);
    static vector<OrderItem> GetOrderItemsByOrderId(int orderId);
    string GetProductName() const;
    int GetProductId() const;
};

#endif

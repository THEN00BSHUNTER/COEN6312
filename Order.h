#ifndef PURCHASEORDER_H
#define PURCHASEORDER_H

#include "OrderItem.h"
#include <vector>
#include <string>

using namespace std;

class OrderItem;

enum class OrderStatus
{
    Placed,
    Processing,
    Shipped,
    Delivered,
    Cancelled
};

class Order
{
private:
    int OrderId;
    int CustomerId;
    vector<int> ItemsOrdered;
    string OrderDate;
    OrderStatus Status;

public:
    Order(int orderId, int customerId, const vector<int> &itemsOrdered, const string &orderDate, const OrderStatus &status);

    // Functions
    void CancelOrder(int orderId);
    void TrackOrder(int orderId);
    void UpdateStatus(OrderStatus status);

    static vector<Order> GetAllOrders();

    int GetOrderId() const;
    int GetCustomerId() const;
    string GetStatus() const;

    vector<OrderItem> GetOrderItems() const;

    static void CreateOrder(int customerId, const string &orderDate, const OrderStatus &status, const vector<int> itemsOrdered);
    static Order GetOrderById(int orderId);
};

#endif

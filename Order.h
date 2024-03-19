#ifndef ORDER_H
#define ORDER_H

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
    double DiscountedPrice;

public:
    Order(int orderId, int customerId, const vector<int> &itemsOrdered, const string &orderDate, const OrderStatus &status, double discountedPrice);

    // Functions
    void CancelOrder(int orderId);
    void TrackOrder(int orderId);
    void UpdateStatus(OrderStatus status);

    static vector<Order> GetAllOrders();

    int GetOrderId() const;
    int GetCustomerId() const;
    string GetStatus() const;

    vector<OrderItem> GetOrderItems() const;

    static void CreateOrder(int customerId, const string &orderDate, const OrderStatus &status, const vector<int> itemsOrdered, const vector<int> itemsCountOrdered, double discountedPrice);
    static Order GetOrderById(int orderId);
};

#endif
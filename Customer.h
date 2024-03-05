#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "User.h"
#include <vector>

using namespace std;

class Customer : public User
{
private:
    string ShippingAddress;
    string PaymentMethod;
    vector<string> OrderHistory;

public:
    Customer(int id, const string &username, const string &password, const string &email,
             const string &phone, const string &address, const string &role,
             const string &shippingAddress, const string &paymentMethod);

    // Functions
    void PlaceOrder();
    void ViewOrderHistory();

    static Customer GetCustomerById(int userId);

    // Static methods for customer login and registration
    static Customer *RegisterCustomer(const string &username, const string &password, const string &email,
                                      const string &phone, const string &address,
                                      const string &shippingAddress, const string &paymentMethod);

    // static Customer *Login(const string &username, const string &password);
};

#endif

#ifndef SUPPLIER_H
#define SUPPLIER_H

#include "User.h"
#include <string>

using namespace std;

class Supplier : public User
{
private:
    string ContactPerson;
    string ProductsSupplied;
    string PaymentTerms;

public:
    Supplier(int id, const string &username, const string &password, const string &email, const string &phone, const string &address, const string &role, const string &contactPerson, const string &productsSupplied, const string &paymentTerms);

    static Supplier *RegisterSupplier(const string &username, const string &password, const string &email, const string &phone, const string &address, const string &contactPerson, const string &productsSupplied, const string &paymentTerms);

    // Functions
    void ViewInventory();
    void UpdateProductInformation();
};

#endif

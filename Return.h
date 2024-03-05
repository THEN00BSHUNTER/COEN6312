#ifndef RETURN_H
#define RETURN_H

#include <vector>
#include <string>

using namespace std;

class Return
{
private:
    int ReturnId;
    int CustomerId;
    vector<int> ItemsReturned;
    string ReturnReason;
    string ReturnDate;
    int OrderId;

public:
    Return(int returnId, int customerId, const vector<int> &itemsReturned, const string &returnReason,
           const string &returnDate, int orderId);

    // Functions
    void ProcessReturn(int returnId);
    void RefundCustomer(int returnId);
    void UpdateInventory(int returnId);
    static void CreateReturn(int customerId, const vector<int> &itemsReturned, const string &returnReason, const string &returnDate, int orderId);
};

#endif

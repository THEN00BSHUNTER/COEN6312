#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <vector>

using namespace std;

class Transaction
{
private:
    int TransactionId;
    string Timestamp;
    string Type;
    int CustomerId;
    int BranchId;
    double TotalAmount;

public:
    Transaction(int transactionId, const string &timestamp, const string &type,
                int customerId, int branchId, double totalAmount);

    // Functions
    void AddTransaction(const string &timestamp, const string &type, int customerId, int branchId, double totalAmount);
    double CalculateTotal();
    void RetrieveTransactionDetails() const;
    static vector<Transaction> GetAllTransactions();
};

#endif

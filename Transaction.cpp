#include "Transaction.h"
#include <iostream>
#include <vector>
#include "sqlite\sqlite3.h"

using namespace std;

Transaction::Transaction(int transactionId, const string &timestamp, const string &type,
                         int customerId, int branchId, double totalAmount)
    : TransactionId(transactionId), Timestamp(timestamp), Type(type), CustomerId(customerId),
      BranchId(branchId), TotalAmount(totalAmount) {}

void Transaction::AddTransaction(const string &timestamp, const string &type,
                                 int customerId, int branchId, double totalAmount)
{
    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return;
    }

    // Prepare SQL statement for inserting a new transaction
    sqlite3_stmt *statement;
    const char *sql = "INSERT INTO Transactions (Timestamp, Type, CustomerId, BranchId, TotalAmount) VALUES (?, ?, ?, ?, ?)";
    if (sqlite3_prepare_v2(db, sql, -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare insert statement\n";
        sqlite3_close(db);
        return;
    }

    // Bind parameters
    sqlite3_bind_text(statement, 1, timestamp.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 2, type.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(statement, 3, customerId);
    sqlite3_bind_int(statement, 4, branchId);
    sqlite3_bind_double(statement, 5, totalAmount);

    // Execute statement
    if (sqlite3_step(statement) != SQLITE_DONE)
    {
        cerr << "Failed to execute insert statement\n";
        sqlite3_finalize(statement);
        sqlite3_close(db);
        return;
    }

    // Get the transactionId of the newly inserted transaction
    int transactionId = sqlite3_last_insert_rowid(db);

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    TransactionId = transactionId;
    Timestamp = timestamp;
    Type = type;
    CustomerId = customerId;
    BranchId = branchId;
    TotalAmount = totalAmount;

    cout << "Transaction added successfully with id: " << transactionId << "\n";
}

double Transaction::CalculateTotal()
{
    return TotalAmount;
}

void Transaction::RetrieveTransactionDetails() const
{
    cout << "Transaction Id: " << TransactionId << endl;
    cout << "Timestamp: " << Timestamp << endl;
    cout << "Type: " << Type << endl;
    cout << "Customer Id: " << CustomerId << endl;
    cout << "Branch Id: " << BranchId << endl;
    cout << "Total Amount: $" << TotalAmount << endl;
}

vector<Transaction> Transaction::GetAllTransactions()
{
    vector<Transaction> transactions;

    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return transactions;
    }

    // Prepare SQL statement for selecting all transactions
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "SELECT * FROM Transactions", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare select statement\n";
        sqlite3_close(db);
        return transactions;
    }

    // Execute statement and fetch rows
    while (sqlite3_step(statement) == SQLITE_ROW)
    {
        int transactionId = sqlite3_column_int(statement, 0);
        const unsigned char *timestamp = sqlite3_column_text(statement, 1);
        const unsigned char *type = sqlite3_column_text(statement, 2);
        int customerId = sqlite3_column_int(statement, 3);
        int branchId = sqlite3_column_int(statement, 4);
        double totalAmount = sqlite3_column_double(statement, 5);

        // Create Transaction object and add it to the vector
        transactions.emplace_back(transactionId,
                                  reinterpret_cast<const char *>(timestamp),
                                  reinterpret_cast<const char *>(type),
                                  customerId, branchId, totalAmount);
    }

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    return transactions;
}

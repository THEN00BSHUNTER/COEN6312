#ifndef MANAGER_H
#define MANAGER_H

#include "User.h"
#include <vector>

using namespace std;

class Manager : public User
{
private:
    string Department;
    vector<int> Subordinates;

public:
    Manager(int id, const string &username, const string &password, const string &email,
            const string &phone, const string &address, const string &role, const string &department);

    static Manager *RegisterManager(const string &username, const string &password, const string &email, const string &phone, const string &address, const string &department);

    // Functions
    void ApproveOrder();
    void GenerateReport();
    void MonitorSales();
};

#endif

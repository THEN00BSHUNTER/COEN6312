#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "User.h"
#include "Employee.h"
#include <string>

using namespace std;

class Employee : public User
{
private:
    int ManagerId;
    int BranchId;
    string Position;
    string Tasks;

public:
    Employee(int id, const string &username, const string &password, const string &email,
             const string &phone, const string &address, const string &role, int managerId, int branchId, const string &position, const string &tasks);

    string GetUsername() const;

    static Employee *RegisterEmployee(const string &username, const string &password, const string &email, const string &phone, const string &address, int managerId, int branchId, const string &position, const string &tasks);

    static bool UpdateEmployeeData(Employee employee);

    // Functions
    void ProcessOrder();
    void UpdateInventory(int productId, int difference);
};

#endif

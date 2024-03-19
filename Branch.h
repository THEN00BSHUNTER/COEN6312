#ifndef BRANCH_H
#define BRANCH_H

#include "Employee.h"
#include <string>
#include <vector>
#include "sqlite\sqlite3.h"

using namespace std;

class Branch
{
private:
    int BranchId;
    string Location;
    int ManagerId;

public:
    Branch(int branchId, const string &location, int managerId);
    int GetBranchId() const;
    int GetManagerId() const;

    //

    void ManageEmployees();
    void HandleCustomerInquiries();
    static vector<Branch> GetAllBranches();
    static void DisplayAllBranches(vector<Branch>);
    vector<Employee> GetAllEmployeesByBranchId(int branchId);
    static bool CanManageBranch(int managerId);
    static void AddBranch(const string &location, int managerId);
};

#endif

#ifndef MENU_H
#define MENU_H

#include "User.h"

using namespace std;

class Menu
{

private:
    void DisplayLoginMenu();
    void DisplayRegistrationMenu();
    void DisplayMainMenu();
    void DisplayUserMenu();

    void DisplayCustomerMenu();

    void DisplayCategoryMenu();
    void DisplaySubcategoryMenu();
    void DisplayProductMenu();
    void DisplayOrderMenu();
    void DisplayReturnMenu();
    void DisplayDiscountMenu();
    void DisplayPromoMenu();

    void DisplayWarehouseMenu();
    void DisplayBranchMenu();
    void DisplayInventoryMenu();
    void DisplayTransactionMenu();

    // validate the role entered by the user
    bool IsValidRole(const string &role);

public:
    // Functions
    void RunLoginMenu();
    void RunRegistrationMenu();
    void RunMainMenu(User *user);
    void RunUserMenu(User *user);
    void RunCustomerMenu();

    void RunCategoryMenu();
    void RunSubcategoryMenu();
    void RunProductMenu();
    void RunOrderMenu();
    void RunReturnMenu();
    void RunDiscountMenu();
    void RunPromoMenu();

    void RunWarehouseMenu();
    void RunBranchMenu();
    void RunInventoryMenu();
    void RunTransactionMenu();
};

#endif

#include <iostream>

#include "Database.h"
#include "Menu.h"

using namespace std;

int main()
{
    if (Database::CreateDatabase() != 0)
    {
        cerr << "The database cannot be created or accessed. Exiting...\n";
        return 1;
    }

    if (Database::SampleData() != 0)
    {
        cerr << "The sample data cannot be inserted. Exiting...\n";
        return 1;
    }

    Menu *menu = new Menu();

    menu->RunLoginMenu();

    return 0;
}

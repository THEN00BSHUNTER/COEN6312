#ifndef CATEGORY_H
#define CATEGORY_H

#include <string>
#include <vector>
#include "sqlite\sqlite3.h"

using namespace std;

class Category
{
private:
    int CategoryId;
    string Name;
    string Description;

public:
    Category(int categoryId, const string &name, const string &description);

    // Functions
    void AddCategory(const string &name, const string &description);
    void UpdateCategoryDetails(int categoryId, const string &name, const string &description);
    void DeleteCategory(int categoryId);

    // Static Methods for Database Interaction
    static vector<Category> GetAllCategories();
    static Category GetCategoryById(int categoryId);
    static void DisplayAllCategories(const vector<Category> &categories);

    // Utility Methods
    void DisplayCategoryDetails() const;
    static bool DoesCategoryExist(int categoryId);
};

#endif

#ifndef SUBCATEGORY_H
#define SUBCATEGORY_H

#include <string>
#include <vector>
#include "Category.h" // Include Category header for foreign key relationship

using namespace std;

class Subcategory
{
private:
    int SubcategoryId;
    string Name;
    string Description;
    int CategoryId; // Foreign key

public:
    Subcategory(int subcategoryId, const string &name, const string &description, int categoryId);

    // Functions
    void AddSubcategory(const string &name, const string &description, int categoryId);
    void UpdateSubcategoryDetails(int subcategoryId, const string &name, const string &description, int categoryId);
    void DeleteSubcategory(int subcategoryId);

    // Static Methods for Database Interaction
    static vector<Subcategory> GetAllSubcategories();
    static Subcategory GetSubcategoryById(int subcategoryId);
    static vector<Subcategory> GetSubcategoriesByCategoryId(int categoryId);
    static void DisplayAllSubcategories(const vector<Subcategory> &subcategories);

    // Utility Methods
    void DisplaySubcategoryDetails() const;
    static bool DoesSubcategoryExist(int subcategoryId);
};

#endif

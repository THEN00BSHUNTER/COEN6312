#include "Category.h"
#include <iostream>
#include "sqlite\sqlite3.h"

// Constructor
Category::Category(int categoryId, const string &name, const string &description)
    : CategoryId(categoryId), Name(name), Description(description) {}

// Functions
void Category::AddCategory(const string &name, const string &description)
{
    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return;
    }

    // Prepare SQL statement for insertion
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "INSERT INTO Categories (Name, Description) VALUES (?, ?)", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare insertion statement\n";
        sqlite3_close(db);
        return;
    }

    // Bind parameters
    sqlite3_bind_text(statement, 1, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 2, description.c_str(), -1, SQLITE_STATIC);

    // Execute statement
    if (sqlite3_step(statement) != SQLITE_DONE)
    {
        cerr << "Failed to execute insertion statement\n";
        sqlite3_finalize(statement);
        sqlite3_close(db);
        return;
    }

    // Get the categoryId of the newly inserted category
    int categoryId = sqlite3_last_insert_rowid(db);

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    CategoryId = categoryId;
    Name = name;
    Description = description;

    cout << "Category added successfully with id: " << categoryId << "\n";
}

void Category::UpdateCategoryDetails(int categoryId, const string &name, const string &description)
{
    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return;
    }

    // Prepare SQL statement for updating
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "UPDATE Categories SET Name = ?, Description = ? WHERE CategoryId = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare update statement\n";
        sqlite3_close(db);
        return;
    }

    // Bind parameters
    sqlite3_bind_text(statement, 1, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 2, description.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(statement, 3, categoryId);

    // Execute statement
    if (sqlite3_step(statement) != SQLITE_DONE)
    {
        cerr << "Failed to execute update statement\n";
        sqlite3_finalize(statement);
        sqlite3_close(db);
        return;
    }

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    Name = name;
    Description = description;

    cout << "Category details updated successfully\n";
}

void Category::DeleteCategory(int categoryId)
{
    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return;
    }

    // Prepare SQL statement for deletion
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "DELETE FROM Categories WHERE CategoryId = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare deletion statement\n";
        sqlite3_close(db);
        return;
    }

    // Bind parameters
    sqlite3_bind_int(statement, 1, categoryId);

    // Execute statement
    if (sqlite3_step(statement) != SQLITE_DONE)
    {
        cerr << "Failed to execute deletion statement\n";
        sqlite3_finalize(statement);
        sqlite3_close(db);
        return;
    }

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    cout << "Category deleted successfully\n";
}

// Static Methods for Database Interaction
vector<Category> Category::GetAllCategories()
{
    vector<Category> categories;

    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return categories;
    }

    // Prepare SQL statement for selecting all categories
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "SELECT * FROM Categories", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare select statement\n";
        sqlite3_close(db);
        return categories;
    }

    // Execute statement
    while (sqlite3_step(statement) == SQLITE_ROW)
    {
        int categoryId = sqlite3_column_int(statement, 0);
        const unsigned char *name = sqlite3_column_text(statement, 1);
        const unsigned char *description = sqlite3_column_text(statement, 2);

        categories.emplace_back(categoryId, reinterpret_cast<const char *>(name), reinterpret_cast<const char *>(description));
    }

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    return categories;
}

Category Category::GetCategoryById(int categoryId)
{
    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return Category(-1, "", "");
    }

    // Prepare SQL statement for selecting a category by Id
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "SELECT * FROM Categories WHERE CategoryId = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare select statement\n";
        sqlite3_close(db);
        return Category(-1, "", "");
    }

    // Bind parameter
    sqlite3_bind_int(statement, 1, categoryId);

    // Execute statement
    if (sqlite3_step(statement) == SQLITE_ROW)
    {
        const unsigned char *name = sqlite3_column_text(statement, 1);
        const unsigned char *description = sqlite3_column_text(statement, 2);

        // Finalize statement and close database
        sqlite3_finalize(statement);
        sqlite3_close(db);

        return Category(categoryId, reinterpret_cast<const char *>(name), reinterpret_cast<const char *>(description));
    }

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    return Category(-1, "", "");
}

// Utility Methods
void Category::DisplayCategoryDetails() const
{
    cout << "Category Id: " << CategoryId << endl;
    cout << "Name: " << Name << endl;
    cout << "Description: " << Description << endl;
}

void Category::DisplayAllCategories(const vector<Category> &categories)
{
    cout << "------------------------------------------------\n";
    cout << "All Categories:\n";
    for (const auto &category : categories)
    {
        category.DisplayCategoryDetails();
        cout << "------------------------------------------------\n";
    }
}

bool Category::DoesCategoryExist(int categoryId)
{
    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return false;
    }

    // Prepare SQL statement for checking if category exists
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM Categories WHERE CategoryId = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare statement\n";
        sqlite3_close(db);
        return false;
    }

    // Bind parameter
    sqlite3_bind_int(statement, 1, categoryId);

    // Execute statement
    if (sqlite3_step(statement) == SQLITE_ROW)
    {
        int count = sqlite3_column_int(statement, 0);

        // Finalize statement and close database
        sqlite3_finalize(statement);
        sqlite3_close(db);

        return count > 0;
    }

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    return false;
}

#include "Subcategory.h"
#include <iostream>
#include "sqlite\sqlite3.h"

// Constructor
Subcategory::Subcategory(int subcategoryId, const string &name, const string &description, int categoryId)
    : SubcategoryId(subcategoryId), Name(name), Description(description), CategoryId(categoryId) {}

// Functions
void Subcategory::AddSubcategory(const string &name, const string &description, int categoryId)
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
    if (sqlite3_prepare_v2(db, "INSERT INTO Subcategories (Name, Description, CategoryId) VALUES (?, ?, ?)", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare insertion statement\n";
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
        cerr << "Failed to execute insertion statement\n";
        sqlite3_finalize(statement);
        sqlite3_close(db);
        return;
    }

    // Get the subcategoryId of the newly inserted subcategory
    int subcategoryId = sqlite3_last_insert_rowid(db);

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    SubcategoryId = subcategoryId;
    Name = name;
    Description = description;
    CategoryId = categoryId;

    cout << "Subcategory added successfully with id: " << categoryId << "\n";
}

void Subcategory::UpdateSubcategoryDetails(int subcategoryId, const string &name, const string &description, int categoryId)
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
    if (sqlite3_prepare_v2(db, "UPDATE Subcategories SET Name = ?, Description = ?, CategoryId = ? WHERE SubcategoryId = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare update statement\n";
        sqlite3_close(db);
        return;
    }

    // Bind parameters
    sqlite3_bind_text(statement, 1, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 2, description.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(statement, 3, categoryId);
    sqlite3_bind_int(statement, 4, subcategoryId);

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
    CategoryId = categoryId;

    cout << "Subcategory details updated successfully\n";
}

void Subcategory::DeleteSubcategory(int subcategoryId)
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
    if (sqlite3_prepare_v2(db, "DELETE FROM Subcategories WHERE SubcategoryId = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare deletion statement\n";
        sqlite3_close(db);
        return;
    }

    // Bind parameters
    sqlite3_bind_int(statement, 1, subcategoryId);

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

    cout << "Subcategory deleted successfully\n";
}

// Static Methods for Database Interaction
vector<Subcategory> Subcategory::GetAllSubcategories()
{
    vector<Subcategory> subcategories;

    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return subcategories;
    }

    // Prepare SQL statement for selecting all subcategories
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "SELECT * FROM Subcategories", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare select statement\n";
        sqlite3_close(db);
        return subcategories;
    }

    // Execute statement
    while (sqlite3_step(statement) == SQLITE_ROW)
    {
        int subcategoryId = sqlite3_column_int(statement, 0);
        const unsigned char *name = sqlite3_column_text(statement, 1);
        const unsigned char *description = sqlite3_column_text(statement, 2);
        int categoryId = sqlite3_column_int(statement, 3);

        subcategories.emplace_back(subcategoryId, reinterpret_cast<const char *>(name), reinterpret_cast<const char *>(description), categoryId);
    }

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    return subcategories;
}

Subcategory Subcategory::GetSubcategoryById(int subcategoryId)
{
    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return Subcategory(-1, "", "", -1);
    }

    // Prepare SQL statement for selecting a subcategory by Id
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "SELECT * FROM Subcategories WHERE SubcategoryId = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare select statement\n";
        sqlite3_close(db);
        return Subcategory(-1, "", "", -1);
    }

    // Bind parameter
    sqlite3_bind_int(statement, 1, subcategoryId);

    // Execute statement
    if (sqlite3_step(statement) == SQLITE_ROW)
    {
        const unsigned char *name = sqlite3_column_text(statement, 1);
        const unsigned char *description = sqlite3_column_text(statement, 2);
        int categoryId = sqlite3_column_int(statement, 3);

        // Finalize statement and close database
        sqlite3_finalize(statement);
        sqlite3_close(db);

        return Subcategory(subcategoryId, reinterpret_cast<const char *>(name), reinterpret_cast<const char *>(description), categoryId);
    }

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    return Subcategory(-1, "", "", -1);
}

vector<Subcategory> Subcategory::GetSubcategoriesByCategoryId(int categoryId)
{
    vector<Subcategory> subcategories;

    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return subcategories;
    }

    // Prepare SQL statement for selecting subcategories by category Id
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "SELECT * FROM Subcategories WHERE CategoryId = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare select statement\n";
        sqlite3_close(db);
        return subcategories;
    }

    // Bind parameter
    sqlite3_bind_int(statement, 1, categoryId);

    // Execute statement
    while (sqlite3_step(statement) == SQLITE_ROW)
    {
        int subcategoryId = sqlite3_column_int(statement, 0);
        const unsigned char *name = sqlite3_column_text(statement, 1);
        const unsigned char *description = sqlite3_column_text(statement, 2);

        subcategories.emplace_back(subcategoryId, reinterpret_cast<const char *>(name), reinterpret_cast<const char *>(description), categoryId);
    }

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    return subcategories;
}

void Subcategory::DisplaySubcategoryDetails() const
{
    cout << "Subcategory Id: " << SubcategoryId << endl;
    cout << "Name: " << Name << endl;
    cout << "Description: " << Description << endl;
    cout << "Category Id: " << CategoryId << endl;
}

void Subcategory::DisplayAllSubcategories(const vector<Subcategory> &subcategories)
{
    cout << "------------------------------------------------\n";
    cout << "All Subcategories:\n";
    for (const auto &subcategory : subcategories)
    {
        subcategory.DisplaySubcategoryDetails();
        cout << "------------------------------------------------\n";
    }
}

bool Subcategory::DoesSubcategoryExist(int subcategoryId)
{
    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return false;
    }

    // Prepare SQL statement for checking if subcategory exists
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM Subcategories WHERE SubcategoryId = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare statement\n";
        sqlite3_close(db);
        return false;
    }

    // Bind parameter
    sqlite3_bind_int(statement, 1, subcategoryId);

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

#include "Database.h"
#include "Product.h"
#include "Discount.h"
#include <iostream>
#include "sqlite\sqlite3.h"

Discount::Discount(int discountId, const string &name, const string &description, const string &startPeriod, const string &endPeriod, int productId, double percentage)
    : DiscountId(discountId), Name(name), Description(description), StartPeriod(startPeriod), EndPeriod(endPeriod), ProductId(productId), Percentage(percentage) {}

int Discount::GetProductId() const
{
    return ProductId;
}

double Discount::GetPercentage() const
{
    return Percentage;
}

void Discount::CreateDiscount(const string &name, const string &description, const string &startPeriod, const string &endPeriod, int productId, double percentage)
{
    // Check for overlapping discounts
    if (CheckOverlappingDiscounts(productId, startPeriod, endPeriod))
    {
        cerr << "Overlapping discounts for the same product category are not allowed\n";
        return;
    }

    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return;
    }

    // Prepare SQL statement for insertion
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "INSERT INTO Discounts (Name, Description, StartPeriod, EndPeriod, ProductId, Percentage) VALUES (?, ?, ?, ?, ?, ?)", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare insertion statement\n";
        sqlite3_close(db);
        return;
    }

    // Bind parameters
    sqlite3_bind_text(statement, 1, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 2, description.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 3, startPeriod.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 4, endPeriod.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(statement, 5, productId);
    sqlite3_bind_int(statement, 6, percentage);

    // Execute statement
    if (sqlite3_step(statement) != SQLITE_DONE)
    {
        cerr << "Failed to execute insertion statement\n";
        sqlite3_finalize(statement);
        sqlite3_close(db);
        return;
    }

    // Get the discountId of the newly inserted discount
    int discountId = sqlite3_last_insert_rowid(db);

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    DiscountId = discountId;
    Name = name;
    Description = description;
    StartPeriod = startPeriod;
    EndPeriod = endPeriod;
    ProductId = productId;
    Percentage = percentage;

    cout << "Discount created successfully with id: " << discountId << "\n";
}

bool Discount::CheckOverlappingDiscounts(int productId, const string &startPeriod, const string &endPeriod)
{
    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return true; // Assume overlap in case of database error
    }

    // Prepare SQL statement for checking overlapping discounts
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "SELECT DiscountId FROM Discounts WHERE ProductId = ? AND ((StartPeriod BETWEEN ? AND ?) OR (EndPeriod BETWEEN ? AND ?))", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare select statement\n";
        sqlite3_close(db);
        return true; // Assume overlap in case of preparation error
    }

    // Bind parameters
    sqlite3_bind_int(statement, 1, productId);
    sqlite3_bind_text(statement, 2, startPeriod.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 3, endPeriod.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 4, startPeriod.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 5, endPeriod.c_str(), -1, SQLITE_STATIC);

    // Execute statement
    bool overlap = false;
    if (sqlite3_step(statement) == SQLITE_ROW)
    {
        overlap = true; // Overlapping discounts found
    }

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    return overlap;
}

Discount Discount::GetDiscountById(int discountId)
{
    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return Discount(-1, "", "", "", "", -1, -1.0f);
    }

    // Prepare SQL statement for selecting a discount by Id
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "SELECT * FROM Discounts WHERE DiscountId = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare select statement\n";
        sqlite3_close(db);
        return Discount(-1, "", "", "", "", -1, -1.0f);
    }

    // Bind parameter
    sqlite3_bind_int(statement, 1, discountId);

    // Execute statement
    if (sqlite3_step(statement) == SQLITE_ROW)
    {
        const unsigned char *name = sqlite3_column_text(statement, 1);
        const unsigned char *description = sqlite3_column_text(statement, 2);
        const unsigned char *startPeriod = sqlite3_column_text(statement, 3);
        const unsigned char *endPeriod = sqlite3_column_text(statement, 4);
        int productId = sqlite3_column_int(statement, 5);
        float percentage = static_cast<float>(sqlite3_column_double(statement, 6));

        // Finalize statement and close database
        sqlite3_finalize(statement);
        sqlite3_close(db);

        return Discount(discountId, reinterpret_cast<const char *>(name), reinterpret_cast<const char *>(description), reinterpret_cast<const char *>(startPeriod), reinterpret_cast<const char *>(endPeriod), productId, percentage);
    }

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    return Discount(-1, "", "", "", "", -1, -1.0f);
}

Discount Discount::GetDiscountByName(string discountName)
{
    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return Discount(-1, "", "", "", "", -1, -1.0f);
    }

    // Prepare SQL statement for selecting a discount by Id
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "SELECT * FROM Discounts WHERE Name = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare select statement\n";
        sqlite3_close(db);
        return Discount(-1, "", "", "", "", -1, -1.0f);
    }

    // Bind parameter
    sqlite3_bind_text(statement, 1, discountName.c_str(), -1, SQLITE_STATIC);

    // Execute statement
    if (sqlite3_step(statement) == SQLITE_ROW)
    {
        int discountId = sqlite3_column_int(statement, 0);
        const unsigned char *name = sqlite3_column_text(statement, 1);
        const unsigned char *description = sqlite3_column_text(statement, 2);
        const unsigned char *startPeriod = sqlite3_column_text(statement, 3);
        const unsigned char *endPeriod = sqlite3_column_text(statement, 4);
        int productId = sqlite3_column_int(statement, 5);
        float percentage = static_cast<float>(sqlite3_column_double(statement, 6));

        // Finalize statement and close database
        sqlite3_finalize(statement);
        sqlite3_close(db);

        return Discount(discountId, reinterpret_cast<const char *>(name), reinterpret_cast<const char *>(description), reinterpret_cast<const char *>(startPeriod), reinterpret_cast<const char *>(endPeriod), productId, percentage);
    }

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    return Discount(-1, "", "", "", "", -1, -1.0f);
}

void Discount::ApplyDiscount(string discountName)
{

    // Find the discount object from the discountId
    Discount discount = Discount::GetDiscountByName(discountName);

    // Find the product associated with the discount
    Product product = Product::GetProductById(discount.GetProductId());

    if (IsDiscountValid(discount.DiscountId))
    {
        // Calculate discounted price
        float discountedPrice = product.GetPrice() * (1 - discount.GetPercentage() / 100);

        // Display the discounted price
        cout << "Discounted price for product " << product.GetName() << ": $" << discountedPrice << endl;
    }
    else
    {
        product.GetPrice();
    }
}

bool Discount::IsDiscountValid(int discountId)
{
    // Find the discount associated with the given Id
    Discount discount = Discount::GetDiscountById(discountId);

    // Get current Unix time
    time_t currentTime = time(nullptr);

    // Convert start and end period strings to Unix time using the database object
    time_t startTime = Database::ConvertToUnixTime(discount.StartPeriod);
    time_t endTime = Database::ConvertToUnixTime(discount.EndPeriod);

    // Check if current time is within the discount period
    return (currentTime >= startTime && currentTime <= endTime);
}

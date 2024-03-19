#include "Promo.h"
#include "Product.h"
#include "Database.h"
#include <iostream>
#include "sqlite\sqlite3.h"

using namespace std;

Promo::Promo(int promoId, const string &name, const string &description, double discountPercentage, const string &startPeriod, const string &endPeriod, int productId, int employeeId)
    : PromoId(promoId), Name(name), Description(description), DiscountPercentage(discountPercentage), StartPeriod(startPeriod), EndPeriod(endPeriod), ProductId(productId), EmployeeId(employeeId) {}

string Promo::GetStartPeriod() const
{
    return StartPeriod;
}
string Promo::GetEndPeriod() const
{
    return EndPeriod;
}

int Promo::GetProductId() const
{
    return ProductId;
}
double Promo::GetDiscountPercentage() const
{
    return DiscountPercentage;
}

int Promo::GetEmployeeId() const
{
    return EmployeeId;
}

bool Promo::CheckOverlappingPromotions(int employeeId, const string &startPeriod, const string &endPeriod)
{
    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return true; // Assume overlap in case of database error
    }

    // Prepare SQL statement for checking overlapping promotions
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "SELECT PromoId FROM Promos WHERE EmployeeId = ? AND ((StartPeriod BETWEEN ? AND ?) OR (EndPeriod BETWEEN ? AND ?))", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare select statement\n";
        sqlite3_close(db);
        return true; // Assume overlap in case of preparation error
    }

    // Bind parameters
    sqlite3_bind_int(statement, 1, employeeId);
    sqlite3_bind_text(statement, 2, startPeriod.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 3, endPeriod.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 4, startPeriod.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 5, endPeriod.c_str(), -1, SQLITE_STATIC);

    // Execute statement
    bool overlap = false;
    if (sqlite3_step(statement) == SQLITE_ROW)
    {
        overlap = true; // Overlapping promotions found
    }

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    return overlap;
}

void Promo::CreatePromotion(const string &name, const string &description, double discountPercentage, const string &startPeriod, const string &endPeriod, int productId, int employeeId)
{

    if (CheckOverlappingPromotions(employeeId, startPeriod, endPeriod))
    {
        cerr << "Employee has overlapping promotions\n";
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
    sqlite3_stmt *insertStatement;
    if (sqlite3_prepare_v2(db, "INSERT INTO Promos (Name, Description, DiscountPercentage, StartPeriod, EndPeriod, ProductId, EmployeeId) VALUES (?, ?, ?, ?, ?, ?, ?)", -1, &insertStatement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare insertion statement\n";
        sqlite3_close(db);
        return;
    }

    // Bind parameters
    sqlite3_bind_text(insertStatement, 1, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(insertStatement, 2, description.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(insertStatement, 3, discountPercentage);
    sqlite3_bind_text(insertStatement, 4, startPeriod.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(insertStatement, 5, endPeriod.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(insertStatement, 6, productId);
    sqlite3_bind_int(insertStatement, 7, employeeId);

    // Execute insertion statement
    if (sqlite3_step(insertStatement) != SQLITE_DONE)
    {
        cerr << "Failed to execute insertion statement\n";
        sqlite3_finalize(insertStatement);
        sqlite3_close(db);
        return;
    }

    // Get the promoId of the newly inserted promotion
    int promoId = sqlite3_last_insert_rowid(db);

    // Finalize statement and close database
    sqlite3_finalize(insertStatement);
    sqlite3_close(db);

    PromoId = promoId;
    Name = name;
    Description = description;
    DiscountPercentage = discountPercentage;
    StartPeriod = startPeriod;
    EndPeriod = endPeriod;
    ProductId = productId;
    EmployeeId = employeeId;

    cout << "Promotion created successfully with id: " << promoId << "\n";
}

double Promo::ApplyPromotion(double totalPrice, int itemCount)
{
    double discountedPrice = totalPrice;

    if (itemCount < 10)
    {
        // Apply 5% discount
        discountedPrice *= 0.95;
    }
    else if (itemCount >= 10 && itemCount <= 20)
    {
        // Apply 10% discount
        discountedPrice *= 0.90;
    }
    else
    {
        // Apply 15% discount
        discountedPrice *= 0.85;
    }

    return discountedPrice;
}

void Promo::ApplyPromotion(string promoName)
{
    // Find the promotion object from the promoId
    Promo promo = Promo::GetPromotionByName(promoName);

    // Find the product associated with the promotion
    Product product = Product::GetProductById(promo.GetProductId());

    if (IsPromotionValid(promo.PromoId))
    {
        // Calculate discounted price or apply other promotion logic
        // For example, if it's a percentage-based discount:
        double discountedPrice = product.GetPrice() * (1 - promo.GetDiscountPercentage() / 100);

        // Display the discounted price
        cout << "Discounted price for product " << product.GetName() << ": $" << discountedPrice << endl;
    }
    else
    {
        cout << "Promotion with Id " << promo.PromoId << " is not valid.\n";
    }
}

bool Promo::IsPromotionValid(int promoId)
{
    // Find the promotion associated with the given Id
    Promo promo = Promo::GetPromotionById(promoId);

    // Get current Unix time
    time_t currentTime = time(nullptr);

    // Convert start and end period strings to Unix time using the database object
    time_t startTime = Database::ConvertToUnixTime(promo.GetStartPeriod());
    time_t endTime = Database::ConvertToUnixTime(promo.GetEndPeriod());

    // Check if the current time is within the promotion period
    return (currentTime >= startTime && currentTime <= endTime);
}

Promo Promo::GetPromotionById(int promoId)
{
    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return Promo(-1, "", "", 0.0, "", "", -1, -1);
    }

    // Prepare SQL statement for selecting a promotion by Id
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "SELECT * FROM Promos WHERE PromoId = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare select statement\n";
        sqlite3_close(db);
        return Promo(-1, "", "", 0.0, "", "", -1, -1);
    }

    // Bind parameter
    sqlite3_bind_int(statement, 1, promoId);

    // Execute statement
    if (sqlite3_step(statement) == SQLITE_ROW)
    {
        const unsigned char *name = sqlite3_column_text(statement, 1);
        const unsigned char *description = sqlite3_column_text(statement, 2);
        double discountPercentage = sqlite3_column_double(statement, 3);
        const unsigned char *startPeriod = sqlite3_column_text(statement, 4);
        const unsigned char *endPeriod = sqlite3_column_text(statement, 5);
        int productId = sqlite3_column_int(statement, 6);
        int employeeId = sqlite3_column_int(statement, 7);

        // Finalize statement and close database
        sqlite3_finalize(statement);
        sqlite3_close(db);

        return Promo(promoId, reinterpret_cast<const char *>(name), reinterpret_cast<const char *>(description),
                     discountPercentage, reinterpret_cast<const char *>(startPeriod), reinterpret_cast<const char *>(endPeriod), productId, employeeId);
    }

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    return Promo(-1, "", "", 0.0, "", "", -1, -1);
}

Promo Promo::GetPromotionByName(string promoName)
{
    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return Promo(-1, "", "", 0.0, "", "", -1, -1);
    }

    // Prepare SQL statement for selecting a promotion by name
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "SELECT * FROM Promos WHERE Name = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare select statement\n";
        sqlite3_close(db);
        return Promo(-1, "", "", 0.0, "", "", -1, -1);
    }

    // Bind parameter
    sqlite3_bind_text(statement, 1, promoName.c_str(), -1, SQLITE_STATIC);

    // Execute statement
    if (sqlite3_step(statement) == SQLITE_ROW)
    {
        int promoId = sqlite3_column_int(statement, 0);
        const unsigned char *name = sqlite3_column_text(statement, 1);
        const unsigned char *description = sqlite3_column_text(statement, 2);
        double discountPercentage = sqlite3_column_double(statement, 3);
        const unsigned char *startPeriod = sqlite3_column_text(statement, 4);
        const unsigned char *endPeriod = sqlite3_column_text(statement, 5);
        int productId = sqlite3_column_int(statement, 6);
        int employeeId = sqlite3_column_int(statement, 7);

        // Finalize statement and close database
        sqlite3_finalize(statement);
        sqlite3_close(db);

        return Promo(promoId, reinterpret_cast<const char *>(name), reinterpret_cast<const char *>(description),
                     discountPercentage, reinterpret_cast<const char *>(startPeriod), reinterpret_cast<const char *>(endPeriod), productId, employeeId);
    }

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    return Promo(-1, "", "", 0.0, "", "", -1, -1);
}

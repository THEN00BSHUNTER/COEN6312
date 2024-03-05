#include "Product.h"
#include <iostream>
#include <vector>
#include "sqlite\sqlite3.h"

Product::Product(int productId, const string &name, const string &description, double price, int subcategoryId, int supplierId)
    : ProductId(productId), Name(name), Description(description), Price(price), SubcategoryId(subcategoryId), SupplierId(supplierId) {}

void Product::AddProduct(const string &name, const string &description, double price, int subcategoryId, int supplierId)
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
    if (sqlite3_prepare_v2(db, "INSERT INTO Products (Name, Description, Price, SubcategoryId, SupplierId) VALUES (?, ?, ?, ?, ?)", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare insertion statement\n";
        sqlite3_close(db);
        return;
    }

    // Bind parameters
    sqlite3_bind_text(statement, 1, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 2, description.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(statement, 3, price);
    sqlite3_bind_int(statement, 4, subcategoryId);
    sqlite3_bind_int(statement, 5, supplierId);

    // Execute statement
    if (sqlite3_step(statement) != SQLITE_DONE)
    {
        cerr << "Failed to execute insertion statement\n";
        sqlite3_finalize(statement);
        sqlite3_close(db);
        return;
    }

    // Get the productId of the newly inserted product
    int productId = sqlite3_last_insert_rowid(db);

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    ProductId = productId;
    Name = name;
    Description = description;
    Price = price;
    SubcategoryId = subcategoryId;
    SupplierId = supplierId;

    cout << "Product added successfully\n";
}

void Product::UpdateProductDetails(int productId, const string &name, const string &description, double price, int subcategoryId, int supplierId)
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
    if (sqlite3_prepare_v2(db, "UPDATE Products SET Name = ?, Description = ?, Price = ?, SubcategoryId = ?, SupplierId = ? WHERE ProductId = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare update statement\n";
        sqlite3_close(db);
        return;
    }

    // Bind parameters
    sqlite3_bind_text(statement, 1, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 2, description.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(statement, 3, price);
    sqlite3_bind_int(statement, 4, subcategoryId);
    sqlite3_bind_int(statement, 5, supplierId);
    sqlite3_bind_int(statement, 6, productId);

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
    Price = price;
    SubcategoryId = subcategoryId;
    SupplierId = supplierId;

    cout << "Product details updated successfully\n";
}

void Product::DeleteProduct(int productId)
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
    if (sqlite3_prepare_v2(db, "DELETE FROM Products WHERE ProductId = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare deletion statement\n";
        sqlite3_close(db);
        return;
    }

    // Bind parameters
    sqlite3_bind_int(statement, 1, productId);

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

    cout << "Product deleted successfully\n";
}

int Product::GetProductId() const
{
    return ProductId;
}

double Product::GetPrice() const
{
    return Price;
}

string Product::GetName() const
{
    return Name;
}

vector<Product> Product::GetAllProducts()
{
    vector<Product> products;

    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return products;
    }

    // Prepare SQL statement for selecting all products
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "SELECT * FROM Products", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare select statement\n";
        sqlite3_close(db);
        return products;
    }

    // Execute statement
    while (sqlite3_step(statement) == SQLITE_ROW)
    {
        int productId = sqlite3_column_int(statement, 0);
        const unsigned char *name = sqlite3_column_text(statement, 1);
        const unsigned char *description = sqlite3_column_text(statement, 2);
        double price = sqlite3_column_double(statement, 3);
        int subcategoryId = sqlite3_column_int(statement, 4);
        int supplierId = sqlite3_column_int(statement, 5);

        products.emplace_back(productId, reinterpret_cast<const char *>(name), reinterpret_cast<const char *>(description), price, subcategoryId, supplierId);
    }

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    return products;
}

void Product::DisplayAllProducts(const vector<Product> &products)
{
    cout << "------------------------------------------------\n";
    cout << "All Products:\n";
    for (const auto &product : products)
    {
        product.DisplayProductDetails();
        cout << "------------------------------------------------\n";
    }
}

void Product::DisplayProductDetails() const
{
    cout << "Product Id: " << ProductId << endl;
    cout << "Name: " << Name << endl;
    cout << "Description: " << Description << endl;
    cout << "Price: " << Price << endl;
    cout << "Subcategory Id: " << SubcategoryId << endl;
    cout << "Supplier Id: " << SupplierId << endl;
}

#include "Product.h"
#include <iostream>
#include "sqlite\sqlite3.h"

Product Product::GetProductById(int productId)
{
    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return Product(-1, "", "", -1.0, -1, -1);
    }

    // Prepare SQL statement for selecting a product by Id
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "SELECT * FROM Products WHERE ProductId = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare select statement\n";
        sqlite3_close(db);
        return Product(-1, "", "", -1.0, -1, -1);
    }

    // Bind parameter
    sqlite3_bind_int(statement, 1, productId);

    // Execute statement
    if (sqlite3_step(statement) == SQLITE_ROW)
    {
        const unsigned char *name = sqlite3_column_text(statement, 1);
        const unsigned char *description = sqlite3_column_text(statement, 2);
        double price = sqlite3_column_double(statement, 3);
        int subcategoryId = sqlite3_column_int(statement, 4);
        int supplierId = sqlite3_column_int(statement, 5);

        // Finalize statement and close database
        sqlite3_finalize(statement);
        sqlite3_close(db);

        return Product(productId, reinterpret_cast<const char *>(name), reinterpret_cast<const char *>(description), price, subcategoryId, supplierId);
    }

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    return Product(-1, "", "", -1.0, -1, -1);
}

vector<Product> Product::GetProductsBySupplierId(int supplierId)
{
    vector<Product> products;

    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return products;
    }

    // Prepare SQL statement for selecting products by supplierId
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "SELECT * FROM Products WHERE SupplierId = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare select statement\n";
        sqlite3_close(db);
        return products;
    }

    // Bind parameter
    sqlite3_bind_int(statement, 1, supplierId);

    // Execute statement and fetch rows
    while (sqlite3_step(statement) == SQLITE_ROW)
    {
        int productId = sqlite3_column_int(statement, 0);
        const unsigned char *name = sqlite3_column_text(statement, 1);
        const unsigned char *description = sqlite3_column_text(statement, 2);
        double price = sqlite3_column_double(statement, 3);
        int subcategoryId = sqlite3_column_int(statement, 4);

        // Create Product object and add to products vector
        products.emplace_back(productId, reinterpret_cast<const char *>(name), reinterpret_cast<const char *>(description), price, subcategoryId, supplierId);
    }

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    return products;
}

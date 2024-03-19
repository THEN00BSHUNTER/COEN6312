#include "Supplier.h"
#include "User.h"
#include "Product.h"
#include "Inventory.h"
#include <iostream>
#include <vector>
#include "sqlite\sqlite3.h"

using namespace std;

Supplier::Supplier(int id, const string &username, const string &password, const string &email, const string &phone, const string &address, const string &role, const string &contactPerson, const string &productsSupplied, const string &paymentTerms)
    : User(id, username, password, email, phone, address, role), ContactPerson(contactPerson), ProductsSupplied(productsSupplied), PaymentTerms(paymentTerms) {}

Supplier *Supplier::RegisterSupplier(const string &username, const string &password, const string &email,
                                     const string &phone, const string &address,
                                     const string &contactPerson, const string &productsSupplied,
                                     const string &paymentTerms)
{
    // Check if username already exists
    if (!User::IsUsernameUnique(username))
    {
        return nullptr;
    }

    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return nullptr;
    }

    // Begin transaction
    if (sqlite3_exec(db, "BEGIN TRANSACTION", nullptr, nullptr, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to begin transaction\n";
        sqlite3_close(db);
        return nullptr;
    }

    // Prepare SQL statement for inserting user
    sqlite3_stmt *userStatement;
    if (sqlite3_prepare_v2(db, "INSERT INTO Users (Username, Password, Email, Phone, Address, Role) VALUES (?, ?, ?, ?, ?, ?)", -1, &userStatement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare user statement\n";
        sqlite3_close(db);
        return nullptr;
    }

    // Bind parameters for user insertion
    sqlite3_bind_text(userStatement, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(userStatement, 2, password.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(userStatement, 3, email.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(userStatement, 4, phone.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(userStatement, 5, address.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(userStatement, 6, "supplier", -1, SQLITE_STATIC);

    // Execute user insertion statement
    if (sqlite3_step(userStatement) != SQLITE_DONE)
    {
        cerr << "Failed to execute user statement\n";
        sqlite3_finalize(userStatement);
        sqlite3_close(db);
        return nullptr;
    }

    // Get the UserId of the newly inserted user
    int userId = sqlite3_last_insert_rowid(db);

    // Finalize user statement
    sqlite3_finalize(userStatement);

    // Prepare SQL statement for inserting supplier
    sqlite3_stmt *supplierStatement;
    if (sqlite3_prepare_v2(db, "INSERT INTO Suppliers (Id, ContactPerson, ProductsSupplied, PaymentTerms) VALUES (?, ?, ?, ?)", -1, &supplierStatement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare supplier statement\n";
        sqlite3_close(db);
        return nullptr;
    }

    // Bind parameters for supplier insertion
    sqlite3_bind_int(supplierStatement, 1, userId);
    sqlite3_bind_text(supplierStatement, 2, contactPerson.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(supplierStatement, 3, productsSupplied.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(supplierStatement, 4, paymentTerms.c_str(), -1, SQLITE_STATIC);

    // Execute supplier insertion statement
    if (sqlite3_step(supplierStatement) != SQLITE_DONE)
    {
        cerr << "Failed to execute supplier statement\n";
        sqlite3_finalize(supplierStatement);
        sqlite3_close(db);
        return nullptr;
    }

    // Get the last inserted row id (SupplierId)
    int newSupplierId = sqlite3_last_insert_rowid(db);

    // Finalize supplier statement
    sqlite3_finalize(supplierStatement);

    // Commit transaction
    if (sqlite3_exec(db, "COMMIT", nullptr, nullptr, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to commit transaction\n";
        sqlite3_close(db);
        return nullptr;
    }

    // Close database
    sqlite3_close(db);

    cout << "Supplier registered successfully with id: " << newSupplierId << "\n";
    // Return the newly created Supplier object
    return new Supplier(userId, username, password, email, phone, address, "supplier", contactPerson, productsSupplied, paymentTerms);
}

void Supplier::ViewInventory()
{
    Inventory::ViewInventory();
}

void Supplier::UpdateProductInformation()
{
    // Get products associated with the current supplier
    vector<Product> products = Product::GetProductsBySupplierId(Id);

    // Display the products
    cout << "Products:\n";
    for (size_t i = 0; i < products.size(); ++i)
    {
        cout << i + 1 << ". ";
        products[i].DisplayProductDetails();
    }

    // Prompt for product selection and updated details
    int selection;
    cout << "Select a product to update (enter the corresponding number): ";
    cin >> selection;
    if (selection < 1 || selection > static_cast<int>(products.size()))
    {
        cerr << "Invalid selection\n";
        return;
    }

    int productId = products[selection - 1].GetProductId();
    string name, description;
    double price;
    int subcategoryId;
    cout << "Enter new name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter new description: ";
    // cin.ignore();
    getline(cin, description);
    cout << "Enter new price: ";
    cin >> price;
    cout << "Enter new subcategory Id: ";
    cin >> subcategoryId;

    // Update product details
    products[selection - 1].UpdateProductDetails(productId, name, description, price, subcategoryId, Id);
}
#include <iostream>
#include <ctime>
#include <sstream>
#include <iomanip>

// for working with sqlite database
#include "sqlite\sqlite3.h"

#include "Database.h"

using namespace std;

// Function to convert string datetime to Unix time
time_t Database::ConvertToUnixTime(const string &datetimeStr)
{
    struct tm time;
    istringstream ss(datetimeStr);
    ss >> get_time(&time, "%Y-%m-%d %H:%M:%S");
    return mktime(&time);
}

// Function to convert Unix time to string datetime
string Database::ConvertDatetimeToString(const time_t &unixTime)
{
    struct tm *time;
    time = gmtime(&unixTime);
    ostringstream oss;
    oss << put_time(time, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

int Database::CreateDatabase()
{
    cout << "Please wait..." << endl;
    sqlite3 *db;
    char *errorMessage = 0;

    // Open the SQLite database
    int result = sqlite3_open("store_management.db", &db);
    if (result)
    {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return 1;
    }

    // Create all the tables
    const char *createTableSQL[] = {
        "CREATE TABLE IF NOT EXISTS Users (Id INTEGER PRIMARY KEY, Username TEXT UNIQUE, Password TEXT, Email TEXT, Phone TEXT, Address TEXT, Role TEXT)",

        "CREATE TABLE IF NOT EXISTS Suppliers (Id INTEGER UNIQUE, ContactPerson TEXT, ProductsSupplied TEXT, PaymentTerms TEXT, IsActive BOOLEAN, Phone TEXT, FOREIGN KEY(Id) REFERENCES Users(Id))",

        "CREATE TABLE IF NOT EXISTS Customers (Id INTEGER UNIQUE, ShippingAddress TEXT, PaymentMethod TEXT, OrderHistory TEXT, FOREIGN KEY(Id) REFERENCES Users(Id))",

        "CREATE TABLE IF NOT EXISTS Managers (Id INTEGER UNIQUE, Department TEXT, FOREIGN KEY(Id) REFERENCES Users(Id))",

        "CREATE TABLE IF NOT EXISTS Branches (BranchId INTEGER PRIMARY KEY, Location TEXT, ManagerId INTEGER, FOREIGN KEY(ManagerId) REFERENCES Managers(ManagerId))",

        "CREATE TABLE IF NOT EXISTS Employees (Id INTEGER UNIQUE, ManagerId INTEGER, BranchId INTEGER, Position TEXT, Tasks TEXT, FOREIGN KEY(Id) REFERENCES Users(Id), FOREIGN KEY(ManagerId) REFERENCES Managers(Id), FOREIGN KEY (BranchId) REFERENCES Branches(BranchId))",

        "CREATE TABLE IF NOT EXISTS Categories (CategoryId INTEGER PRIMARY KEY, Name TEXT, Description TEXT)",

        "CREATE TABLE IF NOT EXISTS Subcategories (SubcategoryId INTEGER PRIMARY KEY, Name TEXT, Description TEXT, CategoryId INTEGER, FOREIGN KEY(CategoryId) REFERENCES Categories(CategoryId))",

        "CREATE TABLE IF NOT EXISTS Products (ProductId INTEGER PRIMARY KEY, Name TEXT, Description TEXT, Price REAL, SubcategoryId INTEGER, SupplierId INTEGER, FOREIGN KEY(SubcategoryId) REFERENCES Subcategories(SubcategoryId), FOREIGN KEY(SupplierId) REFERENCES Suppliers(SupplierId))",

        "CREATE TABLE IF NOT EXISTS Transactions (TransactionId INTEGER PRIMARY KEY, Timestamp TEXT, Type TEXT, CustomerId INTEGER, BranchId INTEGER, TotalAmount REAL, FOREIGN KEY(CustomerId) REFERENCES Customers(CustomerId), FOREIGN KEY(BranchId) REFERENCES Branches(BranchId))",

        "CREATE TABLE IF NOT EXISTS Orders (OrderId INTEGER PRIMARY KEY, CustomerId INTEGER, EmployeeId INTEGER, ItemsOrdered TEXT, OrderDate TEXT, Status TEXT, FOREIGN KEY(CustomerId) REFERENCES Customers(CustomerId), FOREIGN KEY(EmployeeId) REFERENCES Employees(EmployeeId))",

        "CREATE TABLE IF NOT EXISTS OrderItems (OrderItemId INTEGER PRIMARY KEY, OrderId INTEGER, ProductId INTEGER, Quantity INTEGER, FOREIGN KEY(OrderId) REFERENCES Orders(OrderId), FOREIGN KEY(ProductId) REFERENCES Products(ProductId))",

        "CREATE TABLE IF NOT EXISTS Returns (ReturnId INTEGER PRIMARY KEY, CustomerId INTEGER, EmployeeId INTEGER, ItemsReturned TEXT, ReturnReason TEXT, ReturnDate TEXT, OrderId INTEGER, FOREIGN KEY(CustomerId) REFERENCES Customers(CustomerId), FOREIGN KEY(OrderId) REFERENCES Orders(OrderId), FOREIGN KEY(EmployeeId) REFERENCES Employees(EmployeeId))",

        "CREATE TABLE IF NOT EXISTS ReturnItems (ReturnItemId INTEGER PRIMARY KEY, ReturnId INTEGER, ProductId INTEGER, Quantity INTEGER, FOREIGN KEY(ReturnId) REFERENCES Returns(ReturnId), FOREIGN KEY(ProductId) REFERENCES Products(ProductId))",

        "CREATE TABLE IF NOT EXISTS Warehouses (WarehouseId INTEGER PRIMARY KEY, Location TEXT, ManagerId INTEGER, FOREIGN KEY(ManagerId) REFERENCES Managers(ManagerId))",

        "CREATE TABLE IF NOT EXISTS Inventories (InventoryId INTEGER PRIMARY KEY, ProductId INTEGER, WarehouseId INTEGER, ProductAvailability INTEGER, ReorderQuantity INTEGER, FOREIGN KEY(ProductId) REFERENCES Products(ProductId), FOREIGN KEY(WarehouseId) REFERENCES Warehouses(WarehouseId))",

        "CREATE TABLE IF NOT EXISTS Promos (PromoId INTEGER PRIMARY KEY, Name TEXT, Description TEXT, DiscountPercentage REAL, StartPeriod TEXT, EndPeriod TEXT, ProductId INTEGER, FOREIGN KEY(ProductId) REFERENCES Products(ProductId))",

        "CREATE TABLE IF NOT EXISTS Discounts (DiscountId INTEGER PRIMARY KEY, Name TEXT, Description TEXT, StartPeriod TEXT, EndPeriod TEXT, ProductId INTEGER, Percentage REAL, FOREIGN KEY(ProductId) REFERENCES Products(ProductId))"};

    for (const auto &sql : createTableSQL)
    {
        result = sqlite3_exec(db, sql, 0, 0, &errorMessage);
        if (result != SQLITE_OK)
        {
            cerr << "SQL error: " << errorMessage << endl;
            sqlite3_free(errorMessage);
            sqlite3_close(db);
            return 1;
        }
    }

    // Close the database connection
    sqlite3_close(db);
    cout << "Database and the tables created successfully\n";
    return 0;
}

int Database::SampleData()
{
    cout << "Please wait..." << endl;
    sqlite3 *db;
    char *errorMessage = 0;

    // Open the SQLite database
    int result = sqlite3_open("store_management.db", &db);
    if (result)
    {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return 1;
    }

    // Insert sample data to the tables
    const char *insertDataSQL[] = {

        // Sample data for Users table
        "INSERT OR IGNORE INTO Users (Username, Password, Email, Phone, Address, Role) VALUES ('john', '123', 'john@example.com', '123456789', '123 Main St', 'customer')",
        "INSERT OR IGNORE INTO Users (Username, Password, Email, Phone, Address, Role) VALUES ('chris', '123', 'chris@example.com', '555666777', '303 Elm St', 'customer')",
        "INSERT OR IGNORE INTO Users (Username, Password, Email, Phone, Address, Role) VALUES ('susan', '123', 'susan@example.com', '111222333', '789 Maple St', 'customer')",

        "INSERT OR IGNORE INTO Users (Username, Password, Email, Phone, Address, Role) VALUES ('jane', '123', 'jane@example.com', '987654321', '456 Elm St', 'manager')",
        "INSERT OR IGNORE INTO Users (Username, Password, Email, Phone, Address, Role) VALUES ('kevin', '123', 'kevin@example.com', '444555666', '101 Pine St', 'manager')",

        "INSERT OR IGNORE INTO Users (Username, Password, Email, Phone, Address, Role) VALUES ('mike', '123', 'mike@example.com', '456123789', '789 Oak St', 'employee')",
        "INSERT OR IGNORE INTO Users (Username, Password, Email, Phone, Address, Role) VALUES ('emily', '123', 'emily@example.com', '777888999', '202 Oak St', 'employee')",

        "INSERT OR IGNORE INTO Users (Username, Password, Email, Phone, Address, Role) VALUES ('supplier1', '123', 'supplier1@example.com', '123456789', '123 Supplier St', 'supplier')",
        "INSERT OR IGNORE INTO Users (Username, Password, Email, Phone, Address, Role) VALUES ('supplier2', '123', 'supplier2@example.com', '987654321', '456 Supplier Ave', 'supplier')",

        // Sample data for Customsers table
        "INSERT OR IGNORE INTO Customers (Id, ShippingAddress, PaymentMethod, OrderHistory) VALUES (1, '123 Shipping St', 'Credit Card', '')",
        "INSERT OR IGNORE INTO Customers (Id, ShippingAddress, PaymentMethod, OrderHistory) VALUES (2, '456 Shipping St', 'PayPal', '')",
        "INSERT OR IGNORE INTO Customers (Id, ShippingAddress, PaymentMethod, OrderHistory) VALUES (3, '789 Shipping St', 'Cash on Delivery', '')",

        // Sample data for Managers table
        "INSERT OR IGNORE INTO Managers (Id, Department) VALUES (4, 'Sales')",
        "INSERT OR IGNORE INTO Managers (Id, Department) VALUES (5, 'Inventory')",

        // Sample data for Branches table
        "INSERT OR IGNORE INTO Branches (Location, ManagerId) VALUES ('New York', 4)",
        "INSERT OR IGNORE INTO Branches (Location, ManagerId) VALUES ('Los Angeles', 5)",

        // Sample data for Employees table
        "INSERT OR IGNORE INTO Employees (Id, BranchId, ManagerId, Position, Tasks) VALUES (6, 1, 4, 'Sales Associate', 'Assist customers with purchases')",
        "INSERT OR IGNORE INTO Employees (Id, BranchId, ManagerId, Position, Tasks) VALUES (7, 2, 5, 'Warehouse Associate', 'Manage inventory stock levels')",

        // Sample data for Suppliers table
        "INSERT OR IGNORE INTO Suppliers (Id, ContactPerson, ProductsSupplied, PaymentTerms, IsActive, Phone) VALUES (8, 'Supplier A', 'Product A, Product B', 'Net 30', 1, '123-456-7890')",
        "INSERT OR IGNORE INTO Suppliers (Id, ContactPerson, ProductsSupplied, PaymentTerms, IsActive, Phone) VALUES (9, 'Supplier B', 'Product C, Product D', 'Net 45', 1, '987-654-3210')",

        // Sample data for Categoires table
        "INSERT OR IGNORE INTO Categories (Name, Description) VALUES ('Electronics', 'Electronic devices and accessories')",
        "INSERT OR IGNORE INTO Categories (Name, Description) VALUES ('Clothing', 'Apparel and fashion items')",
        "INSERT OR IGNORE INTO Categories (Name, Description) VALUES ('Books', 'Books and literature')",

        // Sample data for subcategoies table
        "INSERT OR IGNORE INTO Subcategories (Name, Description, CategoryId) VALUES ('Smartphones', 'Mobile phones with advanced features', 1)",
        "INSERT OR IGNORE INTO Subcategories (Name, Description, CategoryId) VALUES ('Laptops', 'Portable computers suitable for various tasks.', 1)",
        "INSERT OR IGNORE INTO Subcategories (Name, Description, CategoryId) VALUES ('Desktops', 'Stationary computers suitable for heavy-duty tasks.', 1)",
        "INSERT OR IGNORE INTO Subcategories (Name, Description, CategoryId) VALUES ('Shirts', 'Casual and formal shirts', 2)",
        "INSERT OR IGNORE INTO Subcategories (Name, Description, CategoryId) VALUES ('Smartwatches', 'Wearable devices with various functionalities', 1)",
        "INSERT OR IGNORE INTO Subcategories (Name, Description, CategoryId) VALUES ('Tablets', 'Portable touchscreen computers larger than smartphones.', 1)",
        "INSERT OR IGNORE INTO Subcategories (Name, Description, CategoryId) VALUES ('Fiction', 'Books with fictional stories', 3)",
        "INSERT OR IGNORE INTO Subcategories (Name, Description, CategoryId) VALUES ('Headphones', 'Audio devices worn over the ears for listening to sound.', 1)",
        "INSERT OR IGNORE INTO Subcategories (Name, Description, CategoryId) VALUES ('Speakers', 'Audio output devices for projecting sound into a room.', 1)",
        "INSERT OR IGNORE INTO Subcategories (Name, Description, CategoryId) VALUES ('Running Shoes', 'Footwear designed specifically for running', 2)",
        "INSERT OR IGNORE INTO Subcategories (Name, Description, CategoryId) VALUES ('Casual Shoes', 'Comfortable and stylish shoes for everyday wear', 2)",
        "INSERT OR IGNORE INTO Subcategories (Name, Description, CategoryId) VALUES ('Sandals', 'Open-toed shoes with straps or buckles', 2)",
        "INSERT OR IGNORE INTO Subcategories (Name, Description, CategoryId) VALUES ('Cameras', 'Devices for capturing photographs or videos', 1)",
        "INSERT OR IGNORE INTO Subcategories (Name, Description, CategoryId) VALUES ('Gaming Consoles', 'Devices for playing video games', 1)",
        "INSERT OR IGNORE INTO Subcategories (Name, Description, CategoryId) VALUES ('Smart Home Devices', 'Connected devices for home automation', 1)",

        // Sample data for Products table
        "INSERT OR IGNORE INTO Products (Name, Description, Price, SubcategoryId, SupplierId) VALUES ('iPhone 12', 'Latest smartphone model from Apple', 999.99, 1, 1)",
        "INSERT OR IGNORE INTO Products (Name, Description, Price, SubcategoryId, SupplierId) VALUES ('MacBook Pro', 'High-performance laptop by Apple', 1499.99, 2, 2)",
        "INSERT OR IGNORE INTO Products (Name, Description, Price, SubcategoryId, SupplierId) VALUES ('Dell XPS 15', 'Powerful laptop for professionals', 1299.99, 2, 1)",
        "INSERT OR IGNORE INTO Products (Name, Description, Price, SubcategoryId, SupplierId) VALUES ('Samsung Galaxy S21', 'Flagship smartphone from Samsung', 899.99, 1, 2)",
        "INSERT OR IGNORE INTO Products (Name, Description, Price, SubcategoryId, SupplierId) VALUES ('HP Pavilion Desktop', 'Reliable desktop computer for home and office use', 699.99, 3, 1)",
        "INSERT OR IGNORE INTO Products (Name, Description, Price, SubcategoryId, SupplierId) VALUES ('ASUS ROG Strix', 'Gaming laptop with high-performance components', 1799.99, 2, 2)",
        "INSERT OR IGNORE INTO Products (Name, Description, Price, SubcategoryId, SupplierId) VALUES ('Nike Dri-FIT T-Shirt', 'Men''s athletic shirt with moisture-wicking fabric', 34.99, 4, 1)",
        "INSERT OR IGNORE INTO Products (Name, Description, Price, SubcategoryId, SupplierId) VALUES ('Adidas Ultraboost Shoes', 'Premium running shoes with responsive cushioning', 129.99, 5, 1)",
        "INSERT OR IGNORE INTO Products (Name, Description, Price, SubcategoryId, SupplierId) VALUES ('Levi''s 501 Jeans', 'Classic men''s denim jeans with a straight fit', 59.99, 4, 2)",
        "INSERT OR IGNORE INTO Products (Name, Description, Price, SubcategoryId, SupplierId) VALUES ('Ray-Ban Aviator Sunglasses', 'Iconic aviator sunglasses with polarized lenses', 149.99, 6, 1)",
        "INSERT OR IGNORE INTO Products (Name, Description, Price, SubcategoryId, SupplierId) VALUES ('Sony WH-1000XM4', 'Wireless noise-canceling headphones with premium sound quality', 349.99, 8, 2)",
        "INSERT OR IGNORE INTO Products (Name, Description, Price, SubcategoryId, SupplierId) VALUES ('Bose SoundLink Revolve', 'Portable Bluetooth speaker with 360-degree sound', 199.99, 9, 1)",
        "INSERT OR IGNORE INTO Products (Name, Description, Price, SubcategoryId, SupplierId) VALUES ('JBL Flip 5', 'Waterproof portable speaker with powerful bass', 119.99, 9, 2)",
        "INSERT OR IGNORE INTO Products (Name, Description, Price, SubcategoryId, SupplierId) VALUES ('The Great Gatsby', 'Classic novel by F. Scott Fitzgerald', 9.99, 6, 1)",
        "INSERT OR IGNORE INTO Products (Name, Description, Price, SubcategoryId, SupplierId) VALUES ('To Kill a Mockingbird', 'Seminal novel by Harper Lee', 8.99, 6, 2)",
        "INSERT OR IGNORE INTO Products (Name, Description, Price, SubcategoryId, SupplierId) VALUES ('The Catcher in the Rye', 'Modern classic by J.D. Salinger', 10.99, 6, 1)",
        "INSERT OR IGNORE INTO Products (Name, Description, Price, SubcategoryId, SupplierId) VALUES ('Nike Air Zoom Pegasus', 'Versatile running shoes for everyday training', 119.99, 5, 2)",
        "INSERT OR IGNORE INTO Products (Name, Description, Price, SubcategoryId, SupplierId) VALUES ('Apple AirPods Pro', 'Wireless earbuds with active noise cancellation', 249.99, 1, 1)",
        "INSERT OR IGNORE INTO Products (Name, Description, Price, SubcategoryId, SupplierId) VALUES ('Samsung Galaxy Tab S7', 'Premium Android tablet with S Pen support', 649.99, 3, 2)",
        "INSERT OR IGNORE INTO Products (Name, Description, Price, SubcategoryId, SupplierId) VALUES ('ASUS ZenBook 14', 'Ultra-thin and light laptop for on-the-go productivity', 999.99, 2, 2)",
        "INSERT OR IGNORE INTO Products (Name, Description, Price, SubcategoryId, SupplierId) VALUES ('Microsoft Surface Pro 7', 'Versatile 2-in-1 laptop with touch and pen support', 899.99, 2, 1)",
        "INSERT OR IGNORE INTO Products (Name, Description, Price, SubcategoryId, SupplierId) VALUES ('Canon EOS Rebel T7', 'Entry-level DSLR camera for beginners', 499.99, 4, 1)",
        "INSERT OR IGNORE INTO Products (Name, Description, Price, SubcategoryId, SupplierId) VALUES ('Sony PlayStation 5', 'Next-gen gaming console with blazing-fast SSD', 499.99, 5, 2)",
        "INSERT OR IGNORE INTO Products (Name, Description, Price, SubcategoryId, SupplierId) VALUES ('Nintendo Switch', 'Hybrid gaming console for home and on-the-go play', 299.99, 5, 2)",

        // Sample data for Orders table
        "INSERT OR IGNORE INTO Orders (CustomerId, EmployeeId, ItemsOrdered, OrderDate, Status) VALUES (1, 1, '1,3,5', '2024-02-16', 'Pending')",
        "INSERT OR IGNORE INTO Orders (CustomerId, EmployeeId, ItemsOrdered, OrderDate, Status) VALUES (2, 2, '2,4,6', '2024-02-16', 'Processing')",
        "INSERT OR IGNORE INTO Orders (CustomerId, EmployeeId, ItemsOrdered, OrderDate, Status) VALUES (3, 1, '7,9,11', '2024-02-16', 'Shipped')",
        "INSERT OR IGNORE INTO Orders (CustomerId, EmployeeId, ItemsOrdered, OrderDate, Status) VALUES (1, 2, '8,10,12', '2024-02-16', 'Delivered')",
        "INSERT OR IGNORE INTO Orders (CustomerId, EmployeeId, ItemsOrdered, OrderDate, Status) VALUES (2, 1, '13,15,17', '2024-02-16', 'Pending')",
        "INSERT OR IGNORE INTO Orders (CustomerId, EmployeeId, ItemsOrdered, OrderDate, Status) VALUES (3, 2, '14,16,18', '2024-02-16', 'Processing')",

        // Sample data for OrderItems table
        "INSERT OR IGNORE INTO OrderItems (OrderId, ProductId, Quantity) VALUES (1, 1, 2)",
        "INSERT OR IGNORE INTO OrderItems (OrderId, ProductId, Quantity) VALUES (1, 3, 1)",
        "INSERT OR IGNORE INTO OrderItems (OrderId, ProductId, Quantity) VALUES (1, 5, 3)",
        "INSERT OR IGNORE INTO OrderItems (OrderId, ProductId, Quantity) VALUES (2, 2, 1)",
        "INSERT OR IGNORE INTO OrderItems (OrderId, ProductId, Quantity) VALUES (2, 4, 2)",
        "INSERT OR IGNORE INTO OrderItems (OrderId, ProductId, Quantity) VALUES (2, 6, 1)",
        "INSERT OR IGNORE INTO OrderItems (OrderId, ProductId, Quantity) VALUES (3, 7, 1)",
        "INSERT OR IGNORE INTO OrderItems (OrderId, ProductId, Quantity) VALUES (3, 9, 3)",
        "INSERT OR IGNORE INTO OrderItems (OrderId, ProductId, Quantity) VALUES (3, 11, 2)",
        "INSERT OR IGNORE INTO OrderItems (OrderId, ProductId, Quantity) VALUES (4, 8, 2)",
        "INSERT OR IGNORE INTO OrderItems (OrderId, ProductId, Quantity) VALUES (4, 10, 1)",
        "INSERT OR IGNORE INTO OrderItems (OrderId, ProductId, Quantity) VALUES (4, 12, 3)",
        "INSERT OR IGNORE INTO OrderItems (OrderId, ProductId, Quantity) VALUES (5, 13, 2)",
        "INSERT OR IGNORE INTO OrderItems (OrderId, ProductId, Quantity) VALUES (5, 15, 1)",
        "INSERT OR IGNORE INTO OrderItems (OrderId, ProductId, Quantity) VALUES (5, 17, 4)",
        "INSERT OR IGNORE INTO OrderItems (OrderId, ProductId, Quantity) VALUES (6, 14, 1)",
        "INSERT OR IGNORE INTO OrderItems (OrderId, ProductId, Quantity) VALUES (6, 16, 2)",
        "INSERT OR IGNORE INTO OrderItems (OrderId, ProductId, Quantity) VALUES (6, 18, 2)",

        // Sample data for Transactions table
        "INSERT OR IGNORE INTO Transactions (Timestamp, Type, CustomerId, BranchId, TotalAmount) VALUES ('2024-02-16 09:00:00', 'Sale', 1, 1, 999.99)",
        "INSERT OR IGNORE INTO Transactions (Timestamp, Type, CustomerId, BranchId, TotalAmount) VALUES ('2024-02-16 10:30:00', 'Sale', 2, 2, 29.99)",
        "INSERT OR IGNORE INTO Transactions (Timestamp, Type, CustomerId, BranchId, TotalAmount) VALUES ('2024-02-16 11:45:00', 'Sale', 2, 1, 12.99)",

        // Sample data for Warehouses table
        "INSERT OR IGNORE INTO Warehouses (Location, ManagerId) VALUES ('Warehouse A', 4)",
        "INSERT OR IGNORE INTO Warehouses (Location, ManagerId) VALUES ('Warehouse B', 5)",

        // Sample data for Inventories table
        "INSERT OR IGNORE INTO Inventories (ProductId, WarehouseId, ProductAvailability, ReorderQuantity) VALUES (1, 1, 100, 20)",
        "INSERT OR IGNORE INTO Inventories (ProductId, WarehouseId, ProductAvailability, ReorderQuantity) VALUES (2, 1, 150, 30)",
        "INSERT OR IGNORE INTO Inventories (ProductId, WarehouseId, ProductAvailability, ReorderQuantity) VALUES (3, 2, 200, 40)",
        "INSERT OR IGNORE INTO Inventories (ProductId, WarehouseId, ProductAvailability, ReorderQuantity) VALUES (4, 2, 120, 25)",
        "INSERT OR IGNORE INTO Inventories (ProductId, WarehouseId, ProductAvailability, ReorderQuantity) VALUES (5, 1, 80, 15)",
        "INSERT OR IGNORE INTO Inventories (ProductId, WarehouseId, ProductAvailability, ReorderQuantity) VALUES (6, 2, 180, 35)",
        "INSERT OR IGNORE INTO Inventories (ProductId, WarehouseId, ProductAvailability, ReorderQuantity) VALUES (7, 2, 90, 18)",
        "INSERT OR IGNORE INTO Inventories (ProductId, WarehouseId, ProductAvailability, ReorderQuantity) VALUES (8, 1, 110, 22)",
        "INSERT OR IGNORE INTO Inventories (ProductId, WarehouseId, ProductAvailability, ReorderQuantity) VALUES (9, 2, 160, 28)",
        "INSERT OR IGNORE INTO Inventories (ProductId, WarehouseId, ProductAvailability, ReorderQuantity) VALUES (10, 2, 140, 26)",
        "INSERT OR IGNORE INTO Inventories (ProductId, WarehouseId, ProductAvailability, ReorderQuantity) VALUES (11, 1, 100, 20)",
        "INSERT OR IGNORE INTO Inventories (ProductId, WarehouseId, ProductAvailability, ReorderQuantity) VALUES (12, 1, 150, 30)",
        "INSERT OR IGNORE INTO Inventories (ProductId, WarehouseId, ProductAvailability, ReorderQuantity) VALUES (13, 2, 200, 40)",
        "INSERT OR IGNORE INTO Inventories (ProductId, WarehouseId, ProductAvailability, ReorderQuantity) VALUES (14, 2, 120, 25)",
        "INSERT OR IGNORE INTO Inventories (ProductId, WarehouseId, ProductAvailability, ReorderQuantity) VALUES (15, 1, 80, 15)",
        "INSERT OR IGNORE INTO Inventories (ProductId, WarehouseId, ProductAvailability, ReorderQuantity) VALUES (16, 2, 180, 35)",
        "INSERT OR IGNORE INTO Inventories (ProductId, WarehouseId, ProductAvailability, ReorderQuantity) VALUES (17, 2, 90, 18)",
        "INSERT OR IGNORE INTO Inventories (ProductId, WarehouseId, ProductAvailability, ReorderQuantity) VALUES (18, 2, 110, 22)",

        // Sample data for Returns table
        "INSERT OR IGNORE INTO Returns (CustomerId, EmployeeId, ItemsReturned, ReturnReason, ReturnDate, OrderId) VALUES (1, 6, '1,3,5', 'Wrong size', '2024-02-17', 1)",
        "INSERT OR IGNORE INTO Returns (CustomerId, EmployeeId, ItemsReturned, ReturnReason, ReturnDate, OrderId) VALUES (2, 7, '2,4,6', 'Defective product', '2024-02-18', 2)",
        "INSERT OR IGNORE INTO Returns (CustomerId, EmployeeId, ItemsReturned, ReturnReason, ReturnDate, OrderId) VALUES (3, 6, '7,9,11', 'Changed mind', '2024-02-19', 3)",

        // Sample data for ReturnItems table
        "INSERT OR IGNORE INTO ReturnItems (ReturnId, ProductId, Quantity) VALUES (1, 1, 1)",
        "INSERT OR IGNORE INTO ReturnItems (ReturnId, ProductId, Quantity) VALUES (1, 3, 1)",
        "INSERT OR IGNORE INTO ReturnItems (ReturnId, ProductId, Quantity) VALUES (1, 5, 2)",
        "INSERT OR IGNORE INTO ReturnItems (ReturnId, ProductId, Quantity) VALUES (2, 2, 1)",
        "INSERT OR IGNORE INTO ReturnItems (ReturnId, ProductId, Quantity) VALUES (2, 4, 1)",
        "INSERT OR IGNORE INTO ReturnItems (ReturnId, ProductId, Quantity) VALUES (2, 6, 1)",
        "INSERT OR IGNORE INTO ReturnItems (ReturnId, ProductId, Quantity) VALUES (3, 7, 1)",
        "INSERT OR IGNORE INTO ReturnItems (ReturnId, ProductId, Quantity) VALUES (3, 9, 2)",
        "INSERT OR IGNORE INTO ReturnItems (ReturnId, ProductId, Quantity) VALUES (3, 11, 1)",

        // Sample data for Promos table
        "INSERT OR IGNORE INTO Promos (Name, Description, DiscountPercentage, StartPeriod, EndPeriod, ProductId) VALUES ('Summer Sale', 'Get discounts on selected items during the summer season.', 15.0, '2024-06-01 00:00:00', '2024-08-31 00:00:00', 1)",
        "INSERT OR IGNORE INTO Promos (Name, Description, DiscountPercentage, StartPeriod, EndPeriod, ProductId) VALUES ('Back to School Promo', 'Special discounts for students going back to school.', 10.0, '2024-08-15 00:00:00', '2024-09-15 00:00:00', 3)",
        "INSERT OR IGNORE INTO Promos (Name, Description, DiscountPercentage, StartPeriod, EndPeriod, ProductId) VALUES ('Holiday Season Deal', 'Enjoy holiday discounts on various products.', 20.0, '2024-12-01 00:00:00', '2025-01-01 00:00:00', 5)",

        // Sample data for Discounts table
        "INSERT OR IGNORE INTO Discounts (Name, Description, StartPeriod, EndPeriod, ProductId, Percentage) VALUES ('Clearance Sale', 'Limited time clearance sale on old inventory.', '2024-10-01 00:00:00', '2024-10-31 00:00:00', 2, 10.0)",
        "INSERT OR IGNORE INTO Discounts (Name, Description, StartPeriod, EndPeriod, ProductId, Percentage) VALUES ('Flash Sale', 'Hurry up! Limited time offer on selected items.', '2024-07-15 00:00:00','2024-07-17 00:00:00', 4, 15.0)",
        "INSERT OR IGNORE INTO Discounts (Name, Description, StartPeriod, EndPeriod, ProductId, Percentage) VALUES ('Weekend Special', 'Special discounts for the weekend.', '2024-09-20 00:00:00', '2024-09-22 00:00:00', 6, 20.0)"};

    for (const auto &sql : insertDataSQL)
    {
        result = sqlite3_exec(db, sql, 0, 0, &errorMessage);
        if (result != SQLITE_OK)
        {
            cerr << "SQL error: " << errorMessage << endl;
            sqlite3_free(errorMessage);
            sqlite3_close(db);
            return 1;
        }
    }

    // Close the database connection
    sqlite3_close(db);
    cout << "Sample data inserted to the tables successfully\n";
    return 0;
}

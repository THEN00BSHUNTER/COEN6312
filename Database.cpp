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

        "CREATE TABLE IF NOT EXISTS Orders (OrderId INTEGER PRIMARY KEY, CustomerId INTEGER, EmployeeId INTEGER, ItemsOrdered TEXT, OrderDate TEXT, Status TEXT, DiscountedPrice REAL, FOREIGN KEY(CustomerId) REFERENCES Customers(CustomerId), FOREIGN KEY(EmployeeId) REFERENCES Employees(EmployeeId))",

        "CREATE TABLE IF NOT EXISTS OrderItems (OrderItemId INTEGER PRIMARY KEY, OrderId INTEGER, ProductId INTEGER, Quantity INTEGER, FOREIGN KEY(OrderId) REFERENCES Orders(OrderId), FOREIGN KEY(ProductId) REFERENCES Products(ProductId))",

        "CREATE TABLE IF NOT EXISTS Returns (ReturnId INTEGER PRIMARY KEY, CustomerId INTEGER, EmployeeId INTEGER, ItemsReturned TEXT, ReturnReason TEXT, ReturnDate TEXT, OrderId INTEGER, FOREIGN KEY(CustomerId) REFERENCES Customers(CustomerId), FOREIGN KEY(OrderId) REFERENCES Orders(OrderId), FOREIGN KEY(EmployeeId) REFERENCES Employees(EmployeeId))",

        "CREATE TABLE IF NOT EXISTS ReturnItems (ReturnItemId INTEGER PRIMARY KEY, ReturnId INTEGER, ProductId INTEGER, Quantity INTEGER, FOREIGN KEY(ReturnId) REFERENCES Returns(ReturnId), FOREIGN KEY(ProductId) REFERENCES Products(ProductId))",

        "CREATE TABLE IF NOT EXISTS Warehouses (WarehouseId INTEGER PRIMARY KEY, Location TEXT, ManagerId INTEGER, FOREIGN KEY(ManagerId) REFERENCES Managers(ManagerId))",

        "CREATE TABLE IF NOT EXISTS Inventories (InventoryId INTEGER PRIMARY KEY, ProductId INTEGER, WarehouseId INTEGER, ProductAvailability INTEGER, ReorderThreshold INTEGER, FOREIGN KEY(ProductId) REFERENCES Products(ProductId), FOREIGN KEY(WarehouseId) REFERENCES Warehouses(WarehouseId))",

        "CREATE TABLE IF NOT EXISTS Promos (PromoId INTEGER PRIMARY KEY, Name TEXT, Description TEXT, DiscountPercentage REAL, StartPeriod TEXT, EndPeriod TEXT, ProductId INTEGER, EmployeeId INTEGER, FOREIGN KEY(ProductId) REFERENCES Products(ProductId), FOREIGN KEY(EmployeeId) REFERENCES Employees(EmployeeId))",

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
        "INSERT OR IGNORE INTO Users (Id, Username, Password, Email, Phone, Address, Role) VALUES (1, 'john', '123', 'john@example.com', '123456789', '123 Main St', 'customer')",
        "INSERT OR IGNORE INTO Users (Id, Username, Password, Email, Phone, Address, Role) VALUES (2, 'chris', '123', 'chris@example.com', '555666777', '303 Elm St', 'customer')",
        "INSERT OR IGNORE INTO Users (Id, Username, Password, Email, Phone, Address, Role) VALUES (3, 'susan', '123', 'susan@example.com', '111222333', '789 Maple St', 'customer')",

        "INSERT OR IGNORE INTO Users (Id, Username, Password, Email, Phone, Address, Role) VALUES (4, 'jane', '123', 'jane@example.com', '987654321', '456 Elm St', 'manager')",
        "INSERT OR IGNORE INTO Users (Id, Username, Password, Email, Phone, Address, Role) VALUES (5, 'kevin', '123', 'kevin@example.com', '444555666', '101 Pine St', 'manager')",

        "INSERT OR IGNORE INTO Users (Id, Username, Password, Email, Phone, Address, Role) VALUES (6, 'mike', '123', 'mike@example.com', '456123789', '789 Oak St', 'employee')",
        "INSERT OR IGNORE INTO Users (Id, Username, Password, Email, Phone, Address, Role) VALUES (7, 'emily', '123', 'emily@example.com', '777888999', '202 Oak St', 'employee')",

        "INSERT OR IGNORE INTO Users (Id, Username, Password, Email, Phone, Address, Role) VALUES (8, 'supplier1', '123', 'supplier1@example.com', '123456789', '123 Supplier St', 'supplier')",
        "INSERT OR IGNORE INTO Users (Id, Username, Password, Email, Phone, Address, Role) VALUES (9, 'supplier2', '123', 'supplier2@example.com', '987654321', '456 Supplier Ave', 'supplier')",

        // Sample data for Customsers table
        "INSERT OR IGNORE INTO Customers (Id, ShippingAddress, PaymentMethod, OrderHistory) VALUES (1, '123 Shipping St', 'Credit Card', '')",
        "INSERT OR IGNORE INTO Customers (Id, ShippingAddress, PaymentMethod, OrderHistory) VALUES (2, '456 Shipping St', 'PayPal', '')",
        "INSERT OR IGNORE INTO Customers (Id, ShippingAddress, PaymentMethod, OrderHistory) VALUES (3, '789 Shipping St', 'Cash on Delivery', '')",

        // Sample data for Managers table
        "INSERT OR IGNORE INTO Managers (Id, Department) VALUES (4, 'Sales')",
        "INSERT OR IGNORE INTO Managers (Id, Department) VALUES (5, 'Inventory')",

        // Sample data for Branches table
        "INSERT OR IGNORE INTO Branches (BranchId, Location, ManagerId) VALUES (1, 'New York', 4)",
        "INSERT OR IGNORE INTO Branches (BranchId, Location, ManagerId) VALUES (2, 'Los Angeles', 5)",

        // Sample data for Employees table
        "INSERT OR IGNORE INTO Employees (Id, BranchId, ManagerId, Position, Tasks) VALUES (6, 1, 4, 'Sales Associate', 'Assist customers with purchases')",
        "INSERT OR IGNORE INTO Employees (Id, BranchId, ManagerId, Position, Tasks) VALUES (7, 2, 5, 'Warehouse Associate', 'Manage inventory stock levels')",

        // Sample data for Suppliers table
        "INSERT OR IGNORE INTO Suppliers (Id, ContactPerson, ProductsSupplied, PaymentTerms, IsActive, Phone) VALUES (8, 'Supplier A', 'Product A, Product B', 'Net 30', 1, '123-456-7890')",
        "INSERT OR IGNORE INTO Suppliers (Id, ContactPerson, ProductsSupplied, PaymentTerms, IsActive, Phone) VALUES (9, 'Supplier B', 'Product C, Product D', 'Net 45', 1, '987-654-3210')",

        // Sample data for Categoires table
        "INSERT OR IGNORE INTO Categories (CategoryId, Name, Description) VALUES (1, 'Electronics', 'Electronic devices and accessories')",
        "INSERT OR IGNORE INTO Categories (CategoryId, Name, Description) VALUES (2, 'Clothing', 'Apparel and fashion items')",
        "INSERT OR IGNORE INTO Categories (CategoryId, Name, Description) VALUES (3, 'Books', 'Books and literature')",

        // Sample data for subcategoies table
        "INSERT OR IGNORE INTO Subcategories (SubcategoryId, Name, Description, CategoryId) VALUES (1, 'Smartphones', 'Mobile phones with advanced features', 1)",
        "INSERT OR IGNORE INTO Subcategories (SubcategoryId, Name, Description, CategoryId) VALUES (2, 'Laptops', 'Portable computers suitable for various tasks.', 1)",
        "INSERT OR IGNORE INTO Subcategories (SubcategoryId, Name, Description, CategoryId) VALUES (3, 'Desktops', 'Stationary computers suitable for heavy-duty tasks.', 1)",
        "INSERT OR IGNORE INTO Subcategories (SubcategoryId, Name, Description, CategoryId) VALUES (4, 'Shirts', 'Casual and formal shirts', 2)",
        "INSERT OR IGNORE INTO Subcategories (SubcategoryId, Name, Description, CategoryId) VALUES (5, 'Smartwatches', 'Wearable devices with various functionalities', 1)",
        "INSERT OR IGNORE INTO Subcategories (SubcategoryId, Name, Description, CategoryId) VALUES (6, 'Tablets', 'Portable touchscreen computers larger than smartphones.', 1)",
        "INSERT OR IGNORE INTO Subcategories (SubcategoryId, Name, Description, CategoryId) VALUES (7, 'Fiction', 'Books with fictional stories', 3)",
        "INSERT OR IGNORE INTO Subcategories (SubcategoryId, Name, Description, CategoryId) VALUES (8, 'Headphones', 'Audio devices worn over the ears for listening to sound.', 1)",
        "INSERT OR IGNORE INTO Subcategories (SubcategoryId, Name, Description, CategoryId) VALUES (9, 'Speakers', 'Audio output devices for projecting sound into a room.', 1)",
        "INSERT OR IGNORE INTO Subcategories (SubcategoryId, Name, Description, CategoryId) VALUES (10, 'Running Shoes', 'Footwear designed specifically for running', 2)",
        "INSERT OR IGNORE INTO Subcategories (SubcategoryId, Name, Description, CategoryId) VALUES (11, 'Casual Shoes', 'Comfortable and stylish shoes for everyday wear', 2)",
        "INSERT OR IGNORE INTO Subcategories (SubcategoryId, Name, Description, CategoryId) VALUES (12, 'Sandals', 'Open-toed shoes with straps or buckles', 2)",
        "INSERT OR IGNORE INTO Subcategories (SubcategoryId, Name, Description, CategoryId) VALUES (13, 'Cameras', 'Devices for capturing photographs or videos', 1)",
        "INSERT OR IGNORE INTO Subcategories (SubcategoryId, Name, Description, CategoryId) VALUES (14, 'Gaming Consoles', 'Devices for playing video games', 1)",
        "INSERT OR IGNORE INTO Subcategories (SubcategoryId, Name, Description, CategoryId) VALUES (15, 'Smart Home Devices', 'Connected devices for home automation', 1)",

        // Sample data for Products table
        "INSERT OR IGNORE INTO Products (ProductId, Name, Description, Price, SubcategoryId, SupplierId) VALUES (1, 'iPhone 12', 'Latest smartphone model from Apple', 999.99, 1, 1)",
        "INSERT OR IGNORE INTO Products (ProductId, Name, Description, Price, SubcategoryId, SupplierId) VALUES (2, 'MacBook Pro', 'High-performance laptop by Apple', 1499.99, 2, 2)",
        "INSERT OR IGNORE INTO Products (ProductId, Name, Description, Price, SubcategoryId, SupplierId) VALUES (3, 'Dell XPS 15', 'Powerful laptop for professionals', 1299.99, 2, 1)",
        "INSERT OR IGNORE INTO Products (ProductId, Name, Description, Price, SubcategoryId, SupplierId) VALUES (4, 'Samsung Galaxy S21', 'Flagship smartphone from Samsung', 899.99, 1, 2)",
        "INSERT OR IGNORE INTO Products (ProductId, Name, Description, Price, SubcategoryId, SupplierId) VALUES (5, 'HP Pavilion Desktop', 'Reliable desktop computer for home and office use', 699.99, 3, 1)",
        "INSERT OR IGNORE INTO Products (ProductId, Name, Description, Price, SubcategoryId, SupplierId) VALUES (6, 'ASUS ROG Strix', 'Gaming laptop with high-performance components', 1799.99, 2, 2)",
        "INSERT OR IGNORE INTO Products (ProductId, Name, Description, Price, SubcategoryId, SupplierId) VALUES (7, 'Nike Dri-FIT T-Shirt', 'Men''s athletic shirt with moisture-wicking fabric', 34.99, 4, 1)",
        "INSERT OR IGNORE INTO Products (ProductId, Name, Description, Price, SubcategoryId, SupplierId) VALUES (8, 'Adidas Ultraboost Shoes', 'Premium running shoes with responsive cushioning', 129.99, 5, 1)",
        "INSERT OR IGNORE INTO Products (ProductId, Name, Description, Price, SubcategoryId, SupplierId) VALUES (9, 'Levi''s 501 Jeans', 'Classic men''s denim jeans with a straight fit', 59.99, 4, 2)",
        "INSERT OR IGNORE INTO Products (ProductId, Name, Description, Price, SubcategoryId, SupplierId) VALUES (10, 'Ray-Ban Aviator Sunglasses', 'Iconic aviator sunglasses with polarized lenses', 149.99, 6, 1)",
        "INSERT OR IGNORE INTO Products (ProductId, Name, Description, Price, SubcategoryId, SupplierId) VALUES (11, 'Sony WH-1000XM4', 'Wireless noise-canceling headphones with premium sound quality', 349.99, 8, 2)",
        "INSERT OR IGNORE INTO Products (ProductId, Name, Description, Price, SubcategoryId, SupplierId) VALUES (12, 'Bose SoundLink Revolve', 'Portable Bluetooth speaker with 360-degree sound', 199.99, 9, 1)",
        "INSERT OR IGNORE INTO Products (ProductId, Name, Description, Price, SubcategoryId, SupplierId) VALUES (13, 'JBL Flip 5', 'Waterproof portable speaker with powerful bass', 119.99, 9, 2)",
        "INSERT OR IGNORE INTO Products (ProductId, Name, Description, Price, SubcategoryId, SupplierId) VALUES (14, 'The Great Gatsby', 'Classic novel by F. Scott Fitzgerald', 9.99, 6, 1)",
        "INSERT OR IGNORE INTO Products (ProductId, Name, Description, Price, SubcategoryId, SupplierId) VALUES (15, 'To Kill a Mockingbird', 'Seminal novel by Harper Lee', 8.99, 6, 2)",
        "INSERT OR IGNORE INTO Products (ProductId, Name, Description, Price, SubcategoryId, SupplierId) VALUES (16, 'The Catcher in the Rye', 'Modern classic by J.D. Salinger', 10.99, 6, 1)",
        "INSERT OR IGNORE INTO Products (ProductId, Name, Description, Price, SubcategoryId, SupplierId) VALUES (17, 'Nike Air Zoom Pegasus', 'Versatile running shoes for everyday training', 119.99, 5, 2)",
        "INSERT OR IGNORE INTO Products (ProductId, Name, Description, Price, SubcategoryId, SupplierId) VALUES (18, 'Apple AirPods Pro', 'Wireless earbuds with active noise cancellation', 249.99, 1, 1)",
        "INSERT OR IGNORE INTO Products (ProductId, Name, Description, Price, SubcategoryId, SupplierId) VALUES (19, 'Samsung Galaxy Tab S7', 'Premium Android tablet with S Pen support', 649.99, 3, 2)",
        "INSERT OR IGNORE INTO Products (ProductId, Name, Description, Price, SubcategoryId, SupplierId) VALUES (20, 'ASUS ZenBook 14', 'Ultra-thin and light laptop for on-the-go productivity', 999.99, 2, 2)",
        "INSERT OR IGNORE INTO Products (ProductId, Name, Description, Price, SubcategoryId, SupplierId) VALUES (21, 'Microsoft Surface Pro 7', 'Versatile 2-in-1 laptop with touch and pen support', 899.99, 2, 1)",
        "INSERT OR IGNORE INTO Products (ProductId, Name, Description, Price, SubcategoryId, SupplierId) VALUES (22, 'Canon EOS Rebel T7', 'Entry-level DSLR camera for beginners', 499.99, 4, 1)",
        "INSERT OR IGNORE INTO Products (ProductId, Name, Description, Price, SubcategoryId, SupplierId) VALUES (23, 'Sony PlayStation 5', 'Next-gen gaming console with blazing-fast SSD', 499.99, 5, 2)",
        "INSERT OR IGNORE INTO Products (ProductId, Name, Description, Price, SubcategoryId, SupplierId) VALUES (24, 'Nintendo Switch', 'Hybrid gaming console for home and on-the-go play', 299.99, 5, 2)",

        // Sample data for Orders table
        "INSERT OR IGNORE INTO Orders (OrderId, CustomerId, EmployeeId, ItemsOrdered, OrderDate, Status, DiscountedPrice) VALUES (1, 1, 6, '1,3,5', '2024-02-16', 'Pending', 0)",
        "INSERT OR IGNORE INTO Orders (OrderId, CustomerId, EmployeeId, ItemsOrdered, OrderDate, Status, DiscountedPrice) VALUES (2, 2, 7, '2,4,6', '2024-02-16', 'Processing', 0)",
        "INSERT OR IGNORE INTO Orders (OrderId, CustomerId, EmployeeId, ItemsOrdered, OrderDate, Status, DiscountedPrice) VALUES (3, 3, 6, '7,9,11', '2024-02-16', 'Shipped', 0)",
        "INSERT OR IGNORE INTO Orders (OrderId, CustomerId, EmployeeId, ItemsOrdered, OrderDate, Status, DiscountedPrice) VALUES (4, 1, 7, '8,10,12', '2024-02-16', 'Delivered', 0)",
        "INSERT OR IGNORE INTO Orders (OrderId, CustomerId, EmployeeId, ItemsOrdered, OrderDate, Status, DiscountedPrice) VALUES (5, 2, 6, '13,15,17', '2024-02-16', 'Pending', 0)",
        "INSERT OR IGNORE INTO Orders (OrderId, CustomerId, EmployeeId, ItemsOrdered, OrderDate, Status, DiscountedPrice) VALUES (6, 3, 7, '14,16,18', '2024-02-16', 'Processing', 0)",

        // Sample data for OrderItems table
        "INSERT OR IGNORE INTO OrderItems (OrderItemId, OrderId, ProductId, Quantity) VALUES (1, 1, 1, 2)",
        "INSERT OR IGNORE INTO OrderItems (OrderItemId, OrderId, ProductId, Quantity) VALUES (2, 1, 3, 1)",
        "INSERT OR IGNORE INTO OrderItems (OrderItemId, OrderId, ProductId, Quantity) VALUES (3, 1, 5, 3)",
        "INSERT OR IGNORE INTO OrderItems (OrderItemId, OrderId, ProductId, Quantity) VALUES (4, 2, 2, 1)",
        "INSERT OR IGNORE INTO OrderItems (OrderItemId, OrderId, ProductId, Quantity) VALUES (5, 2, 4, 2)",
        "INSERT OR IGNORE INTO OrderItems (OrderItemId, OrderId, ProductId, Quantity) VALUES (6, 2, 6, 1)",
        "INSERT OR IGNORE INTO OrderItems (OrderItemId, OrderId, ProductId, Quantity) VALUES (7, 3, 7, 1)",
        "INSERT OR IGNORE INTO OrderItems (OrderItemId, OrderId, ProductId, Quantity) VALUES (8, 3, 9, 3)",
        "INSERT OR IGNORE INTO OrderItems (OrderItemId, OrderId, ProductId, Quantity) VALUES (9, 3, 11, 2)",
        "INSERT OR IGNORE INTO OrderItems (OrderItemId, OrderId, ProductId, Quantity) VALUES (10, 4, 8, 2)",
        "INSERT OR IGNORE INTO OrderItems (OrderItemId, OrderId, ProductId, Quantity) VALUES (11, 4, 10, 1)",
        "INSERT OR IGNORE INTO OrderItems (OrderItemId, OrderId, ProductId, Quantity) VALUES (12, 4, 12, 3)",
        "INSERT OR IGNORE INTO OrderItems (OrderItemId, OrderId, ProductId, Quantity) VALUES (13, 5, 13, 2)",
        "INSERT OR IGNORE INTO OrderItems (OrderItemId, OrderId, ProductId, Quantity) VALUES (14, 5, 15, 1)",
        "INSERT OR IGNORE INTO OrderItems (OrderItemId, OrderId, ProductId, Quantity) VALUES (15, 5, 17, 4)",
        "INSERT OR IGNORE INTO OrderItems (OrderItemId, OrderId, ProductId, Quantity) VALUES (16, 6, 14, 1)",
        "INSERT OR IGNORE INTO OrderItems (OrderItemId, OrderId, ProductId, Quantity) VALUES (17, 6, 16, 2)",
        "INSERT OR IGNORE INTO OrderItems (OrderItemId, OrderId, ProductId, Quantity) VALUES (18, 6, 18, 2)",

        // Sample data for Transactions table
        "INSERT OR IGNORE INTO Transactions (TransactionId, Timestamp, Type, CustomerId, BranchId, TotalAmount) VALUES (1, '2024-02-16 09:00:00', 'Sale', 1, 1, 999.99)",
        "INSERT OR IGNORE INTO Transactions (TransactionId, Timestamp, Type, CustomerId, BranchId, TotalAmount) VALUES (2, '2024-02-16 10:30:00', 'Sale', 2, 2, 29.99)",
        "INSERT OR IGNORE INTO Transactions (TransactionId, Timestamp, Type, CustomerId, BranchId, TotalAmount) VALUES (3, '2024-02-16 11:45:00', 'Sale', 2, 1, 12.99)",

        // Sample data for Warehouses table
        "INSERT OR IGNORE INTO Warehouses (WarehouseId, Location, ManagerId) VALUES (1, 'Warehouse A', 4)",
        "INSERT OR IGNORE INTO Warehouses (WarehouseId, Location, ManagerId) VALUES (2, 'Warehouse B', 5)",

        // Sample data for Inventories table
        "INSERT OR IGNORE INTO Inventories (InventoryId, ProductId, WarehouseId, ProductAvailability, ReorderThreshold) VALUES (1, 1, 1, 100, 20)",
        "INSERT OR IGNORE INTO Inventories (InventoryId, ProductId, WarehouseId, ProductAvailability, ReorderThreshold) VALUES (2, 2, 1, 150, 30)",
        "INSERT OR IGNORE INTO Inventories (InventoryId, ProductId, WarehouseId, ProductAvailability, ReorderThreshold) VALUES (3, 3, 2, 200, 40)",
        "INSERT OR IGNORE INTO Inventories (InventoryId, ProductId, WarehouseId, ProductAvailability, ReorderThreshold) VALUES (4, 4, 2, 120, 25)",
        "INSERT OR IGNORE INTO Inventories (InventoryId, ProductId, WarehouseId, ProductAvailability, ReorderThreshold) VALUES (5, 5, 1, 80, 15)",
        "INSERT OR IGNORE INTO Inventories (InventoryId, ProductId, WarehouseId, ProductAvailability, ReorderThreshold) VALUES (6, 6, 2, 180, 35)",
        "INSERT OR IGNORE INTO Inventories (InventoryId, ProductId, WarehouseId, ProductAvailability, ReorderThreshold) VALUES (7, 7, 2, 90, 18)",
        "INSERT OR IGNORE INTO Inventories (InventoryId, ProductId, WarehouseId, ProductAvailability, ReorderThreshold) VALUES (8, 8, 1, 110, 22)",
        "INSERT OR IGNORE INTO Inventories (InventoryId, ProductId, WarehouseId, ProductAvailability, ReorderThreshold) VALUES (9, 9, 2, 160, 28)",
        "INSERT OR IGNORE INTO Inventories (InventoryId, ProductId, WarehouseId, ProductAvailability, ReorderThreshold) VALUES (10, 10, 2, 140, 26)",
        "INSERT OR IGNORE INTO Inventories (InventoryId, ProductId, WarehouseId, ProductAvailability, ReorderThreshold) VALUES (11, 11, 1, 100, 20)",
        "INSERT OR IGNORE INTO Inventories (InventoryId, ProductId, WarehouseId, ProductAvailability, ReorderThreshold) VALUES (12, 12, 1, 150, 30)",
        "INSERT OR IGNORE INTO Inventories (InventoryId, ProductId, WarehouseId, ProductAvailability, ReorderThreshold) VALUES (13, 13, 2, 200, 40)",
        "INSERT OR IGNORE INTO Inventories (InventoryId, ProductId, WarehouseId, ProductAvailability, ReorderThreshold) VALUES (14, 14, 2, 120, 25)",
        "INSERT OR IGNORE INTO Inventories (InventoryId, ProductId, WarehouseId, ProductAvailability, ReorderThreshold) VALUES (15, 15, 1, 80, 15)",
        "INSERT OR IGNORE INTO Inventories (InventoryId, ProductId, WarehouseId, ProductAvailability, ReorderThreshold) VALUES (16, 16, 2, 180, 35)",
        "INSERT OR IGNORE INTO Inventories (InventoryId, ProductId, WarehouseId, ProductAvailability, ReorderThreshold) VALUES (17, 17, 2, 90, 18)",
        "INSERT OR IGNORE INTO Inventories (InventoryId, ProductId, WarehouseId, ProductAvailability, ReorderThreshold) VALUES (18, 18, 2, 110, 22)",

        // Sample data for Returns table
        "INSERT OR IGNORE INTO Returns (ReturnId, CustomerId, EmployeeId, ItemsReturned, ReturnReason, ReturnDate, OrderId) VALUES (1, 1, 6, '1,3,5', 'Wrong size', '2024-02-17', 1)",
        "INSERT OR IGNORE INTO Returns (ReturnId, CustomerId, EmployeeId, ItemsReturned, ReturnReason, ReturnDate, OrderId) VALUES (2, 2, 7, '2,4,6', 'Defective product', '2024-02-18', 2)",
        "INSERT OR IGNORE INTO Returns (ReturnId, CustomerId, EmployeeId, ItemsReturned, ReturnReason, ReturnDate, OrderId) VALUES (3, 3, 6, '7,9,11', 'Changed mind', '2024-02-19', 3)",

        // Sample data for ReturnItems table
        "INSERT OR IGNORE INTO ReturnItems (ReturnItemId, ReturnId, ProductId, Quantity) VALUES (1, 1, 1, 1)",
        "INSERT OR IGNORE INTO ReturnItems (ReturnItemId, ReturnId, ProductId, Quantity) VALUES (2, 1, 3, 1)",
        "INSERT OR IGNORE INTO ReturnItems (ReturnItemId, ReturnId, ProductId, Quantity) VALUES (3, 1, 5, 2)",
        "INSERT OR IGNORE INTO ReturnItems (ReturnItemId, ReturnId, ProductId, Quantity) VALUES (4, 2, 2, 1)",
        "INSERT OR IGNORE INTO ReturnItems (ReturnItemId, ReturnId, ProductId, Quantity) VALUES (5, 2, 4, 1)",
        "INSERT OR IGNORE INTO ReturnItems (ReturnItemId, ReturnId, ProductId, Quantity) VALUES (6, 2, 6, 1)",
        "INSERT OR IGNORE INTO ReturnItems (ReturnItemId, ReturnId, ProductId, Quantity) VALUES (7, 3, 7, 1)",
        "INSERT OR IGNORE INTO ReturnItems (ReturnItemId, ReturnId, ProductId, Quantity) VALUES (8, 3, 9, 2)",
        "INSERT OR IGNORE INTO ReturnItems (ReturnItemId, ReturnId, ProductId, Quantity) VALUES (9, 3, 11, 1)",

        // Sample data for Promos table
        "INSERT OR IGNORE INTO Promos (PromoId, Name, Description, DiscountPercentage, StartPeriod, EndPeriod, ProductId, EmployeeId) VALUES (1, 'Summer Sale', 'Get discounts on selected items during the summer season.', 15.0, '2024-06-01 00:00:00', '2024-08-31 00:00:00', 1, 6)",
        "INSERT OR IGNORE INTO Promos (PromoId, Name, Description, DiscountPercentage, StartPeriod, EndPeriod, ProductId, EmployeeId) VALUES (2, 'Back to School Promo', 'Special discounts for students going back to school.', 10.0, '2024-08-15 00:00:00', '2024-09-15 00:00:00', 3, 7)",
        "INSERT OR IGNORE INTO Promos (PromoId, Name, Description, DiscountPercentage, StartPeriod, EndPeriod, ProductId, EmployeeId) VALUES (3, 'Holiday Season Deal', 'Enjoy holiday discounts on various products.', 20.0, '2024-12-01 00:00:00', '2025-01-01 00:00:00', 5, 6)",

        // Sample data for Discounts table
        "INSERT OR IGNORE INTO Discounts (DiscountId, Name, Description, StartPeriod, EndPeriod, ProductId, Percentage) VALUES (1, 'Clearance Sale', 'Limited time clearance sale on old inventory.', '2024-10-01 00:00:00', '2024-10-31 00:00:00', 2, 10.0)",
        "INSERT OR IGNORE INTO Discounts (DiscountId, Name, Description, StartPeriod, EndPeriod, ProductId, Percentage) VALUES (2, 'Flash Sale', 'Hurry up! Limited time offer on selected items.', '2024-07-15 00:00:00','2024-07-17 00:00:00', 4, 15.0)",
        "INSERT OR IGNORE INTO Discounts (DiscountId, Name, Description, StartPeriod, EndPeriod, ProductId, Percentage) VALUES (3, 'Weekend Special', 'Special discounts for the weekend.', '2024-09-20 00:00:00', '2024-09-22 00:00:00', 6, 20.0)"};

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

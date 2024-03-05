#include <iostream>
#include "Menu.h"

#include "Database.h"
#include "User.h"
#include "Customer.h"
#include "Manager.h"
#include "Employee.h"
#include "Supplier.h"
#include "Transaction.h"
#include "Inventory.h"
#include "Product.h"
#include "Category.h"
#include "Order.h"
#include "OrderItem.h"
#include "Return.h"
#include "Branch.h"
#include "Subcategory.h"
#include "Warehouse.h"
#include "Promo.h"
#include "Discount.h"

using namespace std;

// validate the role entered by the user
bool Menu::IsValidRole(const string &role)
{
    // check if the role is one of these
    return role == "customer" || role == "manager" || role == "employee" || role == "supplier";
}

void Menu::DisplayLoginMenu()
{
    cout << "------------------------------------------------\n";
    cout << "1. Login\n";
    cout << "2. Register\n";
    cout << "3. Quit the Program\n";
    cout << "Enter your choice: ";
}

void Menu::RunLoginMenu()
{
    string choice;

    while (true)
    {
        DisplayLoginMenu();
        cin >> choice;

        if (choice == "1") // login
        {
            string username, password;
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;

            User *user = User::Login(username, password);

            if (user != nullptr)
            {
                cout << "Logged in with: " << User::GetLoggedInUsername() << " as: " << User::GetLoggedInRole() << endl;
                RunMainMenu(user);
            }
            else
            {
                cout << "Login failed\n";
            }
        }
        else if (choice == "2") // register a new user
        {
            string username, password, email, phone, address, role;
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            cout << "Enter email: ";
            cin >> email;
            cout << "Enter phone: ";
            cin >> phone;
            cout << "Enter address: ";
            cin.ignore();
            getline(cin, address);

            // enter correct user role
            while (true)
            {
                cout << "Enter role (customer, manager, employee, supplier): ";
                cin >> role;

                if (IsValidRole(role))
                {
                    break; // Valid role, exit the loop
                }
                else
                {
                    cout << "Invalid role. Please enter correct role.\n";
                }
            }

            User::Register(username, password, email, phone, address, role);
        }
        else if (choice == "3")
        {
            cout << "Exiting...\n";
            return;
        }
        else
        {
            cout << "Invalid choice\n";
        }
    }
}

void Menu::DisplayUserMenu()
{
    cout << "------------------------------------------------\n";
    cout << "1. Update Profile\n";
    cout << "2. Change Password\n";
    cout << "3. Go Back\n";
    cout << "Enter your choice: ";
}

void Menu::RunUserMenu(User *user)
{
    string choice;

    while (true)
    {
        DisplayLoginMenu();
        cin >> choice;

        if (choice == "1") // update profile
        {
            string newEmail, newPhone, newAddress;
            cout << "Enter new email: ";
            cin >> newEmail;
            cout << "Enter new phone: ";
            cin >> newPhone;
            cout << "Enter new address: ";
            cin.ignore();
            getline(cin, newAddress);
            user->UpdateProfile(newEmail, newPhone, newAddress);
        }
        else if (choice == "2") // change password
        {
            string newPassword;
            cout << "Enter new password: ";
            cin >> newPassword;
            user->ChangePassword(newPassword);
        }
        else if (choice == "3") // Go back
        {
            return;
        }
        else
        {
            cout << "Invalid choice\n";
        }
    }
}

void Menu::DisplayMainMenu()
{
    cout << "------------------------------------------------\n";
    string loggedInRole = User::GetLoggedInRole();

    // Display menu options based on the logged-in user's role
    if (loggedInRole == "manager")
    {
        cout << "1. Registration Menu\n";
        cout << "2. Category Menu\n";
        cout << "3. Subcategory Menu\n";
        cout << "4. Product Menu\n";
        cout << "5. Order Menu\n";
        cout << "6. Inventory Menu\n";
        cout << "7. Discount Menu\n";
        cout << "8. Promo Menu\n";
        cout << "9. Warehouse Menu\n";
        cout << "10. Branch Menu\n";
        cout << "11. User Menu\n";
        cout << "12. Transaction History\n";
        cout << "13. Logout\n";
    }
    else if (loggedInRole == "employee")
    {
        cout << "1. Category Menu\n";
        cout << "2. Subcategory Menu\n";
        cout << "3. Product Menu\n";
        cout << "4. Order Menu\n";
        cout << "5. Inventory Menu\n";
        cout << "6. Discount Menu\n";
        cout << "7. Promo Menu\n";
        cout << "8. User Menu\n";
        cout << "9. Logout\n";
    }
    else if (loggedInRole == "customer")
    {
        cout << "1. Customer Profile\n";
        cout << "2. Product Menu\n";
        cout << "3. Order History\n";
        cout << "4. Return Menu\n";
        cout << "5. User Menu\n";
        cout << "6. Logout\n";
    }
    else if (loggedInRole == "supplier")
    {
        cout << "1. Product Menu\n";
        cout << "2. Inventory Menu\n";
        cout << "3. Order Menu\n";
        cout << "4. User Menu\n";
        cout << "5. Logout\n";
    }

    cout << "Enter your choice: ";
}

void Menu::RunMainMenu(User *user)
{
    string choice;
    while (true)
    {
        DisplayMainMenu();
        cin >> choice;

        if (user->GetLoggedInRole() == "manager")
        {
            if (choice == "1") // Registration Menu
            {
                RunRegistrationMenu();
            }
            else if (choice == "2") // Category Menu
            {
                RunCategoryMenu();
            }
            else if (choice == "3") // Subcategory Menu
            {
                RunSubcategoryMenu();
            }
            else if (choice == "4") // Product Menu
            {
                RunProductMenu();
            }
            else if (choice == "5") // Order Menu
            {
                RunOrderMenu();
            }
            else if (choice == "6") // Inventory Menu
            {
                RunInventoryMenu();
            }
            else if (choice == "7") // Discount Menu
            {
                RunDiscountMenu();
            }
            else if (choice == "8") // Promo Menu
            {
                RunPromoMenu();
            }
            else if (choice == "9") // Warehouse Menu
            {
                RunWarehouseMenu();
            }
            else if (choice == "10") // Branch Menu
            {
                RunBranchMenu();
            }
            else if (choice == "11") // User Menu
            {
                RunUserMenu(user);
            }
            else if (choice == "12") // Transaction Menu
            {
                RunTransactionMenu();
            }
            else if (choice == "13") // Logout
            {
                user->Logout();
                user = nullptr;
                cout << "Logged out successfully\n";
                return;
            }
            else
            {
                cout << "Invalid choice\n";
            }
        }
        else if (user->GetLoggedInRole() == "employee")
        {
            if (choice == "1") // Category Menu
            {
                RunCategoryMenu();
            }
            else if (choice == "2") // Subcategory Menu
            {
                RunSubcategoryMenu();
            }
            else if (choice == "3") // Product Menu
            {
                RunProductMenu();
            }
            else if (choice == "4") // Order Menu
            {
                RunOrderMenu();
            }
            else if (choice == "5") // Inventory Menu
            {
                RunInventoryMenu();
            }
            else if (choice == "6") // Discount Menu
            {
                RunDiscountMenu();
            }
            else if (choice == "7") // Promo Menu
            {
                RunPromoMenu();
            }
            else if (choice == "8") // User Menu
            {
                RunUserMenu(user);
            }
            else if (choice == "9") // Logout
            {
                user->Logout();
                user = nullptr;
                cout << "Logged out successfully\n";
                return;
            }
            else
            {
                cout << "Invalid choice\n";
            }
        }
        else if (user->GetLoggedInRole() == "customer")
        {
            if (choice == "1") // Customer Profile
            {
                RunCustomerMenu();
            }
            else if (choice == "2") // Product Menu
            {
                RunProductMenu();
            }
            else if (choice == "3") // Order History
            {
                RunOrderMenu();
            }
            else if (choice == "4") // Return Menu
            {
                RunReturnMenu();
            }
            else if (choice == "5") // User Menu
            {
                RunUserMenu(user);
            }
            else if (choice == "6") // Logout
            {
                user->Logout();
                user = nullptr;
                cout << "Logged out successfully\n";
                return;
            }
            else
            {
                cout << "Invalid choice\n";
            }
        }
        else if (user->GetLoggedInRole() == "supplier")
        {
            if (choice == "1") // Product Menu
            {
                RunProductMenu();
            }
            else if (choice == "2") // Inventory Menu
            {
                RunInventoryMenu();
            }
            else if (choice == "3") // Order Menu
            {
                RunOrderMenu();
            }
            // else if (choice == "4") // Transaction History
            // {
            //     RunTransactionMenu();
            // }
            else if (choice == "4") // User Menu
            {
                RunUserMenu(user);
            }
            else if (choice == "5") // Logout
            {
                user->Logout();
                user = nullptr;
                cout << "Logged out successfully\n";
                return;
            }
            else
            {
                cout << "Invalid choice\n";
            }
        }
    }
}

void Menu::DisplayCategoryMenu()
{
    cout << "------------------------------------------------\n";
    cout << "1. Add Category\n";
    cout << "2. Update Category Details\n";
    cout << "3. Delete Category\n";
    cout << "4. View All Categories\n";
    cout << "5. Go Back\n";
    cout << "Enter your choice: ";
}

void Menu::RunCategoryMenu()
{
    string choice;
    Category category(0, "", ""); // Create a Category object for menu operations

    while (true)
    {
        DisplayCategoryMenu();
        cin >> choice;

        if (choice == "1")
        { // Add Category
            string name, description;
            cout << "Enter category name: ";
            cin.ignore();       // Ignore previous newline character in the input
            getline(cin, name); // get the whole name with white spaces
            cout << "Enter category description: ";
            cin.ignore();
            getline(cin, description);
            category.AddCategory(name, description);
        }
        else if (choice == "2")
        { // Update Category Details
            int categoryId;
            string newName, newDescription;
            cout << "Enter category Id to update: ";
            cin >> categoryId;
            cout << "Enter new category name: ";
            cin.ignore();
            getline(cin, newName);
            cout << "Enter new category description: ";
            cin.ignore();
            getline(cin, newDescription);
            category.UpdateCategoryDetails(categoryId, newName, newDescription);
        }
        else if (choice == "3")
        { // Delete Category
            int categoryId;
            cout << "Enter category Id to delete: ";
            cin >> categoryId;
            category.DeleteCategory(categoryId);
        }
        else if (choice == "4")
        { // View All Categories
            vector<Category> allCategories = Category::GetAllCategories();
            Category::DisplayAllCategories(allCategories);
        }
        else if (choice == "5")
        {           // Go back
            return; // Exit the loop and return to the previous menu
        }
        else
        {
            cout << "Invalid choice\n";
        }
    }
}

void Menu::DisplaySubcategoryMenu()
{
    cout << "------------------------------------------------\n";
    cout << "1. Add Subcategory\n";
    cout << "2. Update Subcategory Details\n";
    cout << "3. Delete Subcategory\n";
    cout << "4. View All Subcategories\n";
    cout << "5. Go Back\n";
    cout << "Enter your choice: ";
}

void Menu::RunSubcategoryMenu()
{
    string choice;
    Subcategory subcategory(0, "", "", 0); // Create a Subcategory object for menu operations

    while (true)
    {
        DisplaySubcategoryMenu();
        cin >> choice;

        if (choice == "1")
        { // Add Subcategory
            int categoryId;
            string name, description;
            cout << "Enter category Id for the subcategory: ";
            cin >> categoryId;
            cout << "Enter subcategory name: ";
            cin.ignore();       // Ignore previous newline character in the input
            getline(cin, name); // get the whole name with white spaces
            cout << "Enter subcategory description: ";
            cin.ignore();
            getline(cin, description);
            subcategory.AddSubcategory(name, description, categoryId);
        }
        else if (choice == "2")
        { // Update Subcategory Details
            int subcategoryId, categoryId;
            string newName, newDescription;
            cout << "Enter subcategory Id to update: ";
            cin >> subcategoryId;
            cout << "Enter new category Id for the subcategory: ";
            cin >> categoryId;
            cout << "Enter new subcategory name: ";
            cin.ignore();
            getline(cin, newName);
            cout << "Enter new subcategory description: ";
            cin.ignore();
            getline(cin, newDescription);
            subcategory.UpdateSubcategoryDetails(subcategoryId, newName, newDescription, categoryId);
        }
        else if (choice == "3")
        { // Delete Subcategory
            int subcategoryId;
            cout << "Enter subcategory Id to delete: ";
            cin >> subcategoryId;
            subcategory.DeleteSubcategory(subcategoryId);
        }
        else if (choice == "4")
        { // View All Subcategories
            vector<Subcategory> allSubcategories = Subcategory::GetAllSubcategories();
            Subcategory::DisplayAllSubcategories(allSubcategories);
        }
        else if (choice == "5")
        {           // Go back
            return; // Exit the loop and return to the previous menu
        }
        else
        {
            cout << "Invalid choice\n";
        }
    }
}

void Menu::DisplayProductMenu()
{
    cout << "------------------------------------------------\n";
    if (User::GetLoggedInRole() == "manager" || User::GetLoggedInRole() == "employee")
    {
        cout << "1. Add Product\n";
        cout << "2. Update Product Details\n";
        cout << "3. Delete Product\n";
    }
    cout << "4. View All Products\n";
    cout << "5. Go Back\n";
    cout << "Enter your choice: ";
}

void Menu::RunProductMenu()
{
    string choice;
    Product product(0, "", "", 0.0, 0, 0); // Create a Product object for menu operations

    while (true)
    {
        DisplayProductMenu();
        cin >> choice;

        if (choice == "1" && (User::GetLoggedInRole() == "manager" || User::GetLoggedInRole() == "employee"))
        { // Add Product
            int subcategoryId, supplierId;
            string name, description;
            double price;
            cout << "Enter subcategory Id for the product: ";
            cin >> subcategoryId;
            cout << "Enter supplier Id for the product: ";
            cin >> supplierId;
            cout << "Enter product name: ";
            cin.ignore();       // Ignore previous newline character in the input
            getline(cin, name); // get the whole name with white spaces
            cout << "Enter product description: ";
            cin.ignore();
            getline(cin, description);
            cout << "Enter product price: ";
            cin >> price;
            product.AddProduct(name, description, price, subcategoryId, supplierId);
        }
        else if (choice == "2" && (User::GetLoggedInRole() == "manager" || User::GetLoggedInRole() == "employee"))
        { // Update Product Details
            int productId, subcategoryId, supplierId;
            string newName, newDescription;
            double newPrice;
            cout << "Enter product Id to update: ";
            cin >> productId;
            cout << "Enter new subcategory Id for the product: ";
            cin >> subcategoryId;
            cout << "Enter new supplier Id for the product: ";
            cin >> supplierId;
            cout << "Enter new product name: ";
            cin.ignore();
            getline(cin, newName);
            cout << "Enter new product description: ";
            cin.ignore();
            getline(cin, newDescription);
            cout << "Enter new product price: ";
            cin >> newPrice;
            product.UpdateProductDetails(productId, newName, newDescription, newPrice, subcategoryId, supplierId);
        }
        else if (choice == "3" && (User::GetLoggedInRole() == "manager" || User::GetLoggedInRole() == "employee"))
        { // Delete Product
            int productId;
            cout << "Enter product Id to delete: ";
            cin >> productId;
            product.DeleteProduct(productId);
        }
        else if (choice == "4")
        { // View All Products
            vector<Product> allProducts = Product::GetAllProducts();
            Product::DisplayAllProducts(allProducts);
        }
        else if (choice == "5")
        {           // Go back
            return; // Exit the loop and return to the previous menu
        }
        else
        {
            cout << "Invalid choice\n";
        }
    }
}

void Menu::DisplayRegistrationMenu()
{
    cout << "------------------------------------------------\n";
    cout << "1. Register a Customer\n";
    cout << "2. Register an Employee\n";
    cout << "3. Register a Manager\n";
    cout << "4. Register a Supplier\n";
    cout << "5. Go Back\n";
    cout << "Enter your choice: ";
}

void Menu::RunRegistrationMenu()
{
    string choice;

    while (true)
    {
        DisplayRegistrationMenu();
        cin >> choice;

        if (choice == "1")
        { // Register a Customer
            string username, password, email, phone, address, shippingAddress, paymentMethod;
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            cout << "Enter email: ";
            cin >> email;
            cout << "Enter phone: ";
            cin >> phone;
            cout << "Enter address: ";
            cin.ignore();
            getline(cin, address);
            cout << "Enter shipping address: ";
            cin.ignore();
            getline(cin, shippingAddress);
            cout << "Enter payment method: ";
            cin.ignore();
            getline(cin, paymentMethod);

            Customer *newCustomer = Customer::RegisterCustomer(username, password, email, phone, address, shippingAddress, paymentMethod);
            if (newCustomer != nullptr)
            {
                cout << "Customer registered successfully\n";
            }
            else
            {
                cout << "Failed to register customer\n";
            }
        }
        else if (choice == "2")
        { // Register an Employee
            string username, password, email, phone, address, position, tasks;
            int managerId, branchId;
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            cout << "Enter email: ";
            cin >> email;
            cout << "Enter phone: ";
            cin >> phone;
            cout << "Enter address: ";
            cin.ignore();
            getline(cin, address);
            cout << "Enter managerId: ";
            cin >> managerId;
            cout << "Enter branchId: ";
            cin >> branchId;
            cout << "Enter position: ";
            cin.ignore();
            getline(cin, position);
            cout << "Enter tasks: ";
            cin.ignore();
            getline(cin, tasks);

            // Registering the Employee
            Employee *newEmployee = Employee::RegisterEmployee(username, password, email, phone, address, managerId, branchId, position, tasks);

            if (newEmployee != nullptr)
            {
                cout << "Employee registered successfully\n";
            }
            else
            {
                cout << "Failed to register employee\n";
            }
        }
        else if (choice == "3")
        { // Register a Manager
            string username, password, email, phone, address, department;
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            cout << "Enter email: ";
            cin >> email;
            cout << "Enter phone: ";
            cin >> phone;
            cout << "Enter address: ";
            cin.ignore();
            getline(cin, address);
            cout << "Enter department: ";
            cin.ignore();
            getline(cin, department);

            Manager *newManager = Manager::RegisterManager(username, password, email, phone, address, department);
            if (newManager != nullptr)
            {
                cout << "Manager registered successfully\n";
            }
            else
            {
                cout << "Failed to register manager\n";
            }
        }
        else if (choice == "4")
        { // Register a Supplier
            string username, password, email, phone, address, contactPerson, productsSupplied, paymentTerms;
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            cout << "Enter email: ";
            cin >> email;
            cout << "Enter phone: ";
            cin >> phone;
            cout << "Enter address: ";
            cin.ignore();
            getline(cin, address);
            cout << "Enter contact person: ";
            cin.ignore();
            getline(cin, contactPerson);
            cout << "Enter products supplied: ";
            cin.ignore();
            getline(cin, productsSupplied);
            cout << "Enter payment terms: ";
            cin.ignore();
            getline(cin, paymentTerms);

            Supplier *newSupplier = Supplier::RegisterSupplier(username, password, email, phone, address, contactPerson, productsSupplied, paymentTerms);
            if (newSupplier != nullptr)
            {
                cout << "Supplier registered successfully\n";
            }
            else
            {
                cout << "Failed to register supplier\n";
            }
        }
        else if (choice == "5")
        { // Go back
            return;
        }
        else
        {
            cout << "Invalid choice\n";
        }
    }
}

void Menu::DisplayCustomerMenu()
{
    cout << "------------------------------------------------\n";
    // cout << "1. Place Order\n";
    cout << "1. View Order History\n";
    cout << "2. Return Order Items\n";
    cout << "3. Go Back\n";
    cout << "Enter your choice: ";
}

void Menu::RunCustomerMenu()
{
    string choice;

    while (true)
    {
        DisplayCustomerMenu();
        cin >> choice;

        if (choice == "-1")
        { // Place Order
            string orderDate = Database::ConvertDatetimeToString(time(nullptr));
            vector<int> itemsOrdered;

            // Get items ordered (product Ids) from the user
            int productId;
            char addAnother;
            do
            {
                cout << "Enter product Id: ";
                cin >> productId;
                itemsOrdered.push_back(productId);
                cout << "Add another product? (y/n): ";
                cin >> addAnother;
            } while (addAnother == 'y' || addAnother == 'Y');

            // Get the logged-in username
            string userName = User::GetLoggedInUsername();

            // Find the user Id from the username
            int userId = User::GetUserIdByUsername(userName);

            // Check if the user Id is valid (-1 indicates not found)
            if (userId != -1)
            {
                // Proceed with creating the order using the retrieved user Id
                Order::CreateOrder(userId, orderDate, OrderStatus::Placed, itemsOrdered);
            }
            else
            {
                // Handle case where the user Id is not found
                cerr << "Error: User Id not found for username: " << userName << endl;
            }
        }
        else if (choice == "1")
        {
            // View Order History

            // Get the logged-in username
            string userName = User::GetLoggedInUsername();

            // Find the user Id from the username
            int userId = User::GetUserIdByUsername(userName);

            // Check if the user Id is valid (-1 indicates not found)
            if (userId != -1)
            {
                Customer customer = Customer::GetCustomerById(userId);
                customer.ViewOrderHistory();
            }
            else
            {
                // Handle case where the user Id is not found
                cerr << "Error: User Id not found for username: " << userName << endl;
            }
        }
        else if (choice == "2")
        { // Return Order Items
          // Get the logged-in username
            string userName = User::GetLoggedInUsername();

            // Find the user Id from the username
            int userId = User::GetUserIdByUsername(userName);

            // Check if the user Id is valid (-1 indicates not found)
            if (userId != -1)
            {
                string returnReason;
                cout << "Enter the return reason: ";
                cin.ignore();
                getline(cin, returnReason);

                Customer customer = Customer::GetCustomerById(userId);

                // View order history for the customer
                customer.ViewOrderHistory();

                // Prompt user to select an order
                int orderId;
                cout << "Enter the Order Id you want to return items from: ";
                cin >> orderId;

                // Get the selected order
                Order order = Order::GetOrderById(orderId);

                if (order.GetCustomerId() != userId)
                {
                    cerr << "Error: You are not authorized to return items for this order." << endl;
                    return;
                }

                // List the items of the selected order
                cout << "Items in the selected order:\n";
                vector<OrderItem> orderItems = order.GetOrderItems();
                for (size_t i = 0; i < orderItems.size(); ++i)
                {
                    cout << i + 1 << ". " << orderItems[i].GetProductName() << " (Product Id: " << orderItems[i].GetProductId() << ")" << endl;
                }

                // Prompt user to select items to return
                vector<int> itemsToReturn;
                char addAnother;
                do
                {
                    int itemIndex;
                    cout << "Enter the number of the item you want to return: ";
                    cin >> itemIndex;

                    if (itemIndex >= 1 && itemIndex <= orderItems.size())
                    {
                        itemsToReturn.push_back(orderItems[itemIndex - 1].GetProductId());
                    }
                    else
                    {
                        cerr << "Invalid item number. Please try again.\n";
                    }

                    cout << "Add another item to return? (y/n): ";
                    cin >> addAnother;
                } while (addAnother == 'y' || addAnother == 'Y');

                // Create a return with the selected items for the customer
                Return::CreateReturn(userId, itemsToReturn, returnReason, Database::ConvertDatetimeToString(time(nullptr)), orderId);
            }
            else
            {
                // Handle case where the user Id is not found
                cerr << "Error: User Id not found for username: " << userName << endl;
            }
        }
        else if (choice == "3")
        { // Go back
            return;
        }
        else
        {
            cout << "Invalid choice\n";
        }
    }
}

void Menu::DisplayOrderMenu()
{
    cout << "------------------------------------------------\n";
    cout << "1. Create Order\n";
    cout << "2. Cancel Order\n";
    cout << "3. Track Order\n";
    cout << "4. Go Back\n";
    cout << "Enter your choice: ";
}

void Menu::RunOrderMenu()
{
    string choice;
    Order order(0, 0, {}, "", OrderStatus::Placed); // Create an Order object for menu operations

    while (true)
    {
        DisplayOrderMenu();
        cin >> choice;

        if (choice == "1")
        { // Create Order
            int customerId;
            string orderDate = Database::ConvertDatetimeToString(time(nullptr));
            vector<int> itemsOrdered;

            cout << "Enter customer Id: ";
            cin >> customerId;

            // Get items ordered (product Ids) from the user
            int productId;
            char addAnother;
            do
            {
                cout << "Enter product Id: ";
                cin >> productId;
                itemsOrdered.push_back(productId);
                cout << "Add another product? (y/n): ";
                cin >> addAnother;
            } while (addAnother == 'y' || addAnother == 'Y');

            order.CreateOrder(customerId, orderDate, OrderStatus::Placed, itemsOrdered);
        }
        else if (choice == "2")
        { // Cancel Order
            int orderId;
            cout << "Enter order Id to cancel: ";
            cin >> orderId;
            order.CancelOrder(orderId);
        }
        else if (choice == "3")
        { // Track Order
            int orderId;
            cout << "Enter order Id to track: ";
            cin >> orderId;
            order.TrackOrder(orderId);
        }
        else if (choice == "4")
        {           // Go back
            return; // Exit the loop and return to the previous menu
        }
        else
        {
            cout << "Invalid choice\n";
        }
    }
}

void Menu::DisplayWarehouseMenu()
{
    cout << "------------------------------------------------\n";
    cout << "1. Add Warehouse\n";
    cout << "2. Update Warehouse Details\n";
    cout << "3. Delete Warehouse\n";
    cout << "4. View All Warehouses\n";
    cout << "5. Go Back\n";
    cout << "Enter your choice: ";
}

void Menu::RunWarehouseMenu()
{
    string choice;
    Warehouse warehouse(0, "", 0, 0); // Create a Warehouse object for menu operations

    while (true)
    {
        DisplayWarehouseMenu();
        cin >> choice;

        if (choice == "1")
        { // Add Warehouse
            string location;
            int managerId;

            cout << "Enter warehouse location: ";
            cin.ignore();
            getline(cin, location);
            cout << "Enter manager Id: ";
            cin >> managerId;

            warehouse.AddWarehouse(location, managerId);
        }
        else if (choice == "2")
        { // Update Warehouse Details
            int warehouseId;
            string newLocation;
            int newManagerId;

            cout << "Enter warehouse Id to update: ";
            cin >> warehouseId;
            cout << "Enter new location: ";
            cin.ignore();
            getline(cin, newLocation);
            cout << "Enter new manager Id: ";
            cin >> newManagerId;

            warehouse.UpdateWarehouseDetails(warehouseId, newLocation, newManagerId);
        }
        else if (choice == "3")
        { // Delete Warehouse
            int warehouseId;
            cout << "Enter warehouse Id to delete: ";
            cin >> warehouseId;
            warehouse.DeleteWarehouse(warehouseId);
        }
        else if (choice == "4")
        { // View All Warehouses
            vector<Warehouse> allWarehouses = Warehouse::GetAllWarehouses();
            Warehouse::DisplayAllWarehouses(allWarehouses);
        }
        else if (choice == "5")
        {           // Go back
            return; // Exit the loop and return to the previous menu
        }
        else
        {
            cout << "Invalid choice\n";
        }
    }
}

void Menu::DisplayBranchMenu()
{
    cout << "------------------------------------------------\n";
    cout << "1. Manage Employees\n";
    cout << "2. Handle Customer Inquiries\n";
    cout << "3. View All Branches\n";
    cout << "4. Go Back\n";
    cout << "Enter your choice: ";
}

void Menu::RunBranchMenu()
{
    string choice;
    vector<Branch> branches = Branch::GetAllBranches();

    while (true)
    {
        DisplayBranchMenu();
        cin >> choice;

        if (choice == "1")
        { // Manage Inventory
            int branchId;
            cout << "Enter branch Id: ";
            cin >> branchId;

            // Find the branch in the vector
            bool found = false;
            for (auto &branch : branches)
            {
                if (branch.GetBranchId() == branchId)
                {
                    branch.ManageEmployees();
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                cout << "Branch not found.\n";
            }
        }
        else if (choice == "2")
        { // Handle Customer Inquiries
            RunReturnMenu();
        }
        else if (choice == "3")
        {
            // View All Branches
            vector<Branch> branches = Branch::GetAllBranches();
            Branch::DisplayAllBranches(branches);
        }
        else if (choice == "4")
        {           // Go back
            return; // Exit the loop and return to the previous menu
        }
        else
        {
            cout << "Invalid choice\n";
        }
    }
}

void Menu::DisplayReturnMenu()
{
    cout << "------------------------------------------------\n";
    cout << "1. Process Return\n";
    if (User::GetLoggedInRole() == "manager" || User::GetLoggedInRole() == "employee")
    {
        cout << "2. Refund Customer\n";
        cout << "3. Update Inventory\n";
    }

    cout << "4. Go Back\n";
    cout << "Enter your choice: ";
}

void Menu::RunReturnMenu()
{
    string choice;
    Return returnObject(0, 0, {}, "", "", 0); // Create a Return object for menu operations

    while (true)
    {
        DisplayReturnMenu();
        cin >> choice;

        if (choice == "1")
        { // Process Return
            int returnId;
            cout << "Enter return Id to process: ";
            cin >> returnId;
            returnObject.ProcessReturn(returnId);
        }
        else if (choice == "2" && (User::GetLoggedInRole() == "manager" || User::GetLoggedInRole() == "employee"))
        { // Refund Customer
            int returnId;
            cout << "Enter return Id to refund customer: ";
            cin >> returnId;
            returnObject.RefundCustomer(returnId);
        }
        else if (choice == "3" && (User::GetLoggedInRole() == "manager" || User::GetLoggedInRole() == "employee"))
        { // Update Inventory
            int returnId;
            cout << "Enter return Id to update inventory: ";
            cin >> returnId;
            returnObject.UpdateInventory(returnId);
        }
        else if (choice == "4")
        {           // Go back
            return; // Exit the loop and return to the previous menu
        }
        else
        {
            cout << "Invalid choice\n";
        }
    }
}

void Menu::DisplayInventoryMenu()
{
    cout << "------------------------------------------------\n";
    cout << "1. Add Item to Inventory\n";
    cout << "2. Remove Item from Inventory\n";
    cout << "3. Update Inventory\n";
    cout << "4. Update Stock Levels\n";
    cout << "5. Go Back\n";
    cout << "Enter your choice: ";
}

void Menu::RunInventoryMenu()
{
    string choice;
    Inventory inventory(0, 0, 0); // Create an Inventory object for menu operations

    while (true)
    {
        DisplayInventoryMenu();
        cin >> choice;

        if (choice == "1")
        { // Add Item to Inventory
            int productId, quantity;
            cout << "Enter product Id: ";
            cin >> productId;
            cout << "Enter quantity to add: ";
            cin >> quantity;
            inventory.AddItemToInventory(productId, quantity);
        }
        else if (choice == "2")
        { // Remove Item from Inventory
            int productId, quantity;
            cout << "Enter product Id: ";
            cin >> productId;
            cout << "Enter quantity to remove: ";
            cin >> quantity;
            inventory.RemoveItemFromInventory(productId, quantity);
        }
        else if (choice == "3")
        { // Update Stock Levels
            int productId, difference;
            cout << "Enter product Id: ";
            cin >> productId;
            cout << "Enter new difference: ";
            cin >> difference;
            inventory.UpdateInventory(productId, difference);
        }
        else if (choice == "4")
        { // Update Stock Levels
            int productId, newAvailability, newReorderQuantity;
            cout << "Enter product Id: ";
            cin >> productId;
            cout << "Enter new availability: ";
            cin >> newAvailability;
            cout << "Enter new reorder quantity: ";
            cin >> newReorderQuantity;
            inventory.UpdateStockLevels(productId, newAvailability, newReorderQuantity);
        }
        else if (choice == "5")
        {           // Go back
            return; // Exit the loop and return to the previous menu
        }
        else
        {
            cout << "Invalid choice\n";
        }
    }
}

void Menu::DisplayDiscountMenu()
{
    cout << "------------------------------------------------\n";
    cout << "1. Create Discount\n";
    cout << "2. Apply Discount\n";
    cout << "3. Is Discount applicable\n";
    cout << "4. Go Back\n";
    cout << "Enter your choice: ";
}

void Menu::RunDiscountMenu()
{
    string choice;
    Discount discount(0, "", "", "", "", 0, 0.0); // Create a Discount object for menu operations

    while (true)
    {
        DisplayDiscountMenu();
        cin >> choice;

        if (choice == "1")
        { // Create Discount
            string name, description, startPeriod, endPeriod;
            int productId;
            double percentge;

            cout << "Enter discount name: ";
            cin.ignore();       // Ignore previous newline character in the input
            getline(cin, name); // get the whole name with white spaces

            cout << "Enter discount description: ";
            cin.ignore();
            getline(cin, description);

            cout << "Enter start period (%Y-%m-%d %H:%M:%S): ";
            cin.ignore();
            getline(cin, startPeriod);

            cout << "Enter end period (%Y-%m-%d %H:%M:%S): ";
            cin.ignore();
            getline(cin, endPeriod);

            cout << "Enter product Id: ";
            cin >> productId;

            cout << "Enter discount percentage: ";
            cin >> percentge;

            discount.CreateDiscount(name, description, startPeriod, endPeriod, productId, percentge);
        }
        else if (choice == "2")
        { // Apply Discount
            string discountName;
            cout << "Enter discount name to apply: ";
            cin.ignore();               // Ignore previous newline character in the input
            getline(cin, discountName); // get the whole name with white spaces
            discount.ApplyDiscount(discountName);
        }
        else if (choice == "3")
        { // Expire Discount
            int discountId;
            cout << "Enter discount Id to check expiration: ";
            cin >> discountId;
            if (Discount::IsDiscountValid(discountId))
            {
                cout << "Discount is valid.";
            }
            else
            {
                cout << "Discount is Invalid.";
            }
        }
        else if (choice == "4")
        {           // Go back
            return; // Exit the loop and return to the previous menu
        }
        else
        {
            cout << "Invalid choice\n";
        }
    }
}

void Menu::DisplayPromoMenu()
{
    cout << "------------------------------------------------\n";
    cout << "1. Create Promotion\n";
    cout << "2. Apply Promotion\n";
    cout << "3. Is Promotion applicable\n";
    cout << "4. Go Back\n";
    cout << "Enter your choice: ";
}

void Menu::RunPromoMenu()
{
    string choice;
    Promo promo(0, "", "", 0.0, "", "", 0); // Create a Promo object for menu operations

    while (true)
    {
        DisplayPromoMenu();
        cin >> choice;

        if (choice == "1")
        { // Create Promotion
            string name, description, startPeriod, endPeriod;
            double discountPercentage;
            int productId;

            cout << "Enter promotion name: ";
            cin.ignore();       // Ignore previous newline character in the input
            getline(cin, name); // get the whole name with white spaces

            cout << "Enter promotion description: ";
            cin.ignore();
            getline(cin, description);

            cout << "Enter discount percentage: ";
            cin >> discountPercentage;

            cout << "Enter start period (%Y-%m-%d %H:%M:%S): ";
            cin.ignore(); // Ignore newline character
            getline(cin, startPeriod);

            cout << "Enter end period (%Y-%m-%d %H:%M:%S): ";
            cin.ignore(); // Ignore newline character
            getline(cin, endPeriod);

            cout << "Enter product Id: ";
            cin >> productId;

            promo.CreatePromotion(name, description, discountPercentage, startPeriod, endPeriod, productId);
        }
        else if (choice == "2")
        { // Apply Promotion
            string promoName;
            cout << "Enter promotion name to apply: ";
            cin.ignore(); // Ignore previous newline character in the input
            getline(cin, promoName);
            promo.ApplyPromotion(promoName);
        }
        else if (choice == "3")
        { // Expire Promotion
            int promoId;
            cout << "Enter promotion Id to check expiration: ";
            cin >> promoId;

            if (Promo::IsPromotionValid(promoId))
            {
                cout << "Promotion is valid.";
            }
            else
            {
                cout << "Promotion is Invalid.";
            }
        }
        else if (choice == "4")
        {           // Go back
            return; // Exit the loop and return to the previous menu
        }
        else
        {
            cout << "Invalid choice\n";
        }
    }
}

void Menu::DisplayTransactionMenu()
{
    cout << "------------------------------------------------\n";
    cout << "1. Add Transaction\n";
    cout << "2. Calculate Total\n";
    cout << "3. Retrieve Transaction Details\n";
    cout << "4. Go Back\n";
    cout << "Enter your choice: ";
}

void Menu::RunTransactionMenu()
{
    string choice;
    Transaction transaction(0, "", "", 0, 0, 0.0); // Create a Transaction object for menu operations

    while (true)
    {
        DisplayTransactionMenu();
        cin >> choice;

        if (choice == "1")
        {
            string timestamp, type;
            int customerId, branchId;
            double totalAmount;

            cout << "Enter timestamp for the transaction (%Y-%m-%d %H:%M:%S): ";
            cin.ignore();
            getline(cin, timestamp);
            cout << "Enter type of transaction: ";
            cin.ignore();
            getline(cin, type);
            cout << "Enter customer Id: ";
            cin >> customerId;
            cout << "Enter branch Id: ";
            cin >> branchId;
            cout << "Enter total amount: ";
            cin >> totalAmount;

            // Create a Transaction object and add the transaction
            Transaction transaction(0, "", "", 0, 0, 0.0);
            transaction.AddTransaction(timestamp, type, customerId, branchId, totalAmount);
        }
        else if (choice == "2")
        { // Calculate Total
            double total = transaction.CalculateTotal();
            cout << "Total amount: " << total << endl;
        }
        else if (choice == "3")
        { // Retrieve Transaction Details
            transaction.RetrieveTransactionDetails();
        }
        else if (choice == "4")
        {           // Go back
            return; // Exit the loop and return to the previous menu
        }
        else
        {
            cout << "Invalid choice\n";
        }
    }
}

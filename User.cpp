#include "User.h"
#include "sqlite\sqlite3.h"
#include <iostream>
using namespace std;

// initialize the static LoggedInUserName
string User::loggedInUsername = "";
string User::loggedInRole = "";

// constructor to initialize user attributes
User::User() {}

User::User(int id, const string &username, const string &password, const string &email,
           const string &phone, const string &address, const string &role)
    : Id(id), Username(username), Password(password), Email(email), Phone(phone), Address(address), Role(role) {}

bool User::IsUsernameUnique(const string &username)
{
    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return false;
    }

    // Check if the username already exists
    sqlite3_stmt *checkStatement;
    if (sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM Users WHERE Username = ?", -1, &checkStatement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare check statement\n";
        sqlite3_close(db);
        return false;
    }

    sqlite3_bind_text(checkStatement, 1, username.c_str(), -1, SQLITE_STATIC);
    int result = sqlite3_step(checkStatement);
    if (result != SQLITE_ROW)
    {
        cerr << "Failed to execute check statement\n";
        sqlite3_finalize(checkStatement);
        sqlite3_close(db);
        return false;
    }
    int userCount = sqlite3_column_int(checkStatement, 0);
    sqlite3_finalize(checkStatement);

    // If username already exists, return without registering
    if (userCount > 0)
    {
        cerr << "Username already exists\n";
        sqlite3_close(db);
        return false;
    }

    return true;
}

void User::SetUsername(const string &username)
{
    this->Username = username;
}

void User::SetPassword(const string &password)
{
    this->Password = password;
}

void User::SetEmail(const string &email)
{
    this->Email = email;
}

void User::SetPhone(const string &phone)
{
    this->Phone = phone;
}

void User::SetAddress(const string &address)
{
    this->Address = address;
}

string User::GetPassword() const
{
    return Password;
}

string User::GetPhone() const
{
    return Phone;
}

string User::GetAddress() const
{
    return Address;
}

string User::GetRole() const
{
    return Role;
}

string User::GetUsername() const
{
    return Username;
}

int User::GetId() const
{
    return Id;
}

string User::GetEmail() const
{
    return Email;
}

// register a new user
void User::Register(const string &username, const string &password, const string &email,
                    const string &phone, const string &address, const string &role)
{

    if (!User::IsUsernameUnique(username))
    {
        return;
    }

    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return;
    }

    // Prepare SQL statement for registration
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "INSERT INTO Users (Username, Password, Email, Phone, Address, Role) VALUES (?, ?, ?, ?, ?, ?)", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare registration statement\n";
        sqlite3_close(db);
        return;
    }

    // Bind parameters
    sqlite3_bind_text(statement, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 2, password.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 3, email.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 4, phone.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 5, address.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 6, role.c_str(), -1, SQLITE_STATIC);

    // Execute statement
    if (sqlite3_step(statement) != SQLITE_DONE)
    {
        cerr << "Failed to execute statement\n";
        sqlite3_close(db);
        return;
    }

    // Get the UserId of the newly inserted user
    int userId = sqlite3_last_insert_rowid(db);

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    cout << "User registered successfully with id: " << userId << "\n";
}

// check if the username and password match a record in the database
User *User::Login(const string &username, const string &password)
{
    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return nullptr;
    }

    // Prepare SQL statement
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "SELECT * FROM Users WHERE Username = ? AND Password = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare statement\n";
        sqlite3_close(db);
        return nullptr;
    }

    // Bind parameters
    sqlite3_bind_text(statement, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 2, password.c_str(), -1, SQLITE_STATIC);

    // Execute statement
    if (sqlite3_step(statement) == SQLITE_ROW)
    {
        int userId = sqlite3_column_int(statement, 0);                                          // first column is Id
        string userEmail = reinterpret_cast<const char *>(sqlite3_column_text(statement, 3));   // fourth column is Email
        string userPhone = reinterpret_cast<const char *>(sqlite3_column_text(statement, 4));   // fifth column is Phone
        string userAddress = reinterpret_cast<const char *>(sqlite3_column_text(statement, 5)); // sixth column is Address
        string userRole = reinterpret_cast<const char *>(sqlite3_column_text(statement, 6));    // seventh column is Role

        sqlite3_finalize(statement);
        sqlite3_close(db);
        loggedInUsername = username;
        loggedInRole = userRole;
        return new User(userId, username, password, userEmail, userPhone, userAddress, userRole);
    }

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);
    return nullptr;
}

// log out
void User::Logout()
{
    loggedInUsername = "";
    loggedInRole = "";
}

// check if any user is logged in
bool User::IsAnyUserLoggedIn()
{
    return !loggedInUsername.empty() && !loggedInRole.empty();
}

int User::GetUserIdByUsername(const string &username)
{
    // Initialize user Id to -1 (indicating not found)
    int userId = -1;

    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return userId;
    }

    // Prepare SQL statement for selecting user Id by username
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "SELECT Id FROM Users WHERE Username = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare select statement\n";
        sqlite3_close(db);
        return userId;
    }

    // Bind parameter (username)
    sqlite3_bind_text(statement, 1, username.c_str(), -1, SQLITE_STATIC);

    // Execute statement
    if (sqlite3_step(statement) == SQLITE_ROW)
    {
        // Retrieve user Id from the query result
        userId = sqlite3_column_int(statement, 0);
    }

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    // Return the retrieved user Id (or -1 if not found)
    return userId;
}

// get the current logged in username
const string &User::GetLoggedInUsername()
{
    return loggedInUsername;
}

// get the current logged in role
const string &User::GetLoggedInRole()
{
    return loggedInRole;
}

void User::SetLoggedInUsername(const string &username)
{
    loggedInUsername = username;
}

void User::SetLoggedInRole(const string &role)
{
    loggedInRole = role;
}

// change user's password
void User::ChangePassword(const string &newPassword)
{
    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return;
    }

    // Prepare SQL statement
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "UPDATE Users SET Password = ? WHERE Id = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare statement\n";
        sqlite3_close(db);
        return;
    }

    // Bind parameters
    sqlite3_bind_text(statement, 1, newPassword.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(statement, 2, Id);

    // Execute statement
    if (sqlite3_step(statement) != SQLITE_DONE)
    {
        cerr << "Failed to execute statement\n";
        sqlite3_close(db);
        return;
    }

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    Password = newPassword;
    cout << "Password changed successfully\n";
}

// update user's profile information
void User::UpdateProfile(const string &newEmail, const string &newPhone, const string &newAddress)
{
    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return;
    }

    // Prepare SQL statement
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "UPDATE Users SET Email = ?, Phone = ?, Address = ? WHERE Id = ?", -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare statement\n";
        sqlite3_close(db);
        return;
    }

    // Bind parameters
    sqlite3_bind_text(statement, 1, newEmail.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 2, newPhone.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(statement, 3, newAddress.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(statement, 4, Id);

    // Execute statement
    if (sqlite3_step(statement) != SQLITE_DONE)
    {
        cerr << "Failed to execute statement\n";
        sqlite3_close(db);
        return;
    }

    // Finalize statement and close database
    sqlite3_finalize(statement);
    sqlite3_close(db);

    Email = newEmail;
    Phone = newPhone;
    Address = newAddress;

    cout << "Profile updated successfully\n";
}

User User::GetUserById(int userId)
{
    User user;

    // Open the database
    sqlite3 *db;
    if (sqlite3_open("store_management.db", &db) != SQLITE_OK)
    {
        cerr << "Can't open database\n";
        return user;
    }

    // Prepare SQL statement for selecting user by userId
    sqlite3_stmt *statement;
    string query = "SELECT * FROM Users WHERE Id = ?";
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &statement, nullptr) != SQLITE_OK)
    {
        cerr << "Failed to prepare statement\n";
        sqlite3_close(db);
        return user;
    }

    // Bind userId parameter
    if (sqlite3_bind_int(statement, 1, userId) != SQLITE_OK)
    {
        cerr << "Failed to bind userId\n";
        sqlite3_finalize(statement);
        sqlite3_close(db);
        return user;
    }

    // Execute the statement
    if (sqlite3_step(statement) == SQLITE_ROW)
    {
        user.Id = (sqlite3_column_int(statement, 0));
        user.Username = (reinterpret_cast<const char *>(sqlite3_column_text(statement, 1)));
        user.Password = (reinterpret_cast<const char *>(sqlite3_column_text(statement, 2)));
        user.Email = (reinterpret_cast<const char *>(sqlite3_column_text(statement, 3)));
        user.Phone = (reinterpret_cast<const char *>(sqlite3_column_text(statement, 4)));
        user.Address = (reinterpret_cast<const char *>(sqlite3_column_text(statement, 5)));
        user.Role = (reinterpret_cast<const char *>(sqlite3_column_text(statement, 6)));
    }

    // Finalize the statement
    sqlite3_finalize(statement);

    // Close the database
    sqlite3_close(db);

    return user;
}
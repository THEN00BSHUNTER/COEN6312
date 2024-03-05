#ifndef USERS_H
#define USERS_H

#include <string>

using namespace std;

class User
{
protected:
    int Id;
    string Username;
    string Password;
    string Email;
    string Phone;
    string Address;
    string Role;
    static string loggedInUsername; // static variable for username of the logged-in user
    static string loggedInRole;     // static variable for role

public:
    User();
    User(int id, const string &username, const string &password, const string &email,
         const string &phone, const string &address, const string &role);

    void SetUsername(const string &username);
    void SetPassword(const string &password);
    void SetEmail(const string &email);
    void SetPhone(const string &phone);
    void SetAddress(const string &address);

    int GetId() const;
    string GetUsername() const;
    string GetPassword() const;
    string GetEmail() const;
    string GetPhone() const;
    string GetAddress() const;
    string GetRole() const;

    // Functions
    static void Register(const string &username, const string &password, const string &email, const string &phone, const string &address, const string &role);
    static User *Login(const string &username, const string &password);
    static bool IsUsernameUnique(const string &username);
    void ChangePassword(const string &newPassword);
    void UpdateProfile(const string &newEmail, const string &newPhone, const string &newAddress);
    static User GetUserById(int userId);
    void Logout();

    // static methods for checking logging in of the user
    static int GetUserIdByUsername(const string &username);
    static bool IsAnyUserLoggedIn();
    static const string &GetLoggedInUsername();
    static const string &GetLoggedInRole();

    static void SetLoggedInUsername(const string &username);
    static void SetLoggedInRole(const string &username);
};

#endif

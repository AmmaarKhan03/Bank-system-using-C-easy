#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
using namespace std;

// Class representing a bank account.
class Account {
private:
    int account_number;  // Unique account number
    string username;     // Owner's username (must be unique)
    string password;     // Account password
    double balance;      // Current balance

    static string maskAccountNumber(int accNum);

public:
    // Constructor (no default constructor)
    Account(int accNumber, const string& ownerName, const string& pswrd, double initialDeposit);

    Account()
        : account_number(0), username(""), password(""), balance(0.0)
    {
        // Possibly log or leave empty
    }
    void changePassword(const string& newPassword);


    // New: Getter for password (for saving purposes)
    string getPassword() const;

    // Getters
    int getAccountNumber() const;
    string getUsername() const;
    double getBalance() const;

    // Verify password
    bool checkPasscode(const string& pswrd) const;

    // Account operations
    void deposit(double amount);
    bool withdraw(double amount);
    void display() const;
};

// The banking system class.
class system1 {
private:
    // Use an unordered_map for fast lookup by account number.
    unordered_map<int, Account> accounts;

    // Helper functions
    bool usernameExists(const string& uname) const;
    int generateUniqueAccountNumber();

    // New: Functions for persistence
    void loadAccounts();
    void saveAccounts();

public:
    system1();
    ~system1();  // Destructor will save accounts on exit

    // Creates a new account; enforces unique username and random account number.
    void create_account();
    Account* searchByUsername(const string& uname);


    // Logs into an account; returns pointer if successful, nullptr otherwise.
    Account* logIn();

    // Closes an account.
    void closeAccount();

    // Menu for a logged-in account.
    void accountMenu(Account* account);

    // (Optional) Displays accounts sorted by balance.
    void displayAccountsSortedByBalance();

    // (Optional) Searches for an account by balance.
    Account* searchAccountByBalance(double threshold);
};

#endif

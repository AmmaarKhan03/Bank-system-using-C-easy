#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <random>
#include <sstream>  // For string stream operations
#include "utility.h"
#include "FinalUtility.h"  // Include only if needed
using namespace std;

// -----------------------------
// Account member function implementations
// -----------------------------
Account::Account(int accNumber, const string& ownerName, const string& pswrd, double initialDeposit)
    : account_number(accNumber), username(ownerName), password(pswrd), balance(initialDeposit) {
}

string Account::getPassword() const {
    return password;
}
void Account::changePassword(const string& newPassword) {
    if (!newPassword.empty()) {
        password = newPassword;
    }
}


int Account::getAccountNumber() const {
    return account_number;
}

string Account::getUsername() const {
    return username;
}

double Account::getBalance() const {
    return balance;
}

bool Account::checkPasscode(const string& pswrd) const {
    return password == pswrd;
}

void Account::deposit(double amount) {
    if (amount > 0) {
        balance += amount;
    }
}

bool Account::withdraw(double amount) {
    if (amount > 0 && amount <= balance) {
        balance -= amount;
        return true;
    }
    return false;
}
string Account::maskAccountNumber(int accNum) {
    string accStr = to_string(accNum);
    string masked(accStr.size(), '*');
    if (accStr.size() > 2) {
        // Show only the last two digits.
        masked.replace(masked.size() - 2, 2, accStr.substr(accStr.size() - 2, 2));
    }
    return masked;
}

void Account::display() const {
    cout << "\nAccount Number: " << maskAccountNumber(account_number) << "\n";
    cout << "Owner: " << username << "\n";
    cout << "Balance: $" << balance << "\n";
}

// -----------------------------
// system1 member function implementations
// -----------------------------

// Constructor: Load accounts from file.
system1::system1() {
    loadAccounts();
}

// Destructor: Save accounts to file.
system1::~system1() {
    saveAccounts();
}

// Helper: Check if a username already exists.
bool system1::usernameExists(const string& uname) const {
    for (const auto& pair : accounts) {
        if (pair.second.getUsername() == uname)
            return true;
    }
    return false;
}

// Helper: Generate a unique random account number.
int system1::generateUniqueAccountNumber() {
    int randomAccountNumber;
    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<int> dist(1000, 9999);
    do {
        randomAccountNumber = dist(mt);
    } while (accounts.find(randomAccountNumber) != accounts.end());
    return randomAccountNumber;
}

// Persistence: Load accounts from "accounts.txt".
void system1::loadAccounts() {
    ifstream infile("accounts.txt");
    if (!infile.is_open()) {
        // No saved data yet.
        return;
    }
    int accNum;
    string uname, pwd;
    double bal;
    while (infile >> accNum >> uname >> pwd >> bal) {
        Account acc(accNum, uname, pwd, bal);
        accounts[accNum] = acc;
    }
    infile.close();
}

// Persistence: Save accounts to "accounts.txt".
void system1::saveAccounts() {
    ofstream outfile("accounts.txt");
    if (!outfile.is_open()) {
        cerr << "Error saving accounts data!" << endl;
        return;
    }
    // Write each account on a separate line.
    for (const auto& pair : accounts) {
        const Account& acc = pair.second;
        outfile << acc.getAccountNumber() << " "
            << acc.getUsername() << " "
            << acc.getPassword() << " "
            << acc.getBalance() << "\n";
    }
    outfile.close();
}

// Creates a new account.
void system1::create_account() {
    string name, pwd;
    double deposit;

    cout << "\n-------- OPEN NEW ACCOUNT --------\n";
    cout << "Enter your username: ";
    cin >> name;

    if (usernameExists(name)) {
        cout << "Username already exists. Please choose a different username.\n";
        return;
    }

    cout << "Enter a password: ";
    cin >> pwd;
    cout << "Enter deposit amount: ";
    cin >> deposit;

    int uniqueAccountNumber = generateUniqueAccountNumber();
    Account newAccount(uniqueAccountNumber, name, pwd, deposit);
    accounts[uniqueAccountNumber] = newAccount;

    cout << "Account created successfully! Your account number is: " << uniqueAccountNumber << "\n";
    // Save immediately after creation.
    saveAccounts();
}

// Logs into an account.
Account* system1::logIn() {
    int accNum;
    string pwd;

    cout << "\n---- LOG IN ----\n";
    cout << "Enter your account number: ";
    cin >> accNum;
    cout << "Enter your password: ";
    cin >> pwd;

    auto it = accounts.find(accNum);
    if (it != accounts.end() && it->second.checkPasscode(pwd)) {
        cout << "Log in successful.\n";
        return &(it->second);
    }
    cout << "Invalid account number or password.\n";
    return nullptr;
}

// Closes an account.
void system1::closeAccount() {
    int accNum;
    cout << "\n-------- CLOSE AN ACCOUNT --------\n";
    cout << "Enter the account number to close: ";
    cin >> accNum;

    auto it = accounts.find(accNum);
    if (it != accounts.end()) {
        accounts.erase(it);
        cout << "Account " << accNum << " closed successfully.\n";
        saveAccounts();
    }
    else {
        cout << "Account not found.\n";
    }
}

// Menu for a logged-in account.
void system1::accountMenu(Account* account) {
    if (account == nullptr) {
        cout << "Error: Invalid account." << endl;
        return;
    }

    int choice = -1;
    while (choice != 0) {
        cout << "\n--- Account Menu ---\n";
        cout << "1. Deposit\n";
        cout << "2. Withdraw\n";
        cout << "3. Display Account Information\n";
        cout << "4. Change Password\n";
        cout << "0. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }

        switch (choice) {
        case 1: {
            double depositAmount;
            cout << "Enter deposit amount: ";
            cin >> depositAmount;
            if (!cin.fail() && depositAmount > 0) {
                account->deposit(depositAmount);
                cout << "Deposited: $" << depositAmount << endl;
            }
            else {
                cout << "Invalid deposit amount entered." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            saveAccounts();
            break;
        }
        case 2: {
            double withdrawAmount;
            cout << "Enter withdrawal amount: ";
            cin >> withdrawAmount;
            if (!cin.fail() && withdrawAmount > 0) {
                if (account->withdraw(withdrawAmount)) {
                    cout << "Withdrawal successful." << endl;
                }
                else {
                    cout << "Insufficient funds or withdrawal failed." << endl;
                }
            }
            else {
                cout << "Invalid withdrawal amount entered." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            saveAccounts();
            break;
        }
        case 3:
            account->display();
            break;
        case 4: {
            string newPwd;
            cout << "Enter new password: ";
            cin >> newPwd;
            account->changePassword(newPwd);
            cout << "Password updated successfully.\n";
            saveAccounts();
            break;
        }
        case 0:
            cout << "Logging out..." << endl;
            break;
        default:
            cout << "Invalid option. Please select again." << endl;
            break;
        }
    }
}

// (Optional) Displays all accounts sorted by balance.
void system1::displayAccountsSortedByBalance() {
    if (accounts.empty()) {
        cout << "No accounts available to display." << endl;
        return;
    }

    vector<Account> sortedAccounts;
    for (const auto& pair : accounts) {
        sortedAccounts.push_back(pair.second);
    }

    sort(sortedAccounts.begin(), sortedAccounts.end(),
        [](const Account& a, const Account& b) {
            return a.getBalance() < b.getBalance();
        });

    cout << "\n--- Accounts Sorted by Balance ---\n";
    for (const auto& acc : sortedAccounts) {
        acc.display();
        cout << "--------------------------\n";
    }
}
Account* system1::searchByUsername(const string& uname) {
    for (auto& pair : accounts) {
        if (pair.second.getUsername() == uname)
            return &pair.second;
    }
    return nullptr;
}


// (Optional) Searches for the first account with a balance >= threshold using binary search.
Account* system1::searchAccountByBalance(double threshold) {
    vector<Account> sortedAccounts;
    for (const auto& pair : accounts) {
        sortedAccounts.push_back(pair.second);
    }
    sort(sortedAccounts.begin(), sortedAccounts.end(),
        [](const Account& a, const Account& b) {
            return a.getBalance() < b.getBalance();
        });

    auto it = std::lower_bound(sortedAccounts.begin(), sortedAccounts.end(), threshold,
        [](const Account& acc, double value) {
            return acc.getBalance() < value;
        });

    if (it != sortedAccounts.end()) {
        int accNum = it->getAccountNumber();
        auto found = accounts.find(accNum);
        if (found != accounts.end()) {
            return &(found->second);
        }
    }
    return nullptr;
}

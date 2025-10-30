#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "FinalUtility.h"
#include "utility.h"
using namespace std;

void displayBank() {
    cout << "      +============================================================================+\n"
        << "      +--------------------------Welcome to Bank System----------------------------+\n"
        << "      +----------------------------------------------------------------------------+\n"
        << "      +****************************************************************************+\n\n"
        << "      Project By: Ammaar Khan\n";
}

void bankMenu() {
    system1 bank;
    int option;

    do {
        cout << "\n                                  :: Main Menu ::\n\n"
            << "                          1. OPEN NEW ACCOUNT\n"
            << "                          2. LOG IN\n"
            << "                          3. CLOSE ACCOUNT\n"
            << "                          4. DISPLAY ACCOUNTS SORTED BY BALANCE\n"
            << "                          5. SEARCH ACCOUNT BY BALANCE\n"
            << "                          6. SEARCH ACCOUNT BY USERNAME\n"
            << "                          0. EXIT\n";

        cout << "\nChoose an option: ";
        cin >> option;

        switch (option) {
        case 1:
            cout << "Setting up new account...\n";
            bank.create_account();
            break;
        case 2: {
            Account* loggedInAccount = bank.logIn();
            if (loggedInAccount != nullptr) {
                bank.accountMenu(loggedInAccount);
            }
            break;
        }
        case 3:
            bank.closeAccount();
            break;
        case 4:
            bank.displayAccountsSortedByBalance();
            break;
        case 5: {
            double threshold;
            cout << "Enter the balance threshold to search for: ";
            cin >> threshold;
            Account* foundAccount = bank.searchAccountByBalance(threshold);
            if (foundAccount) {
                cout << "Found an account with a balance >= " << threshold << ":\n";
                foundAccount->display();
            }
            else {
                cout << "No account found with a balance >= " << threshold << ".\n";
            }
            break;
        }
        case 6: {
            string uname;
            cout << "Enter username to search for: ";
            cin >> uname;
            Account* found = bank.searchByUsername(uname);
            if (found) {
                cout << "Account found:\n";
                found->display();
            }
            else {
                cout << "No account found with that username.\n";
            }
            break;
        }

        case 0:
            cout << "Exiting the system. Goodbye!\n";
            break;
        default:
            cout << "Invalid choice, please select again.\n";
            break;
        }
    } while (option != 0);
}

#include "bstNode.h"

void displayAdminOptions();
void adminMenu(BST& bank);
void displayCustomerOptions();
void customerMenu(BST& bank);

int main()
{
    BST bank;   // create a bank object for the BST class

    // read and import accounts' information into the bank system
    bank.readAccountsFromFile("admin_accounts.txt");
    bank.readAccountsFromFile("customer_accounts.txt");

    cout << std::fixed << std::setprecision(2);

    int choice;

    do
    {   
        // display options that the user can choose.
        cout << "Welcome to the Bank Management System\n";
        cout << "1. Admin Login\n";
        cout << "2. Customer Login\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // use a switch statement to display the options menu for a specific user
        switch (choice)
        {
            case 1:
                adminMenu(bank);
                break;
            case 2:
                customerMenu(bank);
                break;
            case 3:
                cout << "Thank you for using the Bank Management System. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 3);

    bank.writeAccountsToFile();

    return 0;

}
// this function displays all the options for admin accounts.
void displayAdminOptions()      
{
    cout << "Admin Menu\n";
    cout << "1. Add Account\n";
    cout << "2. Delete Account\n";
    cout << "3. View All Accounts\n";
    cout << "4. View Customer Information and Balances\n";
    cout << "5. View Transaction History\n";
    cout << "6. Logout\n";
    cout << "Enter your choice: ";
}
// this function allows the admin to manage the bank system.
void adminMenu(BST& bank)     
{
    
    int accountNum;
    int pin;
    // verify admin logins by using their account number and PIN
    cout << "Enter your account number: ";
    cin >> accountNum;
    cout << "Enter your PIN: ";
    cin >> pin;
    Account *adminAccount = bank.searchAccount(accountNum);
    if (adminAccount == nullptr || adminAccount->getPinNum() != pin)
    {
        cout << "Invalid account number or PIN. Exiting admin menu\n";
        return;
    }


    cout << "\n\tSuccessfully logged in the admin acount " << accountNum << '\n';

    int choice;
    // allow the admin to manage the bank system if their login is valid.
    do
    {
        cout << "\tHello, " << adminAccount->getName() << '\n';

        displayAdminOptions();
        cin >> choice;
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice)
        {
            case 1:
                bank.addAccount();
                break;
            case 2:
            {
                int accountNumber;
                cout << "Enter the account number:  ";
                cin >> accountNumber;
                bool isDeleted = bank.deleteAccount(accountNumber);
                if (isDeleted == true)
                    cout << "The account " << accountNumber << " is deleted.\n";
                break;
            }
            case 3:
                bank.displayAllAccounts();
                break;
            case 4:
            {
                int accountNumSearch;
                cout << "Enter the account number to search: ";
                cin >> accountNumSearch;
                if (bank.searchAccount(accountNumSearch) != nullptr)
                {
                    char answer;
                    cout << "Do you want to display the information of the account? (y/n): ";
                    cin >> answer;
                    answer = toupper(answer);
                    if (answer == 'Y')
                        bank.displayAccountInfo(accountNumSearch);
                }
                else
                {
                    cout << "The account number " << accountNumSearch << " does not exist. Please try other account number.\n";
                }
                break;
            }
            case 5:
                cout << "The feature is not available.\n";
                break;
            case 6:
                cout << "Logging out from admin account.\n";
                cout << "Goodbye, " << adminAccount->getName() << "!\n";
                return;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 6);
}
// this function displays all the options for customer accounts.
void displayCustomerOptions()   
{
    cout << "Customer Menu\n";
    cout << "1. View Account Balance\n";
    cout << "2. Deposit\n";
    cout << "3. Withdraw\n";
    cout << "4. View Transaction History\n";
    cout << "5. Exchange Currencies\n";
    cout << "6. Create Child Account\n";
    cout << "7. Manage Chile Account\n";
    cout << "8. Logout\n";
    cout << "Enter your choice: ";
}
// verify customer logins by using their account number and PIN
void customerMenu(BST& bank)        
{
    int accountNum;
    int pin;
    // verify the customer login by using their account number and PIN.
    cout << "Enter your account number: ";
    cin >> accountNum;
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cout << "Enter your PIN: ";
    cin >> pin;
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    Account *customerAccount = bank.searchAccount(accountNum);
    if (customerAccount == nullptr || customerAccount->getPinNum() != pin)
    {
        cout << "Invalid account number or PIN. Exiting customer menu\n";
        return;
    }

    cout << "\nSuccessfully logged in the admin acount " << accountNum << '\n';

    int choice; 
    do
    {
        cout << "\tHello, " << customerAccount->getName() << '\n';

        displayCustomerOptions();
        cin >> choice;
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice)
        {
            case 1:
                cout << "Balance: " << customerAccount->getBalance();
                break;
            case 2:
            {
                double amount;
                cout << "Enter the amount to deposit: ";
                cin >> amount;
                customerAccount->deposit(amount);
                break;
            }
            case 3:
            {
                double amount;
                cout << "Enter the amount to withdraw: ";
                cin >> amount;
                customerAccount->withdraw(amount);
                break;
            }
            case 4:
                cout << "The feature is not available.\n";
                break;
            case 5:
            {
                double amount;
                cout << "Enter the amount to exchange: ";
                cin >> amount;
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                string currency;
                cout << "Enter the currency to exchange (GBP or EUR): ";
                cin >> currency;
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::transform(currency.begin(), currency.end(), currency.begin(), ::toupper);

                while (currency != "GBP" && currency != "EUR")
                {
                    cout << "The currency is invalid.\n";
                    cout << "Enter the amount to exchange: ";
                    cin >> amount;
                    cin.clear();
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                    cout << "Enter the currency to exchange (GBP or EUR): ";
                    cin >> currency;
                    cin.clear();
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                    std::transform(currency.begin(), currency.end(), currency.begin(), ::toupper);
                }

                customerAccount->exchangeCurrency(amount, currency);
                break;
            }
            case 6:
                bank.createChildAccount(customerAccount);
                break;
            case 7:
                bank.manageChildAccount(customerAccount);
                break;
            case 8:
                cout << "Logging out from customer account.\n";
                cout << "Goodbye, " << customerAccount->getName() << "!\n";
                return;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 8);
}
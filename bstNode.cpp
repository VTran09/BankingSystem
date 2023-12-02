#include "bstNode.h"

// default constructor
Account::Account()
{
    name = "";
    address = "";
    accountNum = 0;
    accountType = "";
    systemAccountType = "";
    pinNum = 0;
    balance = 0;
}
// constructor for the Account class
Account::Account(string Name, string Address, string SystemAccountType, string AccountType, int AccountNum, int PinNum, double Balance)
{
    this->name = Name;
    this->address = Address;
    this->accountNum = AccountNum;
    this->systemAccountType = SystemAccountType;
    this->accountType = AccountType;
    this->pinNum = PinNum;
    this->balance = Balance;
}
// function writes each node's information into a text file.
void Account::writeToFile(std::ofstream &outFile) const
{
    outFile << std::fixed << std::setprecision(2);

    outFile << accountNum << ',' << systemAccountType << ',' << accountType << ',' << name << ',' << address << ',' << pinNum << ',' << balance << '\n';

}
// function is used to deposit money into an account's balance
void Account::deposit(double amount)
{
    balance += amount;
    cout << "Current USD balance: " << balance << '\n';
}
// function is used to withdraw money from an account's balance
void Account::withdraw(double amount)
{
    if (amount > balance)
    {
        cout << "Insufficient balance or invalid currency type. Withdrawal failed." << '\n';
        return;
    }

    balance -= amount;
    cout << "Current USD balance: " << balance << '\n';
}
// function is used by customers to exchange currencies
void Account::exchangeCurrency(double amount, const string &toCurrency)
{
    // Exchange rates. You should adjust these based on real exchange rates.
    const double usdToGbpRate = 0.7;
    const double usdToEurRate = 0.9;


    if (toCurrency == "GBP")
    {
    double exchangeAmount = amount * usdToGbpRate;
    if (balance >= amount)
        {
            balance -= amount;
            double gbpBalance = exchangeAmount;
            cout << "Current USD balance: " << balance << '\n';
            cout << "Current GBP balance: " << gbpBalance << '\n';
            return;
        }
    }
    else if (toCurrency == "EUR")
    {
        double exchangeAmount = amount * usdToEurRate;
        if (balance >= amount)
        {
            balance -= amount;
            double eurBalance = exchangeAmount;
            cout << "Current USD balance: " << balance << '\n';
            cout << "Current EUR balance: " << eurBalance << '\n';
            return;
        }
    }

    cout << "Insufficient balance or invalid currency type. Currency exchange failed." << '\n';
}
// constructor for the Node class
Node::Node(Account *acc)
{
    account = acc;
    left = nullptr;
    right = nullptr;
}
// remove the child account from the children vector that is contained in their parent account node.
void Node::removeChildAccount(Account *accountToRemove)
{
    auto it = std::find(children.begin(), children.end(), accountToRemove);

    if (it != children.end())
    {
        children.erase(it);
        delete accountToRemove;
    }
}
// function to insert a node for adding an account
void BST::insert(Node *node, Node *currentRoot)
{
    if (node->getAccount()->getAccountNum() <  currentRoot->getAccount()->getAccountNum())
    {
        if (currentRoot->getLeftChild() == nullptr)
        {
            currentRoot->setLeftChild(node);
            node->setParentNode(currentRoot);
        }
        else
        {
            insert(node, currentRoot->getLeftChild());
        }
    }
    else if (node->getAccount()->getAccountNum() >  currentRoot->getAccount()->getAccountNum())
    {
        if (currentRoot->getRightChild() == nullptr)
        {
            currentRoot->setRightChild(node);
            node->setParentNode(currentRoot);
        }
        else
        {
            insert(node, currentRoot->getRightChild());
        }
    }
    else
    {
        cout << "The account number already exists.\n"; 
    }
}
// function to search aa node in BST
Node *BST::search(const int accountNumber, Node *currentRoot)
{
    if (currentRoot == nullptr || accountNumber == currentRoot->getAccount()->getAccountNum())
        return currentRoot;
    
    if (accountNumber < currentRoot->getAccount()->getAccountNum())
        return search(accountNumber, currentRoot->getLeftChild());
    else
        return search(accountNumber, currentRoot->getRightChild());
}
// function to delete a node in BST
void BST::deleteNode(Node *node)
{
    if (node->isLeaf() == true) 
    {
        Node *parent = node->getParentNode();
        if (parent)
        {
            if (parent->getLeftChild() == node)
                parent->setLeftChild(nullptr);
            else
                parent->setRightChild(nullptr);
        }
        else
        {
            root = nullptr;
        }
        delete node;
    }
    else if (node->getLeftChild() == nullptr)
    {
        Node *parent = node->getParentNode();
        if (parent)
        {
            if (parent->getLeftChild() == node)
                parent->setLeftChild(node->getRightChild());
            else
                parent->setRightChild(node->getRightChild());

            node->getRightChild()->setParentNode(parent);
        }
        else
        {
            root = node->getRightChild();
            root->setParentNode(nullptr);
        }
        delete node;
    }
    else if (node->getRightChild() == nullptr)
    {
        Node *parent = node->getParentNode();
        if (parent)
        {
            if (parent->getLeftChild() == node)
                parent->setLeftChild(node->getLeftChild());
            else
                parent->setRightChild(node->getLeftChild());

            node->getLeftChild()->setParentNode(parent);
        }
        else
        {
            root == node->getLeftChild();
        }

        delete node;
    }
    else
    {
        Node *minimum = node->getRightChild();
        while (minimum->getLeftChild() != nullptr)
        {
            minimum = minimum->getLeftChild();
        }

        const std::vector<Account*> childAccounts = minimum->getChildren();
        node->removeChildAccounts();
        for (const auto& account : childAccounts)
        {
            node->addChildAccount(account);
        }
        node->setAccount(minimum->getAccount());
        node->setParentAccount(minimum->getParentAccount());
        deleteNode(minimum);
    }
}
// function to check account's system account type to write its info to the relevant text file
void BST::writeAccountsToFile(Node *node, std::ofstream &adminOutFile, std::ofstream &customerOutFile) const
{
    if (node == nullptr)
    {
        return;
    }

    Account *account = node->getAccount();
    if (account)
    {
        if (account->getSystemAccountType() == "ADMIN")
        {
            account->writeToFile(adminOutFile);
        }
        else if (account->getSystemAccountType() == "CUSTOMER")
        {
            account->writeToFile(customerOutFile);
        }
    }

    writeAccountsToFile(node->getLeftChild(), adminOutFile, customerOutFile);
    writeAccountsToFile(node->getRightChild(), adminOutFile, customerOutFile);
}
// display all accounts' information for the admin
void BST::displayAllAccounts(Node *node) const
{
    if (node == nullptr)
    {
        return;
    }

    Account *account = node->getAccount();
    if (account)
    {
        cout << "Account Number: " << account->getAccountNum() << '\n';
        cout << "Account Name: " << account->getName() << '\n';
        cout << "System Account Type: " << account->getSystemAccountType() << '\n';
        cout << "Account Type: " << account->getAccountType() << '\n';
        cout << "Account Balance: " << account->getBalance() << '\n';
        cout << '\n';
    }

    displayAllAccounts(node->getLeftChild());
    displayAllAccounts(node->getRightChild());
}
// function for a parent to create child accounts for their children.
void BST::createChildAccount(Node *node, Account *parentAccount)
{
    if (node == nullptr)
    {
        return;
    }

    if (node->getAccount() == parentAccount)
    {
        if (parentAccount->getAccountType() != "PARENT")
        {
            cout << "Error: Invalid parent account type. Cannot create child account.\n";
            return;
        }

        string name, address, systemAccountType, accountType;
        int accountNum, pinNum;
        double usdBalance;
        string firstName, lastName;

        // require to enter name
        cout << "Enter your first name: ";
        std::getline(cin, firstName);
        firstName[0] = toupper(firstName[0]);
        cout << "Enter your last name: ";
        std::getline(cin, lastName);
        lastName[0] = toupper(lastName[0]);
        name = firstName + " " + lastName;

        // require to enter address
        address = parentAccount->getAddress();

        //generate the account number randomly
        accountNum = generateRandomAccountNumber(existingAccountNumbers);

        // require to create PIN for the account
        cout << "Create your PIN: ";
        cin >> pinNum;
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // require to enter the initial balance for the account
        cout << "Enter USD balance: ";
        cin >> usdBalance;
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        while (usdBalance < 0)
        {
            cout << "Balance cannot be negative.\n";
            cout << "Enter USD balance: ";
            cin >> usdBalance;
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        
        // child account are always considered as a customer account.
        systemAccountType = "CUSTOMER";
        accountType = "CHILD";

        Account *acc = new Account(name, address, systemAccountType, accountType, accountNum, pinNum, usdBalance);
        
        if (searchAccount(accountNum) == nullptr)
        {
    
            Node *newNode = new Node(acc);
            newNode->setParentAccount(parentAccount);
            node->addChildAccount(acc);
            insert(newNode, root);
            cout << "The account " << accountNum << " is created successfully.\n";
            
        }
        else
        {
            cout << "The account " << accountNum << " already exists.\n";
        }
    }

    createChildAccount(node->getLeftChild(), parentAccount);
    createChildAccount(node->getRightChild(), parentAccount);

}
// funtion allows parents to manage (deposit, withdraw, or delete) their child accounts 
void BST::manageChildAccount(Node *node, Account *parentAccount)
{
    if (node == nullptr)
    {
        return;
    }

    if (node->getAccount() == parentAccount)
    {
        if (parentAccount->getAccountType() != "PARENT")
        {
            cout << "Error: Invalid parent account type. Cannot manage child account.\n";
            return;
        }

        int childAccountNum;
        cout << "Enter child account number: ";
        cin >> childAccountNum;
        Account *childAcc = searchAccount(childAccountNum);
        if (childAcc == nullptr)
        {
            cout << "Invalid child account number. Cannot access to the child account.\n";
            return;
        }
        else
        {
            int choice;

            do
            {
                cout << "You have accessed to the child account\n";
                cout << "Select one of the options:\n";
                cout << "1. Deposit\n";
                cout << "2. Withdraw\n";
                cout << "3. Delete the child accounnt\n";
                cout << "4. Exit\n";
                cout << "Enter your choice: ";
                cin >> choice;
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                switch (choice)
                {
                    case 1:
                    {
                        double amount;
                        cout << "Enter the amount to deposit: ";
                        cin >> amount;
                        cin.clear();
                        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        if (amount < 0)
                        {
                            cout << "The amount cannot be negative.\n";
                            cout << "Exiting to the menu.\n";
                            break;
                        }
                        childAcc->deposit(amount);
                        break;
                    }
                    case 2:
                    {
                        double amount;
                        cout << "Enter the amount to withdraw: ";
                        cin >> amount;
                        cin.clear();
                        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        if (amount < 0)
                        {
                            cout << "The amount cannot be negative.\n";
                            cout << "Exiting to the menu.\n";
                            break;
                        }
                        childAcc->withdraw(amount);
                        break;
                    }
                    case 3:
                    {
                        char answer;
                        cout << "Would you like to delete this child account? (y/n): ";
                        cin >> answer;
                        cin.clear();
                        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        answer = toupper(answer);
                        if (answer == 'Y')
                        {
                            cout << "Deleting the account is confirmed.\n";
                            deleteAccount(childAccountNum);
                            node->removeChildAccount(childAcc);
                            cout << "The account" << childAccountNum << " is deleted.\n";
                        }
                        break;
                    }
                    case 4:
                        cout << "Exiting to the menu.\n";
                        return;
                    default:
                        cout << "Invalid choice. Please try again.\n";
                }
            } while (choice != 4);
            
        }
    }

    manageChildAccount(node->getLeftChild(), parentAccount);
    manageChildAccount(node->getRightChild(), parentAccount);
}
// fucntion to add a node (an account) to BST
void BST::addAccount(Account *acc)
{
    Node *newNode = new Node(acc);
    if (root == nullptr)
        root = newNode;
    else
        insert(newNode, root);
}
// fucntion to add a node (an account) to BST
void BST::addAccount()
{
    string name, address, systemAccountType, accountType;
    int accountNum, pinNum;
    double usdBalance;
    string firstName, lastName;

    // require to enter name
    cout << "Enter your first name: ";
    std::getline(cin, firstName);
    firstName[0] = toupper(firstName[0]);
    cout << "Enter your last name: ";
    std::getline(cin, lastName);
    lastName[0] = toupper(lastName[0]);
    name = firstName + " " + lastName;

    // require to enter address
    cout << "Enter your address (Ex:1234 N Main St): ";
    std::getline(cin, address);
    std::transform(address.begin(), address.end(), address.begin(), ::toupper);

    //generate the account number randomly
    accountNum = generateRandomAccountNumber(existingAccountNumbers);

    // require to choose the system account type (Admin or Customer)
    int systemAccountTypeChoice;
    cout << "System Account Type:\n";
    cout << "1. Admin Account\n";
    cout << "2. Customer Account\n";
    cout << "Enter your choice: ";
    cin >> systemAccountTypeChoice;
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // validate the input
    while (systemAccountTypeChoice < 1 || systemAccountTypeChoice > 2)
    {
        cout << "System Account Type:\n";
        cout << "1. Admin Account\n";
        cout << "2. Customer Account\n";
        cout << "Enter your choice: ";
        cin >> systemAccountTypeChoice;
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    // If it is an admin account, no balances will be needed for it.
    if (systemAccountTypeChoice == 1)
    {
        cout << "Enter 4 digits for PIN: ";
        cin >> pinNum;
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        while (pinNum < 1000 || pinNum > 9999)
        {
            cout << "PIN must be 4 digits.\n";
            cout << "Re-enter the PIN: ";
            cin >> pinNum;
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        usdBalance = 0;
        systemAccountType = "ADMIN";
        accountType = "Non-Customer";
    }
    else if (systemAccountTypeChoice == 2)  // If it is a customer account, the program will ask some more information
    {   
        cout << "Enter 4 digits for PIN: ";
        cin >> pinNum;
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        while (pinNum < 1000 || pinNum > 9999)
        {
            cout << "PIN must be 4 digits.\n";
            cout << "Re-enter the PIN: ";
            cin >> pinNum;
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        // require to enter balances of different currencies (for the currency exchange functionality)
        cout << "Enter USD balance: ";
        cin >> usdBalance;
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        while (usdBalance < 0)
        {
            cout << "Balance cannot be negative.\n";
            cout << "Enter USD balance: ";
            cin >> usdBalance;
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        systemAccountType = "CUSTOMER";
        accountType = "PARENT";
    }

    Account *acc = new Account(name, address, systemAccountType, accountType, accountNum, pinNum, usdBalance);
    
    if (searchAccount(accountNum) == nullptr)
    {
        Node *newNode = new Node(acc);
        if (root == nullptr)
            root = newNode;
        else
            insert(newNode, root);
        
        cout << "The account " << accountNum << " is created successfully.\n";
        displayAccountInfo(accountNum);
    }
    else
    {
        cout << "The account " << accountNum << " already exists.\n";
    }
}
// function to search an account if it exists by using the account number
Account *BST::searchAccount(const int accountNumber)
{
    Node *foundNode = search(accountNumber, root);
    if (foundNode)
        return foundNode->getAccount();
    else
        return nullptr;
}
// function to delete an account if it exists by using the account number
bool BST::deleteAccount(const int accountNumber)
{
    Node *nodeToDelete = search(accountNumber, root);
    if (nodeToDelete == nullptr)
        return false;
    
    deleteNode(nodeToDelete);
    return true;
}
// function to check the system account type of each account in BST 
// and write its information into a particulare text file
void BST::writeAccountsToFile() const
{
    if (root == nullptr)
    {
        cout << "The BST is empty. THere are no accounts to write." << '\n';
        return;
    }

    std::ofstream adminOutFile("admin_accounts.txt");
    if (!adminOutFile)
    {
        cout << "Error creating the file: admin_accounts.txt" << '\n';
        return;
    }    
    std::ofstream customerOutFile("customer_accounts.txt");
    if (!customerOutFile)
    {
        cout << "Error creating the file: customer_accounts.txt" << '\n';
    }

    writeAccountsToFile(root, adminOutFile, customerOutFile);
    adminOutFile.close();
    customerOutFile.close();
    
    cout << "Accounts have been written to the files: admin_accounts.txt and customer_accounts.txt" << '\n';
}
// function to read all accounts from text files and import them into BST
void BST::readAccountsFromFile(const string &fileName)
{
    std::ifstream inFile(fileName);
    if (!inFile)
    {
        cout << "Error: cannot open file " << fileName << '\n';
        return;
    }

    string line;
    string name, address, accountType, systemAccountType;
    int pinNum, accountNum;
    double balance;

    while (std::getline(inFile, line))
    {
        std::vector<string> fields = {};

        string field;
        std::stringstream ss(line);

        while (std::getline(ss, field, ','))
        {
            fields.push_back(field);
            field = "";
        }

        accountNum = std::stoi(fields[0]);
        systemAccountType = fields[1];
        accountType = fields[2];
        name = fields[3];
        address = fields[4];
        pinNum = std::stoi(fields[5]);
        balance = std::stod(fields[6]);

        Account *acc = new Account(name, address, systemAccountType, accountType, accountNum, pinNum, balance);
        addAccount(acc);
    }
}
// function to display all accounts' information 
void BST::displayAllAccounts() const
{
    if (root == nullptr)
    {
        cout << "No accounts found in the bank system.\n";
        return;
    }
    cout << "All Accounts in the Bank System.\n";
    displayAllAccounts(root);
}
// function to display the information of a specific account
void BST::displayAccountInfo(const int accountNumber)
{
    Account *account = searchAccount(accountNumber);
    cout << "Account Number: " << account->getAccountNum() << '\n';
    cout << "Account Name: " << account->getName() << '\n';
    cout << "System Account Type: " << account->getSystemAccountType() << '\n';
    cout << "Account Type: " << account->getAccountType() << '\n';
    cout << "Balance: " << account->getBalance() << '\n';
    cout << '\n';
}
// funtion to create a 5-digit random number for the account number
// and check if it already exists
int BST::generateRandomAccountNumber(std::set<int> &existingAccountNumbers)
{
    int randomNumber;
    do
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distribution(10000, 99999);
        randomNumber = distribution(gen);

    } while (existingAccountNumbers.count(randomNumber) > 0);

    existingAccountNumbers.insert(randomNumber);

    return randomNumber;
}
// overloaded function to create a child account
void BST::createChildAccount(Account *parentAccount)
{
    createChildAccount(root, parentAccount);
}
// overloaded function to manage a child account
void BST::manageChildAccount(Account *parentAccount)
{
    manageChildAccount(root, parentAccount);
}

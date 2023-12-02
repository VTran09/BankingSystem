#ifndef H_BST_NODE
#define H_BST_NODE

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <random>
#include <set>
#include <iomanip>
#include <limits>


using   std::cin, std::cout, std::string;

// Account class is used to collect information of account objects
class Account
{
    private:
        string name;
        string address;
        int accountNum;
        int pinNum;
        double balance;
        string accountType;        // parent or child
        string systemAccountType; //admin or customer

    public:
        // functions are needed to manipulate information in each class object
        Account();
        Account(string, string, string, string, int, int, double);
        string getName() const { return name; }
        string getAddress() const { return address; }
        int getAccountNum() const { return accountNum; }
        int getPinNum() const { return pinNum; }
        double getBalance() const { return balance; }
        string getAccountType() const { return accountType; }
        string getSystemAccountType() const { return systemAccountType; }

        void setAccountType(string type) { accountType = type; }
        void setSystemAccountType(string type) { systemAccountType = type; }
        void writeToFile(std::ofstream&) const;

        void deposit(double);
        void withdraw(double);
        void exchangeCurrency(double, const string&);
};

// Node class is used to store essential information that each node must contain
class Node
{
    private:
        Node *left;
        Node *right;
        Node *parentNode;
        Account *parentAccount;
        std::vector<Account*> children;
        Account *account;
        
    public:
        // functions are needed to manipulate information in each node
        Node(Account*);
        Node *getLeftChild() const { return left; }
        Node *getRightChild() const { return right; }
        Node *getParentNode() const { return parentNode; }
        Account *getParentAccount() const { return parentAccount; }
        const std::vector<Account*> &getChildren() const { return children; }
        Account *getAccount() const { return account; }
        
        void setLeftChild(Node *node) { left = node; }
        void setRightChild(Node *node) { right = node; }
        void setParentNode(Node *node) { parentNode = node; }
        void setParentAccount(Account *acc) { parentAccount = acc; }
        void addChildAccount(Account *acc) { children.push_back(acc); }
        void setAccount(Account *acc) { account = acc; }
        bool isLeaf() { return (left == nullptr && right == nullptr); }
        void removeChildAccount(Account*);
        void removeChildAccounts() { children.clear(); }

};

// BST class is used to create an object that manages a bank system
class BST
{   
    // functions are needed to manage nodes in the bank object.
    private:
        std::set<int> existingAccountNumbers;            
        Node *root;
        void insert(Node*, Node*);
        Node *search(const int, Node*);
        void deleteNode(Node *node);
        void writeAccountsToFile(Node*, std::ofstream&, std::ofstream&) const;
        void displayAllAccounts(Node *node) const;
        void createChildAccount(Node*, Account*);
        void manageChildAccount(Node*, Account*);
        
    public:  
        BST() { root = nullptr; };
        void addAccount(Account*);
        void addAccount();
        Account *searchAccount(const int);
        bool deleteAccount(const int);
        void writeAccountsToFile() const;
        void readAccountsFromFile(const string&);
        void displayAllAccounts() const;
        void displayAccountInfo(int);
        int generateRandomAccountNumber(std::set<int>&);
        void createChildAccount(Account*);
        void manageChildAccount(Account*);
        
};

#endif
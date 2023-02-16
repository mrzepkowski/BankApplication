#include <iostream>

#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H

class BankAccount {
    int account_id;
    std::string owner_name;
    float balance;
    std::string password;
public:
    BankAccount(): account_id(0), balance(0.0) {}
    BankAccount(int id, std::string name, float bal, std::string pass): account_id(id), owner_name(name), balance(bal), password(pass) {}
    ~BankAccount() {}
    int get_id() {return account_id;}
    std::string get_owner_name() {return owner_name;}
    float get_balance() {return balance;}
    std::string get_password() {return password;}
    void deposit(float amount) {balance += amount;}
    bool withdraw(float amount) {
        if (balance >= amount) {
            balance -= amount;
            return true;
        } else {
            return false;
        }
    }
    void clear() {
        account_id = 0;
        owner_name.clear();
        password.clear();
    }
    bool pass_correct(std::string pass) {return pass == password;}
    bool is_empty() {
        if (account_id == 0 && owner_name.empty() && password.empty()) {
            return true;
        } else {
            return false;
        }
    }
};

#endif
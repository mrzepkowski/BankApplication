#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <map>
#include <utility>

#ifndef BANKSYSTEM_H
#define BANKSYSTEM_H

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

class Bank {
    std::map<int, BankAccount>accounts;
    int current_id; //pole przechowujace id zalogowanego uzytkownika zamiast calego obiektu BankAccount
    std::string base;
    bool is_logged;
    void read_from_base() {
        std::fstream file(base);
        if (file.is_open()) {
            std::string line, name, password, column;
            int id;
            float balance;
            while (getline(file, line)) {
                std::stringstream str(line);
                getline(str, column, ',');
                id = stoi(column);
                getline(str, column, ',');
                name = column;
                getline(str, column, ',');
                balance = stof(column);
                getline(str, column);
                password = column;
                create_account(id, name, balance, password);
            }
            file.close();
        } else {
            std::cout << "Cannot read from base!\n" << '\n';
        }
    }
    void write_to_base() {
        std::fstream file(base, std::ios::out);
        if (file.is_open()) {
            file.clear();
            for (auto &acc : accounts) {
                BankAccount &account = acc.second;
                if (!account.is_empty()) {
                    file << account.get_id() << ',' << account.get_owner_name() << ',' << account.get_balance() << ',' << account.get_password() << '\n';
                }
            }
        } else {
            std::cout << "Cannot  write to base!\n";
        }
    }
public:
    Bank(char *filename): current_id(0), base(filename), is_logged(false) {
        srand(time(NULL));
        read_from_base();
    }
    ~Bank() { //nie ma potrzeby wywolywania logout pod koniec, bo nie trzeba przekazywac danych z obiektu do mapy
        /* if (is_logged) 
            logout(); */
        write_to_base();
    }
    void create_account(int id, std::string name, float balance, std::string password) {
        if (accounts.count(id) == 0) {
            BankAccount acc(id, name, balance, password);
            accounts[id] = acc;
        } else {
            std::cout << "This account already exists!\n";
        }
    }
    void create_account(std::string name, std::string password) {
        int id;
        do {
            id = (rand() % 10000) + 1;
        } while (accounts.count(id) != 0);
        BankAccount acc(id, name, 0, password);
        accounts[id] = acc;
        std::cout << "Account created succesfully. Your ID: " << id << "\n\n";
    }
    void remove_account() { //korzystanie z current_id jako klucza zamiast z calego obiektu
        if (is_logged) {
            std::string password;
            do {
                std::cout << "Enter your password (0 if you changed your mind): ";
                std::cin.ignore();
                std::getline(std::cin, password);
                if (password == "0")
                    return;
                if (!accounts[current_id].pass_correct(password))
                    std::cout << "Wrong password! Try again.\n";
            } while (!accounts[current_id].pass_correct(password));
            accounts.erase(current_id);
            std::cout << "Account removed succesfully.\n";
            is_logged = false;
        } else {
            std::cout << "User is not logged!\n";
        }
    }
    bool login(int id, std::string password) {
        if (accounts.count(id) == 0 || !accounts[id].pass_correct(password)) {
            std::cout << "Incorrect ID or password!\n";
            return false;
        }
        current_id = id; //zapamietywane jest tylko ID zalogowanego uzytkownika
        is_logged = true;
        std::cout << "Hello, " << accounts[id].get_owner_name() << "!\n";
        std::cout << "Your current balance: " << accounts[id].get_balance() << "\n";
        return true;
    }
    void logout() { //bez zapamietywania nowego stanu obiektu w mapie
        is_logged = false;
        current_id = 0;
    }
    void deposit(float amount) { //tez uzyte current_id
        if (amount > 0) {
            accounts[current_id].deposit(amount);
            std::cout << "Your current balance: " << accounts[current_id].get_balance() << "\n\n";
        }
    }
    bool withdraw(float amount) { //tak samo uzyte current_id
        if (amount < 0) {
            std::cout << "The amount should be grater than 0!\n";
            return false;
        } else if (amount == 0) {
            return true;
        }
        if (accounts[current_id].withdraw(amount)) {
            std::cout << "Money has been succesfully withdrawn.\n";
            std::cout << "Your current balance: " << accounts[current_id].get_balance() << "\n\n";
            return true;
        } else {
            std::cout << "Your balance is too low to withdraw this amount of money. Try again!\n";
            return false;
        }
    }
    bool transfer(int id, float amount) {
        if (accounts.count(id) == 0) {
            std::cout << "Incorrect reciever ID!\n";
            return false;
        }
        if (id == current_id) {
            std::cout << "Cannot transfer money to yourself!\n";
            return false;
        }
        if (amount < 0) {
            std::cout << "The amount should be grater than 0!\n";
            return false;
        } else if (amount == 0) {
            return true;
        }
        if (!accounts[current_id].withdraw(amount)) {
            std::cout << "Your balance is too low to transfer this amount of money.\n\n";
            return false;
        }
        accounts[id].deposit(amount);
        std::cout << "Your current balance: " << accounts[current_id].get_balance() << "\n\n";
        return true;
    }
    void show_recievers() { //nowa metoda, ktora odpowiada za wyswietlenie wszystkich odbiorcow, wywolywana przy wyborze przelewu
        std::cout << "List of other recievers with IDs:\n";
        for (auto &acc : accounts) {
            BankAccount &account = acc.second;
            if (!account.is_empty()) {
                std::cout << account.get_owner_name() << ", " << account.get_id();
                if (account.get_id() == current_id) {
                    std::cout << " (you)";
                }
                std::cout << '\n';
            }
            }
    }
};

#endif
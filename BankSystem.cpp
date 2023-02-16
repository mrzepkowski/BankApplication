#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <map>
#include <utility>
#include "BankSystem.h"
#include "BankAccount.h"

void Bank::read_from_base() {
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

void Bank::write_to_base() {
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

void Bank::create_account(int id, std::string name, float balance, std::string password) {
    if (accounts.count(id) == 0) {
        BankAccount acc(id, name, balance, password);
        accounts[id] = acc;
    } else {
        std::cout << "This account already exists!\n";
    }
}

void Bank::create_account(std::string name, std::string password) {
    int id;
    do {
        id = (rand() % 10000) + 1;
    } while (accounts.count(id) != 0);
    BankAccount acc(id, name, 0, password);
    accounts[id] = acc;
    std::cout << "Account created succesfully. Your ID: " << id << "\n\n";
}

void Bank::remove_account() {
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

bool Bank::login(int id, std::string password) {
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

void Bank::logout() {
    is_logged = false;
    current_id = 0;
}

void Bank::deposit(float amount) {
    if (amount > 0) {
        accounts[current_id].deposit(amount);
        std::cout << "Your current balance: " << accounts[current_id].get_balance() << "\n\n";
    }
}

bool Bank::withdraw(float amount) {
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

bool Bank::transfer(int id, float amount) {
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

void Bank::show_recievers() {
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
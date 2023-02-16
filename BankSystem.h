#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <map>
#include <utility>
#include "BankAccount.h"

#ifndef BANKSYSTEM_H
#define BANKSYSTEM_H

class Bank {
    std::map<int, BankAccount>accounts;
    int current_id; //pole przechowujace id zalogowanego uzytkownika
    std::string base;
    bool is_logged;
    void read_from_base();
    void write_to_base();
public:
    Bank(char *filename): current_id(0), base(filename), is_logged(false) {
        srand(time(NULL));
        read_from_base();
    }
    ~Bank() {
        write_to_base();
    }
    void create_account(int id, std::string name, float balance, std::string password);
    void create_account(std::string name, std::string password);
    void remove_account();
    bool login(int id, std::string password);
    void logout();
    void deposit(float amount);
    bool withdraw(float amount);
    bool transfer(int id, float amount);
    void show_recievers();
};

#endif
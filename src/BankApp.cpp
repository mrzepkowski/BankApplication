#include <iostream>
#include <string>
#include "BankSystem.h"


int main(int argc, char *argv[]) {
    if (argc != 2) return -1;
    Bank bank(argv[1]);
    int x;
    bool is_logged = false;

    
    std::cout << "\nWelcome to Polish Express!\n";
    do {
        if (!is_logged) {
            std::cout << "\nChoose one of the following options:\n";
            std::cout << "1: Create account\n";
            std::cout << "2: Log in\n";
            std::cout << "0: Close app\n";
            std::cin >> x;
            std::cout << '\n';

            std::string name, password, password2;
            int id;
            switch(x) {
            case 1:
                std::cout << "ENTER 0 IF YOU CHANGED YOUR MIND.\n";
                std::cout << "Enter your name: ";
                std::cin.ignore(1000000, '\n');
                std::getline(std::cin, name);
                if (name == "0") break;
                do {
                    std::cout << "Enter your password: ";
                    std::getline(std::cin, password);
                    if (password == "0") break;
                    std::cout << "Repeat your password: ";
                    std::getline(std::cin, password2);
                    if (password2 == "0") break;
                    if (password2 != password) {
                        std::cout << "Passwords are different!\n";
                    }
                } while (password2 != password);
                if (password == "0" || password2 == "0") break;
                bank.create_account(name, password);
                break;
            case 2:
                do {
                    std::cout << "Enter your ID (0 if you changed your mind):\n";
                    std::cin >> id;
                    if (id == 0) {
                        break;
                    }
                    std::cout << "Enter your password (0 if you changed your mind):\n";
                    std::cin.ignore();
                    std::getline(std::cin, password);
                    if (password == "0") break;
                } while (!bank.login(id, password));
                if (id > 0 && password != "0") is_logged = true;
                break;
            case 0:
                break;
            default:
                std::cout << "Unknown option!\n";
                break;
            }
        } else {
            std::cout << "\nChoose one of the following options:\n";
            std::cout << "1: Deposit money\n";
            std::cout << "2: Withdraw money\n";
            std::cout << "3: Transfer money\n";
            std::cout << "4: Log out\n";
            std::cout << "5: Remove account\n";
            std::cout << "0: Close app\n";
            std::cin >> x;
            std::cout << '\n';

            std::string name, password;
            int id;
            float amount;
            switch(x) {
            case 1:
                std::cout << "Enter amount of money you want to deposit (0 if you changed your mind): ";
                std::cin >> amount;
                bank.deposit(amount);
                break;
            case 2:
                do {
                    std::cout << "Enter amount of money you want to withdraw (0 if you changed your mind): ";
                    std::cin >> amount;
                    if (amount == 0) break;
                } while (!bank.withdraw(amount));
                break;
            case 3:
                bank.show_recievers(); //wyswietlenie odbiorcow
                do {
                    std::cout << "Enter an ID of a receiver (0 if you changed your mind): ";
                    std::cin >> id;
                    if (id == 0) break;
                    std::cout << "Enter amount of money you want to transfer (0 if you changed your mind): ";
                    std::cin >> amount;
                } while (!bank.transfer(id, amount));
                break;
            case 4:
                bank.logout();
                is_logged = false;
                break;
            case 5:
                bank.remove_account();
                is_logged = false;
                break;
            case 0:
                break;
            default:
                std::cout << "Unknown option!\n";
                break;
            }
        }
        
    } while (x != 0);
}
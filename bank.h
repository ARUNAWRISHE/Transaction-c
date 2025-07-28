#ifndef BANK_H
#define BANK_H

#include <stddef.h>

#define FILENAME "accounts.dat"

typedef struct {
    int account_number;
    char name[50];
    int pin;
    double balance;
} Account;

void create_account();
int  generate_account_number();
int  generate_pin();
void view_balance();
void deposit();
void withdraw();
void change_pin();
Account* login();
void save_account(Account* acc);
void update_account(Account* acc);
void delete_account();
void detials();
void register_transaction(int account_number, const char *type, double amount, double balance);
void get_transaction_history(int account_number, char *history, size_t history_size, int entered_pin, int *pin_ok);

#endif

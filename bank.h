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

int load_accounts(Account **arr_out, int *count_out);
void save_accounts(Account *arr, int count);
Account* find_account_bsearch(int accnum, Account *arr, int count);


Account* login_fast(int acc_num, int pin); 
int deposit_fast(int acc_num, int pin, double amt, double *new_balance);
int withdraw_fast(int acc_num, int pin, double amt, double *new_balance);
int change_pin_fast(int acc_num, int old_pin, int new_pin);
int delete_account_fast(int acc_num, int pin);


int create_account_gui(const char *name, int *out_accnum, int *out_pin);
void register_transaction(int account_number, const char *type, double amount, double balance);
void get_transaction_history(int account_number, char *history, size_t history_size, int entered_pin, int *pin_ok);

#endif

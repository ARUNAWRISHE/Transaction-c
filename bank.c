#include "bank.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define HISTFILE "histor.txt"

int load_accounts(Account **arr_out, int *count_out) {
    FILE *fp = fopen(FILENAME, "rb");
    if (!fp) { *arr_out = NULL; *count_out = 0; return 0; }
    fseek(fp, 0, SEEK_END);
    long sz = ftell(fp);
    int count = sz / sizeof(Account);
    rewind(fp);
    if (count == 0) { fclose(fp); *arr_out = NULL; *count_out = 0; return 0; }
    *arr_out = malloc(sz);
    fread(*arr_out, sizeof(Account), count, fp);
    fclose(fp);
    *count_out = count;
    return 1;
}

void save_accounts(Account *arr, int count) {
    FILE *fp = fopen(FILENAME, "wb");
    if (!fp) return;
    fwrite(arr, sizeof(Account), count, fp);
    fclose(fp);
}

static int cmp_acc(const void *a, const void *b) {
    int an = ((const Account*)a)->account_number;
    int bn = ((const Account*)b)->account_number;
    return (an - bn);
}

Account* find_account_bsearch(int accnum, Account *arr, int count) {
    Account key; key.account_number = accnum;
    return (Account*)bsearch(&key, arr, count, sizeof(Account), cmp_acc);
}

int create_account_gui(const char *name, int *out_accnum, int *out_pin) {
    Account *arr; int count;
    int res = load_accounts(&arr, &count);
    int next = 100001;
    if (res && count) {
        qsort(arr, count, sizeof(Account), cmp_acc);
        next = arr[count-1].account_number + 1;
        free(arr);
    }
    Account acc;
    strncpy(acc.name, name, sizeof(acc.name)-1);
    acc.name[sizeof(acc.name)-1] = 0;
    acc.account_number = next;
    acc.pin = 1000 + rand() % 9000;
    acc.balance = 0.0;
    FILE *fp = fopen(FILENAME, "ab");
    if (!fp) return 0;
    fwrite(&acc, sizeof(Account), 1, fp);
    fclose(fp);
    *out_accnum = acc.account_number;
    *out_pin = acc.pin;
    return 1;
}

Account* login_fast(int acc_num, int pin) {
    Account *arr; int count;
    if (!load_accounts(&arr, &count)) return NULL;
    qsort(arr, count, sizeof(Account), cmp_acc);
    Account *acc = find_account_bsearch(acc_num, arr, count);
    static Account result;
    if (acc && acc->pin == pin) {
        result = *acc;
        free(arr);
        return &result;
    }
    free(arr);
    return NULL;
}

int deposit_fast(int acc_num, int pin, double amt, double *new_balance) {
    Account *arr; int count;
    if (!load_accounts(&arr, &count)) return 0;
    qsort(arr, count, sizeof(Account), cmp_acc);
    Account *acc = find_account_bsearch(acc_num, arr, count);
    if (!acc || acc->pin != pin || amt <= 0) { free(arr); return 0; }
    acc->balance += amt;
    *new_balance = acc->balance;
    save_accounts(arr, count);
    register_transaction(acc_num, "DEPOSIT", amt, acc->balance);
    free(arr);
    return 1;
}

int withdraw_fast(int acc_num, int pin, double amt, double *new_balance) {
    Account *arr; int count;
    if (!load_accounts(&arr, &count)) return 0;
    qsort(arr, count, sizeof(Account), cmp_acc);
    Account *acc = find_account_bsearch(acc_num, arr, count);
    if (!acc || acc->pin != pin || amt <= 0 || amt > acc->balance) { free(arr); return 0; }
    acc->balance -= amt;
    *new_balance = acc->balance;
    save_accounts(arr, count);
    register_transaction(acc_num, "WITHDRAW", amt, acc->balance);
    free(arr);
    return 1;
}

int change_pin_fast(int acc_num, int old_pin, int new_pin) {
    Account *arr; int count;
    if (!load_accounts(&arr, &count)) return 0;
    qsort(arr, count, sizeof(Account), cmp_acc);
    Account *acc = find_account_bsearch(acc_num, arr, count);
    if (!acc || acc->pin != old_pin || new_pin < 1000 || new_pin > 9999) { free(arr); return 0; }
    acc->pin = new_pin;
    save_accounts(arr, count);
    free(arr);
    return 1;
}

int delete_account_fast(int acc_num, int pin) {
    Account *arr; int count;
    if (!load_accounts(&arr, &count)) return 0;
    qsort(arr, count, sizeof(Account), cmp_acc);
    int i; int found = 0;
    for (i = 0; i < count; ++i) {
        if (arr[i].account_number == acc_num && arr[i].pin == pin) {
            found = 1;
            break;
        }
    }
    if (!found) { free(arr); return 0; }
    if (i < count-1) memmove(&arr[i], &arr[i+1], (count-i-1)*sizeof(Account));
    save_accounts(arr, count-1);
    free(arr);
    return 1;
}

void register_transaction(int account_number, const char *type, double amount, double balance) {
    FILE *fp = fopen(HISTFILE, "a");
    if (!fp) return;
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char timestr[32];
    strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", tm_info);
    fprintf(fp, "%d;%s;%s;%.2lf;%.2lf\n", account_number, timestr, type, amount, balance);
    fclose(fp);
}

void get_transaction_history(int account_number, char *history, size_t history_size, int entered_pin, int *pin_ok) {
    *pin_ok = 0;
    Account *arr; int count;
    if (!load_accounts(&arr, &count)) { snprintf(history, history_size, "Cannot open accounts file."); return; }
    qsort(arr, count, sizeof(Account), cmp_acc);
    Account *acc = find_account_bsearch(account_number, arr, count);
    if (acc && acc->pin == entered_pin) *pin_ok = 1;
    free(arr);

    if (!*pin_ok) { snprintf(history, history_size, "Invalid credentials."); return; }

    FILE *fp = fopen(HISTFILE, "r");
    if (!fp) { snprintf(history, history_size, "No history yet."); return; }
    char line[256];
    snprintf(history, history_size, "Date & Time           | Type    | Amount    | Balance\n"
                                    "--------------------------------------------------------\n");
    while (fgets(line, sizeof(line), fp)) {
        int anum;
        char tstamp[32], typ[16];
        double amt, bal;
        if (sscanf(line, "%d;%31[^;];%15[^;];%lf;%lf",
                   &anum, tstamp, typ, &amt, &bal) == 5) {
            if (anum == account_number) {
                char entry[128];
                snprintf(entry, sizeof(entry), "%-19s | %-7s | %-9.2lf | %.2lf\n", tstamp, typ, amt, bal);
                strncat(history, entry, history_size - strlen(history) - 1);
            }
        }
    }
    fclose(fp);
}

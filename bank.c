#include "bank.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define HISTFILE "histor.txt"

int generate_account_number() {
    int last_num = 100000;
    FILE *fp = fopen(FILENAME, "rb");
    Account acc;
    if (fp) {
        while (fread(&acc, sizeof(Account), 1, fp))
            if (acc.account_number > last_num) last_num = acc.account_number;
        fclose(fp);
    }
    return last_num + 1;
}

int generate_pin() {
    return 1000 + rand() % 9000;
}

void save_account(Account *acc) {
    FILE *fp = fopen(FILENAME, "ab");
    if (!fp) { perror("File error"); return; }
    fwrite(acc, sizeof(Account), 1, fp);
    fclose(fp);
}

void update_account(Account *acc) {
    FILE *fp = fopen(FILENAME, "rb+");
    if (!fp) { perror("File error"); return; }
    Account tmp; long pos;
    while ((pos = ftell(fp)), fread(&tmp, sizeof(Account), 1, fp)) {
        if (tmp.account_number == acc->account_number) {
            fseek(fp, pos, SEEK_SET);
            fwrite(acc, sizeof(Account), 1, fp);
            break;
        }
    }
    fclose(fp);
}

Account* login() {
    static Account acc;
    int acc_num, pin;
    printf("\nAccount number: ");
    scanf("%d", &acc_num);
    printf("PIN: ");
    scanf("%d", &pin);

    FILE *fp = fopen(FILENAME, "rb");
    if (!fp) { perror("File error"); return NULL; }

    while (fread(&acc, sizeof(Account), 1, fp))
        if (acc.account_number == acc_num && acc.pin == pin) {
            fclose(fp);
            return &acc;
        }
    fclose(fp);
    puts("Invalid credentials!");
    return NULL;
}

void create_account() {
    Account acc;
    acc.account_number = generate_account_number();
    printf("Enter your name: ");
    int c; while ((c = getchar()) != '\n' && c != EOF);
    fgets(acc.name, sizeof(acc.name), stdin);
    acc.name[strcspn(acc.name, "\n")] = 0;
    acc.pin = generate_pin();
    acc.balance = 0.0;
    save_account(&acc);

    puts("\nAccount created successfully!");
    printf("Account Number: %d\nPIN: %d\n", acc.account_number, acc.pin);
    puts("Please remember your credentials.\n");
}

void view_balance() {
    Account *acc = login();
    if (acc)
        printf("\nWelcome, %s!\nAccount: %d\nBalance: Rs. %.2lf\n",
               acc->name, acc->account_number, acc->balance);
}

void deposit() {
    Account *acc = login();
    if (!acc) return;
    double amt;
    printf("Amount to deposit: Rs. ");
    if (scanf("%lf", &amt) != 1 || amt <= 0) { puts("Invalid amount."); return; }
    acc->balance += amt;
    update_account(acc);
    printf("Deposit successful. New balance: Rs. %.2lf\n", acc->balance);
    register_transaction(acc->account_number, "DEPOSIT", amt, acc->balance);
}

void withdraw() {
    Account *acc = login();
    if (!acc) return;
    double amt;
    printf("Amount to withdraw: Rs. ");
    if (scanf("%lf", &amt) != 1 || amt <= 0 || amt > acc->balance) {
        puts("Invalid amount."); return;
    }
    acc->balance -= amt;
    update_account(acc);
    printf("Withdraw successful. New balance: Rs. %.2lf\n", acc->balance);
    register_transaction(acc->account_number, "WITHDRAW", amt, acc->balance);
}

void change_pin() {
    Account *acc = login();
    if (!acc) return;
    int old_pin, new_pin;
    printf("Re-enter current PIN to confirm: ");
    scanf("%d", &old_pin);
    if (old_pin != acc->pin) { puts("PIN mismatch."); return; }
    printf("Enter new 4-digit PIN: ");
    scanf("%d", &new_pin);
    if (new_pin < 1000 || new_pin > 9999) { puts("PIN must be 4 digits."); return; }
    acc->pin = new_pin;
    update_account(acc);
    puts("PIN changed successfully.");
}

void delete_account() {
    int acc_num, pin, found = 0;
    printf("\nEnter your account number to delete: ");
    scanf("%d", &acc_num);
    printf("Enter your PIN: ");
    scanf("%d", &pin);

    FILE *fp = fopen(FILENAME, "rb");
    if (!fp) { puts("File error / No accounts found."); return; }
    FILE *tmp = fopen("temp.dat", "wb");
    if (!tmp) { puts("Cannot create temp file."); fclose(fp); return; }

    Account acc;
    while (fread(&acc, sizeof(Account), 1, fp)) {
        if (acc.account_number == acc_num && acc.pin == pin) {
            printf("\nAccount %d deleted successfully!\n", acc.account_number);
            found = 1;
            continue;
        }
        fwrite(&acc, sizeof(Account), 1, tmp);
    }
    fclose(fp); fclose(tmp);

    if (found) {
        remove(FILENAME);
        rename("temp.dat", FILENAME);
    } else {
        remove("temp.dat");
        puts("Account not found, or PIN incorrect.");
    }
}

void detials() {
    char username[30], password[30];
    printf("\n--- Admin Login Required ---\n");
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    if (strcmp(username, "admin") != 0 || strcmp(password, "90253") != 0) {
        puts("Access denied. Invalid admin credentials.");
        return;
    }

    FILE *fp = fopen(FILENAME, "rb");
    if (!fp) { puts("No accounts found."); return; }
    Account acc;
    printf("\n---- Stored accounts ----\n");
    while (fread(&acc, sizeof(Account), 1, fp)) {
        printf("Acc: %d, Name: %s, PIN: %d, Bal: %.2lf\n",
               acc.account_number, acc.name, acc.pin, acc.balance);
    }
    puts("-------------------------");
    fclose(fp);
}

// ---- TRANSACTION HISTORY ----
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
    // First, check PIN in accounts file
    *pin_ok = 0;
    FILE *fp = fopen(FILENAME, "rb");
    if (!fp) { snprintf(history, history_size, "Cannot open accounts file."); return; }
    Account acc;
    while (fread(&acc, sizeof(Account), 1, fp)) {
        if (acc.account_number == account_number) {
            if (acc.pin == entered_pin) *pin_ok = 1;
            break;
        }
    }
    fclose(fp);
    if (!*pin_ok) { snprintf(history, history_size, "Invalid credentials."); return; }

    fp = fopen(HISTFILE, "r");
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

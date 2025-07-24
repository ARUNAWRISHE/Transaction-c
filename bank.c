#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

int main() {

    srand((unsigned) time(NULL));

    int choice;
    while (1) {
        printf("\n==== Bank Transaction Menu ====\n");
        printf("1. Create Account\n");
        printf("2. View Balance\n");
        printf("3. Deposit\n");
        printf("4. Withdraw\n");
        printf("5. Change PIN\n");
        printf("6. Delete Account\n");
        printf("7. Detials (only can seen by admin)\n");
        printf("8. Exit\n");
        printf("Choose an option: ");
        if (scanf("%d", &choice) != 1) break;

        switch (choice) {
            case 1: create_account();   break;
            case 2: view_balance();     break;
            case 3: deposit();          break;
            case 4: withdraw();         break;
            case 5: change_pin();       break;
            case 6: delete_account();   break;
            case 7: detials(); break;
            case 8: puts("Thank you."); exit(0);
            default: puts("Invalid option.");
        }
    }
    return 0;
}


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

void create_account() {
    Account acc;
    acc.account_number = generate_account_number();
    printf("Enter your name: ");
    getchar(); 
    fgets(acc.name, sizeof(acc.name), stdin);
    acc.name[strcspn(acc.name, "\n")] = 0; 
    acc.pin = generate_pin();
    acc.balance = 0.0;
    save_account(&acc);

    puts("\nAccount created successfully!");
    printf("Account Number: %d\nPIN: %d\n", acc.account_number, acc.pin);
    puts("Please remember your credentials.\n");
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

void save_account(Account *acc) {
    FILE *fp = fopen(FILENAME, "ab");
    if (!fp) { perror("File error"); return; }
    fwrite(acc, sizeof(Account), 1, fp);
    fclose(fp);
}

void update_account(Account *acc) {
    FILE *fp = fopen(FILENAME, "rb+");
    if (!fp) { perror("File error"); return; }
    Account tmp;
    while (fread(&tmp, sizeof(Account), 1, fp)) {
        if (tmp.account_number == acc->account_number) {
            fseek(fp, -sizeof(Account), SEEK_CUR);
            fwrite(acc, sizeof(Account), 1, fp);
            break;
        }
    }
    fclose(fp);
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

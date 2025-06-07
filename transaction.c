#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_ACCOUNTS 100

// ANSI color codes for style
#define COLOR_RESET   "\x1b[0m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_CYAN    "\x1b[36m"

// Structure for client data
struct clientData {
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
};

// Function declarations
void enterChoice(FILE *cfPtr);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);
void flushInputBuffer(void);

int main(int argc, char *argv[]) {
    FILE *cfPtr;

    // Open binary file for reading and writing
    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL) {
        printf(COLOR_RED "❌ %s: File could not be opened.\n" COLOR_RESET, argv[0]);
        exit(EXIT_FAILURE);
    }

    enterChoice(cfPtr);
    fclose(cfPtr);
    return 0;
}

// Clear any leftover input
void flushInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Export formatted data to a text file
void textFile(FILE *readPtr) {
    FILE *writePtr;
    struct clientData client = {0, "", "", 0.0};

    if ((writePtr = fopen("accounts.txt", "w")) == NULL) {
        printf(COLOR_RED "❌ Could not create accounts.txt\n" COLOR_RESET);
        return;
    }

    rewind(readPtr);
    fprintf(writePtr, "%-6s%-25s%-10s\n", "Acct", "Full Name", "Balance");

    while (fread(&client, sizeof(struct clientData), 1, readPtr) == 1) {
        if (client.acctNum != 0) {
            fprintf(writePtr, "%-6d%-15s %-10s%.2f\n",
                    client.acctNum, client.lastName, client.firstName, client.balance);
        }
    }

    fclose(writePtr);
    printf(COLOR_GREEN "✅ accounts.txt successfully generated.\n" COLOR_RESET);
}

// Update account record
void updateRecord(FILE *fPtr) {
    unsigned int account;
    double transaction;
    char transactionType;
    struct clientData client = {0, "", "", 0.0};

    printf("Enter account number to update (1 - 100): ");
    if (scanf("%u", &account) != 1 || account < 1 || account > MAX_ACCOUNTS) {
        printf(COLOR_RED "❌ Invalid account number.\n" COLOR_RESET);
        flushInputBuffer();
        return;
    }

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0) {
        printf(COLOR_YELLOW "⚠️  Account #%u does not exist.\n" COLOR_RESET, account);
        return;
    }

    printf(COLOR_CYAN "\n📄 Account Details:\n" COLOR_RESET);
    printf("  ID      : %u\n", client.acctNum);
    printf("  Name    : %s %s\n", client.firstName, client.lastName);
    printf("  Balance : ₹%.2f\n", client.balance);

    printf("\nProceed with transaction? (y/n): ");
    char confirm;
    scanf(" %c", &confirm);
    if (tolower(confirm) != 'y') return;

    printf("Enter transaction type (+ to charge, - to pay): ");
    scanf(" %c", &transactionType);

    printf("Enter amount: ");
    if (scanf("%lf", &transaction) != 1 || transaction < 0) {
        printf(COLOR_RED "❌ Invalid transaction amount.\n" COLOR_RESET);
        flushInputBuffer();
        return;
    }

    if (transactionType == '+') {
        client.balance += transaction;
    } else if (transactionType == '-') {
        client.balance -= transaction;
    } else {
        printf(COLOR_RED "❌ Invalid transaction type.\n" COLOR_RESET);
        return;
    }

    fseek(fPtr, -(long int)sizeof(struct clientData), SEEK_CUR);
    fwrite(&client, sizeof(struct clientData), 1, fPtr);
    fflush(fPtr);

    printf(COLOR_GREEN "✅ Account updated successfully.\n" COLOR_RESET);
}
void displayAccountById(FILE *fPtr) {
    struct clientData client = {0, "", "", 0.0};
    unsigned int account;

    printf("Enter account number to view (1 - 100): ");
    if (scanf("%u", &account) != 1 || account < 1 || account > MAX_ACCOUNTS) {
        printf(COLOR_RED "❌ Invalid account number.\n" COLOR_RESET);
        flushInputBuffer();
        return;
    }

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0) {
        printf(COLOR_YELLOW "⚠️  Account #%u does not exist.\n" COLOR_RESET, account);
    } else {
        printf(COLOR_CYAN "\n📄 Account Details:\n" COLOR_RESET);
        printf("  ID      : %u\n", client.acctNum);
        printf("  Name    : %s %s\n", client.firstName, client.lastName);
        printf("  Balance : ₹%.2f\n", client.balance);
    }
}

// Add a new account
void newRecord(FILE *fPtr) {
    struct clientData client = {0, "", "", 0.0};
    unsigned int accountNum;

    printf("Enter new account number (1 - 100): ");
    if (scanf("%u", &accountNum) != 1 || accountNum < 1 || accountNum > MAX_ACCOUNTS) {
        printf(COLOR_RED "❌ Invalid account number.\n" COLOR_RESET);
        flushInputBuffer();
        return;
    }

    fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0) {
        printf(COLOR_YELLOW "⚠️  Account #%d already exists.\n" COLOR_RESET, client.acctNum);
        printf("  Name    : %s %s\n", client.firstName, client.lastName);
        printf("  Balance : ₹%.2f\n", client.balance);
        return;
    }

    printf("Enter lastname, firstname, balance: ");
    if (scanf("%14s %9s %lf", client.lastName, client.firstName, &client.balance) != 3) {
        printf(COLOR_RED "❌ Invalid input format.\n" COLOR_RESET);
        flushInputBuffer();
        return;
    }

    client.acctNum = accountNum;
    fseek(fPtr, (client.acctNum - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&client, sizeof(struct clientData), 1, fPtr);
    fflush(fPtr);

    printf(COLOR_GREEN "✅ Account #%d created successfully.\n" COLOR_RESET, client.acctNum);

    // Refresh text file
    textFile(fPtr);
}

// Delete an account
void deleteRecord(FILE *fPtr) {
    unsigned int accountNum;
    struct clientData client, blankClient = {0, "", "", 0.0};

    printf("Enter account number to delete (1 - 100): ");
    if (scanf("%u", &accountNum) != 1 || accountNum < 1 || accountNum > MAX_ACCOUNTS) {
        printf(COLOR_RED "❌ Invalid account number.\n" COLOR_RESET);
        flushInputBuffer();
        return;
    }

    fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0) {
        printf(COLOR_YELLOW "⚠️  Account %u does not exist.\n" COLOR_RESET, accountNum);
        return;
    }

    printf(COLOR_CYAN "\n📄 Account to be Deleted:\n" COLOR_RESET);
    printf("  ID      : %u\n", client.acctNum);
    printf("  Name    : %s %s\n", client.firstName, client.lastName);
    printf("  Balance : ₹%.2f\n", client.balance);

    printf("\nAre you sure you want to delete this account? (y/n): ");
    char confirm;
    scanf(" %c", &confirm);
    if (tolower(confirm) != 'y') return;

    fseek(fPtr, (accountNum - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&blankClient, sizeof(struct clientData), 1, fPtr);
    fflush(fPtr);

    printf(COLOR_GREEN "✅ Account %u deleted successfully.\n" COLOR_RESET, accountNum);
}

// Menu
void enterChoice(FILE *cfPtr) {
    unsigned int menuChoice;

    do {
        printf(COLOR_CYAN "\n===== ACCOUNT MANAGER =====\n" COLOR_RESET);
        printf("1 - Generate accounts.txt (formatted text)\n");
        printf("2 - Update an account\n");
        printf("3 - Add a new account\n");
        printf("4 - Delete an account\n");
        printf("5 - Exit program\n");
        printf("6 - Show account details by ID\n? ");
        scanf("%u", &menuChoice);

        switch (menuChoice) {
            case 1:
                textFile(cfPtr);
                break;
            case 2:
                updateRecord(cfPtr);
                break;
            case 3:
                newRecord(cfPtr);
                break;
            case 4:
                deleteRecord(cfPtr);
                break;
            case 5:
                printf(COLOR_GREEN "✅ Program terminated. Goodbye!\n" COLOR_RESET);
                break;
            case 6:
                displayAccountById(cfPtr);
                break;
            default:
                printf(COLOR_RED "❌ Invalid choice. Try again.\n" COLOR_RESET);
        }
    } while (menuChoice != 5);
}

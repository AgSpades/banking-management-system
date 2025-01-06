#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 50
#define DATA_FILE "bank_data.txt"

// Structure to hold account details
typedef struct {
    int accountNumber;
    char name[MAX_NAME];
    float balance;
} Account;

// Function prototypes
void createAccount();
void viewAccounts();
void depositFunds();
void withdrawFunds();
void saveAccount(Account account);
int generateAccountNumber();

int main() {
    int choice;
    do {
        printf("\n--- Bank Management System ---\n");
        printf("1. Create Account\n");
        printf("2. View Accounts\n");
        printf("3. Deposit Funds\n");
        printf("4. Withdraw Funds\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: createAccount(); break;
            case 2: viewAccounts(); break;
            case 3: depositFunds(); break;
            case 4: withdrawFunds(); break;
            case 5: printf("Exiting...\n"); break;
            default: printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);

    return 0;
}

// Function to create a new account
void createAccount() {
    Account account;
    account.accountNumber = generateAccountNumber();

    printf("Enter Name: ");
    scanf(" %[^\n]", account.name);

    printf("Enter Initial Deposit: ");
    scanf("%f", &account.balance);

    saveAccount(account);
    printf("Account created successfully! Account Number: %d\n", account.accountNumber);
}

// Function to view all accounts
void viewAccounts() {
    FILE *file = fopen(DATA_FILE, "r");
    if (!file) {
        printf("No accounts found.\n");
        return;
    }

    Account account;
    printf("\n--- Account Details ---\n");
    printf("Account Number | Name                 | Balance\n");
    printf("------------------------------------------------\n");
    while (fread(&account, sizeof(Account), 1, file)) {
        printf("%13d | %-20s | %.2f\n", account.accountNumber, account.name, account.balance);
    }

    fclose(file);
}

// Function to deposit funds into an account
void depositFunds() {
    int accNumber;
    float amount;
    Account account;
    int found = 0;

    printf("Enter Account Number: ");
    scanf("%d", &accNumber);

    FILE *file = fopen(DATA_FILE, "r+b");
    if (!file) {
        printf("No accounts found.\n");
        return;
    }

    while (fread(&account, sizeof(Account), 1, file)) {
        if (account.accountNumber == accNumber) {
            printf("Enter Amount to Deposit: ");
            scanf("%f", &amount);
            account.balance += amount;

            fseek(file, -sizeof(Account), SEEK_CUR);
            fwrite(&account, sizeof(Account), 1, file);

            printf("Deposit successful! New Balance: %.2f\n", account.balance);
            found = 1;
            break;
        }
    }

    if (!found) printf("Account not found.\n");
    fclose(file);
}

// Function to withdraw funds from an account
void withdrawFunds() {
    int accNumber;
    float amount;
    Account account;
    int found = 0;

    printf("Enter Account Number: ");
    scanf("%d", &accNumber);

    FILE *file = fopen(DATA_FILE, "r+b");
    if (!file) {
        printf("No accounts found.\n");
        return;
    }

    while (fread(&account, sizeof(Account), 1, file)) {
        if (account.accountNumber == accNumber) {
            printf("Enter Amount to Withdraw: ");
            scanf("%f", &amount);
            if (account.balance >= amount) {
                account.balance -= amount;

                fseek(file, -sizeof(Account), SEEK_CUR);
                fwrite(&account, sizeof(Account), 1, file);

                printf("Withdrawal successful! New Balance: %.2f\n", account.balance);
            } else {
                printf("Insufficient funds.\n");
            }
            found = 1;
            break;
        }
    }

    if (!found) printf("Account not found.\n");
    fclose(file);
}

// Function to save account details to a file
void saveAccount(Account account) {
    FILE *file = fopen(DATA_FILE, "ab");
    if (!file) {
        printf("Error saving account.\n");
        return;
    }
    fwrite(&account, sizeof(Account), 1, file);
    fclose(file);
}

// Function to generate a unique account number
int generateAccountNumber() {
    static int lastAccountNumber = 1000; // Starts from 1001
    return ++lastAccountNumber;
}

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<time.h>

typedef struct {
    char name[100];
    char gender[10];
    long long int account_number;
    int age;
    char phone_number[11];
    int balance;
    int amount_deposited_last_time;
    int amount_withdrawn;
} client;

void AddAccount(client **clients_info, int *num_accounts, char name[100], char gender[10], long long int account_number, int age, char phone_number[11]) {
    *num_accounts += 1;
    clients_info = (client)realloc(*clients_info, (*num_accounts) * sizeof(client));
    if (*clients_info == NULL) {
        printf("Memory allocation failed! Exiting...\n");
        exit(EXIT_FAILURE);
    }
    strcpy((*clients_info)[*num_accounts-1].name, name);
    strcpy((*clients_info)[*num_accounts-1].gender, gender);
    (*clients_info)[*num_accounts-1].account_number = account_number;
    (*clients_info)[*num_accounts-1].age = age;
    strcpy((*clients_info)[*num_accounts-1].phone_number, phone_number);
    (*clients_info)[*num_accounts-1].balance = 0; // Initial balance
    printf("Account added successfully!\n");
}

void ViewAllAccounts(client *clients_info, int num_accounts) {
    if (num_accounts == 0) {
        printf("No accounts available!\n");
        return;
    }
    printf("\nAll Accounts:\n");
    printf("---------------------------------------------------------\n");
    for (int i = 0; i < num_accounts; i++) {
        printf("Name: %s\n", clients_info[i].name);
        printf("Gender: %s\n", clients_info[i].gender);
        printf("Account Number: %lld\n", clients_info[i].account_number);
        printf("Age: %d\n", clients_info[i].age);
        printf("Phone Number: %s\n", clients_info[i].phone_number);
        printf("Balance: %d\n", clients_info[i].balance);
        printf("---------------------------------------------------------\n");
    }
}

void ViewAccount(client *clients_info, int num_accounts) {
    long long int account_number_to_view;
    printf("Enter the account number to view: ");
    scanf("%lld", &account_number_to_view);

    int account_index = -1;
    for (int i = 0; i < num_accounts; i++) {
        if (clients_info[i].account_number == account_number_to_view) {
            account_index = i;
            break;
        }
    }

    if (account_index == -1) {
        printf("Account not found!\n");
        return;
    }

    client account = clients_info[account_index];
    printf("Name: %s\n", account.name);
    printf("Gender: %s\n", account.gender);
    printf("Account Number: %lld\n", account.account_number);
    printf("Age: %d\n", account.age);
    printf("Phone Number: %s\n", account.phone_number);
    printf("Balance: %d\n", account.balance);
}

void DepositMoney(client *clients_info, int num_accounts) {
    long long int account_number_to_deposit;
    int deposit_amount;
    printf("Enter the account number to deposit money: ");
    scanf("%lld", &account_number_to_deposit);

    int account_index = -1;
    for (int i = 0; i < num_accounts; i++) {
        if (clients_info[i].account_number == account_number_to_deposit) {
            account_index = i;
            break;
        }
    }

    if (account_index == -1) {
        printf("Account not found!\n");
        return;
    }

    printf("Enter the deposit amount: ");
    scanf("%d", &deposit_amount);
    if (deposit_amount <= 0) {
        printf("Invalid deposit amount!\n");
        return;
    }

    clients_info[account_index].balance += deposit_amount;
    clients_info[account_index].amount_deposited_last_time = deposit_amount;
    printf("Deposited %d to account number %lld. New balance: %d\n", deposit_amount, account_number_to_deposit, clients_info[account_index].balance);
}

void DeleteAccount(client **clients_info, int *num_accounts) {
    long long int account_number_to_delete;
    printf("Enter the account number to delete: ");
    scanf("%lld", &account_number_to_delete);

    // Find the account in the array
    int account_index = -1;
    for (int i = 0; i < *num_accounts; i++) {
        if ((*clients_info)[i].account_number == account_number_to_delete) {
            account_index = i;
            break;
        }
    }

    if (account_index == -1) {
        printf("Account not found!\n");
        return;
    }

    // Shift the elements to delete the account from the array
    for (int i = account_index; i < *num_accounts - 1; i++) {
        (*clients_info)[i] = (*clients_info)[i + 1];
    }

    *num_accounts -= 1;
    clients_info = (client)realloc(*clients_info, (*num_accounts) * sizeof(client));

    if (*clients_info == NULL && *num_accounts > 0) {
        printf("Memory reallocation failed! Exiting...\n");
        exit(EXIT_FAILURE);
    }

    printf("Account with number %lld deleted successfully!\n", account_number_to_delete);
}

int main() {
    int entry, num_Accounts = 0;
    client *clients_info = NULL;
    srand((unsigned int)time(NULL));

    while(1) {
        printf("Enter 1 to add account, 2 to view account, 3 to delete account, 4 to view all accounts, 5 to deposit money, 6 to exit: ");
        scanf("%d", &entry);
        if (entry == 6) {
            free(clients_info);
            return 0;
        }
        getchar();
        
        printf("\n\n*\n\n");
        
        printf("\t\t\tWelcome to Magadha Bank!\n\n");
        
        printf("*\n\n");
        
        if (entry == 1) {
            client member;
            printf("Enter the account holder name: ");
            scanf("%[^\n]", member.name);
            getchar();
            
            printf("Enter your gender (Male / Female / Others): ");
            scanf("%s", member.gender);
            if (strcmp(member.gender, "Male") != 0 && 
                strcmp(member.gender, "Female") != 0 && 
                strcmp(member.gender, "Others") != 0) {
                printf("Invalid Gender!\n");
                continue;
            }
            
            printf("Enter your age: ");
            scanf("%d", &member.age);
            if (member.age <= 8) {
                printf("Invalid age!\n");
                continue;
            }
            
            printf("Enter your phone number: ");
            scanf("%s", member.phone_number);
            if (strlen(member.phone_number) != 10) {
                printf("Invalid Phone Number!\n");
                continue;
            }
            
            member.account_number = ((long long int)rand() * rand()) % 9000000000LL + 1000000000LL;
            printf("Here is your allocated Account Number: %lld\n", member.account_number);
            
            AddAccount(&clients_info, &num_Accounts, member.name, member.gender, 
                      member.account_number, member.age, member.phone_number);
        }
        else if (entry == 2) {
            ViewAccount(clients_info, num_Accounts);
        }
        else if (entry == 3) {
            DeleteAccount(&clients_info, &num_Accounts);
        }
        else if (entry == 4) {
            ViewAllAccounts(clients_info, num_Accounts);
        }
        else if (entry == 5) {
            DepositMoney(clients_info, num_Accounts);
        }
        else {
            printf("Invalid option! Try again.\n");
        }
    }
    return 0;
}
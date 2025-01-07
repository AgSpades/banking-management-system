#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>

#define MAX_NAME_LENGTH 100
#define MAX_GENDER_LENGTH 10
#define MAX_PHONE_LENGTH 11
#define MAX_TRANS_TYPE_LENGTH 20
#define MIN_ACCOUNT_AGE 18
#define MIN_TRANSFER_AMOUNT 1
#define MAX_TRANSFER_AMOUNT 1000000
#define DATA_FILE "clients_data.dat"

typedef struct
{
    char name[MAX_NAME_LENGTH];
    char gender[MAX_GENDER_LENGTH];
    long long int account_number;
    int age;
    char phone_number[MAX_PHONE_LENGTH];
    int balance;
    int last_transaction_amount;
    char last_transaction_type[MAX_TRANS_TYPE_LENGTH];
    time_t creation_date;
} client;

// Global variables
client *clients_info = NULL;
int num_accounts = 0;

// Function prototypes
void AddAccount();
void ViewAccount();
void ViewAllAccounts();
void DeleteAccount();
void DepositMoney();
void WithdrawMoney();
void TransferFunds();
void SaveData();
void LoadData();
void GenerateReport();
void DisplayMenu();
void ClearScreen();
void ClearInputBuffer();
bool ValidatePhoneNumber(const char *phone);
bool ValidateName(const char *name);
bool ValidateGender(const char *gender);
void PrintReceipt(const char *transaction_type, long long int account_number, long long amount, int new_balance);

// Utility functions
long long int GenerateAccountNumber();
int FindAccountIndex(long long int account_number);

void ClearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

bool ValidatePhoneNumber(const char *phone)
{
    if (strlen(phone) != 10)
        return false;
    for (int i = 0; phone[i]; i++)
    {
        if (!isdigit(phone[i]))
            return false;
    }
    return true;
}

bool ValidateName(const char *name)
{
    if (strlen(name) < 2 || strlen(name) >= MAX_NAME_LENGTH)
        return false;
    for (int i = 0; name[i]; i++)
    {
        if (!isalpha(name[i]) && !isspace(name[i]))
            return false;
    }
    return true;
}

bool ValidateGender(const char *gender)
{
    return (strcasecmp(gender, "Male") == 0 ||
            strcasecmp(gender, "Female") == 0 ||
            strcasecmp(gender, "Other") == 0);
}

void PrintReceipt(const char *transaction_type, long long int account_number, long long amount, int new_balance)
{
    printf("\n----- Transaction Receipt -----\n");
    printf("Transaction Type: %s\n", transaction_type);
    printf("Account Number: %lld\n", account_number);
    printf("Amount: %d\n", amount);
    printf("New Balance: %d\n", new_balance);
    printf("Date: %s", ctime(&(time_t){time(NULL)}));
    printf("----------------------------\n");
}

void AddAccount()
{
    ClearScreen();
    client new_client;
    char temp_name[MAX_NAME_LENGTH];

    printf("Enter the account holder's name: ");
    fgets(temp_name, MAX_NAME_LENGTH, stdin);
    temp_name[strcspn(temp_name, "\n")] = 0;

    if (!ValidateName(temp_name))
    {
        printf("Invalid name format! Use only letters and spaces.\n");
        return;
    }
    strcpy(new_client.name, temp_name);

    printf("Enter the gender (Male/Female/Other): ");
    scanf("%s", new_client.gender);
    ClearInputBuffer();
    if (!ValidateGender(new_client.gender))
    {
        printf("Invalid gender! Please enter Male, Female, or Other.\n");
        return;
    }

    printf("Enter the age: ");
    if (scanf("%d", &new_client.age) != 1 || new_client.age < MIN_ACCOUNT_AGE)
    {
        ClearInputBuffer();
        printf("Invalid age! Must be %d or above.\n", MIN_ACCOUNT_AGE);
        return;
    }
    ClearInputBuffer();

    printf("Enter the phone number (10 digits): ");
    scanf("%s", new_client.phone_number);
    ClearInputBuffer();
    if (!ValidatePhoneNumber(new_client.phone_number))
    {
        printf("Invalid phone number! Must be 10 digits.\n");
        return;
    }

    new_client.account_number = GenerateAccountNumber();
    new_client.balance = 0;
    new_client.last_transaction_amount = 0;
    strcpy(new_client.last_transaction_type, "None");
    new_client.creation_date = time(NULL);

    num_accounts++;
    client *temp = realloc(clients_info, num_accounts * sizeof(client));
    if (!temp)
    {
        printf("Memory allocation failed! Exiting...\n");
        exit(EXIT_FAILURE);
    }
    clients_info = temp;

    clients_info[num_accounts - 1] = new_client;
    printf("Account created successfully! Your account number is: %lld\n", new_client.account_number);
}

void ViewAccount()
{
    ClearScreen();
    long long int account_number;
    printf("Enter the account number to view: ");
    if (scanf("%lld", &account_number) != 1)
    {
        ClearInputBuffer();
        printf("Invalid account number format!\n");
        return;
    }
    ClearInputBuffer();

    int index = FindAccountIndex(account_number);
    if (index == -1)
    {
        printf("Account not found!\n");
        return;
    }

    client account = clients_info[index];
    printf("\n----- Account Details -----\n");
    printf("Name: %s\n", account.name);
    printf("Gender: %s\n", account.gender);
    printf("Account Number: %lld\n", account.account_number);
    printf("Age: %d\n", account.age);
    printf("Phone Number: %s\n", account.phone_number);
    printf("Balance: %d\n", account.balance);
    printf("Account Creation Date: %s", ctime(&account.creation_date));
    printf("Last Transaction: %s %d\n", account.last_transaction_type, account.last_transaction_amount);
    printf("------------------------\n");
}

void TransferFunds()
{
    ClearScreen();
    long long int from_account, to_account;
    long long amount;

    printf("Enter the account number to transfer from: ");
    if (scanf("%lld", &from_account) != 1)
    {
        ClearInputBuffer();
        printf("Invalid account number format!\n");
        return;
    }
    ClearInputBuffer();

    int from_index = FindAccountIndex(from_account);
    if (from_index == -1)
    {
        printf("Source account not found!\n");
        return;
    }

    printf("Enter the account number to transfer to: ");
    if (scanf("%lld", &to_account) != 1)
    {
        ClearInputBuffer();
        printf("Invalid account number format!\n");
        return;
    }
    ClearInputBuffer();

    if (from_account == to_account)
    {
        printf("Cannot transfer to the same account!\n");
        return;
    }

    int to_index = FindAccountIndex(to_account);
    if (to_index == -1)
    {
        printf("Destination account not found!\n");
        return;
    }

    printf("Enter the transfer amount: ");
    if (scanf("%d", &amount) != 1)
    {
        ClearInputBuffer();
        printf("Invalid amount format!\n");
        return;
    }
    ClearInputBuffer();

    if (amount < MIN_TRANSFER_AMOUNT || amount > MAX_TRANSFER_AMOUNT)
    {
        printf("Transfer amount must be between %d and %d!\n",
               MIN_TRANSFER_AMOUNT, MAX_TRANSFER_AMOUNT);
        return;
    }

    if (amount > clients_info[from_index].balance)
    {
        printf("Insufficient funds for transfer!\n");
        return;
    }

    clients_info[from_index].balance -= amount;
    clients_info[to_index].balance += amount;

    clients_info[from_index].last_transaction_amount = amount;
    strcpy(clients_info[from_index].last_transaction_type, "Transfer Out");

    clients_info[to_index].last_transaction_amount = amount;
    strcpy(clients_info[to_index].last_transaction_type, "Transfer In");

    PrintReceipt("Transfer Out", from_account, amount, clients_info[from_index].balance);
    PrintReceipt("Transfer In", to_account, amount, clients_info[to_index].balance);
}

void SaveData()
{
    FILE *file = fopen(DATA_FILE, "wb");
    if (!file)
    {
        printf("Error opening file for saving!\n");
        return;
    }

    fwrite(&num_accounts, sizeof(int), 1, file);
    size_t written = fwrite(clients_info, sizeof(client), num_accounts, file);

    if (written != num_accounts)
    {
        printf("Error writing data to file!\n");
        fclose(file);
        return;
    }

    fclose(file);
    printf("Data saved successfully!\n");
}

void LoadData()
{
    FILE *file = fopen(DATA_FILE, "rb");
    if (!file)
    {
        printf("No saved data found!\n");
        return;
    }

    int temp_num_accounts;
    if (fread(&temp_num_accounts, sizeof(int), 1, file) != 1)
    {
        printf("Error reading file header!\n");
        fclose(file);
        return;
    }

    client *temp = malloc(temp_num_accounts * sizeof(client));
    if (!temp)
    {
        printf("Memory allocation failed!\n");
        fclose(file);
        return;
    }

    size_t read = fread(temp, sizeof(client), temp_num_accounts, file);
    if (read != temp_num_accounts)
    {
        printf("Error reading client data!\n");
        free(temp);
        fclose(file);
        return;
    }

    free(clients_info);
    clients_info = temp;
    num_accounts = temp_num_accounts;

    fclose(file);
    printf("Data loaded successfully!\n");
}

void ViewAllAccounts()
{
    ClearScreen();
    if (num_accounts == 0)
    {
        printf("No accounts to display!\n");
        return;
    }

    printf("\n----- All Accounts -----\n");
    for (int i = 0; i < num_accounts; i++)
    {
        client account = clients_info[i];
        printf("Name: %s\n", account.name);
        printf("Gender: %s\n", account.gender);
        printf("Account Number: %lld\n", account.account_number);
        printf("Age: %d\n", account.age);
        printf("Phone Number: %s\n", account.phone_number);
        printf("Balance: %d\n", account.balance);
        printf("Account Creation Date: %s", ctime(&account.creation_date));
        printf("Last Transaction: %s %d\n", account.last_transaction_type, account.last_transaction_amount);
        printf("------------------------\n");
    }
}

void DeleteAccount()
{
    ClearScreen();
    long long int account_number;
    printf("Enter the account number to delete: ");
    if (scanf("%lld", &account_number) != 1)
    {
        ClearInputBuffer();
        printf("Invalid account number format!\n");
        return;
    }
    ClearInputBuffer();

    int index = FindAccountIndex(account_number);
    if (index == -1)
    {
        printf("Account not found!\n");
        return;
    }

    for (int i = index; i < num_accounts - 1; i++)
    {
        clients_info[i] = clients_info[i + 1];
    }
    num_accounts--;
    client *temp = realloc(clients_info, num_accounts * sizeof(client));
    if (!temp && num_accounts > 0)
    {
        printf("Memory reallocation failed! Exiting...\n");
        exit(EXIT_FAILURE);
    }
    clients_info = temp;

    printf("Account deleted successfully!\n");
}

void DepositMoney()
{
    ClearScreen();
    long long int account_number;
    long long amount;

    printf("Enter the account number to deposit into: ");
    if (scanf("%lld", &account_number) != 1)
    {
        ClearInputBuffer();
        printf("Invalid account number format!\n");
        return;
    }
    ClearInputBuffer();

    int index = FindAccountIndex(account_number);
    if (index == -1)
    {
        printf("Account not found!\n");
        return;
    }

    printf("Enter the deposit amount: ");
    if (scanf("%d", &amount) != 1 || amount <= 0)
    {
        ClearInputBuffer();
        printf("Invalid amount format!\n");
        return;
    }
    ClearInputBuffer();

    clients_info[index].balance += amount;
    clients_info[index].last_transaction_amount = amount;
    strcpy(clients_info[index].last_transaction_type, "Deposit");

    PrintReceipt("Deposit", account_number, amount, clients_info[index].balance);
}

void WithdrawMoney()
{
    ClearScreen();
    long long int account_number;
    long long amount;

    printf("Enter the account number to withdraw from: ");
    if (scanf("%lld", &account_number) != 1)
    {
        ClearInputBuffer();
        printf("Invalid account number format!\n");
        return;
    }
    ClearInputBuffer();

    int index = FindAccountIndex(account_number);
    if (index == -1)
    {
        printf("Account not found!\n");
        return;
    }

    printf("Enter the withdrawal amount: ");
    if (scanf("%d", &amount) != 1 || amount <= 0)
    {
        ClearInputBuffer();
        printf("Invalid amount format!\n");
        return;
    }
    ClearInputBuffer();

    if (amount > clients_info[index].balance)
    {
        printf("Insufficient funds for withdrawal!\n");
        return;
    }

    clients_info[index].balance -= amount;
    clients_info[index].last_transaction_amount = amount;
    strcpy(clients_info[index].last_transaction_type, "Withdrawal");

    PrintReceipt("Withdrawal", account_number, amount, clients_info[index].balance);
}

void GenerateReport()
{
    ClearScreen();
    if (num_accounts == 0)
    {
        printf("No accounts to generate report!\n");
        return;
    }

    int total_balance = 0;
    for (int i = 0; i < num_accounts; i++)
    {
        total_balance += clients_info[i].balance;
    }

    printf("\n----- Bank Report -----\n");
    printf("Total number of accounts: %d\n", num_accounts);
    printf("Total balance across all accounts: %d\n", total_balance);
    printf("------------------------\n");
}

void DisplayMenu()
{
    printf("\n----- Magadha Bank Management System -----\n");
    printf("1. Add Account\n");
    printf("2. View Account\n");
    printf("3. View All Accounts\n");
    printf("4. Delete Account\n");
    printf("5. Deposit Money\n");
    printf("6. Withdraw Money\n");
    printf("7. Transfer Funds\n");
    printf("8. Generate Report\n");
    printf("9. Save Data\n");
    printf("10. Load Data\n");
    printf("11. Exit\n");
    printf("------------------------------------------\n");
}

long long int GenerateAccountNumber()
{
    return (rand() % 9000000000LL) + 1000000000LL;
}

int FindAccountIndex(long long int account_number)
{
    for (int i = 0; i < num_accounts; i++)
    {
        if (clients_info[i].account_number == account_number)
        {
            return i;
        }
    }
    return -1;
}

void ClearScreen()
{
    printf("\033[H\033[J");
}
int main()
{
    srand((unsigned int)time(NULL));
    LoadData(); // Load existing data at startup

    int choice;
    while (1)
    {
        DisplayMenu();
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            ClearInputBuffer();
            printf("Invalid input! Please enter a number.\n");
            continue;
        }
        ClearInputBuffer();

        switch (choice)
        {
        case 1:
            AddAccount();
            break;
        case 2:
            ViewAccount();
            break;
        case 3:
            ViewAllAccounts();
            break;
        case 4:
            DeleteAccount();
            break;
        case 5:
            DepositMoney();
            break;
        case 6:
            WithdrawMoney();
            break;
        case 7:
            TransferFunds();
            break;
        case 8:
            GenerateReport();
            break;
        case 9:
            SaveData();
            break;
        case 10:
            LoadData();
            break;
        case 11:
            SaveData();
            free(clients_info);
            printf("Thank you for using Magadha Bank! Goodbye!\n");
            return 0;
        default:
            printf("Invalid choice! Please enter a number between 1 and 11.\n");
        }
    }
}
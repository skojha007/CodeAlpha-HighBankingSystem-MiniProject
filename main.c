#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ACCOUNTS 100

struct BankAccount {
    int accountNumber;
    char holderName[50];
    float balance;
};

/* Global account store */
struct BankAccount accounts[MAX_ACCOUNTS];
int accountCount = 0;

/* Saves all accounts to binary file "account.dat" (count + all structs) */
void saveAllToFile() {
    FILE *fp = fopen("account.dat", "wb");
    if (fp == NULL) {
        printf("Error: Could not open file for writing.\n");
        return;
    }
    fwrite(&accountCount, sizeof(int), 1, fp);
    fwrite(accounts, sizeof(struct BankAccount), accountCount, fp);
    fclose(fp);
}

/* Loads all accounts from "account.dat"; starts empty if file doesn't exist */
void loadAllFromFile() {
    FILE *fp = fopen("account.dat", "rb");
    if (fp == NULL) {
        accountCount = 0;
        return;
    }
    fread(&accountCount, sizeof(int), 1, fp);
    fread(accounts, sizeof(struct BankAccount), accountCount, fp);
    fclose(fp);
    printf("Loaded %d account(s) from file.\n", accountCount);
}

/* Searches accounts by account number; returns index or -1 if not found */
int findAccount(int accNum) {
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accNum) {
            return i;
        }
    }
    return -1;
}

/* Checks whether an account number already exists in the store */
int accountExists(int accNum) {
    return findAccount(accNum) != -1;
}

/* Creates a new bank account, checks for duplicates, and saves to file */
void createAccount() {
    if (accountCount >= MAX_ACCOUNTS) {
        printf("Error: Maximum account limit (%d) reached.\n", MAX_ACCOUNTS);
        return;
    }

    struct BankAccount newAcc;

    printf("\n--- Create New Account ---\n");
    printf("Enter Account Number: ");
    scanf("%d", &newAcc.accountNumber);

    if (accountExists(newAcc.accountNumber)) {
        printf("Error: Account number %d already exists.\n", newAcc.accountNumber);
        return;
    }

    printf("Enter Account Holder Name: ");
    getchar();
    fgets(newAcc.holderName, 50, stdin);
    newAcc.holderName[strcspn(newAcc.holderName, "\n")] = '\0';

    printf("Enter Initial Deposit Amount: Rs.");
    scanf("%f", &newAcc.balance);
    while (newAcc.balance < 0) {
        printf("Initial deposit cannot be negative. Enter amount: Rs.");
        scanf("%f", &newAcc.balance);
    }

    accounts[accountCount++] = newAcc;
    saveAllToFile();
    printf("Account created successfully! (Account Number: %d)\n", newAcc.accountNumber);
}

/* Deposits a validated amount into the account with the given number */
void deposit() {
    int accNum;
    printf("\nEnter Account Number: ");
    scanf("%d", &accNum);

    int idx = findAccount(accNum);
    if (idx == -1) {
        printf("Error: Account number %d not found.\n", accNum);
        return;
    }

    float amount;
    printf("Enter amount to deposit: Rs.");
    scanf("%f", &amount);
    if (amount <= 0) {
        printf("Error: Deposit amount must be greater than 0.\n");
        return;
    }

    accounts[idx].balance += amount;
    saveAllToFile();
    printf("Amount Rs.%.2f deposited successfully!\n", amount);
    printf("New Balance: Rs.%.2f\n", accounts[idx].balance);
}

/* Withdraws a validated amount from the account with the given number */
void withdraw() {
    int accNum;
    printf("\nEnter Account Number: ");
    scanf("%d", &accNum);

    int idx = findAccount(accNum);
    if (idx == -1) {
        printf("Error: Account number %d not found.\n", accNum);
        return;
    }

    float amount;
    printf("Enter amount to withdraw: Rs.");
    scanf("%f", &amount);
    if (amount <= 0) {
        printf("Error: Withdrawal amount must be greater than 0.\n");
        return;
    }
    if (amount > accounts[idx].balance) {
        printf("Error: Insufficient Funds.\n");
        return;
    }

    accounts[idx].balance -= amount;
    saveAllToFile();
    printf("Amount Rs.%.2f withdrawn successfully!\n", amount);
    printf("Remaining Balance: Rs.%.2f\n", accounts[idx].balance);
}

/* Displays the account number, holder name, and balance for a given account */
void checkBalance() {
    int accNum;
    printf("\nEnter Account Number: ");
    scanf("%d", &accNum);

    int idx = findAccount(accNum);
    if (idx == -1) {
        printf("Error: Account number %d not found.\n", accNum);
        return;
    }

    printf("\n--- Balance Enquiry ---\n");
    printf("Account Number : %d\n", accounts[idx].accountNumber);
    printf("Account Holder : %s\n", accounts[idx].holderName);
    printf("Current Balance: Rs.%.2f\n", accounts[idx].balance);
}

/* Closes an account by number after showing details and asking for confirmation */
void closeAccount() {
    int accNum;
    printf("\nEnter Account Number to close: ");
    scanf("%d", &accNum);

    int idx = findAccount(accNum);
    if (idx == -1) {
        printf("Error: Account number %d not found.\n", accNum);
        return;
    }

    printf("\n--- Account to be Closed ---\n");
    printf("Account Number : %d\n", accounts[idx].accountNumber);
    printf("Account Holder : %s\n", accounts[idx].holderName);
    printf("Current Balance: Rs.%.2f\n", accounts[idx].balance);

    if (accounts[idx].balance > 0) {
        printf("Warning: This account still has Rs.%.2f. Closing will forfeit the balance.\n",
               accounts[idx].balance);
    }

    printf("Are you sure you want to close this account? (y/n): ");
    char confirm;
    getchar();
    scanf("%c", &confirm);

    if (confirm != 'y' && confirm != 'Y') {
        printf("Account closure cancelled.\n");
        return;
    }

    /* Shift all accounts after idx one position left to fill the gap */
    for (int i = idx; i < accountCount - 1; i++) {
        accounts[i] = accounts[i + 1];
    }
    accountCount--;
    saveAllToFile();

    printf("Account %d has been closed successfully.\n", accNum);
}

/* Prints a summary of all accounts currently stored */
void listAllAccounts() {
    if (accountCount == 0) {
        printf("\nNo accounts found.\n");
        return;
    }
    printf("\n--- All Accounts (%d) ---\n", accountCount);
    printf("%-15s %-25s %-15s\n", "Acct Number", "Holder Name", "Balance");
    printf("-------------------------------------------------------\n");
    for (int i = 0; i < accountCount; i++) {
        printf("%-15d %-25s Rs.%-12.2f\n",
               accounts[i].accountNumber,
               accounts[i].holderName,
               accounts[i].balance);
    }
}

/* Prints the main menu options */
void displayMenu() {
    printf("\n*************************************\n");
    printf("*   BANK ACCOUNT MANAGEMENT SYSTEM  *\n");
    printf("*************************************\n");
    printf("1. Create New Account\n");
    printf("2. Deposit\n");
    printf("3. Withdraw\n");
    printf("4. Balance Enquiry\n");
    printf("5. List All Accounts\n");
    printf("6. Close Account\n");
    printf("7. Exit\n");
    printf("Enter your choice: ");
}

int main() {
    loadAllFromFile();

    int choice;
    while (1) {
        displayMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                createAccount();
                break;
            case 2:
                deposit();
                break;
            case 3:
                withdraw();
                break;
            case 4:
                checkBalance();
                break;
            case 5:
                listAllAccounts();
                break;
            case 6:
                closeAccount();
                break;
            case 7:
                printf("Thank you for banking with us!\n");
                exit(0);
            default:
                printf("Invalid choice. Please enter 1-7.\n");
        }
    }

    return 0;
}

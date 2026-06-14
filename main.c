#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct BankAccount {
    int accountNumber;
    char holderName[50];
    float balance;
};

/* Saves the bank account struct to a binary file "account.dat" */
void saveToFile(struct BankAccount acc) {
    FILE *fp = fopen("account.dat", "wb");
    if (fp == NULL) {
        printf("Error: Could not open file for writing.\n");
        return;
    }
    fwrite(&acc, sizeof(struct BankAccount), 1, fp);
    fclose(fp);
}

/* Creates a new bank account by taking user input and saving to file */
void createAccount(struct BankAccount *acc) {
    printf("\n--- Create New Account ---\n");
    printf("Enter Account Number: ");
    scanf("%d", &acc->accountNumber);
    printf("Enter Account Holder Name: ");
    getchar();
    fgets(acc->holderName, 50, stdin);
    acc->holderName[strcspn(acc->holderName, "\n")] = '\0';
    printf("Enter Initial Deposit Amount: Rs.");
    scanf("%f", &acc->balance);
    while (acc->balance < 0) {
        printf("Initial deposit cannot be negative. Enter amount: Rs.");
        scanf("%f", &acc->balance);
    }
    saveToFile(*acc);
    printf("Account created successfully!\n");
}

/* Loads account data from binary file; creates new account if file is missing */
void loadFromFile(struct BankAccount *acc) {
    FILE *fp = fopen("account.dat", "rb");
    if (fp == NULL) {
        printf("No existing account found. Please create a new account.\n");
        createAccount(acc);
        return;
    }
    fread(acc, sizeof(struct BankAccount), 1, fp);
    fclose(fp);
}

/* Deposits a validated amount into the account and saves updated data */
void deposit(struct BankAccount *acc) {
    float amount;
    printf("\nEnter amount to deposit: Rs.");
    scanf("%f", &amount);
    if (amount <= 0) {
        printf("Error: Deposit amount must be greater than 0.\n");
        return;
    }
    acc->balance += amount;
    saveToFile(*acc);
    printf("Amount Rs.%.2f deposited successfully!\n", amount);
    printf("New Balance: Rs.%.2f\n", acc->balance);
}

/* Withdraws a validated amount from the account if sufficient funds exist */
void withdraw(struct BankAccount *acc) {
    float amount;
    printf("\nEnter amount to withdraw: Rs.");
    scanf("%f", &amount);
    if (amount <= 0) {
        printf("Error: Withdrawal amount must be greater than 0.\n");
        return;
    }
    if (amount > acc->balance) {
        printf("Error: Insufficient Funds.\n");
        return;
    }
    acc->balance -= amount;
    saveToFile(*acc);
    printf("Amount Rs.%.2f withdrawn successfully!\n", amount);
    printf("Remaining Balance: Rs.%.2f\n", acc->balance);
}

/* Displays the account number, holder name, and current balance */
void checkBalance(struct BankAccount acc) {
    printf("\n--- Balance Enquiry ---\n");
    printf("Account Number : %d\n", acc.accountNumber);
    printf("Account Holder : %s\n", acc.holderName);
    printf("Current Balance: Rs.%.2f\n", acc.balance);
}

/* Prints the main menu options */
void displayMenu() {
    printf("\n*************************************\n");
    printf("*   BANK ACCOUNT MANAGEMENT SYSTEM  *\n");
    printf("*************************************\n");
    printf("1. Deposit\n");
    printf("2. Withdraw\n");
    printf("3. Balance Enquiry\n");
    printf("4. Exit\n");
    printf("Enter your choice: ");
}

int main() {
    struct BankAccount account;
    int choice;

    loadFromFile(&account);

    while (1) {
        displayMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                deposit(&account);
                break;
            case 2:
                withdraw(&account);
                break;
            case 3:
                checkBalance(account);
                break;
            case 4:
                printf("Thank you for banking with us!\n");
                exit(0);
            default:
                printf("Invalid choice. Please enter 1-4.\n");
        }
    }

    return 0;
}

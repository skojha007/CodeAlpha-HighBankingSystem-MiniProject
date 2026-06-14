#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ACCOUNTS 100

struct BankAccount {
    int accountNumber;
    char holderName[50];
    float balance;
};

/* Global account store */
struct BankAccount accounts[MAX_ACCOUNTS];
int accountCount = 0;

/* Appends a single transaction record to "transactions.log" with timestamp */
void logTransaction(int accNum, const char *type, float amount, float balanceAfter) {
    FILE *fp = fopen("transactions.log", "a");
    if (fp == NULL) {
        printf("Warning: Could not write to transaction log.\n");
        return;
    }
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", t);
    fprintf(fp, "[%s] Acct:%-6d | %-18s | Amount: Rs.%10.2f | Balance: Rs.%10.2f\n",
            timestamp, accNum, type, amount, balanceAfter);
    fclose(fp);
}

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
    logTransaction(newAcc.accountNumber, "ACCOUNT OPENED", newAcc.balance, newAcc.balance);
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
    logTransaction(accounts[idx].accountNumber, "DEPOSIT", amount, accounts[idx].balance);
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
    logTransaction(accounts[idx].accountNumber, "WITHDRAWAL", amount, accounts[idx].balance);
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

/* Transfers a validated amount from one account to another atomically */
void transferFunds() {
    int fromAccNum, toAccNum;

    printf("\n--- Fund Transfer ---\n");
    printf("Enter Source Account Number: ");
    scanf("%d", &fromAccNum);

    int fromIdx = findAccount(fromAccNum);
    if (fromIdx == -1) {
        printf("Error: Source account %d not found.\n", fromAccNum);
        return;
    }

    printf("Enter Destination Account Number: ");
    scanf("%d", &toAccNum);

    if (fromAccNum == toAccNum) {
        printf("Error: Source and destination accounts cannot be the same.\n");
        return;
    }

    int toIdx = findAccount(toAccNum);
    if (toIdx == -1) {
        printf("Error: Destination account %d not found.\n", toAccNum);
        return;
    }

    printf("Transfer from : %s (Balance: Rs.%.2f)\n",
           accounts[fromIdx].holderName, accounts[fromIdx].balance);
    printf("Transfer to   : %s\n", accounts[toIdx].holderName);

    float amount;
    printf("Enter amount to transfer: Rs.");
    scanf("%f", &amount);

    if (amount <= 0) {
        printf("Error: Transfer amount must be greater than 0.\n");
        return;
    }
    if (amount > accounts[fromIdx].balance) {
        printf("Error: Insufficient Funds in source account.\n");
        printf("Available Balance: Rs.%.2f\n", accounts[fromIdx].balance);
        return;
    }

    /* Perform the atomic transfer — debit source, credit destination */
    accounts[fromIdx].balance -= amount;
    accounts[toIdx].balance   += amount;
    saveAllToFile();
    logTransaction(accounts[fromIdx].accountNumber, "TRANSFER OUT", amount, accounts[fromIdx].balance);
    logTransaction(accounts[toIdx].accountNumber,   "TRANSFER IN",  amount, accounts[toIdx].balance);

    printf("\nTransfer successful!\n");
    printf("Rs.%.2f transferred from Account %d to Account %d.\n",
           amount, fromAccNum, toAccNum);
    printf("%-20s New Balance: Rs.%.2f\n",
           accounts[fromIdx].holderName, accounts[fromIdx].balance);
    printf("%-20s New Balance: Rs.%.2f\n",
           accounts[toIdx].holderName, accounts[toIdx].balance);
}

/* Reads transactions.log and displays entries for a given account number */
void viewTransactionHistory() {
    int accNum;
    printf("\nEnter Account Number to view history: ");
    scanf("%d", &accNum);

    if (findAccount(accNum) == -1) {
        printf("Error: Account number %d not found.\n", accNum);
        return;
    }

    FILE *fp = fopen("transactions.log", "r");
    if (fp == NULL) {
        printf("No transaction history found.\n");
        return;
    }

    char line[256];
    char searchKey[16];
    /* Build the exact account field pattern to match log entries */
    snprintf(searchKey, sizeof(searchKey), "Acct:%-6d", accNum);

    int found = 0;
    printf("\n--- Transaction History for Account %d ---\n", accNum);
    printf("--------------------------------------------------------------------\n");
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (strstr(line, searchKey) != NULL) {
            printf("%s", line);
            found++;
        }
    }
    fclose(fp);

    if (found == 0) {
        printf("No transactions found for account %d.\n", accNum);
    } else {
        printf("--------------------------------------------------------------------\n");
        printf("Total transactions: %d\n", found);
    }
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
    printf("6. Transaction History\n");
    printf("7. Transfer Funds\n");
    printf("8. Close Account\n");
    printf("9. Exit\n");
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
                viewTransactionHistory();
                break;
            case 7:
                transferFunds();
                break;
            case 8:
                closeAccount();
                break;
            case 9:
                printf("Thank you for banking with us!\n");
                exit(0);
            default:
                printf("Invalid choice. Please enter 1-9.\n");
        }
    }

    return 0;
}

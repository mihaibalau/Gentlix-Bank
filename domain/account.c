#include <stdlib.h>
#include <string.h>
#include "domain.h"

Account* createAccount(float main_account_balance, const char* tag, const char* first_name,
                       const char* second_name, const char* password, const char* iban,
                       const char* phone_number, Date birthday) {
    Account* account = (Account*)malloc(sizeof(Account));
    if (account == NULL) return NULL;

    account->main_account_balance = main_account_balance;
    account->tag = strdup(tag);
    account->first_name = strdup(first_name);
    account->second_name = strdup(second_name);
    account->password = strdup(password);
    account->iban = strdup(iban);
    account->phone_number = strdup(phone_number);
    account->birthday = birthday;
    account->affiliates = NULL;
    account->transactions = NULL;
    account->transactions_number = 0;
    account->affiliates_number = 0;

    return account;
}

void destroyAccount(Account* account) {
    if (account == NULL) return;
    free(account->tag);
    free(account->first_name);
    free(account->second_name);
    free(account->password);
    free(account->iban);
    free(account->phone_number);

    for (int i = 0; i < account->affiliates_number; i++) {
        destroyAffiliates(&account->affiliates[i]);
    }
    free(account->affiliates);

    for (int i = 0; i < account->transactions_number; i++) {
        destroyTransaction(&account->transaction[i]);
    }
    free(account->transaction);

    free(account);
}

// Getters
float getAccountBalance(const Account* account) {
    return account->main_account_balance;
}

const char* getAccountTag(const Account* account) {
    return account->tag;
}

const char* getAccountFirstName(const Account* account) {
    return account->first_name;
}

const char* getAccountSecondName(const Account* account) {
    return account->second_name;
}

const char* getAccountPassword(const Account* account) {
    return account->password;
}

const char* getAccountIban(const Account* account) {
    return account->iban;
}

const char* getAccountPhoneNumber(const Account* account) {
    return account->phone_number;
}

Date getAccountBirthday(const Account* account) {
    return account->birthday;
}

int getAccountTransactionsNumber(const Account* account) {
    return account->transactions_number;
}

int getAccountAffiliatesNumber(const Account* account) {
    return account->affiliates_number;
}

// Setters
void setAccountBalance(Account* account, float balance) {
    account->main_account_balance = balance;
}

void setAccountTag(Account* account, const char* tag) {
    free(account->tag);
    account->tag = strdup(tag);
}

void setAccountFirstName(Account* account, const char* first_name) {
    free(account->first_name);
    account->first_name = strdup(first_name);
}

void setAccountSecondName(Account* account, const char* second_name) {
    free(account->second_name);
    account->second_name = strdup(second_name);
}

void setAccountPassword(Account* account, const char* password) {
    free(account->password);
    account->password = strdup(password);
}

void setAccountIban(Account* account, const char* iban) {
    free(account->iban);
    account->iban = strdup(iban);
}

void setAccountPhoneNumber(Account* account, const char* phone_number) {
    free(account->phone_number);
    account->phone_number = strdup(phone_number);
}

void setAccountBirthday(Account* account, Date birthday) {
    account->birthday = birthday;
}

// Funcții pentru adăugarea și eliminarea tranzacțiilor și afiliaților
void addTransactionToAccount(Account* account, Transaction transaction) {
    account->transactions_number++;
    account->transactions = realloc(account->transactions, account->transactions_number * sizeof(Transaction));
    account->transactions[account->transactions_number - 1] = transaction;
}

void addAffiliateToAccount(Account* account, Affiliate affiliate) {
    account->affiliates_number++;
    account->affiliates = realloc(account->affiliates, account->affiliates_number * sizeof(Affiliate));
    account->affiliates[account->affiliates_number - 1] = affiliate;
}

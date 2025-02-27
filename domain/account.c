#include <stdlib.h>
#include <string.h>
#include "domain.h"

Account* createAccount(float mainAccountBalance, const char* tag, const char* firstName,
                       const char* second_name, const char* password, const char* iban,
                       const char* phone_number, Date birthday) {
    Account* account = (Account*)malloc(sizeof(Account));
    if (account == NULL) return NULL;

    account->mainAccountBalance = mainAccountBalance;
    account->tag = strdup(tag);
    account->firstName = strdup(firstName);
    account->secondName = strdup(second_name);
    account->password = strdup(password);
    account->iban = strdup(iban);
    account->phoneNumber = strdup(phone_number);
    account->birthday = birthday;

    account->transactionsCapacity = 512;
    account->affiliatesCapacity = 128;
    account->userAccountsCapacity = 8;

    account->affiliates = malloc(account->affiliatesCapacity * sizeof(Affiliate*));;
    account->transactions = malloc(account->affiliatesCapacity * sizeof(Transaction *));
    account->userAccounts = malloc(account->affiliatesCapacity * sizeof(UserAccounts *));

    if (account->affiliates == NULL || account->transactions == NULL || account->userAccounts == NULL) {
        free(account->affiliates);
        free(account->transactions);
        free(account->userAccounts);
        free(account->tag);
        free(account->firstName);
        free(account->secondName);
        free(account->password);
        free(account->iban);
        free(account->phoneNumber);
        free(account);
        return NULL;
    }

    account->transactionsNumber = 0;
    account->affiliatesNumber = 0;
    account->userAccountsNumber = 0;

    return account;
}

void destroyAccount(Account* account) {
    if (account == NULL) return;

    free(account->tag);
    free(account->firstName);
    free(account->secondName);
    free(account->password);
    free(account->iban);
    free(account->phoneNumber);

    for (int i = 0; i < account->affiliatesNumber; i++) {
        destroyAffiliates(account->affiliates[i]);
    }
    free(account->affiliates);

    for (int i = 0; i < account->transactionsNumber; i++) {
        destroyTransaction(account->transactions[i]);
    }
    free(account->transactions);

    for (int i = 0; i < account->userAccountsNumber; i++) {
        destroyUserAccount(account->userAccounts[i]);
    }
    free(account->userAccounts);

    free(account);
}


// Getters
float getAccountBalance(const Account* account) {
    return account->mainAccountBalance;
}

const char* getAccountTag(const Account* account) {
    return account->tag;
}

const char* getAccountFirstName(const Account* account) {
    return account->firstName;
}

const char* getAccountSecondName(const Account* account) {
    return account->secondName;
}

const char* getAccountPassword(const Account* account) {
    return account->password;
}

const char* getAccountIban(const Account* account) {
    return account->iban;
}

const char* getAccountPhoneNumber(const Account* account) {
    return account->phoneNumber;
}

Date getAccountBirthday(const Account* account) {
    return account->birthday;
}

int getAccountTransactionsNumber(const Account* account) {
    return account->transactionsNumber;
}

int getAccountAffiliatesNumber(const Account* account) {
    return account->affiliatesNumber;
}

int getAccountUserAccountsNumber(const Account* account) {
    return account->affiliatesNumber;
}

int getAccountTransactionsCapacity(const Account* account) {
    return account->transactionsCapacity;
}

int getAccountAffiliatesCapacity(const Account* account) {
    return account->affiliatesCapacity;
}

int getAccountUserAccountsCapacity(const Account* account) {
    return account->affiliatesCapacity;
}


// Setters
void setAccountBalance(Account* account, float balance) {
    account->mainAccountBalance = balance;
}

void setAccountTag(Account* account, const char* tag) {
    free(account->tag);
    account->tag = strdup(tag);
}

void setAccountFirstName(Account* account, const char* firstName) {
    free(account->firstName);
    account->firstName = strdup(firstName);
}

void setAccountSecondName(Account* account, const char* second_name) {
    free(account->secondName);
    account->secondName = strdup(second_name);
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
    free(account->phoneNumber);
    account->phoneNumber = strdup(phone_number);
}

void setAccountBirthday(Account* account, Date birthday) {
    account->birthday = birthday;
}

#include "services.h"
#include <gtk/gtk.h>
#include <string.h>
#include "../domain/domain.h"
#include "../repository/repository.h"

////////////////////
//
//  Memory management functions
//
////////////////////

int addTransactionForUser(Account* account, Transaction* newTransaction) {
    if (account == NULL)
        return -201; // Invalid account

    if (newTransaction == NULL)
        return -202; // Invalid transaction

    if (account->transactionsNumber >= account->transactionsCapacity) {
        int newCapacity = account->transactionsCapacity * 2 + 1;
        Transaction** newTransactions = realloc(account->transactions, newCapacity * sizeof(Transaction*));
        if (newTransactions == NULL) {
            return -203; // Memory management error
        }
        account->transactions = newTransactions;
        account->transactionsCapacity = newCapacity;
    }

    account->transactions[account->transactionsNumber] = newTransaction;
    account->transactionsNumber++;

    return 1;
}

Transaction* getLatestTransaction(const Account* account) {
    if (account->transactionsNumber == 0) {
        return NULL;
    }

    return account->transactions[account->transactionsNumber - 1];
}

int addAffiliateToAccount(Account* account, Affiliate* newAffiliate) {
    if (account == NULL)
        return -211; // Invalid account

    if (newAffiliate == NULL)
        return -212; // Invalid transaction

    for (int i = 0; i < account->affiliatesNumber; i++) {
        if (strcmp(getAffiliatesTag(account->affiliates[i]), getAffiliatesTag(newAffiliate)) == 0) {
            return -213; // Affiliate already exists
        }
    }

    if (account->affiliatesNumber >= account->affiliatesCapacity) {
        int newCapacity = account->affiliatesCapacity * 2 + 1;
        Affiliate** newAffiliates = realloc(account->affiliates, newCapacity * sizeof(Affiliate*));
        if (newAffiliates == NULL) {
            return -214; // Memory reallocation failed
        }
        account->affiliates = newAffiliates;
        account->affiliatesCapacity = newCapacity;
    }

    account->affiliates[account->affiliatesNumber] = newAffiliate;
    account->affiliatesNumber++;

    return 1;
}


//void displayAffiliates(Account* account) {
//    for (int i = 0; i < account->affiliatesNumber; i++) {
//        Affiliate* affiliate = account->affiliates[i];
//        // Display affiliate details here, e.g., tag, name, IBAN, etc.
//        printf("Tag: %s, Name: %s %s, IBAN: %s\n", affiliate->tag, affiliate->firstName, affiliate->secondName, affiliate->iban);
//    }
//}

int removeAffiliateFromAccount(Account* account, const char* affiliateTag) {
    if (account == NULL)
        return -221; // Invalid account

    if (affiliateTag == NULL)
        return -222; // Invalid transaction

    int indexToRemove = -1;
    for (int i = 0; i < account->affiliatesNumber; i++) {
        if (strcmp(account->affiliates[i]->tag, affiliateTag) == 0) {
            indexToRemove = i;
            break;
        }
    }

    if (indexToRemove == -1) {
        return -223; // Affiliate not found
    }

    destroyAffiliates(account->affiliates[indexToRemove]);
    free(account->affiliates[indexToRemove]);

    for (int i = indexToRemove; i < account->affiliatesNumber - 1; i++) {
        account->affiliates[i] = account->affiliates[i + 1];
    }

    account->affiliatesNumber--;

    return 1;
}

int addNewUserAccount(Account* account, UserAccounts* newUserAccount) {
    if (account == NULL)
        return -231; // Invalid account

    if (newUserAccount == NULL)
        return -232; // Invalid user account

    for (int i = 0; i < account->userAccountsNumber; i++) {
        if (strcmp(getUserAccountType(account->userAccounts[i]), getUserAccountType(newUserAccount)) == 0) {
            return -233; // User account already exists
        }
    }

    if (account->userAccountsNumber >= account->userAccountsCapacity) {
        int newCapacity = account->userAccountsCapacity * 2 + 1;
        UserAccounts** newUserAccounts = realloc(account->userAccounts, newCapacity * sizeof(UserAccounts*));
        if (newUserAccounts == NULL) {
            return -234; // Memory reallocation failed
        }
        account->userAccounts = newUserAccounts;
        account->userAccountsCapacity = newCapacity;
    }

    account->userAccounts[account->userAccountsNumber] = newUserAccount;
    account->userAccountsNumber++;

    return 1;
}

int removeAnUserAccount(Account* account, const char* userAccountType) {
    if (account == NULL)
        return -241; // Invalid account

    if (userAccountType == NULL)
        return -242; // Invalid user account type

    int indexToRemove = -1;
    for (int i = 0; i < account->userAccountsNumber; i++) {
        if (strcmp(account->userAccounts[i]->type, userAccountType) == 0) {
            indexToRemove = i;
            break;
        }
    }

    if (indexToRemove == -1) {
        return -243; // User account not found
    }

    destroyUserAccount(account->userAccounts[indexToRemove]);
    free(account->userAccounts[indexToRemove]);

    for (int i = indexToRemove; i < account->userAccountsNumber - 1; i++) {
        account->userAccounts[i] = account->userAccounts[i + 1];
    }

    account->userAccountsNumber--;

    return 1;
}

//void displayUserAccounts(Account* account) {
//    for (int i = 0; i < account->userAccountsNumber; i++) {
//        UserAccounts* userAccount = account->userAccounts[i];
//        // Display user account details here, e.g., type, balance
//        printf("Type: %s, Balance: %.2f\n", userAccount->type, userAccount->accountBalance);
//    }
//}

////////////////////
//
//  Check functions
//
////////////////////

int accountTagUsed(const RepositoryFormat* receivedRepository, const gchar *checkedTag){
    const char *charString = checkedTag;
    return accountTagUsedRepo(receivedRepository, charString);
}

int stringOnlyWithLetters(const gchar *checkedString){
    char normalString[20];
    g_strlcpy(normalString, checkedString, sizeof(normalString)); // copy the char allocated dynamic in normal one to be checked
    size_t length = strlen(normalString);
    for(int index = 0; index < length; index++)
        if(normalString[index] < 'A' || normalString[index] > 'z')
            return 0;
    return 1;
}

int stringOnlyWithDigitsExtended(const gchar *checkedString){ // extended means can have also . and , for float numbers
    char normalString[11];
    g_strlcpy(normalString, checkedString, sizeof(normalString)); // copy the char allocated dynamic in normal one to be checked
    size_t length = strlen(normalString);
    for(int index = 0; index < length; index++)
        if((normalString[index] < '0' || normalString[index] > '9') && normalString[index] != ',' && normalString[index] != '.')
            return 0;
    return 1;
}

int stringOnlyWithDigits(const gchar *checkedString){
    char normalString[11];
    g_strlcpy(normalString, checkedString, sizeof(normalString)); // copy the char allocated dynamic in normal one to be checked
    size_t length = strlen(normalString);
    for(int index = 0; index < length; index++)
        if(normalString[index] < '0' || normalString[index] > '9')
            return 0;
    return 1;
}

int differentPassword(const gchar *password1, const gchar *password2){
    if(strcmp(password1, password2))
        return 1;
    return 0;
}

short validDateWithBypass(const gchar *day, const gchar *month, const gchar *year){ // bypass means you can leave the fields blank

    if(!stringOnlyWithDigits(day)){
        return 101; // The day need to be a number!
    }
    else if(!stringOnlyWithDigits(month)){
        return 102; // The month need to be a number!
    }
    else if(!stringOnlyWithDigits(year)){
        return 103; // The year need to be a number
    }

    guint64 intDay = g_ascii_strtoull(day, NULL, 10); // transform a string formatted from digits to an integer
    guint64 intMonth = g_ascii_strtoull(month, NULL, 10); // this function is from gtk input options
    guint64 intYear = g_ascii_strtoull(year, NULL, 10);

    if(strlen(year) != 0) {
        if (intYear < 1700) {
            return 104; // The entered year is far too far away!
        } else if (intYear > 2006) {
            return 105; // You do not have a minimum age to open an account!
        }
    }

    if(strlen(month) != 0) {
        if (intMonth < 1 || intMonth > 12) {
            return 106; // The month does not exist!
        }
    }

    if(strlen(day) != 0) {
        if (intDay < 1 || intDay > 31) {
            return 107; // The day does not exist!
        } else if (intDay == 31 && (intMonth == 2 || intMonth == 4 || intMonth == 6 || intMonth == 9 || intMonth == 11)) {
            return 108; // This month has only 30 days!
        } else if (intDay == 30 && intMonth == 2) {
            return 109; // The month can have a maximum of 29 days!
        } else if (intDay == 29 && intMonth == 2 && intYear % 4 != 0) {
            return 110; // "The year February has a maximum of 28 days!"
        }
    }

    return 1;
}

short validDate(const gchar *day, const gchar *month, const gchar *year){ // available date means it is valid inside the calendar

    if(!stringOnlyWithDigits(day))
        return 121; // The day need to be a number!

    else if(!stringOnlyWithDigits(month))
        return 122; // The month need to be a number!

    else if(!stringOnlyWithDigits(year))
        return 123; // The year need to be a number!

    guint64 intDay = g_ascii_strtoull(day, NULL, 10); // transform a string formatted from digits to an integer
    guint64 intMonth = g_ascii_strtoull(month, NULL, 10); // this function is from gtk input options
    guint64 intYear = g_ascii_strtoull(year, NULL, 10);

    if(intYear < 1700)
        return 124; // The entered year is far too far away!

    else if(intYear > 2006)
        return 125; // You do not have a minimum age to open an account

    if(intMonth < 1 || intMonth > 12)
        return 126; // The month does not exist!

    if(intDay < 1 || intDay > 31)
        return 127; // The day does not exist!

    else if(intDay == 31 && (intMonth == 2 || intMonth == 4 || intMonth == 6 || intMonth == 9 || intMonth == 11) )
        return 128; // The month has only 30 days!

    else if(intDay == 30 && intMonth == 2)
        return 129; // The month can have a maximum of 29 days!

    else if(intDay == 29 && intMonth == 2 && intYear % 4 != 0)
        return 130; // The year February has a maximum of 28 days!

    return 1;
}


int validDateForTransaction(const gchar *day, const gchar *month, const gchar *year, const Account* account) {
    if (!stringOnlyWithDigits(day))
        return 141; // The day needs to be a number!
    else if (!stringOnlyWithDigits(month))
        return 142; // The month needs to be a number!
    else if (!stringOnlyWithDigits(year))
        return 143; // The year needs to be a number!

    guint64 intDay = g_ascii_strtoull(day, NULL, 10);
    guint64 intMonth = g_ascii_strtoull(month, NULL, 10);
    guint64 intYear = g_ascii_strtoull(year, NULL, 10);

    if (intYear < 1700)
        return 144; // There were no banks this year. People kept currency hidden in their homes!
    else if (intYear > 9999)
        return 145; // The year format is invalid. It must have a maximum of 4 digits!

    if (intMonth < 1 || intMonth > 12)
        return 146; // This month does not exist!

    if (intDay < 1 || intDay > 31)
        return 147; // This day does not exist!
    else if (intDay == 31 && (intMonth == 2 || intMonth == 4 || intMonth == 6 || intMonth == 9 || intMonth == 11))
        return 148; // This month has only 30 days!
    else if (intDay == 30 && intMonth == 2)
        return 149; // This month can have a maximum of 29 days!
    else if (intDay == 29 && intMonth == 2 && intYear % 4 != 0)
        return 150; // This year February has a maximum of 28 days!

    Transaction* latestTransaction = getLatestTransaction(account);
    if (latestTransaction != NULL) {
        Date latestDate = getTransactionDate(latestTransaction);
        if (intYear < latestDate.year ||
            (intYear == latestDate.year && intMonth < latestDate.month) ||
            (intYear == latestDate.year && intMonth == latestDate.month && intDay < latestDate.day))
            return 151; // The last transaction was recorded in the future. You can't introduce now something that has already happened.
    }

    return 1;
}

////////////////////
//
//  Services functions
//
////////////////////


Account* loginService(RepositoryFormat* repository, const char* username, const char* password) {
    if (repository == NULL || username == NULL || password == NULL) {
        return NULL; // Invalid input
    }

    return loginRepository(repository, username, password);
}


// getTransactions
// getAccountStatement

// createAccount
// editAccount
// deleteAccount
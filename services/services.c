#include "services.h"
#include <gtk/gtk.h>
#include <string.h>
#include "../domain/domain.h"
#include "../repository/repository.h"

////////////////////
//
//  Check functions
//
////////////////////

int accountTagUsed(const RepositoryFormat* receivedRepository, const gchar *checkedTag){
    const char *charString = checkedTag;
    return accountTagUsedRepo(receivedRepository, charString);
}

int stringOnlyWithLetters(const gchar *checked_string){
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

    if(!stringHaveOnlyDigits(day))
        return 121; // The day need to be a number!

    else if(!stringHaveOnlyDigits(month))
        return 122; // The month need to be a number!

    else if(!stringHaveOnlyDigits(year))
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
    if (!stringHaveOnlyDigits(day))
        return 141; // The day needs to be a number!
    else if (!stringHaveOnlyDigits(month))
        return 142; // The month needs to be a number!
    else if (!stringHaveOnlyDigits(year))
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



int addTransactionForUser(Account* account, Transaction* newTransaction) {
    if (account->transactionsNumber >= account->transactionsCapacity) {
        int newCapacity = account->transactionsCapacity * 2 + 1;
        Transaction** newTransactions = realloc(account->transactions, newCapacity * sizeof(Transaction*));
        if (newTransactions == NULL) {
            return -1;
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


// createNewTransaction (deposit money in account, withdraw money from account, pay, transfer, cancel)
// getTransactions
// getAccountStatement

// createAccount
// editAccount
// deleteAccount

// newAffiliate
// getAffiliates
// deleteAffiliate

// newUserAccount
// listUserAccounts
// deleteUserAccount
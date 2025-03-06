#include "services.h"
#include <gtk/gtk.h>
#include <string.h>
#include "../domain/domain.h"
#include "../repository/repository.h"

////////////////////
//
//  Manage data functions
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


// Manage the inputs from new trancation interface and work with them inside the memory to add money in user account
// GtkApplication *app - isn't used inside the function but is required from previous gtk function call
// gpointer data - provides the location inside the memory for inptus
void add_to_balance(GtkWidget *widget, gpointer data){

    GtkWidget **entries = (GtkWidget **)data;
    const gchar *amount = gtk_entry_get_text(GTK_ENTRY(entries[0]));
    const gchar *description = gtk_entry_get_text(GTK_ENTRY(entries[1]));
    const gchar *day = gtk_entry_get_text(GTK_ENTRY(entries[2]));
    const gchar *month = gtk_entry_get_text(GTK_ENTRY(entries[3]));
    const gchar *year = gtk_entry_get_text(GTK_ENTRY(entries[4]));

    short error_encountered = 0;
    gdouble money_amount;

    if(strlen(amount) == 0){
        error_encountered = 1;
        show_error("You need to add an amount!");}

    if(string_have_only_digits_extended(amount))
        money_amount = g_ascii_strtod(amount, NULL); // this function transorm an string of digits into a double number
    else{
        error_encountered = 1;
        show_error("The amount isn't a number!");}

    if(strlen(description) > 99){
        error_encountered = 1;
        show_error("The description is too long! You can use maximum 100 characters.");}

    if(!check_date_for_transaction(day, month, year) && error_encountered == 0){

        guint64 int_day = g_ascii_strtoull(day, NULL, 10);
        guint64 int_month = g_ascii_strtoull(month, NULL, 10);
        guint64 int_year = g_ascii_strtoull(year, NULL, 10);

        account_database[current_account].transaction_index ++;
        int last_transaction_index = account_database[current_account].transaction_index;

        account_database[current_account].account_balace += money_amount;
        account_database[current_account].transaction[last_transaction_index].date.day = int_day;
        account_database[current_account].transaction[last_transaction_index].date.month = int_month;
        account_database[current_account].transaction[last_transaction_index].date.year = int_year;
        account_database[current_account].transaction[last_transaction_index].amount = money_amount;
        strcpy(account_database[current_account].transaction[last_transaction_index].description, description);
        strcpy(account_database[current_account].transaction[last_transaction_index].category, "deposit");

        gtk_widget_destroy(last_window);
        show_account_interface();
    }
}

// Manage the inputs from new trancation interface and work with them inside the memory to remove money from user account
// GtkApplication *app - isn't used inside the function but is required from previous gtk function call
// gpointer data - provides the location inside the memory for inptus
void withdraw_from_balance(GtkWidget *widget, gpointer data){

    GtkWidget **entries = (GtkWidget **)data;
    const gchar *amount = gtk_entry_get_text(GTK_ENTRY(entries[0]));
    const gchar *description = gtk_entry_get_text(GTK_ENTRY(entries[1]));
    const gchar *day = gtk_entry_get_text(GTK_ENTRY(entries[2]));
    const gchar *month = gtk_entry_get_text(GTK_ENTRY(entries[3]));
    const gchar *year = gtk_entry_get_text(GTK_ENTRY(entries[4]));

    short error_encountered = 0;
    gdouble money_amount;

    if(strlen(amount) == 0){
        error_encountered = 1;
        show_error("You need to add an amount!");}

    if(string_have_only_digits_extended(amount)) {
        money_amount = g_ascii_strtod(amount, NULL); // this function transorm an string of digits into a double number
        if (account_database[current_account].account_balace < money_amount){
            error_encountered = 1;
            show_error("You have a lower balance than inputed number!");}}
    else{
        error_encountered = 1;
        show_error("The amount isn't a number!");}

    if(strlen(description) > 99){
        error_encountered = 1;
        show_error("The description is too long! You can use maximum 100 characters.");}

    if(!check_date_for_transaction(day, month, year) && error_encountered == 0){

        guint64 int_day = g_ascii_strtoull(day, NULL, 10);
        guint64 int_month = g_ascii_strtoull(month, NULL, 10);
        guint64 int_year = g_ascii_strtoull(year, NULL, 10);

        account_database[current_account].transaction_index ++;
        int last_transaction_index = account_database[current_account].transaction_index;

        account_database[current_account].account_balace -= money_amount;
        account_database[current_account].transaction[last_transaction_index].date.day = int_day;
        account_database[current_account].transaction[last_transaction_index].date.month = int_month;
        account_database[current_account].transaction[last_transaction_index].date.year = int_year;
        account_database[current_account].transaction[last_transaction_index].amount = money_amount;
        strcpy(account_database[current_account].transaction[last_transaction_index].description, description);
        strcpy(account_database[current_account].transaction[last_transaction_index].category, "withdraw");

        gtk_widget_destroy(last_window);
        show_account_interface();

    }
}

// Manage the inputs from new trancation interface and work with them inside the memory to transfer money from user account to another one
// GtkApplication *app - isn't used inside the function but is required from previous gtk function call
// gpointer data - provides the location inside the memory for inptus
void make_a_transaction(GtkWidget *widget, gpointer data){

    GtkWidget **entries = (GtkWidget **)data;
    const gchar *amount = gtk_entry_get_text(GTK_ENTRY(entries[0]));
    const gchar *description = gtk_entry_get_text(GTK_ENTRY(entries[1]));
    const gchar *day = gtk_entry_get_text(GTK_ENTRY(entries[2]));
    const gchar *month = gtk_entry_get_text(GTK_ENTRY(entries[3]));
    const gchar *year = gtk_entry_get_text(GTK_ENTRY(entries[4]));

    short error_encountered = 0;
    gdouble money_amount;

    if(strlen(amount) == 0){
        error_encountered = 1;
        show_error("You need to add an amount!");}

    if(string_have_only_digits_extended(amount)) {
        money_amount = g_ascii_strtod(amount, NULL); // this function transorm an string of digits into a double number
        if (account_database[current_account].account_balace < money_amount){
            error_encountered = 1;
            show_error("You have a lower balance than inputed number!");}}
    else{
        error_encountered = 1;
        show_error("The amount isn't a number!");}

    if(strlen(description) > 99){
        error_encountered = 1;
        show_error("The description is too long! You can use maximum 100 characters.");}

    if(!check_date_for_transaction(day, month, year) && error_encountered == 0){

        guint64 int_day = g_ascii_strtoull(day, NULL, 10);
        guint64 int_month = g_ascii_strtoull(month, NULL, 10);
        guint64 int_year = g_ascii_strtoull(year, NULL, 10);

        account_database[current_account].transaction_index ++;
        int last_transaction_index = account_database[current_account].transaction_index;

        account_database[current_account].account_balace -= money_amount;
        account_database[current_account].transaction[last_transaction_index].date.day = int_day;
        account_database[current_account].transaction[last_transaction_index].date.month = int_month;
        account_database[current_account].transaction[last_transaction_index].date.year = int_year;
        account_database[current_account].transaction[last_transaction_index].amount = money_amount;
        strcpy(account_database[current_account].transaction[last_transaction_index].description, description);
        strcpy(account_database[current_account].transaction[last_transaction_index].category, "transaction");

        gtk_widget_destroy(last_window);
        show_account_interface();

    }
}

// Manage the inputs from new trancation interface and work with them inside the memory to remove money from user account
// GtkApplication *app - isn't used inside the function but is required from previous gtk function call
// gpointer data - provides the location inside the memory for inptus
void make_a_payment(GtkWidget *widget, gpointer data){

    GtkWidget **entries = (GtkWidget **)data;
    const gchar *amount = gtk_entry_get_text(GTK_ENTRY(entries[0]));
    const gchar *description = gtk_entry_get_text(GTK_ENTRY(entries[1]));
    const gchar *day = gtk_entry_get_text(GTK_ENTRY(entries[2]));
    const gchar *month = gtk_entry_get_text(GTK_ENTRY(entries[3]));
    const gchar *year = gtk_entry_get_text(GTK_ENTRY(entries[4]));

    short error_encountered = 0;
    gdouble money_amount;

    if(strlen(amount) == 0){
        error_encountered = 1;
        show_error("You need to add an amount!");}

    if(string_have_only_digits_extended(amount)) {
        money_amount = g_ascii_strtod(amount, NULL);
        if (account_database[current_account].account_balace < money_amount){
            error_encountered = 1;
            show_error("You have a lower balance than inputed number!");}}
    else{
        error_encountered = 1;
        show_error("The amount isn't a number!");
    }

    if(strlen(description) > 99)
        show_error("The description is too long! You can use maximum 100 characters.");

    if(!check_date_for_transaction(day, month, year) && error_encountered == 0){

        guint64 int_day = g_ascii_strtoull(day, NULL, 10);
        guint64 int_month = g_ascii_strtoull(month, NULL, 10);
        guint64 int_year = g_ascii_strtoull(year, NULL, 10);

        account_database[current_account].transaction_index ++;
        int last_transaction_index = account_database[current_account].transaction_index;

        account_database[current_account].account_balace -= money_amount;
        account_database[current_account].transaction[last_transaction_index].date.day = int_day;
        account_database[current_account].transaction[last_transaction_index].date.month = int_month;
        account_database[current_account].transaction[last_transaction_index].date.year = int_year;
        account_database[current_account].transaction[last_transaction_index].amount = money_amount;
        strcpy(account_database[current_account].transaction[last_transaction_index].description, description);
        strcpy(account_database[current_account].transaction[last_transaction_index].category, "payment");

        gtk_widget_destroy(last_window);
        show_account_interface();

    }
}

void logout_from_an_account(){
    current_account = -1;
}

void delete_an_account(){
    for(int index = current_account; index < accounts_number; index++)
        account_database[index] = account_database[index + 1];
    current_account = -1;
    accounts_number--;
}

// Manage the inputs from edit interface
// GtkApplication *app - isn't used inside the function but is required from previous gtk function call
// gpointer data - provides the location inside the memory for inptus
void edit_an_account(GtkApplication *app, gpointer data){

    GtkWidget **entries = (GtkWidget **)data;
    const gchar *current_password = gtk_entry_get_text(GTK_ENTRY(entries[0]));
    const gchar *account_password = gtk_entry_get_text(GTK_ENTRY(entries[1]));
    const gchar *account_password2 = gtk_entry_get_text(GTK_ENTRY(entries[2]));
    const gchar *account_type = gtk_entry_get_text(GTK_ENTRY(entries[3]));
    const gchar *user_phone_number = gtk_entry_get_text(GTK_ENTRY(entries[4]));
    const gchar *user_first_name = gtk_entry_get_text(GTK_ENTRY(entries[5]));
    const gchar *user_second_name = gtk_entry_get_text(GTK_ENTRY(entries[6]));
    const gchar *user_birthday_day = gtk_entry_get_text(GTK_ENTRY(entries[7]));
    const gchar *user_birthday_month = gtk_entry_get_text(GTK_ENTRY(entries[8]));
    const gchar *user_birthday_year = gtk_entry_get_text(GTK_ENTRY(entries[9]));

    short error_encountered = 0;
    if(password_differ(current_password, account_database[current_account].password)){
        error_encountered = 1;
        show_error("Wrong password!");
    }
    else if(strlen(account_password) > 32){
        error_encountered = 1;
        show_error("Inputed password is too long or dosen't exist! Maximum 32 characters.");}
    else if(strlen(account_password2) > 32){
        show_error("Inputed confirm password is too long  or dosen't exist! Maximum 32 characters.");}
    else if(strlen(account_type) > 16){
        error_encountered = 1;
        show_error("Inputed account type is too long or dosen't exist!\nAvailable types: savings, checking, credit.");}
    else if(strlen(user_phone_number) > 11){
        error_encountered = 1;
        show_error("Inputed phone number type is too long or dosen't exist! Maximum 11 characters.");}
    else if(strlen(user_first_name) > 16){
        error_encountered = 1;
        show_error("Inputed first name type is too long or dosen't exist! Maximum 16 characters.");}
    else if(strlen(user_second_name) > 16){
        error_encountered = 1;
        show_error("Inputed second name type is too long or dosen't exist! Maximum 16 characters.");}
    else if(password_differ(account_password, account_password2)){
        error_encountered = 1;
        show_error("The passwords do not match!");}
    else if(!available_account_type(account_type) && strlen(account_type) != 0){
        error_encountered = 1;
        show_error("Invalid account type!\nAvailable types: savings, checking, credit.");}
    else if(!string_have_only_letters(user_first_name)){
        error_encountered = 1;
        show_error("First name can have only letters!");}
    else if(!string_have_only_letters(user_second_name)){
        error_encountered = 1;
        show_error("Second name can have only letters!");}
    else if(!stringHaveOnlyDigits(user_phone_number)){
        error_encountered = 1;
        show_error("Phone number can have only digits!");}
    else
        error_encountered = check_available_date_with_bypass(user_birthday_day, user_birthday_month, user_birthday_year);

    if(!error_encountered){
        if(strlen(account_password) != 0)
            g_strlcpy(account_database[current_account].password, account_password, sizeof(account_database[current_account].password));
        if(strlen(account_type) != 0)
            g_strlcpy(account_database[current_account].type, account_type, sizeof(account_database[current_account].type));
        if(strlen(user_phone_number) != 0)
            g_strlcpy(account_database[current_account].phone_number, user_phone_number, sizeof(account_database[current_account].phone_number));
        if(strlen(user_first_name) != 0)
            g_strlcpy(account_database[current_account].first_name, user_first_name, sizeof(account_database[current_account].first_name));
        if(strlen(user_second_name) != 0 )
            g_strlcpy(account_database[current_account].second_name, user_second_name, sizeof(account_database[current_account].second_name));

        guint64 int_day = g_ascii_strtoull(user_birthday_day, NULL, 10);
        guint64 int_month = g_ascii_strtoull(user_birthday_month, NULL, 10);
        guint64 int_year = g_ascii_strtoull(user_birthday_year, NULL, 10);

        if(strlen(user_birthday_day) != 0 )
            account_database[current_account].birthday.day = int_day;
        if(strlen(user_birthday_month) != 0 )
            account_database[current_account].birthday.month = int_month;
        if(strlen(user_birthday_year) != 0 )
            account_database[current_account].birthday.year = int_year;

        gtk_widget_destroy(last_window);
        show_account_interface();
    }
}


// Manage the inputs from login interface
// GtkApplication *app - isn't used inside the function but is required from previous gtk function call
// gpointer data - provides the location inside the memory for inptus
void login_to_an_account(GtkApplication *app, gpointer data){

    GtkWidget **entries = (GtkWidget **)data;
    const gchar *account_tag = gtk_entry_get_text(GTK_ENTRY(entries[0]));
    const gchar *account_password = gtk_entry_get_text(GTK_ENTRY(entries[1]));

    short error_encountered = 0;
    if(strlen(account_tag) > 20){
        error_encountered = 1;
        show_error("Account tag input is too long! Maximum 20 characters.");}
    else if(strlen(account_password) > 32){
        error_encountered = 1;
        show_error("Inputed password is too long! Maximum 32 characters.");}
    else if(!string_have_only_letters(account_tag)){
        error_encountered = 1;
        show_error("Account tag can have only letters!");}

    if(!error_encountered){ // Check the usertag in databse only if respect the input conditions
        short account_found = 0;
        for(int index = 0; index < accounts_number; index++){
            if(strcmp(account_tag, account_database[index].tag) == 0){
                account_found = 1;
                if(strcmp(account_password,account_database[index].password) == 0) {
                    current_account = index;
                    gtk_widget_destroy(last_window);
                    show_account_interface();
                }
                else
                    show_error("Wrong password!");
            }
        }
        if(!account_found)
            show_error("Account tag can't be find!");
    }
}

// Manage the inputs from register interface
// GtkApplication *app - isn't used inside the function but is required from previous gtk function call
// gpointer data - provides the location inside the memory for inptus
void create_an_account(GtkApplication *app, gpointer data){

    GtkWidget **entries = (GtkWidget **)data;
    const gchar *account_tag = gtk_entry_get_text(GTK_ENTRY(entries[0]));
    const gchar *account_password = gtk_entry_get_text(GTK_ENTRY(entries[1]));
    const gchar *account_password2 = gtk_entry_get_text(GTK_ENTRY(entries[2]));
    const gchar *account_type = gtk_entry_get_text(GTK_ENTRY(entries[3]));
    const gchar *user_phone_number = gtk_entry_get_text(GTK_ENTRY(entries[4]));
    const gchar *user_first_name = gtk_entry_get_text(GTK_ENTRY(entries[5]));
    const gchar *user_second_name = gtk_entry_get_text(GTK_ENTRY(entries[6]));
    const gchar *user_birthday_day = gtk_entry_get_text(GTK_ENTRY(entries[7]));
    const gchar *user_birthday_month = gtk_entry_get_text(GTK_ENTRY(entries[8]));
    const gchar *user_birthday_year = gtk_entry_get_text(GTK_ENTRY(entries[9]));

    short error_encountered = 0;
    if(accounts_number >= 50){
        show_error("The database can't support more users!");
        error_encountered = 1;}
    else if(strlen(account_tag) > 20 || strlen(account_tag) == 0){
        error_encountered = 1;
        show_error("Account tag input is too long or dosen't exist! Maximum 20 characters.");}
    else if(strlen(account_password) > 32 || strlen(account_password) == 0){
        error_encountered = 1;
        show_error("Inputed password is too long or dosen't exist! Maximum 32 characters.");}
    else if(strlen(account_password2) > 32 || strlen(account_password2) == 0){
        show_error("Inputed confirm password is too long  or dosen't exist! Maximum 32 characters.");}
    else if(strlen(account_type) > 16 || strlen(account_type) == 0){
        error_encountered = 1;
        show_error("Inputed account type is too long or dosen't exist!\nAvailable types: savings, checking, credit.");}
    else if(strlen(user_phone_number) > 11 || strlen(user_phone_number) == 0){
        error_encountered = 1;
        show_error("Inputed phone number type is too long or dosen't exist! Maximum 11 characters.");}
    else if(strlen(user_first_name) > 16 || strlen(user_first_name) == 0){
        error_encountered = 1;
        show_error("Inputed first name type is too long or dosen't exist! Maximum 16 characters.");}
    else if(strlen(user_second_name) > 16 || strlen(user_second_name) == 0 ){
        error_encountered = 1;
        show_error("Inputed second name type is too long or dosen't exist! Maximum 16 characters.");}
    else if(account_tag_is_used(account_tag)){
        error_encountered = 1;
        show_error("This account tag is already in use!");}
    else if(!string_have_only_letters(account_tag)){
        error_encountered = 1;
        show_error("Account tag can have only letters!");}
    else if(password_differ(account_password, account_password2)){
        error_encountered = 1;
        show_error("The passwords do not match!");}
    else if(!available_account_type(account_type)){
        error_encountered = 1;
        show_error("Invalid account type!\nAvailable types: savings, checking, credit.");}
    else if(!string_have_only_letters(user_first_name)){
        error_encountered = 1;
        show_error("First name can have only letters!");}
    else if(!string_have_only_letters(user_second_name)){
        error_encountered = 1;
        show_error("Second name can have only letters!");}
    else if(!stringHaveOnlyDigits(user_phone_number)){
        error_encountered = 1;
        show_error("Phone number can have only digits!");}
    else
        error_encountered = check_available_date(user_birthday_day, user_birthday_month, user_birthday_year);

    if(!error_encountered){
        g_strlcpy(account_database[accounts_number].tag, account_tag, sizeof(account_database[accounts_number].tag));
        g_strlcpy(account_database[accounts_number].password, account_password, sizeof(account_database[accounts_number].password));
        g_strlcpy(account_database[accounts_number].type, account_type, sizeof(account_database[accounts_number].type));
        g_strlcpy(account_database[accounts_number].phone_number, user_phone_number, sizeof(account_database[accounts_number].phone_number));
        g_strlcpy(account_database[accounts_number].first_name, user_first_name, sizeof(account_database[accounts_number].first_name));
        g_strlcpy(account_database[accounts_number].second_name, user_second_name, sizeof(account_database[accounts_number].second_name));

        guint64 int_day = g_ascii_strtoull(user_birthday_day, NULL, 10);
        guint64 int_month = g_ascii_strtoull(user_birthday_month, NULL, 10);
        guint64 int_year = g_ascii_strtoull(user_birthday_year, NULL, 10);

        account_database[accounts_number].birthday.day = int_day;
        account_database[accounts_number].birthday.month = int_month;
        account_database[accounts_number].birthday.year = int_year;

        current_account = accounts_number;
        accounts_number++;
        gtk_widget_destroy(last_window);
        show_account_interface();
    }
}
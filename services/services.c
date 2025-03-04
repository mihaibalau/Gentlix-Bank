#include "services.h"
#include <gtk/gtk.h>
#include "../domain/domain.h"
#include "../repository/repository.h"

////////////////////
//
//  Check functions
//
////////////////////

int accountTagUsed(const gchar *checked_tag){
    for(int index = 0; index < accounts_number; index++)
        if(strcmp(checked_tag, account_database[index].tag))
            return 1;
    return 0;
}

int stringOnlyWithLetters(const gchar *checked_string){
    char normal_string[20];
    g_strlcpy(normal_string, checked_string, sizeof(normal_string)); // copy the char allocated dynamic in normal one to be checked
    for(int index = 0; index < strlen(normal_string); index++)
        if(normal_string[index] < 'A' || normal_string[index] > 'z')
            return 0;
    return 1;
}

int stringOnlyWithDigitsExtended(const gchar *checked_string){ // extended means can have also . and , for float numbers
    char normal_string[11];
    g_strlcpy(normal_string, checked_string, sizeof(normal_string)); // copy the char allocated dynamic in normal one to be checked
    for(int index = 0; index < strlen(normal_string); index++)
        if((normal_string[index] < '0' || normal_string[index] > '9') && normal_string[index] != ',' && normal_string[index] != '.')
            return 0;
    return 1;
}

int stringOnlyWithDigits(const gchar *checked_string){
    char normal_string[11];
    g_strlcpy(normal_string, checked_string, sizeof(normal_string)); // copy the char allocated dynamic in normal one to be checked
    for(int index = 0; index < strlen(normal_string); index++)
        if(normal_string[index] < '0' || normal_string[index] > '9')
            return 0;
    return 1;
}

int differentPassword(const gchar *password1, const gchar *password2){
    if(strcmp(password1, password2))
        return 1;
    return 0;
}

short validDateWithBypass(const gchar *day, const gchar *month, const gchar *year){ // bypass means you can leave the fields blank

    if(!string_have_only_digits(day)){
        show_error("The day need to be a number!");
        return 1;
    }
    else if(!string_have_only_digits(month)){
        show_error("The month need to be a number!");
        return 1;
    }
    else if(!string_have_only_digits(year)){
        show_error("The year need to be a number!");
        return 1;
    }

    guint64 int_day = g_ascii_strtoull(day, NULL, 10); // transform a string formatted from digits to an integer
    guint64 int_month = g_ascii_strtoull(month, NULL, 10); // this function is from gtk input options
    guint64 int_year = g_ascii_strtoull(year, NULL, 10);

    if(strlen(year) != 0) {
        if (int_year < 1700) {
            show_error("The entered year is far too far away!");
            return 1;
        } else if (int_year > 2005) {
            show_error("You do not have a minimum age to open an account!");
            return 1;
        }
    }

    if(strlen(month) != 0) {
        if (int_month < 1 || int_month > 12) {
            show_error("The month does not exist!");
            return 1;
        }
    }

    if(strlen(day) != 0) {
        if (int_day < 1 || int_day > 31) {
            show_error("The day does not exist!");
            return 1;
        } else if (int_day == 31 &&
                   (int_month == 2 || int_month == 4 || int_month == 6 || int_month == 9 || int_month == 11)) {
            show_error("The month has only 30 days!");
            return 1;
        } else if (int_day == 30 && int_month == 2) {
            show_error("The month can have a maximum of 29 days!");
            return 1;
        } else if (int_day == 29 && int_month == 2 && int_year % 4 != 0) {
            show_error("The year February has a maximum of 28 days!");
            return 1;
        }
    }
    return 0;
}

short validDate(const gchar *day, const gchar *month, const gchar *year){ // available date means it is valid inside the calendar

    if(!string_have_only_digits(day)){
        show_error("The day need to be a number!");
        return 1;
    }
    else if(!string_have_only_digits(month)){
        show_error("The month need to be a number!");
        return 1;
    }
    else if(!string_have_only_digits(year)){
        show_error("The year need to be a number!");
        return 1;
    }

    guint64 int_day = g_ascii_strtoull(day, NULL, 10); // transform a string formatted from digits to an integer
    guint64 int_month = g_ascii_strtoull(month, NULL, 10); // this function is from gtk input options
    guint64 int_year = g_ascii_strtoull(year, NULL, 10);

    if(int_year < 1700){
        show_error("The entered year is far too far away!");
        return 1;
    }
    else if(int_year > 2005){
        show_error("You do not have a minimum age to open an account!");
        return 1;
    }

    if(int_month < 1 || int_month > 12){
        show_error("The month does not exist!");
        return 1;
    }

    if(int_day < 1 || int_day > 31){
        show_error("The day does not exist!");
        return 1;
    }
    else if(int_day == 31 && (int_month == 2 || int_month == 4 || int_month == 6 || int_month == 9 || int_month == 11) ){
        show_error("The month has only 30 days!");
        return 1;
    }
    else if(int_day == 30 && int_month == 2){
        show_error("The month can have a maximum of 29 days!");
        return 1;
    }
    else if(int_day == 29 && int_month == 2 && int_year % 4 != 0){
        show_error("The year February has a maximum of 28 days!");
        return 1;
    }
    return 0;
}

int validDateForTransaction(const gchar *day, const gchar *month, const gchar *year){
    if(!string_have_only_digits(day)){
        show_error("The day need to be a number!");
        return 1;}
    else if(!string_have_only_digits(month)){
        show_error("The month need to be a number!");
        return 1;}
    else if(!string_have_only_digits(year)){
        show_error("The year need to be a number!");
        return 1;}

    guint64 int_day = g_ascii_strtoull(day, NULL, 10);
    guint64 int_month = g_ascii_strtoull(month, NULL, 10);
    guint64 int_year = g_ascii_strtoull(year, NULL, 10);
    int flag = 0; // This will be used to know if we are in the same year/month as the previous transaction.
    int last_transaction_index = account_database[current_account].transaction_index;

    if(int_year < 1700){
        show_error("There were no banks this year. People kept currency hidden in their homes!");
        return 1;}
    else if(int_year > 9999){
        show_error("The year format is invalid. It must have a maximum of 4 digits!");
        return 1;}
    else if(last_transaction_index > 0 && int_year < account_database[current_account].transaction[last_transaction_index].date.year){ // Initially we check if there is a previous transaction
        show_error("The last transaction was recorded in the future. You can't introduce now something that has already happened.");
        return 1;}
    else if(last_transaction_index > 0 && int_year ==account_database[current_account].transaction[last_transaction_index].date.year) // If we are in the same year we set the flag to 1
        flag = 1;

    if(int_month < 1 || int_month > 12){
        show_error("This month does not exist!");
        return 1;}
    else if(flag == 1 && int_month < account_database[current_account].transaction[last_transaction_index].date.month) {// The flag is 1 so we automatically have a previous transaction
        show_error("The last transaction was recorded in the future. You can't introduce now something that has already happened.");
        return 1;}
    else if(flag == 1 && int_month > account_database[current_account].transaction[last_transaction_index].date.month) // Check if we are in the same month as the previous transaction
        flag = 0;

    if(int_day < 1 || int_day > 31){
        show_error("This day does not exist!");
        return 1;}
    else if(int_day == 31 && (int_month == 2 || int_month == 4 || int_month == 6 || int_month == 9 || int_month == 11) ){
        show_error("This month has only 30 days!");
        return 1;}
    else if(int_day == 30 && int_month == 2){
        show_error("This month can have a maximum of 29 days!");
        return 1;}
    else if(int_day == 29 && int_month == 2 && int_year % 4 != 0){
        show_error("This year February has a maximum of 28 days!");
        return 1;}
    else if(flag == 1 && int_day < account_database[current_account].transaction[last_transaction_index].date.day){ // If the months and years are the same, we cannot take a day smaller than the previous one.
        show_error("The last transaction was recorded in the future. You can't introduce now something that has already happened.");
        return 1;}
    return 0;
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
    else if(!string_have_only_digits(user_phone_number)){
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
    else if(!string_have_only_digits(user_phone_number)){
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
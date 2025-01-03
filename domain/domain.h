#ifndef GENTLIX_BANK_DOMAIN_H
#define GENTLIX_BANK_DOMAIN_H

typedef struct {
    short day, month, year;
} Date;

Date createDate(short day, short month, short year);
short getDay(Date* received_date);
short getMonth(Date* received_date);
short getYear(Date* received_date);
void setDay(Date* received_date, short received_day);
void setMonth(Date* received_date, short received_month);
void setYear(Date* received_date, short received_year);


typedef struct {
    float amount;
    char* type;
    char* receiver_iban;
    char* category;
    char* description;
    Date date;
} Transaction;

typedef struct {
    char* tag;
    char* first_name;
    char* second_name;
    char* iban;
    char* activity_domain;
    char* phone;
} Affiliate;

typedef struct{
    float main_account_balance;
    char* tag;
    char* first_name;
    char* second_name;
    char* password;
    char* iban;
    char* phone_number;
    Date birthday;
    Affiliate* affiliates;
    Transaction* transactions;
    int transactions_number;
    int affiliates_number;
} Account;


#endif

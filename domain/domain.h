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
    char* userAccount;
    char* type;
    char* receiverIBAN;
    char* category;
    char* description;
    Date date;
} Transaction;

Transaction* createTransaction(float amount, const char* userAccount, const char* type, const char* receiverIBAN,
                               const char* category, const char* description, Date date);
void destroyTransaction(Transaction* transaction);
float getTransactionAmount(const Transaction* transaction);
const char* getTransactionUserAccount(const Transaction* transaction);
const char* getTransactionType(const Transaction* transaction);
const char* getTransactionReceiverIban(const Transaction* transaction);
const char* getTransactionCategory(const Transaction* transaction);
const char* getTransactionDescription(const Transaction* transaction);
Date getTransactionDate(const Transaction* transaction);
void setTransactionAmount(Transaction* transaction, float amount);
void setTransactionUserAccount(Transaction* transaction, const char* userAccount);
void setTransactionType(Transaction* transaction, const char* type);
void setTransactionReceiverIban(Transaction* transaction, const char* receiverIBAN);
void setTransactionCategory(Transaction* transaction, const char* category);
void setTransactionDescription(Transaction* transaction, const char* description);
void setTransactionDate(Transaction* transaction, Date date);



typedef struct {
    char* tag;
    char* firstName;
    char* secondName;
    char* iban;
    char* activityDomain;
    char* phone;
} Affiliate;

Affiliate* createAffiliates(const char* tag, const char* firstName, const char* secondName,
                             const char* iban, const char* activityDomain, const char* phone);
void destroyAffiliates(Affiliate* affiliates);
const char* getAffiliatesTag(const Affiliate* affiliates);
const char* getAffiliatesFirstName(const Affiliate* affiliates);
const char* getAffiliatesSecondName(const Affiliate* affiliates);
const char* getAffiliatesIban(const Affiliate* affiliates);
const char* getAffiliatesActivityDomain(const Affiliate* affiliates);
const char* getAffiliatesPhone(const Affiliate* affiliates);
void setAffiliatesTag(Affiliate* affiliates, const char* tag);
void setAffiliatesFirstName(Affiliate* affiliates, const char* firstName);
void setAffiliatesSecondName(Affiliate* affiliates, const char* secondName);
void setAffiliatesIban(Affiliate* affiliates, const char* iban);
void setAffiliatesActivityDomain(Affiliate* affiliates, const char* activityDomain);
void setAffiliatesPhone(Affiliate* affiliates, const char* phone);



typedef struct {
    float accountBalance;
    char* type;
} UserAccounts;

UserAccounts* createUserAccount(float balance, const char* type);
void destroyUserAccount(UserAccounts* account);
float getUserAccountBalance(const UserAccounts* account);
const char* getUserAccountType(const UserAccounts* account);
void setUserAccountBalance(UserAccounts* account, float balance);
void setUserAccountType(UserAccounts* account, const char* type);



typedef struct{
    float mainAccountBalance;
    char* tag;
    char* firstName;
    char* secondName;
    char* password;
    char* iban;
    char* phoneNumber;
    Date birthday;
    Affiliate** affiliates;
    Transaction** transactions;
    UserAccounts** userAccounts;
    int transactionsNumber;
    int affiliatesNumber;
    int userAccountsNumber;
    int transactionsCapacity;
    int affiliatesCapacity;
    int userAccountsCapacity;
} Account;

Account* createAccount(float mainAccountBalance, const char* tag, const char* firstName,
                       const char* secondName, const char* password, const char* iban,
                       const char* phoneNumber, Date birthday);
void destroyAccount(Account* account);
float getAccountBalance(const Account* account);
const char* getAccountTag(const Account* account);
const char* getAccountFirstName(const Account* account);
const char* getAccountSecondName(const Account* account);
const char* getAccountPassword(const Account* account);
const char* getAccountIban(const Account* account);
const char* getAccountPhoneNumber(const Account* account);
Date getAccountBirthday(const Account* account);
int getAccountTransactionsNumber(const Account* account);
int getAccountAffiliatesNumber(const Account* account);
int getAccountUserAccountsNumber(const Account* account);
int getAccountTransactionsCapacity(const Account* account);
int getAccountAffiliatesCapacity(const Account* account);
int getAccountUserAccountsCapacity(const Account* account);

void setAccountBalance(Account* account, float balance);
void setAccountTag(Account* account, const char* tag);
void setAccountFirstName(Account* account, const char* firstName);
void setAccountSecondName(Account* account, const char* second_name);
void setAccountPassword(Account* account, const char* password);
void setAccountIban(Account* account, const char* iban);
void setAccountPhoneNumber(Account* account, const char* phone_number);
void setAccountBirthday(Account* account, Date birthday);

#endif
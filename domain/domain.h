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


#endif

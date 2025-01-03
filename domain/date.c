#include "domain.h"

Date createDate(short day, short month, short year){
    Date new_date;

    new_date.day = day;
    new_date.month = month;
    new_date.year = year;

    return new_date;
}

short getDay(Date* received_date){
    return received_date->day;
}

short getMonth(Date* received_date){
    return received_date->month;
}

short getYear(Date* received_date){
    return received_date->year;
}

void setDay(Date* received_date, short received_day){
    received_date->day = received_day;
}

void setMonth(Date* received_date, short received_month){
    received_date->month = received_month;
}

void setYear(Date* received_date, short received_year){
    received_date->year = received_year;
}

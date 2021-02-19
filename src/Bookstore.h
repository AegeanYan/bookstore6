//
// Created by 罗皓天 on 2021/2/4.
//

#ifndef BOOKSTORE_2020_BOOKSTORE_H
#define BOOKSTORE_2020_BOOKSTORE_H
#include <iostream>
#include <string>
#include "Users.h"
using namespace std;

class Book {
public:
    double price = 0;
    char ISBN[61] {};
    char name[61] {};
    char author[61] {};
    char keywords[61] {};
    int quantity = 0;
    Book();
    Book(const double &Pric , const char * ISBN_ , const char * Name , const char *Author , const char * keywd , const int &Quantity);
    bool operator < (const Book &ob)const;
    void show();
};
bool check_BooknameorAuthor(const char* ob);

bool check_keyword(const char* ob);

bool check_isbn(const char* ob);

bool check_quantity(const string &ob);

void initialize();

class Transaction{
public:
    char user_id[61] {};
    char ISBN[61] {};
    int quantity = 0;
    double total_price = 0;
    Transaction();
    Transaction(const char* ISBN , const char* user_id , const int &quantity , const double & price);
};

class totle_money{
public:
    double income = 0;
    double expense = 0;
    totle_money();
};

class report_employee{
public:
    char name[61] {};
    char action[201] {};
    double money = 0;
    report_employee();
    report_employee(const char *name_ , const string &action_ , const double &money_);
};

class report_finance{
public:
    char action[201] {};
    double money = 0;
    report_finance();
    report_finance(const string& action_ , const double &money_);
};

class LOG{
public:
    char name[61] {};
    char action[201] {};
    double money = 0;
    LOG();
    LOG(const char* name_ , const string& action_ , const double &money_);
};
#endif //BOOKSTORE_2020_BOOKSTORE_H





























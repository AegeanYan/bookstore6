//
// Created by 罗皓天 on 2021/2/4.
//

#include "Bookstore.h"
#include <cstring>
#include <fstream>
#include <iomanip>

extern Linked_list userid_list;
Book::Book(const double &Pric, const char *ISBN_, const char *Name, const char *Author, const char *keywd,
           const int &Quantity) {
    price = Pric;
    strcpy(ISBN , ISBN_);
    strcpy(name , Name);
    strcpy(author , Author);
    strcpy(keywords , keywd);
    quantity = Quantity;
}

Book::Book() {
    price = 0;
    ISBN[60] = '\0';
    name[60] = '\0';
    author[60] = '\0';
    keywords[60] = '\0';
    quantity = 0;
}
bool check_BooknameorAuthor(const char* ob){
    if (strlen(ob) > 60 || strlen(ob) == 0)return false;
    for (int i = 0 ; i < strlen(ob) ; ++i){
        if (ob[i] == '\"')return false;
    }
    return true;
}

bool check_keyword(const char* ob){
    if (strlen(ob) > 60 || strlen(ob) == 0)return false;
    for (int i = 0 ; i < strlen(ob) ; ++i){
        if (ob[i] == '\"')return false;
    }
    return true;
}

bool check_isbn(const char* ob){
    if (strlen(ob) > 20 || strlen(ob) == 0)return false;
    else return true;
}

bool check_quantity(const string &ob){
    for(int i = 0; i < ob.length() ; ++i){
        if (ob[i] == '.')return false;
        if (ob[i] > '9' || ob[i] < '0')return false;
    }
    return true;
}

bool Book::operator<(const Book &ob)const     {
    int flag = strcmp(this->ISBN , ob.ISBN);
    if (flag < 0)return true;
    else return false;
}
bool comp(const Book &a , const Book &b){
    return a.ISBN < b.ISBN;
}

void Book::show() {
    cout << ISBN << "\t" << name << "\t" << author << "\t" << keywords
    << "\t" <<  setiosflags(ios::fixed) << setprecision(2) << price << '\t' << quantity << endl;
}

void initialize(){
    fstream io;
    io.open("userid.dat" , ios ::in);//将ios改为fstream就好了？
    if (!io){
     io.close();
     io.open("userid.dat" , ios::out);
     io.close();
     io.open("user.dat" , ios::out);
     io.close();
     io.open("totlemoney.dat" , ios::out);
     io.close();
     io.open("transaction.dat" , ios::out);
     io.close();
     io.open("book.dat" , ios::out);
     io.close();
     io.open("ISBN.dat" , ios::out);
     io.close();
     io.open("author.dat" , ios::out);
     io.close();
     io.open("name.dat" , ios::out);
     io.close();
     io.open("bookkey.dat" , ios::out);
     io.close();
     io.open("reportf.dat" , ios::out);
     io.close();
     io.open("reporte.dat" , ios::out);
     io.close();
     io.open("LOG.dat" , ios::out);
     io.close();
     Users root("root" , "sjtu" , "root" , 7);
     int offset = my_write<Users>("user.dat" , root);
     node tmp(offset , "root");
     userid_list.addnode(tmp);
     totle_money tmp_mon;
     io.open("totlemoney.dat" , ios::in | ios::out | ios::binary);
     io.seekp(0,ios::beg);
     io.write(reinterpret_cast<char*>(&tmp_mon) , sizeof(totle_money));
     io.close();
    } else{
        io.close();
    }
}

Transaction::Transaction(){
    user_id[60] = '\0';
    ISBN[60] = '\0';
    quantity = 0;
    total_price = 0;
}
Transaction::Transaction(const char *ISBN_, const char *user_id_, const int &quantity_, const double &price_) {
    strcpy(ISBN , ISBN_);
    strcpy(user_id , user_id_);
    quantity = quantity_;
    total_price = price_;
}

totle_money::totle_money() {
    income = 0;
    expense = 0;
}

report_employee::report_employee(){
    name[60] = '\0';
    action[200] = '\0';
    money = 0;
}
report_employee::report_employee(const char *name_, const string &action_, const double &money_) {
    strcpy(name , name_);
    strcpy(action , action_.c_str());
    money = money_;
}

report_finance::report_finance() {
    action[200] = '\0';
    money = 0;
}

report_finance::report_finance(const string &action_, const double &money_) {
    strcpy(action , action_.c_str());
    money = money_;
}

LOG::LOG() {
    name[60] = '\0';
    action[200] = '\0';
    money = 0;
}
LOG::LOG(const char *name_, const string &action_, const double &money_) {
    strcpy(name ,name_);
    strcpy(action , action_.c_str());
    money = money_;
}
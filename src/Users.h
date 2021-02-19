//
// Created by 罗皓天 on 2021/2/4.
//

#ifndef BOOKSTORE_2020_USERS_H
#define BOOKSTORE_2020_USERS_H
#include "Block_linked_list.h"
#include <fstream>
using namespace std;
class Users {
public:
    int privilege = 0;
    char user_id[61] = {};
    char passwd[61] = {};
    char name[61] = {};
    int selected = -1;
    Users();
    Users(const Users &ou);
    Users(const char *User_id , const char *Passwd , const char * Name , const int & Privilege);
};

void su(const char * userid , const char* password);

void logout();

void useradd(const char *User_id , const char *Passwd , const int & Privilege , const char *Name);

void my_register(const char * User_id , const char *Passwd , const char *Name);

void my_delete(const char *User_id);

void passwd(const char * User_id , const char *newpas , const char* oldpas = nullptr);

void select_book(const char* ISBN);

void modify(const char *new_ISBN = nullptr, const char *new_name = nullptr,
            const char *new_author = nullptr, const char *new_keyword = nullptr, double new_price = -1);

void import(int quantity , double cost_price);

void splitkey(const string &str , vector<string>&str_vec);

void show();

void show(const char* key ,Linked_list &list);

void showfina(int time = -1);

void buy(const char *ISBN , int quantity);

void clear();

void Run(string &cmd);

template<class T>
int my_write(const char* file , T &ot , int offset = -1){
    fstream io;
    io.open(file , ios::in | ios ::out |ios::binary);
    if(!io)throw "cannot open file in my_write";
    if(offset < 0){
        io.seekp(0 , ios::end);
        offset = io.tellp();
    } else{
        io.seekp(offset);
    }
    io.write(reinterpret_cast<char*>(&ot) , sizeof(T));
    io.close();
    return offset;
}

template<class T>
T my_read(const char* file , int offset){
    fstream io;
    io.open(file , ios::in | ios ::out |ios::binary);
    if (!io)throw "Error in open file in my_read.";
    T tmp;
    io.seekg(offset);
    io.read(reinterpret_cast<char*>(&tmp) , sizeof(T));
    io.close();
    return tmp;
}

bool checkname(const string &a);
#endif //BOOKSTORE_2020_USERS_H

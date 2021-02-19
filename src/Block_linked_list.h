//
// Created by 罗皓天 on 2021/2/4.
//

#ifndef BOOKSTORE_2020_BLOCK_LINKED_LIST_H
#define BOOKSTORE_2020_BLOCK_LINKED_LIST_H
#include <iostream>
#include <cstring>

#include <fstream>
#include <vector>
using namespace std;
class node{
public:
    char key[81]{};
    int offset = -1;
    node();
    explicit node(int offset , const char *ky = nullptr);
    node(const node &othernode);
    node &operator = (const node &othernode);
};
//offset为查询位置
class block{
public:
    int nxt = -1;
    int pre = -1;
    int len = 0;
    node nodearray[200];
    block();
};

class Linked_list{
public:
    char file[500]{};
    explicit Linked_list(const char *otherlist);
    void addnode(const node &nod);
    void findnode(const char* key , vector<int>&posibleoffset);
    void deletenode(const node &nod);
    void splitblock(int blo);
    void mergeblock(int blo1 , int blo2);
    int nxtbloc(int offset);
    Linked_list();
};

#endif //BOOKSTORE_2020_BLOCK_LINKED_LIST_H


// Created by 罗皓天 on 2021/2/4.
//

#include "Block_linked_list.h"
node::node() {
    offset = -1;
    key[80] = '\0';
}

node::node(int ofst, const char *ky) {
    offset = ofst;
    if (ky == nullptr){
        key[80] = '\0';
        return;
    }
    strcpy(key , ky);
}

node::node(const node &othernode) {
    offset = othernode.offset;
    strcpy(key,othernode.key);
}

node & node::operator=(const node &othernode) {
    if (this == &othernode)
        return *this;
    else{
        offset = othernode.offset;
        strcpy(key,othernode.key);
        return *this;
    }
}

block::block(){
    nxt = -1;
    pre = -1;
    len = 0;
}

Linked_list::Linked_list() {
    file[499] = '\0';
}

Linked_list::Linked_list(const char *otherlist) {
    strcpy(file,otherlist);
}

int Linked_list::nxtbloc(int offset) {
    fstream fin;
    fin.open(file,ios::in |ios::binary);
    block tmp;
    fin.seekg(offset);
    fin.read(reinterpret_cast<char*>(&tmp),sizeof(block));
    fin.close();
    return tmp.nxt;
}

void Linked_list::addnode(const node &nod) {
    fstream io;
    io.open(file, ios::in | ios::out | ios::binary);
    if (!io) throw "Error in opening file in add.";
    io.seekg(0, ios::end);
    if (io.tellg() == 0) {
        block tmp;
        io.seekp(0, ios::beg);
        io.write(reinterpret_cast<char *>(&tmp), sizeof(block));
        //此处一定可以类型转换吗？&tmp能不能换为tmp.nodearray?
        //不行，我要将block中的pre、nxt也写进文件。
    }
    int current = 0, next = nxtbloc(current);
    while (next != -1) {
        block tmp;
        io.seekg(next);
        io.read(reinterpret_cast<char *>(&tmp), sizeof(block));
        if (strcmp(nod.key, tmp.nodearray[0].key) < 0)break;
        current = next;
        next = nxtbloc(current);
    }
    io.seekg(current);
    block tmp;
    io.read(reinterpret_cast<char *>(&tmp), sizeof(block));
    int place_ = -1;
    for (int i = 0; i < tmp.len; ++i) {
        if (strcmp(nod.key, tmp.nodearray[i].key) < 0) {
            place_ = i;
            break;
        }
    }
    if (place_ == -1)place_ = tmp.len;
    for (int i = tmp.len; i > place_; i--) {
        tmp.nodearray[i] = tmp.nodearray[i - 1];
    }
    tmp.nodearray[place_] = nod;
    tmp.len++;
    io.seekp(current);
    io.write(reinterpret_cast<char*>(&tmp),sizeof(block));
    io.close();
    //io.close();
    if (tmp.len > 150){
        splitblock(current);
    }
}

void Linked_list::deletenode(const node &nod) {
    fstream fin;
    fin.open(file , ios::in |ios::out | ios::binary);
    if (!fin)throw "Error in opening file in delete.";
    fin.seekg(0,ios::end);
    if (fin.tellg() == 0){
        throw "Error:there is no block to delete";
    }
    int current = 0;
    int next = nxtbloc(current);
    while (next != -1){
        block tmp;
        fin.seekg(next);
        fin.read(reinterpret_cast<char*>(&tmp),sizeof(block));
        if (strcmp(nod.key,tmp.nodearray[0].key) < 0) break;
        current = next;
        next = nxtbloc(current);
    }
    fin.seekg(current);
    block tmp;
    bool flag = false;
    int place_;
    fin.read(reinterpret_cast<char*>(&tmp) , sizeof(block));
    for(int i = 0 ; i < tmp.len ; ++i){
        if(strcmp(nod.key,tmp.nodearray[i].key) == 0 && nod.offset ==tmp.nodearray[i].offset){
            place_ = i;
            flag = true;
            break;
        }
    }
    if (!flag)throw "we didn't find the same nod";
    for(int i = place_ ; i < tmp.len - 1 ; ++i){
        tmp.nodearray[i] = tmp.nodearray[i + 1];
    }
    tmp.len--;
    fin.seekp(current);
    fin.write(reinterpret_cast<char*>(&tmp) , sizeof(block));
    next = tmp.nxt;
    int nxtlen = 0;
    if(next != -1){
        block blo;
        fin.seekg(next);
        fin.read(reinterpret_cast<char*>(&blo) , sizeof(block));
        nxtlen = blo.len;
    }
    fin.close();
    if(tmp.len + nxtlen < 100 && next != -1)mergeblock(current,next);
}

void Linked_list::mergeblock(int blo1, int blo2){//左小右大
    fstream io;
    io.open(file , ios::in | ios ::out |ios::binary);
    if(!io)throw "Error in opening file in merge.";
    block tmp1 , tmp2;
    io.seekg(blo1);
    io.read(reinterpret_cast<char*>(&tmp1) ,sizeof(block));
    io.seekg(blo2);
    io.read(reinterpret_cast<char*>(&tmp2) ,sizeof(block));
    //不用检查是否相邻；
    tmp1.nxt = tmp2.nxt;
    for (int i = 0;i < tmp2.len ; ++i){
        tmp1.nodearray[tmp1.len + i] = tmp2.nodearray[i];
    }
    tmp1.len += tmp2.len;
    if (tmp2.nxt != -1){
        io.seekp(tmp2.nxt + sizeof(int));
        io.write(reinterpret_cast<char*>(&blo1) , sizeof(int));
    }
    io.seekp(blo1);
    io.write(reinterpret_cast<char*>(&tmp1) , sizeof(block));
    io.close();
}

void Linked_list::splitblock(int blo) {
    fstream io;
    io.open(file , ios::in | ios ::out |ios::binary);
    if (!io)throw "Error in open file in split.";
    block Block1 , Block2 , Block_tmp;
    io.seekg(blo);
    io.read(reinterpret_cast<char*>(&Block_tmp) , sizeof(block));
    for(int i = 0 ; i < Block_tmp.len ; i++){
        if(i < 75){
            Block1.nodearray[i] = Block_tmp.nodearray[i];
        } else{
            Block2.nodearray[i - 75] = Block_tmp.nodearray[i];
        }
    }
    Block1.len = 75;
    Block2.len = Block_tmp.len - 75;
    io.seekg(0,ios::end);//新的block放在流尾
    int place_ = io.tellg();
    if (Block_tmp.nxt != -1){
        io.seekp(Block_tmp.nxt + sizeof(int));
        io.write(reinterpret_cast<char*>(&place_) , sizeof(int));
    }
    Block2.nxt = Block_tmp.nxt;
    Block2.pre = blo;
    Block1.nxt = place_;
    Block1.pre = Block_tmp.pre;
    io.seekp(blo);
    io.write(reinterpret_cast<char*>(&Block1),sizeof(block));
    io.seekp(place_);
    io.write(reinterpret_cast<char*>(&Block2),sizeof(block));
    io.close();
}

void Linked_list::findnode(const char* key , vector<int>&posibleoffset) {
    fstream io;
    io.open(file , ios::in | ios ::out |ios::binary);
    if (!io)throw "Error in opening file in findnode.";
    io.seekg(0,ios::end);
    if(io.tellg() == 0){
        block tmp;
        io.seekp(0);
        io.write(reinterpret_cast<char*>(&tmp) , sizeof(block));
        io.close();
        //throw "we have no node";bug:不能在此处直接跳出
    }
    block tmp;
    int current = 0;
    io.seekg(current);
    io.read(reinterpret_cast<char*>(&tmp) , sizeof(block));
    if (strcmp(key , tmp.nodearray[0].key) < 0){
        return;
    }
    int nxt = tmp.nxt;
    int pre = 0;
    while (nxt != -1){
        block blo1;
        io.seekg(nxt);
        io.read(reinterpret_cast<char*>(&blo1) , sizeof(block));
        if (strcmp(key , blo1.nodearray[0].key) < 0) break;
        io.seekg(current);
        io.read(reinterpret_cast<char*>(&blo1) , sizeof(block));
        if (strcmp(key , blo1.nodearray[0].key) > 0){
            pre = current;
        }
        current = nxt;
        nxt = nxtbloc(nxt);
    }
    while (true){
        io.seekg(pre);
        block blo;
        io.read(reinterpret_cast<char*>(&blo) , sizeof(block));
        for (int i = 0; i < blo.len ; ++i){
            if (strcmp(key ,blo.nodearray[i].key) == 0)
                posibleoffset.push_back(blo.nodearray[i].offset);
        }
        if (pre == nxt)break;
        pre = blo.nxt;
    }
    io.close();
}
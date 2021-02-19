#include <iostream>
#include "Bookstore.h"
#include <fstream>
/* io.open("init",ios::binary);
    io.write(reinterpret_cast<char*>(1) , sizeof(int));
    io.close();
    io.open("userid" , ios::out);
    io.close();
    io.open("user" , ios::out);
    io.close();
    io.open("totlemoney" , ios::out);
    io.close();
    io.open("transaction" , ios::out);
    io.close();
    io.open("book" , ios::out);
    io.close();
    io.open("ISBN" , ios::out);
    io.close();
    io.open("author" , ios::out);
    io.close();
    io.open("name" , ios::out);
    io.close();
    io.open("bookkey" , ios::out);
    io.close();
    io.open("reportf" , ios::out);
    io.close();
    io.open("reporte" , ios::out);
    io.close();
    io.open("LOG" , ios::out);
    io.close();
    */

using namespace std;
int main() {
    //freopen("1.in","r",stdin);
    //freopen("myout" , "w" , stdout);
    initialize();
    string cmd;
    while (getline(cin,cmd)){
        Run(cmd);
    }
    return 0;
}
//
// Created by 罗皓天 on 2021/2/4.
//
#include "Users.h"
#include "Bookstore.h"
#include <sstream>
#include <iomanip>
#include <stack>
#include <algorithm>
Linked_list userid_list("userid.dat");
Linked_list author_list("author.dat");
Linked_list isbn_list("ISBN.dat");
Linked_list name_list("name.dat");
Linked_list keyword_list("bookkey.dat");

stack<Users> USER_STATUS;
Users::Users(const char *User_id, const char *Passwd, const char *Name, const int &Privilege) {
    strcpy(user_id,User_id);
    strcpy(passwd,Passwd);
    strcpy(name,Name);
    privilege = Privilege;
}

Users::Users(const Users &ou) {
    privilege = ou.privilege;
    strcpy(user_id , ou.user_id);
    strcpy(passwd , ou.passwd);
    strcpy(name , ou.name);
    selected = ou.selected;
}

Users::Users(){
    user_id[60] = '\0';
    passwd[60] = '\0';
    name[60] = '\0';
    privilege = 0;
    selected = -1;
}

void su(const char *userid , const char *password){
    if (strlen(password) == 0){
        vector<int>possible_offset;
        userid_list.findnode(userid , possible_offset);
        if (possible_offset.empty()){cout << "Invalid\n";return;}
            //throw "cannot find the user_id.";
        Users tmp(my_read<Users>("user.dat" , possible_offset[0]));
        //cout << tmp.privilege;
        if (USER_STATUS.top().privilege > tmp.privilege){
            USER_STATUS.push(tmp);
        }else{
            //throw"Invalid";
            cout << "Invalid\n";return;
        }
    } else{
        vector<int>possible_offset;
        userid_list.findnode(userid , possible_offset);
        //cout << possible_offset[0] << '#';
        if (possible_offset.empty()){
            //cout << -7;
             //throw("cannot find the user_id.");
            cout << "Invalid\n";return;
        }
        Users tmp(my_read<Users>("user.dat" , possible_offset[0]));
        if (strcmp(password , tmp.passwd) == 0 && strcmp(userid , tmp.user_id) == 0){
            USER_STATUS.push(tmp);
            //cout << -2;
        }
        else {
            //cout << -3;
            //throw "Invalid.";
            cout << "Invalid\n";return;
        }
    }
}

void logout(){
    if (USER_STATUS.empty()){cout << "Invalid\n";return;}
        //throw "Invalid : empty in stack.";
    else USER_STATUS.pop();
}

void useradd(const char *User_id , const char *Passwd , const int & Privilege , const char *Name){
    Users tmp_user(User_id,Passwd,Name,Privilege);
    vector<int>possible_offset;
    userid_list.findnode(User_id,possible_offset);
    if(!possible_offset.empty()){cout << "Invalid\n";return;}
        //throw "Invalid";
    int offset = my_write("user.dat" , tmp_user);
    node nod(offset , tmp_user.user_id);
    userid_list.addnode(nod);
    //以下是report
    fstream io;
    io.open("reporte.dat",ios::in | ios::out | ios::binary);
    string u_id = User_id;
    report_employee tmp_re_em(USER_STATUS.top().name ,"useradd : "+ u_id , 0);
    io.seekp(0,ios::end);
    io.write(reinterpret_cast<char*>(&tmp_re_em) , sizeof(report_employee));
    io.close();
    io.open("LOG.dat",ios::in | ios::out | ios::binary);
    io.seekp(0 , ios::end);
    LOG loge(USER_STATUS.top().name , "add new user" , 0);
    io.write(reinterpret_cast<char*>(&loge) , sizeof(LOG));
    io.close();
}

void my_register(const char * User_id , const char *Passwd , const char *Name){
    Users tmp_user(User_id , Passwd , Name , 1);
    vector<int>possible_offset;
    userid_list.findnode(User_id , possible_offset);
    if (!possible_offset.empty()) {cout << "Invalid\n";return;}
        //throw "Invalid : the id had been registered";
    int offset = my_write("user.dat" , tmp_user);
    node nod (offset , User_id);
    userid_list.addnode(nod);
    fstream io;
    io.open("LOG.dat",ios::in | ios::out | ios::binary);
    io.seekp(0,ios::end);
    LOG log("tourist" , "register a new account" , 0);
    io.write(reinterpret_cast<char*>(&log) , sizeof(LOG));
    io.close();
}

void my_delete(const char *User_id){
    if (USER_STATUS.empty()){cout << "Invalid\n";return;}
    if (strcmp(USER_STATUS.top().user_id , "root") != 0){cout << "Invalid\n";return;}
        //throw "Invalid";
    if (strcmp(User_id ,"root") == 0){cout << "Invalid\n";return;}
    // throw "Invalid";
    vector<int>possible_offset;
    userid_list.findnode(User_id , possible_offset);
    if (possible_offset.empty()){cout << "Invalid\n";return;}
        //throw "we did't find the ID";
    node Nod(possible_offset[0] , User_id);
    userid_list.deletenode(Nod);
    //
    fstream io;
    io.open("reporte.dat",ios::in | ios::out | ios::binary);
    io.seekp(0 ,ios::end);
    string str = User_id;
    report_employee tmp_re_em(USER_STATUS.top().name , "delete an account : "+str,0);
    io.write(reinterpret_cast<char*>(&tmp_re_em) , sizeof(report_employee));
    io.close();
    io.open("LOG.dat",ios::in | ios::out | ios::binary);
    io.seekp(0,ios::end);
    LOG log(USER_STATUS.top().name, "delete an account : "+str , 0);
    io.write(reinterpret_cast<char*>(&log) , sizeof(LOG));
    io.close();
}

void passwd(const char * User_id , const char *newpas , const char* oldpas){
    if (oldpas == nullptr){
            //throw "Invalid : we need your oldpasswd";
        if (USER_STATUS.empty()) {cout << "Invalid\n";return;};
        if (strcmp(USER_STATUS.top().user_id , "root") != 0){cout << "Invalid\n";return;}
        vector<int>possible_offset;
        userid_list.findnode(User_id , possible_offset);
        if (possible_offset.empty()){cout << "Invalid\n";return;}
        //throw "Invalid : wrong ID";
        Users tmp_user(my_read<Users>("user.dat" , possible_offset[0]));
        strcpy(tmp_user.passwd , newpas);
        my_write<Users>("user.dat" , tmp_user , possible_offset[0]);
        fstream io;
        io.open("LOG.dat",ios::in | ios::out | ios::binary);
        io.seekp(0 , ios::end);
          LOG log(USER_STATUS.top().name , "changed password" , 0);
        io.write(reinterpret_cast<char*>(&log) , sizeof(LOG));
        io.close();
        return;
    }
    vector<int>possible_offset;
    userid_list.findnode(User_id , possible_offset);
    if (possible_offset.empty()){cout << "Invalid\n";return;}
        //throw "Invalid : wrong ID";
    Users tmp_user(my_read<Users>("user.dat" , possible_offset[0]));
    if (strcmp(oldpas , tmp_user.passwd) != 0 ){cout << "Invalid\n";return;}
        // throw "Invalid : wrong passwd";
    strcpy(tmp_user.passwd , newpas);
    my_write<Users>("user.dat" , tmp_user , possible_offset[0]);
    fstream io;
    io.open("LOG.dat",ios::in | ios::out | ios::binary);
    io.seekp(0 , ios::end);
    LOG log(USER_STATUS.top().name , "changed password" , 0);
    io.write(reinterpret_cast<char*>(&log) , sizeof(LOG));
    io.close();
}

void select_book(const char* ISBN){
    if(USER_STATUS.empty()){cout << "Invalid\n";return;}
        //throw "Invalid : no loged_user now";
    else{
        if (USER_STATUS.top().privilege < 3){cout << "Invalid\n";return;}
            //throw "Invalid : you need higher privilege";
        else{
            vector<int>possible_offset;
            isbn_list.findnode(ISBN,possible_offset);
            if (possible_offset.empty()){
                Book tmp_book(0, ISBN,"","","",0);
                fstream io;
                io.open("book.dat",ios::in | ios ::out |ios::binary);
                if (!io) {cout << "Invalid\n";return;}
                    //throw "Error in select in open file";
                io.seekp(0,ios::end);
                int offset = io.tellp();
                io.write(reinterpret_cast<char *>(&tmp_book) , sizeof(Book));
                USER_STATUS.top().selected = offset;
                node nod(offset,ISBN);
                isbn_list.addnode(nod);
                io.close();
            } else{
             USER_STATUS.top().selected = possible_offset[0];
            }
        }
    }
}

void modify(const char *new_ISBN , const char *new_name ,
            const char *new_author , const char *new_keyword , double new_price ){
    if (USER_STATUS.empty()){cout << "Invalid\n";return;}
    int offset = USER_STATUS.top().selected;
    if (offset == -1){cout << "Invalid\n";return;}
        //throw"Invalid : you need to select first.";
    Book tmp_book = my_read<Book>("book.dat" , offset);
    fstream io;
    io.open("book.dat",ios::in | ios::out | ios::binary);
    if(new_ISBN != nullptr){
        vector<int>possible_offset;
        isbn_list.findnode(new_ISBN , possible_offset);
        if (!possible_offset.empty()){
            //node nod(offset , tmp_book.ISBN);
            //isbn_list.deletenode(nod);
            //throw"Invalid";
            cout << "Invalid\n";return;
        }
        node nod(offset , tmp_book.ISBN);
        isbn_list.deletenode(nod);
        node nod_(offset , new_ISBN);
        isbn_list.addnode(nod_);
        strcpy(tmp_book.ISBN , new_ISBN);
        io.seekp(offset);
        io.write(reinterpret_cast<char*>(&tmp_book) , sizeof(Book));
        io.close();
        io.open("reporte.dat",ios::in | ios::out | ios::binary);
        io.seekp(0 , ios::end);
        string str = new_ISBN;
        report_employee tmp_re_em(USER_STATUS.top().name , "modify ISBN" + str , 0);
        io.write(reinterpret_cast<char*>(&tmp_re_em), sizeof(report_employee));
        io.close();
        io.open("LOG.dat" ,ios::in | ios::out | ios::binary);
        io.seekp(0 , ios::end);
        LOG tmp_log(USER_STATUS.top().name , "modify ISBN" + str , 0);
        io.write(reinterpret_cast<char*>(&tmp_log) , sizeof(LOG));
        io.close();
    }
    if(new_author != nullptr){
        vector<int>possible_offset;
        author_list.findnode(tmp_book.author, possible_offset);
        if (!possible_offset.empty()){
            node nod(offset , tmp_book.author);
            author_list.deletenode(nod);
        }
        node nod(offset , new_author);
        author_list.addnode(nod);
        strcpy(tmp_book.author , new_author);
        io.seekp(offset);
        io.write(reinterpret_cast<char*>(&tmp_book) , sizeof(Book));
        io.close();

        io.open("reporte.dat",ios::in | ios::out | ios::binary);
        io.seekp(0 , ios::end);
        string str = new_author;
        report_employee tmp_re_em(USER_STATUS.top().name , "modify author" + str , 0);
        io.write(reinterpret_cast<char*>(&tmp_re_em), sizeof(report_employee));
        io.close();
        io.open("LOG.dat" ,ios::in | ios::out | ios::binary);
        io.seekp(0 , ios::end);
        LOG tmp_log(USER_STATUS.top().name , "modify author" + str , 0);
        io.write(reinterpret_cast<char*>(&tmp_log) , sizeof(LOG));
        io.close();
    }
    if(new_name != nullptr){
        vector<int>possible_offset;
        name_list.findnode(tmp_book.name , possible_offset);
        if (!possible_offset.empty()){
            node nod(offset , tmp_book.name);
            name_list.deletenode(nod);
        }
        node nod(offset , new_name);
        name_list.addnode(nod);
        strcpy(tmp_book.name , new_name);
        io.seekp(offset);
        io.write(reinterpret_cast<char*>(&tmp_book) , sizeof(Book));
        io.close();

        io.open("reporte.dat",ios::in | ios::out | ios::binary);
        io.seekp(0 , ios::end);
        string str = new_name;
        report_employee tmp_re_em(USER_STATUS.top().name , "modify name" + str , 0);
        io.write(reinterpret_cast<char*>(&tmp_re_em), sizeof(report_employee));
        io.close();
        io.open("LOG.dat" ,ios::in | ios::out | ios::binary);
        io.seekp(0 , ios::end);
        LOG tmp_log(USER_STATUS.top().name , "modify name" + str , 0);
        io.write(reinterpret_cast<char*>(&tmp_log) , sizeof(LOG));
        io.close();
    }
    if(new_keyword != nullptr){
        vector<string>old_key;
        int offsets = USER_STATUS.top().selected;
        fstream iot;
        iot.open("book.dat",ios::in | ios::out| ios::binary);
        iot.seekg(offsets);
        Book tmp_books;
        iot.read(reinterpret_cast<char*>(&tmp_books),sizeof(Book));
        string key = tmp_books.keywords;
        splitkey(key,old_key);
        sort(old_key.begin(),old_key.end());
        if(!old_key.empty()){
            for(auto & k : old_key){
                node nod(offsets,k.c_str());
                keyword_list.deletenode(nod);
            }
        }
        vector<string>new_key;
        splitkey(new_keyword,new_key);
        for(int i = 0 ; i < new_key.size() - 1;i++){
            if(strcmp(new_key[i].c_str(),new_key[i + 1].c_str()) == 0){cout << "Invalid\n";return;}
                //throw"Invalid : same key";
        }
        for(auto & j : new_key){
            node nod(offsets,j.c_str());
            keyword_list.addnode(nod);
        }
        strcpy(tmp_books.keywords,new_keyword);
        iot.seekp(offsets);
        iot.write(reinterpret_cast<char*>(&tmp_books),sizeof(Book));
        iot.close();
        iot.open("reporte.dat",ios::in | ios::out | ios::binary);
        report_employee tmp_re_em(USER_STATUS.top().name,"modify keyword " + key,0);
        iot.seekp(0,ios::end);
        iot.write(reinterpret_cast<char*>(&tmp_re_em),sizeof(report_employee));
        iot.close();
        iot.open("LOG.dat",ios::in | ios::out | ios::binary);
        iot.seekp(0,ios::end);
        LOG tmp_log(USER_STATUS.top().name,"modify keyword " + key,0);
        iot.write(reinterpret_cast<char*>(&tmp_log),sizeof(LOG));
        iot.close();
    }
    if(new_price != -1){
        tmp_book.price = new_price;
        io.seekp(offset);
        io.write(reinterpret_cast<char*>(&tmp_book),sizeof(Book));
        io.close();

        io.open("reporte.dat",ios::in | ios::out | ios::binary);
        io.seekp(0 , ios::end);
        stringstream ss;
        ss << new_price;
        string str;
        str = ss.str();
        report_employee tmp_re_em(USER_STATUS.top().name , "modify price" + str , 0);
        io.write(reinterpret_cast<char*>(&tmp_re_em), sizeof(report_employee));
        io.close();
        io.open("LOG.dat" ,ios::in | ios::out | ios::binary);
        io.seekp(0 , ios::end);
        LOG tmp_log(USER_STATUS.top().name , "modify price" + str , 0);
        io.write(reinterpret_cast<char*>(&tmp_log) , sizeof(LOG));
        io.close();
        ss.clear();
    }
}

void import(int quantity , double cost_price){
    if (USER_STATUS.empty()){cout << "Invalid\n";return;}
    if (USER_STATUS.top().privilege < 3){cout << "Invalid\n";return;}
        //throw"Invalid";
    else{
        int offset = USER_STATUS.top().selected;
        if (offset == -1){cout << "Invalid\n";return;}
            //throw"Invalid";
        fstream io;
        io.open("book.dat",ios::in | ios::out | ios::binary);
        if (!io){cout << "Invalid\n";return;}
            //throw"Error in open book in import";
        io.seekg(offset);
        Book tmp_book;
        io.read(reinterpret_cast<char*>(&tmp_book) , sizeof(Book));
        tmp_book.quantity += quantity;
        io.seekp(offset);
        io.write(reinterpret_cast<char*>(&tmp_book) , sizeof(Book));
        io.close();
        Transaction tmp_trans(tmp_book.ISBN,USER_STATUS.top().user_id, quantity , - cost_price);
        io.open("transaction.dat",ios::in | ios::out | ios::binary);
        if (!io){cout << "Invalid\n";return;}
            //throw "Error in open transaction in import";
        io.seekp(0 , ios::end);
        io.write(reinterpret_cast<char*>(&tmp_trans) , sizeof(Transaction));
        io.close();
        io.open("totlemoney.dat",ios::in | ios::out | ios::binary);
        if(!io){cout << "Invalid\n";return;}
            // throw "Error in open totlemoney in import";
        io.seekg(0 ,ios::beg);
        totle_money tmp_money;
        io.read(reinterpret_cast<char*>(&tmp_money) , sizeof(totle_money));
        tmp_money.expense += cost_price;
        io.seekp(0 , ios::beg);
        io.write(reinterpret_cast<char*>(&tmp_money) , sizeof(totle_money));
        io.close();
        io.open("reporte.dat",ios::in | ios::out | ios::binary);
        report_employee tmp_re_em(USER_STATUS.top().name , "import" , cost_price);
        io.seekp(0 , ios::beg);
        io.write(reinterpret_cast<char*>(&tmp_re_em) , sizeof(report_employee));
        io.close();
        io.open("reportf.dat",ios::in | ios::out | ios::binary);
        io.seekp(0 , ios::end);
        string str = tmp_book.name;
        report_finance tmp_re_fi("import +"+str + " - " , cost_price);
        io.write(reinterpret_cast<char*>(&tmp_re_fi) , sizeof(report_finance));
        io.close();
        io.open("LOG.dat",ios::in | ios::out | ios::binary);
        io.seekp(0 , ios::end);
        stringstream ss;
        ss << quantity;
        string quan ;
        quan = ss.str();
        LOG tmp_log(USER_STATUS.top().name , "import +" + str + "(" + quan + ")" +" - " , cost_price);
        io.write(reinterpret_cast<char*>(&tmp_log) , sizeof(LOG));
        io.close();
    }
}

void splitkey(const string &str , vector<string>&str_vec){
    stringstream sstream(str);
    string tmp_str;
    while (getline(sstream, tmp_str ,'|'))str_vec.push_back(tmp_str);
}

void show(){
    if (USER_STATUS.empty()){cout << "Invalid\n";return;}
    if (USER_STATUS.top().privilege < 1){cout << "Invalid\n";return;}
        //throw "Invalid";
    fstream io;
    io.open("book.dat",ios::in | ios::out | ios::binary);
    io.seekg(0 , ios::end);
    int offsetmax = io.tellg();
    int current = 0;
    vector<Book>book_stack;
    while (current < offsetmax){
        Book tmp_book;
        io.seekg(current);
        io.read(reinterpret_cast<char*>(&tmp_book) , sizeof(Book));
        book_stack.push_back(tmp_book);
        current += sizeof(Book);
    }
    if (book_stack.empty()){
        cout << "\n";
        return;
    }
    sort(book_stack.begin() , book_stack.end());
    for(auto &i :book_stack){
        i.show();
    }
    io.close();
}

void show(const char* key , Linked_list &list){
    if (USER_STATUS.empty()){cout << "Invalid\n";return;}
    if (USER_STATUS.top().privilege < 1) {cout << "Invalid\n";return;}
        //throw "Invalid";
    vector<int>possible_offset;
    list.findnode(key , possible_offset);
    if(possible_offset.empty()){
        cout << "\n";
        return;
    }
    vector<Book>book_stack;
    int i = 0;
    fstream io;
    io.open("book.dat",ios::in | ios::out | ios::binary);
    if (!io){cout << "Invalid\n";return;}
        //throw "Error in open file in show";
    while (i < possible_offset.size()){
        io.seekg(possible_offset[i]);
        Book tmp_book;
        io.read(reinterpret_cast<char*>(&tmp_book) , sizeof(Book));
        book_stack.push_back(tmp_book);
        i++;
    }
    io.close();
    if (book_stack.empty()){
        cout << "\n";
        return;
    }
    sort(book_stack.begin() , book_stack.end());
    if (&list == &author_list){
        for (auto & p :book_stack){
            if(strcmp(p.author , key) == 0)p.show();
            else continue;
        }
        return;
    }
    if (&list == &isbn_list){
        for (auto & p :book_stack){
            if (strcmp(p.ISBN , key) == 0)p.show();
            else continue;
        }
        return;
    }
    if (&list == &name_list){
        for (auto & p :book_stack){
            if (strcmp(p.name , key) == 0)p.show();
            else continue;
        }
        return;
    }
    if (&list == &keyword_list) {
        for (auto &p: book_stack) {
            vector<string> str_vec;
            splitkey(p.keywords, str_vec);
            bool have_or_not = false;
            for (auto &it : str_vec) {
                if (it == key) {
                    have_or_not = true;
                    break;
                }
            }
            if (have_or_not)p.show();
            else continue;
        }
        return;
    }
}

void showfina(int time ){
    if (USER_STATUS.empty() || USER_STATUS.top().privilege != 7){cout << "Invalid\n";return;}
    fstream io;
    if (time == -1){
        io.open("totlemoney.dat",ios::in | ios::out | ios::binary);
        io.seekg(0);
        totle_money tmp_money;
        io.read(reinterpret_cast<char*>(&tmp_money) , sizeof(tmp_money));
        cout << "+ " << setiosflags(ios::fixed) << setprecision(2) << tmp_money.income <<" - " <<tmp_money.expense <<endl;
        io.close();
        return;
    } else{
        io.open("transaction.dat",ios::in | ios::out | ios::binary);
        io.seekg(0 , ios::end);
        int maxoffset = io.tellg();
        unsigned long current = maxoffset - sizeof(Transaction) * (time);
        int p = maxoffset - sizeof(Transaction)*time;
        if (p < 0){cout << "Invalid\n";return;}
        double expense = 0, income = 0;
        for(int i = 0 ; i < time ; ++i){
            io.seekg(current);
            Transaction tmp_trans;
            io.read(reinterpret_cast<char*>(&tmp_trans) ,sizeof(Transaction));
            if (tmp_trans.total_price > 0){
                income += tmp_trans.total_price;
            }else{
                expense -= tmp_trans.total_price;
            }
            current += sizeof(Transaction);
        }
        cout << setiosflags(ios::fixed)<<setprecision(2) << "+ " << income << " - " <<expense <<endl;
    }
}

void buy(const char *ISBN , int quantity) {
    vector<int> possibleoffset;
    isbn_list.findnode(ISBN, possibleoffset);
    if (possibleoffset.empty()){cout << "Invalid\n";return;}
        //throw "Invalid : we don't have this book";
    int offset = possibleoffset[0];
    Book tmp_book;
    fstream io;
    io.open("book.dat", ios::in | ios::out | ios::binary);
    if (!io){cout << "Invalid\n";return;}
        //throw "Error in open book in buy";
    io.seekg(offset);
    io.read(reinterpret_cast<char *>(&tmp_book), sizeof(Book));
    if (tmp_book.quantity < quantity){cout << "Invalid\n";return;}
        //throw "Invalid : we don't have enough books";
    else {
        tmp_book.quantity -= quantity;
        io.seekp(offset);
        io.write(reinterpret_cast<char *>(&tmp_book), sizeof(Book));
        io.close();
        io.open("transaction.dat",ios::in | ios::out | ios::binary);
        cout << setiosflags(ios::fixed) << setprecision(2) << quantity * tmp_book.price << endl;
        if (!io){cout << "Invalid\n";return;}
            //throw "Error in open transaction in buy";
        Transaction tmp_trans(ISBN , USER_STATUS.top().user_id , quantity , quantity * tmp_book.price);
        io.seekp(0 , ios::end);
        io.write(reinterpret_cast<char*>(&tmp_trans) , sizeof(Transaction));
        io.close();
        io.open("totlemoney.dat",ios::in | ios::out |ios::binary);
        if (!io){cout << "Invalid\n";return;}
            //throw "Error in open money in buy";
        io.seekg(0);
        totle_money tmp_money;
        io.read(reinterpret_cast<char*>(&tmp_money) , sizeof(totle_money));
        tmp_money.income += quantity * tmp_book.price;
        io.seekp(0);
        io.write(reinterpret_cast<char*>(&tmp_money) , sizeof(totle_money));
        io.close();

        string str = tmp_book.name;
        if (USER_STATUS.top().privilege == 3){
            io.open("reporte.dat",ios::in | ios::out |ios::binary);
            io.seekp(0,ios::end);
            stringstream ss;
            ss << quantity;
            string num = ss.str();
            report_employee tmp_re_em(USER_STATUS.top().name , "buy " + str + "(" +num +")" , quantity * tmp_book.price);
            ss.clear();
            io.write(reinterpret_cast<char*>(&tmp_re_em) , sizeof(report_employee));
            io.close();
        }
        io.open("reportf.dat",ios::in | ios::out |ios::binary);
        io.seekp(0 , ios::end);
        stringstream  ss;
        ss << quantity;
        string num = ss.str();
        report_finance tmp_re_fi("sell " + str + "(" +num +")" , quantity * tmp_book.price);
        io.write(reinterpret_cast<char*>(&tmp_re_fi) , sizeof(report_finance));
        ss.clear();
        io.close();
        io.open("LOG.dat",ios::in | ios::out |ios::binary);
        io.seekp(0 , ios::end);
        LOG tmp_log(USER_STATUS.top().name , "buy " + str +"(" +num +")" , quantity * tmp_book.price);
        io.write(reinterpret_cast<char*>(&tmp_log) , sizeof(LOG));
        io.close();
    }
}

void clear(){
    fstream io;
    io.open("userid.dat",ios::out);
    io.close();
    io.open("user.dat",ios::out);
    io.close();
    io.open("totlemoney.dat",ios::out);
    io.close();
    io.open("transaction.dat",ios::out);
    io.close();
    io.open("book.dat",ios::out);
    io.close();
    io.open("ISBN.dat",ios::out);
    io.close();
    io.open("author.dat",ios::out);
    io.close();
    io.open("name.dat",ios::out);
    io.close();
    io.open("bookkey.dat",ios::out);
    io.close();
    io.open("reportf.dat",ios::out);
    io.close();
    io.open("reporte.dat",ios::out);
    io.close();
    io.open("LOG.dat",ios::out);
    io.close();
    Users root("root" , "sjtu" , "root" , 7);
    int offset = my_write<Users>("user.dat" ,root);
    node nod(offset ,"root");
    userid_list.addnode(nod);
    totle_money tmp_money;
    io.open("totlemoney.dat",ios::in | ios::out |ios::binary);
    io.seekp(0);
    io.write(reinterpret_cast<char*>(&tmp_money) , sizeof(totle_money));
    io.close();
}

void Run(string &cmd){
    int len = cmd.length();
    string type;
    int i = 0;
    for(;i < len && cmd[i] != ' '; ++i){
        type += cmd[i];
    }
    if (type == "su"){
        char user_id[31] = "";
        char passwd[31] = "";
        user_id[30] = '\0';
        passwd[30] = '\0';
        for(;cmd[i] == ' ' ; ++i);
        for(int j = 0; i < len && cmd[i] != ' ' && cmd[i] != '\0' ;++j, ++i){
            user_id[j] = cmd[i];
        }
        for(;cmd[i] == ' ' ; ++i);
        for (int j = 0 ; i < len && cmd[i] != ' ' && cmd[i] != '\0' ; ++j , ++i){
            passwd[j] = cmd[i];
        }
        try {
            su(user_id , passwd);
        } catch (...) {
            //throw "problem in su";
            cout << "Invalid\n";return;
        }
        return;
    }
    else if (type == "logout"){
        logout();
    }
    else if (type == "useradd"){
        char user_id[31] = "";
        char passwd[31] = "";
        char privilege = ' ';
        char name[31] = "";
        user_id[30] = '\0';
        passwd[30] = '\0';
        name[30] = '\0';
        char prev = ' ';
        int num = 0;
        for(int j = 0; cmd[j] != '\0' ; ++j){
            if (prev == ' ' && cmd[j] != ' ') {
                num++;
            }
            prev = cmd[j];
        }
        if (num != 5) {cout << "Invalid\n";return;}
        for(;cmd[i] == ' ' ; ++i);
        for(int j = 0; i < len && cmd[i] != ' ' && cmd[i] != '\0' ;++j, ++i){
            user_id[j] = cmd[i];
        }
        for(;cmd[i] == ' ' ; ++i);
        for (int j = 0 ; i < len && cmd[i] != ' ' && cmd[i] != '\0' ; ++j , ++i){
            passwd[j] = cmd[i];
        }
        for(;cmd[i] == ' ' ; ++i);
        privilege = cmd[i];
        i++;//bug::未++
        for(;cmd[i] == ' ' ; ++i);
        for (int j = 0 ; i < len && cmd[i] != ' ' && cmd[i] != '\0' ; ++j , ++i){
            name[j] = cmd[i];
        }
        if (USER_STATUS.empty()){cout << "Invalid\n";return;}
        if (privilege > '9' || privilege < '0'){cout << "Invalid\n";return;}
        if (privilege - '0' < USER_STATUS.top().privilege)
        {try {
                useradd(user_id , passwd , privilege - '0' ,name);
            } catch (...) {
                //throw "problem in useradd";
                cout << "Invalid\n";return;
            }
        } else {cout << "Invalid\n";return;}
            //throw "Invalid";
        return;
    }
    else if(type == "import"){
        char prev = ' ';
        int num = 0;
        for(int j = 0; cmd[j] != '\0' ; ++j){
            if (prev == ' ' && cmd[j] != ' ') {
                num++;
            }
            prev = cmd[j];
        }
        if (num != 3){cout << "Invalid\n";return;}
        char quantity[31] = "";
        char totalprice[31] = "";
        quantity[30] = '\0';
        totalprice[30] = '\0';
        for (; cmd[i] == ' '; ++i);
        for (int j = 0; i < len && cmd[i] != ' ' && cmd[i] != '\0'; ++j, ++i) {
            quantity[j] = cmd[i];
        }
        for (; cmd[i] == ' '; ++i);
        for (int j = 0; i < len && cmd[i] != ' ' && cmd[i] != '\0'; ++j, ++i) {
            totalprice[j] = cmd[i];
        }
        bool flag = check_quantity(quantity);
        if (!flag){cout << "Invalid\n";return;}
        stringstream ss;
        ss << quantity;
        int dou1;
        ss >> dou1;
        double dou2 = atof(totalprice);
        dou2 = ((int)(dou2 * 1000))/1000.0;
        import(dou1 , dou2);
    }
    else if (type == "register"){
        char user_id[31] = "";
        char passwd[31] = "";
        char name[31] = "";
        user_id[30] = '\0';
        passwd[30] = '\0';
        name[30] = '\0';
        char prev = ' ';
        int num = 0;
        for(int j = 0; cmd[j] != '\0' ; ++j){
            if (prev == ' ' && cmd[j] != ' ') {
                num++;
            }
            prev = cmd[j];
        }
        if (num == 4) {
            for (; cmd[i] == ' '; ++i);
            for (int j = 0; i < len && cmd[i] != ' ' && cmd[i] != '\0'; ++j, ++i) {
                user_id[j] = cmd[i];
            }
            for (; cmd[i] == ' '; ++i);
            for (int j = 0; i < len && cmd[i] != ' ' && cmd[i] != '\0'; ++j, ++i) {
                passwd[j] = cmd[i];
            }
            for (; cmd[i] == ' '; ++i);
            for (int j = 0; i < len && cmd[i] != ' ' && cmd[i] != '\0'; ++j, ++i) {
                name[j] = cmd[i];
            }
            try {
                my_register(user_id, passwd, name);
            } catch (...) {
                //throw "problem in register";
                cout << "Invalid\n";return;
            }
        } else {cout << "Invalid\n";return;}
            //throw "Invalid";
    }
    else if (type == "delete"){
        char user_id[31] = "";
        user_id[30] = '\0';
        for(;cmd[i] == ' ' ; ++i);
        for(int j = 0; i < len && cmd[i] != ' ' && cmd[i] != '\0' ;++j, ++i){
            user_id[j] = cmd[i];
        }
        try {
            my_delete(user_id);
        } catch (...) {
            //throw "problem in delete";
            cout << "Invalid\n";return;
        }
        return;
    }
    else if (type == "passwd"){
        char prev = ' ';
        int num = 0;
        for(int j = 0; cmd[j] != '\0' ; ++j){
            if (prev == ' ' && cmd[j] != ' ') {
                num++;
            }
                prev = cmd[j];
        }
        if (num == 4){
            char user_id[31] = "";
            char old_passwd[31] = "";
            char new_passwd[31] = "";
            user_id[30] = '\0';
            old_passwd[30] = '\0';
            new_passwd[30] = '\0';
            for(;cmd[i] == ' ' ; ++i);
            for(int j = 0; i < len && cmd[i] != ' ' && cmd[i] != '\0' ;++j, ++i){
                user_id[j] = cmd[i];
            }
            for(;cmd[i] == ' ' ; ++i);
            for (int j = 0 ; i < len && cmd[i] != ' ' && cmd[i] != '\0' ; ++j , ++i){
                old_passwd[j] = cmd[i];
            }
            for(;cmd[i] == ' ' ; ++i);
            for (int j = 0 ; i < len && cmd[i] != ' ' && cmd[i] != '\0' ; ++j , ++i){
                new_passwd[j] = cmd[i];
            }
            try {
                passwd(user_id,new_passwd,old_passwd);
            } catch (...) {
                //throw "problem in passwd";
                cout << "Invalid\n";return;
            }
        } else if(num == 3){
            char user_id[31] = "";
            char new_passwd[31] = "";
            user_id[30] = '\0';
            new_passwd[30] = '\0';
            for(;cmd[i] == ' ' ; ++i);
            for(int j = 0; i < len && cmd[i] != ' ' && cmd[i] != '\0' ;++j, ++i){
                user_id[j] = cmd[i];
            }
            for(;cmd[i] == ' ' ; ++i);
            for (int j = 0 ; i < len && cmd[i] != ' ' && cmd[i] != '\0' ; ++j , ++i){
                new_passwd[j] = cmd[i];
            }
            try {
                passwd(user_id , new_passwd , nullptr);
            } catch (...) {
                //throw "problem in passwd";
                cout << "Invalid\n";return;
            }
        } else{
            //throw "Invalid";
            cout << "Invalid\n";return;
        }
    }
    else if (type == "select"){
        char prev = ' ';
        int num = 0;
        for(int j = 0; cmd[j] != '\0' ; ++j){
            //bug:括号
            if (prev == ' ' && cmd[j] != ' ') num++;
            prev = cmd[j];
        }
        if (num == 2){
            char isbn[31] = "";
            isbn[30] = '\0';
            for(;cmd[i] == ' ' ; ++i);
            for(int j = 0; i < len && cmd[i] != ' ' && cmd[i] != '\0' ;++j, ++i){
                isbn[j] = cmd[i];
            }
            try {
                select_book(isbn);
            } catch (...) {
                //throw "problem in select";
                cout << "Invalid\n";return;
            }

        } else{
            //throw "Invalid";
            cout << "Invalid\n";return;
        }
    }
    else if (type == "modify"){
        if (USER_STATUS.empty()){cout << "Invalid\n";return;}
        if (USER_STATUS.top().selected == -1){cout << "Invalid\n";return;}
        while (cmd[i] != '\0'){
            for(;cmd[i] == ' ' || cmd[i] == '\"' ; ++i);
            char Mod[61] = "", Num[61] = "";
            Mod[60] = '\0';
            Num[60] = '\0';
            for(int j = 0; i < len && cmd[i] != ' ' && cmd[i] != '='  && cmd[i] != '\0' ;++j, ++i){
                Mod[j] = cmd[i];
            }
            try {
                switch (Mod[1]) {
                    case 'I': {
                        for (; cmd[i] == ' '  || cmd[i] == '\"' || cmd[i] == '='; ++i);
                        for (int j = 0; i < len && cmd[i] != ' ' && cmd[i] != '\"' && cmd[i] != '\0'; ++j, ++i) {
                            Num[j] = cmd[i];
                        }
                        modify(Num, nullptr, nullptr, nullptr , -1);
                        break;
                    }
                    case 'n': {
                        for (; cmd[i] == ' '  || cmd[i] == '\"' || cmd[i] == '='; ++i);
                        for (int j = 0; i < len && cmd[i] != ' ' && cmd[i] != '\"' && cmd[i] != '\0'; ++j, ++i) {
                            Num[j] = cmd[i];
                        }
                        modify(nullptr, Num, nullptr, nullptr , -1);
                        break;
                    }
                    case 'a': {
                        for (; cmd[i] == ' '  || cmd[i] == '\"' || cmd[i] == '='; ++i);
                        for (int j = 0; i < len && cmd[i] != ' ' && cmd[i] != '\"' && cmd[i] != '\0'; ++j, ++i) {
                            Num[j] = cmd[i];
                        }
                        modify(nullptr, nullptr, Num, nullptr , -1);
                        break;
                    }
                    case 'k': {
                        for (; cmd[i] == ' '  || cmd[i] == '\"' || cmd[i] == '='; ++i);
                        for (int j = 0; i < len && cmd[i] != ' ' && cmd[i] != '\"' && cmd[i] != '\0'; ++j, ++i) {
                            Num[j] = cmd[i];
                        }
                        modify(nullptr, nullptr, nullptr, Num , -1);
                        break;
                    }
                    case 'p':{
                        for (; cmd[i] == ' '  || cmd[i] == '\"' || cmd[i] == '='; ++i);
                        for (int j = 0; i < len && cmd[i] != ' ' && cmd[i] != '\"' && cmd[i] != '\0'; ++j, ++i) {
                            Num[j] = cmd[i];
                        }
                        double result;
                        stringstream sstream;
                        sstream << Num;
                        sstream >> result;
                        modify(nullptr , nullptr , nullptr , nullptr , result);
                    }
                }
            } catch (...) {
                //throw "problem in modify";
                cout << "Invalid\n";return;
            }
        }
    }
    else if (type == "show"){
        char prev = ' ';
        int num = 0;
        for(int j = 0; cmd[j] != '\0' ; ++j){
            if (prev == ' ' && cmd[j] != ' ') {
                num++;
            }
                prev = cmd[j];
        }
        if (num == 1) {
            try {
                show();
            } catch (...) {
                //throw "problem in show";
                cout << "Invalid\n";return;
            }
        }
        else if (num == 2){
            for(;cmd[i] == ' ' ; ++i);
            char Mod[61] = "", Num[61] = "";
            Mod[60] = '\0';
            Num[60] = '\0';
            for(int j = 0; i < len && cmd[i] != ' ' && cmd[i] != '='  && cmd[i] != '\0' ;++j, ++i){
                Mod[j] = cmd[i];
            }
            for(;cmd[i] == ' ' || cmd[i] == '=' || cmd[i] == '\"'; ++i);
            for (int j = 0; i < len && cmd[i] != ' ' && cmd[i] != '\"' && cmd[i] != '\0'; ++j, ++i) {
                Num[j] = cmd[i];
            }
            try {
                switch (Mod[1]) {
                    case 'I': {
                        show(Num, isbn_list);
                        break;
                    }
                    case 'n':{
                        show(Num , name_list);
                        break;
                    }
                    case 'a':{
                        show(Num , author_list);
                        break;
                    }
                    case 'k':{
                        show(Num , keyword_list);
                        break;
                    }
                    case 'i':{
                        showfina(-1);
                        break;
                    }
                    default:{//throw "Invalid";
                    cout << "Invalid\n";return;}
                }
            } catch (...) {
                //throw "problem in show";
                cout << "Invalid\n";return;
            }
        }
        else if (num == 3){
            for(;cmd[i] == ' ' ; ++i);
            char Mod[31] = "";
            Mod[30] = '\0';
            string time;
            for(int j = 0; i < len && cmd[i] != ' ' && cmd[i] != '='  && cmd[i] != '\0' ;++j, ++i){
                Mod[j] = cmd[i];
            }
            for(;cmd[i] == ' ' || cmd[i] == '=' || cmd[i] == '\"'; ++i);
            for (int j = 0; i < len && cmd[i] != ' ' && cmd[i] != '\"' && cmd[i] != '\0'; ++j, ++i) {
                time += cmd[i];
            }
            int Time = 0;
            stringstream ss(time);
            ss >> Time;
            try {
                showfina(Time);
            } catch (...) {
                //throw "problem in showfinace";
                cout << "Invalid\n";return;
            }
        } else {cout << "Invalid\n";return;}
            //throw "Invalid";
    }
    else if (type == "buy"){
        char isbn[31] = "";
        isbn[30] = '\0';
        string quan;
        int quantity = 0;
        for(;cmd[i] == ' ' ; ++i);
        for(int j = 0; i < len && cmd[i] != ' ' && cmd[i] != '='  && cmd[i] != '\0' ;++j, ++i){
            isbn[j] = cmd[i];
        }
        for(;cmd[i] == ' ' || cmd[i] == '=' || cmd[i] == '\"'; ++i);
        int j = 0;
        for (; i < len && cmd[i] != ' ' && cmd[i] != '\"' && cmd[i] != '\0'; ++j, ++i) {
            quan += cmd[i];
        }
        bool flag = check_quantity(quan);
        if (!flag){cout << "Invalid\n";return;}
        stringstream ss(quan);
        ss >> quantity;
        ss.clear();
        try {
            buy(isbn , quantity);
        } catch (...) {
            //throw "problem in buy";
            cout << "Invalid\n";return;
        }
    }
    else if (type == "report"){
        string token;
        for(;cmd[i] == ' ' ; ++i);
        for(int j = 0; i < len && cmd[i] != ' ' && cmd[i] != '='  && cmd[i] != '\0' ;++j, ++i){
            token[j] = cmd[i];
        }
        if ( !USER_STATUS.empty() && USER_STATUS.top().privilege >=3 ){
            if (token == "employee" && USER_STATUS.top().privilege == 7){
                fstream io;
                io.open("reporte.dat",ios::in |ios::out |ios::binary);
                io.seekg(0 , ios::end);
                int maxoffset = io.tellg();
                int cnt = 0;
                io.seekg(0 , ios::beg);
                while (cnt < maxoffset){
                    report_employee tmp_re_em;
                    io.read(reinterpret_cast<char*>(&tmp_re_em) , sizeof(report_employee));
                    if (tmp_re_em.money == 0){
                        cout << tmp_re_em.name << '\t' << tmp_re_em.action <<endl;
                    } else{
                        cout << tmp_re_em.name << '\t' << tmp_re_em.action << '\t' << tmp_re_em.money <<endl;
                    }
                    cnt += sizeof(report_employee);
                }
            }
            else if (token == "finance" && USER_STATUS.top().privilege == 7){
                fstream io;
                io.open("reportf.dat" ,ios::in | ios::out | ios::binary);
                io.seekg(0 , ios::end);
                int maxoffset = io.tellg();
                int cnt = 0;
                io.seekg(0 ,ios::beg);
                while (cnt < maxoffset){
                    report_finance tmp_re_fi;
                    io.read(reinterpret_cast<char*>(&tmp_re_fi) , sizeof(report_finance));
                    cout << tmp_re_fi.action << abs(tmp_re_fi.money) <<endl;
                    cnt += sizeof(report_finance);
                }
            } else if(token == "myself"){
                fstream io;
                io.open("reporte.dat",ios::in |ios::out |ios::binary);
                io.seekg(0 , ios::end);
                int maxoffset = io.tellg();
                int cnt = 0;
                io.seekg(0 , ios::beg);
                while (cnt < maxoffset){
                    report_employee tmp_re_em;
                    io.read(reinterpret_cast<char*>(&tmp_re_em) , sizeof(report_employee));
                    if (strcmp(USER_STATUS.top().name , tmp_re_em.name) == 0) {
                        if (tmp_re_em.money == 0) {
                            cout << tmp_re_em.name << '\t' << tmp_re_em.action << endl;
                        } else {
                            cout << tmp_re_em.name << '\t' << tmp_re_em.action << '\t' << tmp_re_em.money << endl;
                        }
                    }
                    cnt += sizeof(report_employee);
                }
            } else {cout << "Invalid\n";return;}
                //throw "Invalid";
        } else //throw "Invalid";
        {cout << "Invalid\n";return;}
    }
    else if (type == "log"){
        if (!USER_STATUS.empty() && USER_STATUS.top().privilege == 7){
            fstream io;
            io.open("LOG.dat" ,ios::in | ios::out | ios::binary);
            io.seekg(0 , ios::end);
            int maxoffset = io.tellg();
            int cnt = 0;
            io.seekg(0 ,ios::beg);
            while (cnt < maxoffset){
                LOG tmp_log;
                io.read(reinterpret_cast<char*>(&tmp_log) , sizeof(LOG));
                if (tmp_log.money == 0){
                    cout << tmp_log.name << '\t' << tmp_log.action <<endl;
                } else{
                    cout << tmp_log.name << '\t' << tmp_log.action << '\t' << tmp_log.money;
                }
                cnt += sizeof(report_finance);
            }
        } else //throw "Invalid";
        {cout << "Invalid\n";return;}
    }
    else if (type == "quit" || type == "exit"){
        exit(0);
    }
    else if (type == "clear"){
        clear();
    }
    else cout << "Invalid\n";
}

bool checkname(const string &a){
    if(strlen(a.c_str()) > 30 || strlen(a.c_str()) == 0)return false;
    for(int i = 0 ; i < strlen(a.c_str()) ; i++)
    {
        if(a[i] == '\"' || a[i] == ' ')return false;
    }
    return true;
}
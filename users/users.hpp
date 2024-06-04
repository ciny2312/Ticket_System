#include <fstream>
#include <iostream>
#include "../ticket/ticket.hpp"

extern ticket t;

struct user{
    String username,pass,name,mail;
    int pr;bool login;
    String output(){
        return username;
    }
};
class user_system{
    friend class ticket;
    sjtu::map<String,bool>logstack;
    readwrite<String,user> f;
    int n;
    public:
    user_system():logstack(),f("user.db","user_BPT.db"){
        std::fstream ff;
        ff.open("user.db");
        ff.seekg(0);
        ff.read(reinterpret_cast<char*>(&n),sizeof(int));
        logstack.clear();
        ff.close();
    }
    ~user_system(){
        std::fstream ff;
        ff.open("user.db");
        ff.seekp(0);
        ff.write(reinterpret_cast<char*>(&n),sizeof(int));
        ff.close();
        logstack.clear();
    }
    bool logged(String id){
        if(logstack.count(id)) return logstack[id];
        return false;
    }
    void add_user(String id,String _user,String _pass,String _name,String _mail,int _pr){
        if(n==0){
            user nw;
            nw.username=_user;
            nw.pass=_pass;
            nw.name=_name;
            nw.mail=_mail;
            nw.pr=10;
            f.insert(nw.username,nw,n);
            printf("0\n");
            return ;
        }
        user nw;int pos;
        if(!f.find(id,nw,pos)){
            printf("-1\n");
            return ;
        }
        if(!logged(id)){
            printf("-1\n");
            return ;
        }
        if(nw.pr<=_pr){
            printf("-1\n");
            return ;
        }
        user p;
        if(f.find(_user,p,pos)){
            printf("-1\n");
            return ;
        }
        p.username=_user;
        p.pass=_pass;
        p.name=_name;
        p.mail=_mail;
        p.pr=_pr;
        f.insert(_user,p,n);
        printf("0\n");
    }
    void log_in(String id,String _pass){
        user nw;int pos;
        if(!f.find(id,nw,pos)){
            printf("-1\n");
            return ;
        }
        if(logged(id)||_pass!=nw.pass){
            printf("-1\n");
            return ;
        }
        logstack[id]=true;
        printf("0\n");
    }
    void logout(String id){
        user nw;int pos;
        if(!f.find(id,nw,pos)){
            printf("-1\n");
            return ;
        }
        if(!logstack[id]){
            printf("-1\n");
            return ;
        }
        logstack[id]=false;
        printf("0\n");
    }
    void query_profile(String id,String _user){
        user nw;int pos;
        if(!f.find(id,nw,pos)){
            printf("-1\n");
            return ;
        }
        if(!logged(id)){
            printf("-1\n");
            return ;
        }
        user p;
        if(!f.find(_user,p,pos)){
            printf("-1\n");
            return ;
        }
        if(nw.pr<=p.pr&&nw.username!=_user){
            printf("-1\n");
            return ;
        }
        std::cout<<p.username<<' '<<p.name<<' '<<p.mail<<' '<<p.pr<<std::endl;
    }
    void modify_profile(String id,String _user,String _pass,bool f1,String _name,bool f2,String _mail,bool f3,int _pr){
        user nw;int pos;
        if(!f.find(id,nw,pos)){
            printf("-1\n");
            return ;
        }
        if(!logged(id)){
            printf("-1\n");
            return ;
        }
        user p;
        if(!f.find(_user,p,pos)){
            printf("-1\n");
            return ;
        }
        if(nw.pr<=p.pr&&nw.username!=_user){
            printf("-1\n");
            return ;
        }
        if(nw.pr<=_pr&&_pr!=-1){
            printf("-1\n");
            return ;
        }
        if(f1){
            p.pass=_pass;
        }
        if(f2){
            p.name=_name;
        }
        if(f3){
            p.mail=_mail;
        }
        if(_pr!=-1){
            p.pr=_pr;
        }
        f.updata(pos,p);
        std::cout<<p.username<<' '<<p.name<<' '<<p.mail<<' '<<p.pr<<std::endl;
    }
    void buy_ticket(String _user,String id,date_time d,int n,String from,String to,bool q=false){
        user nw;int pos;
        if(!f.find(_user,nw,pos)){
            printf("-1\n");
            return ;
        }
        if(!logged(_user)){
            printf("-1\n");
            return ;
        }
        t.buy_ticket(_user,id,d,n,from,to,q);
    }
    void query_order(String id){
        user nw;int pos;
        if(!f.find(id,nw,pos)){
            printf("-1\n");
            return ;
        }
        if(!logged(id)){
            printf("-1\n");
            return ;
        }
        t.query_order(id);
    }
    void refund_ticket(String id,int n=1){
        user nw;int pos;
        if(!f.find(id,nw,pos)){
            printf("-1\n");
            return ;
        }
        if(!logged(id)){
            printf("-1\n");
            return ;
        }
        t.refund_ticket(id,n);
    }
    void clear(){
        n=1;
        logstack.clear();
        f.clear();
    }
};
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include "bpt.hpp"
#include "vector.hpp"
#include "map.hpp"
#include "priority_queue.hpp"
template <class T1,class T2>
class readwrite{
    std::fstream f1;
    BPtree<T1,int> f;
    public:
    readwrite(std::string s1,std::string s2):f(s2){
        f1.open(s1);
    }
    ~readwrite(){
        f1.close();
    }
    bool find(T1 &key,T2 &val,int &pos){
        sjtu::vector<int>x;
        f.find(key,x);
        if(x.size()==0){
            return false;
        }
        pos = x[0];
        f1.seekg(sizeof(int)+sizeof(T2)*(pos-1));
        f1.read(reinterpret_cast<char*>(&val),sizeof(T2));
        return true;
    }
    void getall(T1 &key,sjtu::vector<T2> &ans){
        sjtu::vector<int>x;
        f.find(key,x);
        int n=x.size();T2 val;
        for(int i=0;i<n;i++){
            f1.seekp(sizeof(int)+sizeof(T2)*(x[i]-1));
            f1.read(reinterpret_cast<char*>(&val),sizeof(T2));
            ans.push_back(val);
        }
        return ;
    }
    void insert(T1 &key,T2 &val,int &n){
        f1.seekp(sizeof(int)+sizeof(T2)*n);
        f1.write(reinterpret_cast<char*>(&val),sizeof(T2));
        n++;
        f.insert(key,n);
    }
    void do_insert(T1 key,int n){
        f.insert(key,n);
    }
    void del(T1 &key,int n){
        f.del(key,n);
    }
    void updata(int n,T2 &val){
        f1.seekp(sizeof(int)+sizeof(T2)*(n-1));
        f1.write(reinterpret_cast<char*>(&val),sizeof(T2));
    }
    void clear(){
        f.clear();
    }
};
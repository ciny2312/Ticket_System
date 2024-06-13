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
        if(!f1.is_open()){
            int n=0;
            f1.open(s1,std::fstream::out);
            f1.seekp(0);f1.write(reinterpret_cast<char*>(&n),sizeof(n));
            f1.close();f1.open(s1);
        }
    /*    if (!f1.is_open()) {
            std::cerr << "readwrite:File not open." << std::endl;
            return;
        }*/
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
    void getall(T1 &key,sjtu::vector<int> &ans){
        f.find(key,ans);
        return ;
    }
    void ask_pos(int pos,T2 &ans){
        f1.seekg(sizeof(int)+sizeof(T2)*(pos-1));
        f1.read(reinterpret_cast<char*>(&ans),sizeof(T2));
        return ans;
    }
    void insert(T1 &key,T2 &val,int &n){
        f1.seekp(sizeof(int)+sizeof(T2)*n);
        f1.write(reinterpret_cast<char*>(&val),sizeof(T2));
        n++;
        f.insert(key,n);
    /*    // 检查文件是否已打开
        if (!f1.is_open()) {
            std::cerr << "insert:File not open." << std::endl;
            return;
        }
        
        // 计算写入位置
        std::streampos write_pos = sizeof(int) + sizeof(T2) * n;
        std::cout << "Attempting to seek to position: " << write_pos << std::endl;

        // 移动写指针并检查
        f1.seekp(write_pos);
        if (!f1) {
            std::cerr << "Seekp failed. Error state: " << f1.rdstate() << std::endl;
            return;
        }

        // 写入数据并检查
        f1.write(reinterpret_cast<char*>(&val), sizeof(T2));
        if (!f1) {
            std::cerr << "Write failed." << std::endl;
            return;
        }

        // 强制刷新到磁盘
        f1.flush();

        // 移动读指针并检查
        f1.seekg(write_pos);
        if (!f1) {
            std::cerr << "Seekg failed." << std::endl;
            return;
        }

        // 读取数据并检查
        T2 nw;
        f1.read(reinterpret_cast<char*>(&nw), sizeof(T2));
        if (!f1) {
            std::cerr << "Read failed." << std::endl;
            return;
        }

        // 输出调试信息
        std::cout << "HERE3: " << val.output() << std::endl;
        std::cout << "HERE3: " << nw.output() << std::endl;

        // 增加索引
        n++;

        // 插入键值对到容器/映射
        f.insert(key, n);
        
        */
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
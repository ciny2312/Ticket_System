#include<iostream>
#include"../file/clock.hpp"
struct train{
    static const int N=105;
    String id,sta[N];
    int sta_num,seat_num,seat[N][N],price[N],t_arrive[N],t_depart[N];
    date_time l,r,st;
    char tp;bool rele;
};
class train_system{
    int n;
    readwrite<String,train> f;
    readwrite<sjtu::pair<String,String>,train> road;
    readwrite<String,train> passby;

    public:
    train_system():f("train.db","train_BPT.db"),
    road("train.db","road_BPT.db"),
    passby("train.db","passby_BPT.db"){
        std::fstream ff;
        ff.open("train.db");
        if(!ff.is_open()){
            ff.open("train.db",std::fstream::out);
            ff.close();ff.open("train.db");
            n=0;ff.seekp(0);
            ff.write(reinterpret_cast<char*>(&n),sizeof(int));
        }
        else{
            ff.seekg(0);
            ff.read(reinterpret_cast<char*>(&n),sizeof(int));
        }
        ff.close();
    }
    ~train_system(){
        std::fstream ff;
        ff.open("train.db");
        ff.seekp(0);
        ff.write(reinterpret_cast<char*>(&n),sizeof(int));
        ff.close();
    }
    void add_train(String _id,int _num,int _seat,String sta[],int price[],date_time st,int t_drive[],int t_stop[],date_time l,date_time r,char tp){
        train nw;int pos;
        if(f.find(_id,nw,pos)){
            printf("-1\n");
            return ;
        }
        nw.id=_id;
        nw.sta_num=_num;
        nw.seat_num=_seat;
        int tot=count_day(l,r);
        for(int i=0;i<nw.sta_num-1;i++){
            for(int j=1;j<=tot;j++)
                nw.seat[j][i]=_seat;
        }
        for(int i=0;i<nw.sta_num;i++){
            nw.sta[i]=sta[i];
        }
        for(int i=1;i<nw.sta_num;i++){
            nw.price[i]=nw.price[i-1]+price[i];
        }
        nw.st=st;
        nw.t_depart[0]=0;
        for(int i=1;i<nw.sta_num;i++){
            nw.t_arrive[i]=nw.t_depart[i-1]+t_drive[i];
            if(i!=nw.sta_num-1) nw.t_depart[i]=nw.t_arrive[i]+t_stop[i];
        }
        nw.l=l;nw.l.x=st.x;nw.l.y=st.y;
        nw.r=r;nw.r.x=st.x;nw.r.y=st.y;
        nw.tp=tp;
        f.insert(_id,nw,n);
        printf("0\n");
    }
    void del_train(String _id){
        train nw;int pos;
        if(!f.find(_id,nw,pos)){
            printf("-1\n");
            return ;
        }
        if(nw.rele){
            printf("-1\n");
            return ;
        }
        f.del(_id,pos);
        printf("0\n");
    }
    void release(String _id){
        train nw;int pos;
        if(!f.find(_id,nw,pos)){
            printf("-1\n");
            return ;
        }
        if(nw.rele){
            printf("-1\n");
            return ;
        }
        nw.rele=true;
        f.updata(pos,nw);
        for(int i=0;i<nw.sta_num;i++){
            for(int j=i+1;j<nw.sta_num;j++){
                road.do_insert(sjtu::pair<String, String>(nw.sta[i],nw.sta[j]),pos);
            }
        }
        for(int i=0;i<nw.sta_num;i++){
            passby.do_insert(nw.sta[i],pos);
        }
        printf("0\n");
    }
    void query_train(String _id,date_time d){
        train nw;int pos;
        if(!f.find(_id,nw,pos)){
            printf("-1\n");
            return ;
        }
        if(nw.l>d||nw.r<d){
            printf("-1\n");
            return ;
        }
        std::cout<<nw.id<<' '<<nw.tp<<std::endl;
        date_time t=nw.st,cur;
        t.dx=d.dx;t.dy=d.dy;
        int day=count_day(nw.l,d);
        for(int i=0;i<nw.sta_num;i++){
            std::cout<<nw.sta[i]<<' ';
            if(i==0){
                std::cout<<"xx-xx xx:xx -> ";
            }
            else{
                cur=t;
                cur.y+=nw.t_arrive[i];
                cur.format();
                std::cout<<cur<<" -> ";
            }
            if(i==nw.sta_num-1){
                std::cout<<"xx-xx xx:xx ";
            }
            else{
                cur=t;
                cur.y+=nw.t_depart[i];
                cur.format();
                std::cout<<cur<<' ';
            }
            std::cout<<nw.price[i]<<' ';
            if(i==nw.sta_num-1){
                std::cout<<'x';
            }
            else std::cout<<nw.seat[day][i];
            std::cout<<std::endl;
        }
    }
    void clear(){
        n=0;
        f.clear();
        road.clear();
        passby.clear();
    }
};
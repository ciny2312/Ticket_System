#include<iostream>
#include"../file/clock.hpp"
struct train{
    static const int N=105;
    String id;
    int sta_num,seat_num,seat[N][N],price[N],t_arrive[N],t_depart[N],sta[N];
    date_time l,r,st;
    char tp;bool rele=false;
    String output(){
        return id;
    }
};
class train_system{
    friend class ticket;
    int n,sta_n;
    readwrite<String,train> f;
    readwrite<String,String> station;
    readwrite<sjtu::pair<int,int>,train> road;
    readwrite<int,train> passby;

    public:
    train_system():f("train.db","train_BPT.db"),
    station("station.db","station_BPT.db"),
    road("train.db","road_BPT.db"),
    passby("train.db","passby_BPT.db"){
        std::fstream ff;
        ff.open("train.db");
        ff.seekg(0);
        ff.read(reinterpret_cast<char*>(&n),sizeof(int));
        ff.close();
        ff.open("station.db");
        ff.seekg(0);
        ff.read(reinterpret_cast<char*>(&sta_n),sizeof(int));
        ff.close();
    }
    ~train_system(){
        std::fstream ff;
        ff.open("train.db");
        ff.seekp(0);
        ff.write(reinterpret_cast<char*>(&n),sizeof(int));
        ff.close();
        ff.open("station.db");
        ff.seekp(0);
        ff.write(reinterpret_cast<char*>(&sta_n),sizeof(int));
        ff.close();
    }
    String station_print(int pos){
        String val;
        station.ask_pos(pos,val);
        return val;
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
        for(int i=0;i<nw.sta_num;i++){
            for(int j=1;j<=tot;j++)
                nw.seat[j][i]=_seat;
        }
        for(int i=0;i<nw.sta_num;i++){
            int pos;String x;
            if(station.find(sta[i],x,pos)){
                nw.sta[i]=pos;
            }
            else{
                station.insert(sta[i],sta[i],sta_n);
                nw.sta[i]=sta_n;
            }
        }
        nw.price[0]=0;
        for(int i=1;i<nw.sta_num;i++){
            nw.price[i]=nw.price[i-1]+price[i];
        }
        nw.t_depart[0]=0;
        for(int i=1;i<nw.sta_num;i++){
            nw.t_arrive[i]=nw.t_depart[i-1]+t_drive[i];
            if(i!=nw.sta_num-1) nw.t_depart[i]=nw.t_arrive[i]+t_stop[i];
        }
        nw.st=st;nw.rele=false;
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
                road.do_insert(sjtu::pair<int,int>(nw.sta[i],nw.sta[j]),pos);
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
        nw.l.x=nw.l.y=nw.r.x=nw.r.y=0;
        if(nw.l>d||nw.r<d){
            printf("-1\n");
            return ;
        }
        std::cout<<nw.id<<' '<<nw.tp<<std::endl;
        date_time t=nw.st,cur;
        t.dx=d.dx;t.dy=d.dy;
        int day=count_day(nw.l,d);
        for(int i=0;i<nw.sta_num;i++){
            std::cout<<station_print(nw.sta[i])<<' ';
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
        n=0;sta_n=0;
        f.clear();
        road.clear();
        passby.clear();
        station.clear();
    }
};
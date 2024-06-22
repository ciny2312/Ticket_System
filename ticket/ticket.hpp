#include <fstream>
#include <iostream>
#include "../train/train.hpp"
extern train_system ts;
struct order{
    String username,id,from,to;
    date_time l,r;
    int price,num,status,from_n,to_n,day,pos;
    friend std::ostream& operator <<(std::ofstream &o,const order &x){
        if(x.status==-1){
            o<<"[refunded]";
        }
        else if(x.status==0){
            o<<"[pending]";
        }
        else{
            o<<"[success]";
        }
        o<<' '<<x.username<<' '<<x.from<<' '<<x.l<<' '<<" to "<<x.to<<' '<<x.r<<' '<<' '<<x.price<<' '<<x.num;
        return o;
    }
    String output(){
        return id;
    }
};
struct comp{
    int x,y;
    String id1,id2;
    bool operator < (const comp &oth)const{
        if(x!=oth.x) return x<oth.x;
        if(y!=oth.y) return y<oth.y;
        if(id1!=id2) return id1<oth.id1;
        return id2<oth.id2;
    }
    bool operator > (const comp &oth)const{
        if(x!=oth.x) return x>oth.x;
        if(y!=oth.y) return y>oth.y;
        if(id1!=id2) return id1>oth.id1;
        return id2>oth.id2;
    }
};
struct trans{
    String id1,id2;
    date_time a1,a2,b1,b2; 
    int s,price1,price2,num1,num2;
    bool operator < (const trans &oth)const{
        return id2<oth.id2;
    }
    bool operator > (const trans &oth)const{
        return id2>oth.id2;
    }
};
class ticket{
    int n;
    readwrite<String,order> f;
    readwrite<sjtu::pair<String,int>,order> waitqueue;
    public:
    ticket():f("ticket.db","orders_BPT.db"),waitqueue("ticket.db","queue_BPT.db"){
        std::fstream ff;
        ff.open("ticket.db");
        ff.seekg(0);
        ff.read(reinterpret_cast<char*>(&n),sizeof(int));
        ff.close();
    }
    ~ticket(){
        std::fstream ff;
        ff.open("ticket.db");
        ff.seekp(0);
        ff.write(reinterpret_cast<char*>(&n),sizeof(int));
        ff.close();
    }
    String station_print(int pos){
        String val;
        ts.station.ask_pos(pos,val);
        return val;
    }
    void buy_ticket(String user,String id,date_time d,int num,String from,String to,bool q=false){
        train nw;int pos;
        if(!ts.f.find(id,nw,pos)){
            printf("-1\n");
            return ;
        }
        if(!nw.rele){
            printf("-1\n");
            return ;
        }
        if(nw.seat_num<num){
            printf("-1\n");
            return ;
        }
        int from_x,to_y;
        if(!ts.station.find(from,from,from_x)){
            printf("-1\n");
            return ;
        }
        if(!ts.station.find(to,to,to_y)){
            printf("-1\n");
            return ;
        }
        int l=find_pos(nw,from_x),r=find_pos(nw,to_y);
        if(l==-1||r==-1||l>=r){
            printf("-1\n");
            return ;
        }
        date_time t1=nw.l,t2=nw.r;
        t1.y+=nw.t_depart[l];
        t2.y+=nw.t_depart[l];
        t1.format();t2.format();
        t1.x=t1.y=t2.x=t2.y=0;
        if(d<t1||t2<d){
            printf("-1\n");
            return ;
        }
        int day=count_day(t1,d);
        ///
        ///
        //
        int rest_num=ask_seat(nw,day,l,r);
        if(rest_num<num&&!q){
            printf("-1\n");
            return ;
        }
        order x;
        x.username=user;
        x.id=id;
        x.from=from;x.to=to;
        x.num=num;x.day=day;
        x.from_n=l;x.to_n=r;
        
        x.l=x.r=nw.l;
        x.l.y+=nw.t_depart[l];
        x.r.y+=nw.t_arrive[r];
        x.l.format();x.r.format();
        int ex=count_day(x.l,d)-1;
        x.l.dy+=ex;x.r.dy+=ex;
        x.l.format();x.r.format();

        x.price=nw.price[r]-nw.price[l];
        if(rest_num>=num){
            for(int i=l;i<r;i++){
                nw.seat[day][i]-=num;
            }
            ts.f.updata(pos,nw);
            x.status=1;x.pos=n+1;
            f.insert(user,x,n);
            printf("%d\n",x.price*num);
            return ;
        }
        x.status=0;x.pos=n+1;
        f.insert(user,x,n);
        waitqueue.do_insert(sjtu::pair<String,int>(id,day),n);
        printf("queue\n");
    }
    void query_order(String user){
        sjtu::vector<int> ans;
        f.getall(user,ans);
        std::cout<<ans.size()<<std::endl;
        for(int i=ans.size()-1;i>=0;i--){
            order x;f.ask_pos(ans[i],x);
            if(x.status==-1){
                std::cout<<"[refunded]";
            }
            else if(x.status==0){
                std::cout<<"[pending]";
            }
            else{
                std::cout<<"[success]";
            }
            std::cout<<' '<<x.id<<' '<<x.from<<' '<<x.l<<" -> "
            <<x.to<<' '<<x.r<<' '<<x.price<<' '<<x.num<<std::endl;
        }
    }
    void query_ticket(String s,String t,date_time d,bool type=true){
        int s_x,t_y;
        if(!ts.station.find(s,s,s_x)){
            printf("0\n");
            return ;
        }
        if(!ts.station.find(t,t,t_y)){
            printf("0\n");
            return ;
        }
        sjtu::pair<int,int>key(s_x,t_y);
        sjtu::vector<int> ans;
        ts.road.getall(key,ans);
        sjtu::priority_queue<sjtu::pair<int,sjtu::pair<String,int> > > q;
        for(int p=0;p<ans.size();p++){
            train x;ts.f.ask_pos(ans[p],x);
            int i=find_pos(x,s_x);
            date_time t1=x.l,t2=x.r;
            t1.y+=x.t_depart[i];
            t2.y+=x.t_depart[i];
            t1.format();t2.format();
            t1.x=t1.y=t2.x=t2.y=0;
            if(d<t1||t2<d) continue;
            sjtu::pair<String,int>key(x.id,p);
            int j=find_pos(x,t_y);
            if(type){
                int tim=x.t_arrive[j]-x.t_depart[i];
                q.push(sjtu::pair<int,sjtu::pair<String,int> >(tim,key));
            }
            else{
                int cos=x.price[j]-x.price[i];
                q.push(sjtu::pair<int,sjtu::pair<String,int> >(cos,key));
            }
        }
        std::cout<<q.size()<<std::endl;
        while(!q.empty()){
            int i=q.top().second.second;q.pop();
            train x;ts.f.ask_pos(ans[i],x);
            date_time t1,t2;
            int l=find_pos(x,s_x),r=find_pos(x,t_y);
            t1=x.l;t1.y+=x.t_depart[l];t1.format();
            t2=x.l;t2.y+=x.t_arrive[r];t2.format();
            int ex=count_day(t1,d)-1;
            t1.dy+=ex;t1.format();
            t2.dy+=ex;t2.format();
            std::cout<<x.id<<' '<<s<<' '<<t1<<" -> "<<t<<' '<<t2<<' '<<x.price[r]-x.price[l]<<' ';
            std::cout<<ask_seat(x,ex+1,l,r)<<std::endl;
        }
    }
    void query_transfer(String s,String t,date_time d,bool type=true){
        int s_x,t_y;
        if(!ts.station.find(s,s,s_x)){
            printf("0\n");
            return ;
        }
        if(!ts.station.find(t,t,t_y)){
            printf("0\n");
            return ;
        }
        sjtu::vector<int>ans;
        ts.passby.getall(s_x,ans);
        sjtu::priority_queue<sjtu::pair<comp,trans> >q;
        for(int i=0;i<ans.size();i++){
            train x;ts.f.ask_pos(ans[i],x);
            int si=find_pos(x,s_x);

            date_time t1=x.l,t2=x.r;
            t1.y+=x.t_depart[si];
            t2.y+=x.t_depart[si];
            t1.format();t2.format();
            t1.x=t1.y=t2.x=t2.y=0;
            if(d<t1||t2<d) continue;

            t1=x.l;t1.y+=x.t_depart[si];t1.format();
            int ex=count_day(t1,d)-1;
            t1.dx=d.dx;t1.dy=d.dy;

            for(int ti=si+1;ti<x.sta_num;ti++){
                t2=x.l;t2.y+=x.t_arrive[ti];t2.dy+=ex;t2.format();

                sjtu::pair<int, int> key(x.sta[ti],t_y);
                sjtu::vector<int> ans2;
                ts.road.getall(key,ans2);
                for(int j=0;j<ans2.size();j++){
                    train y;ts.f.ask_pos(ans2[j],y);
                    if(x.id==y.id) continue;
                    int ssi=find_pos(y,x.sta[ti]),tti=find_pos(y,t_y);
                    date_time b1=y.r;b1.y+=y.t_depart[ssi];b1.format();
                    if(b1<t2){
                        continue;
                    }
                    b1=y.l;b1.y+=y.t_depart[ssi];b1.format();
                    int ex2=0;
                    if(b1<t2){
                        int day=count_day(b1,t2)-1;
                        b1.dy+=day;b1.format();
                        ex2+=day;
                        if(b1<t2){
                            b1.dy++;ex2++;
                            b1.format();
                        }
                    }

                    date_time b2=y.l;b2.y+=y.t_arrive[tti];b2.format();
                    b2.dy+=ex2;b2.format();
                    comp key;trans val;
                    val.price1=x.price[ti]-x.price[si];
                    val.price2=y.price[tti]-y.price[ssi];
                    val.s=x.sta[ti];
                    val.a1=t1;
                    val.a2=t2;
                    val.b1=b1;
                    val.b2=b2;
                    val.num1=ask_seat(x,ex+1,si,ti);
                    val.num2=ask_seat(y,ex2+1,ssi,tti);
                    val.id1=x.id;val.id2=y.id;
                    key.id1=x.id;key.id2=y.id;
                    key.x=val.price1+val.price2;
                    key.y=y.t_arrive[tti]-y.t_depart[ssi]+x.t_arrive[ti]-x.t_depart[si]+ask_num(t2,b1);
                    if(type){
                        std::swap(key.x,key.y);
                    }
                    q.push(sjtu::pair<comp,trans>(key,val));

                }
            }
        }
        if(q.size()==0){
            printf("0\n");
            return ;
        }
        trans x=q.top().second;
        std::cout<<x.id1<<' '<<s<<' '<<x.a1<<" -> "<<station_print(x.s)<<' '<<x.a2<<' '<<x.price1<<' '<<x.num1<<std::endl;
        std::cout<<x.id2<<' '<<station_print(x.s)<<' '<<x.b1<<" -> "<<t<<' '<<x.b2<<' '<<x.price2<<' '<<x.num2<<std::endl;
    }
    void refund_ticket(String user,int num){
        sjtu::vector<int> ans;
        f.getall(user,ans);
        if(ans.size()<num){
            printf("-1\n");
            return ;
        }
        order nw;
        f.ask_pos(ans[ans.size()-num],nw);
        if(nw.status==-1){
            printf("-1\n");
            return ;
        }
        if(nw.status==1){
            nw.status=-1;
            train p;int pos;
            ts.f.find(nw.id,p,pos);
            for(int i=nw.from_n;i<nw.to_n;i++){
                p.seat[nw.day][i]+=nw.num;
            }
            sjtu::vector<int>queue;
            sjtu::pair<String,int> key(nw.id,nw.day);
            waitqueue.getall(key,queue);
            for(int i=0;i<queue.size();i++){
                order x;f.ask_pos(queue[i],x);
                if(x.from_n>nw.to_n||x.to_n<nw.from_n){
                    continue;
                }
                if(ask_seat(p,x.day,x.from_n,x.to_n)>=x.num){
                    for(int j=x.from_n;j<x.to_n;j++){
                        p.seat[x.day][j]-=x.num;
                    }
                    x.status=1;
                    f.updata(x.pos,x);
                    waitqueue.del(key,x.pos);
                }
            }
            f.updata(nw.pos,nw);
            ts.f.updata(pos,p);
            printf("0\n");
            return ;
        }
        nw.status=-1;
        sjtu::pair<String,int> key(nw.id,nw.day);
        waitqueue.del(key,nw.pos);
        f.updata(nw.pos,nw);
        printf("0\n");
    }
    int find_pos(train &t,int s){
        for(int i=0;i<t.sta_num;i++){
            if(t.sta[i]==s){
                return i;
            }
        }
        return -1;
    }
    int ask_seat(train &t,int day,int l,int r){
        int ans=1e9;
        for(int i=l;i<r;i++){
            ans=std::min(ans,t.seat[day][i]);
        }
        return ans;
    }
    void clear(){
        f.clear();
        waitqueue.clear();
        n=0;
    }
};
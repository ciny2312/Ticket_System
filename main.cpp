#include"users/users.hpp"
#include <fstream>
#include <sstream>
user_system us;
train_system ts;
ticket t;
int main(){
//    freopen("3.in","r",stdin);
//    freopen("a.out","w",stdout);
    
    bool flag;
    std::string line,time_stamp,s;
    while(getline(std::cin, line)){
        std::stringstream ss;
        ss.clear();
        ss << line;
        ss >> time_stamp;flag=time_stamp=="[1910288]";
        std::cout << time_stamp << " ";
        ss >> s;
        if(s=="exit"){
            printf("bye\n");
            break;
        }
        else if(s=="clean"){
            us.clear();
            ts.clear();
            t.clear();
        }
        else if(s=="add_user"){
            std::string tmp, cur_username, username, password, name, mailAddr;
            int privilege = -1;
            while (ss >> tmp) {
                switch (tmp[1]) {
                    case 'c': {
                        ss >> cur_username;
                        break;
                    }
                    case 'u': {
                        ss >> username;
                        break;
                    }
                    case 'p': {
                        ss >> password;
                        break;
                    }
                    case 'n': {
                        ss >> name;
                        break;
                    }
                    case 'm': {
                        ss >> mailAddr;
                        break;
                    }
                    case 'g': {
                        ss >> privilege;
                        break;
                    }
                }
            }
            us.add_user(cur_username, username, password, name, mailAddr, privilege);
        }
        else if(s=="login"){
            std::string tmp, username, password;
            while(ss >> tmp) {
                switch (tmp[1]) {
                    case 'u': {
                        ss >> username;
                        break;
                    }
                    case 'p': {
                        ss >> password;
                        break;
                    }
                }
            }
            us.log_in(username, password);
        }
        else if(s=="logout"){
            std::string tmp, username;
            ss >> tmp >> username;
            us.logout(username);
        }
        else if(s=="query_profile"){
            std::string tmp, cur_username, username;
            while(ss >> tmp){
                switch (tmp[1]) {
                    case 'c': {
                        ss >> cur_username;
                        break;
                    }
                    case 'u': {
                        ss >> username;
                        break;
                    }
                }
            }
            us.query_profile(cur_username, username);

        }
        else if(s=="modify_profile"){
            std::string tmp, cur_username, username, password, name, mailAddr;
            int privilege = -1;
            while (ss >> tmp) {
                switch (tmp[1]) {
                    case 'c': {
                        ss >> cur_username;
                        break;
                    }
                    case 'u': {
                        ss >> username;
                        break;
                    }
                    case 'p': {
                        ss >> password;
                        break;
                    }
                    case 'n': {
                        ss >> name;
                        break;
                    }
                    case 'm': {
                        ss >> mailAddr;
                        break;
                    }
                    case 'g': {
                        ss >> privilege;
                        break;
                    }
                }
            }
            us.modify_profile(cur_username, username, password, !password.empty(), name, !name.empty(), mailAddr, !mailAddr.empty(), privilege);
        }
        else if(s=="add_train"){
            std::string tmp, trainID, stations, prices, startTime, travelTimes, stopoverTimes, saleDate;
            int stationNum, seatNum,t_drive[105],t_stop[105];
            char _type;
            String sta[105];
            int price[105];
            date_time saleL,saleR,ST;
            while(ss >> tmp) {
                switch (tmp[1]) {
                    case 'i': {
                        ss >> trainID;
                        break;
                    }
                    case 'n': {
                        ss >> stationNum;
                        break;
                    }
                    case 'm': {
                        ss >> seatNum;
                        break;
                    }
                    case 's': {
                        ss >> stations;
                        std::string ch="";int tot=0;
                        for(int i=0;i<stations.length();i++){
                            if(stations[i]=='|'){
                                sta[tot++]=ch;
                                ch="";
                            }
                            else ch+=stations[i];
                        }
                        sta[tot++]=ch;
                        break;
                    }
                    case 'p': {
                        ss >> prices;
                        int ans=0;int tot=0;
                        for(int i=0;i<prices.length();i++){
                            if(prices[i]=='|'){
                                price[++tot]=ans;
                                ans=0;
                            }
                            else ans=ans*10+prices[i]-'0';
                        }
                        price[++tot]=ans;
                        break;
                    }
                    case 'x': {
                        ss >> startTime;
                        ST.x=(startTime[0]-'0')*10+startTime[1]-'0';
                        ST.y=(startTime[3]-'0')*10+startTime[4]-'0';
                        break;
                    }
                    case 't': {
                        ss >> travelTimes;
                        int ans=0;int tot=0;
                        for(int i=0;i<travelTimes.length();i++){
                            if(travelTimes[i]=='|'){
                                t_drive[++tot]=ans;
                                ans=0;
                            }
                            else ans=ans*10+travelTimes[i]-'0';
                        }
                        t_drive[++tot]=ans;
                        break;
                    }
                    case 'o': {
                        ss >> stopoverTimes;
                        int ans=0;int tot=0;
                        for(int i=0;i<stopoverTimes.length();i++){
                            if(stopoverTimes[i]=='|'){
                                t_stop[++tot]=ans;
                                ans=0;
                            }
                            else ans=ans*10+stopoverTimes[i]-'0';
                        }
                        t_stop[++tot]=ans;
                        break;
                    }
                    case 'd': {
                        ss >> saleDate;
                        saleL.dx=(saleDate[0]-'0')*10+saleDate[1]-'0';
                        saleL.dy=(saleDate[3]-'0')*10+saleDate[4]-'0';
                        saleR.dx=(saleDate[6]-'0')*10+saleDate[7]-'0';
                        saleR.dy=(saleDate[9]-'0')*10+saleDate[10]-'0';
                        break;
                    }
                    case 'y': {
                        ss >> _type;
                        break;
                    }
                }
            }
            ts.add_train(trainID, stationNum, seatNum, sta, price, ST, t_drive, t_stop, saleL,saleR, _type);
     
        }
        else if(s=="delete_train"){
            std::string tmp, trainID;
            ss >> tmp >> trainID;
            ts.del_train(trainID);
        }
        else if(s=="release_train"){
            std::string tmp, trainID;
            ss >> tmp >> trainID;
            ts.release(trainID);
        }
        else if(s=="query_train"){
            std::string tmp, trainID, date;
            date_time d;
            while(ss >> tmp) {
                switch (tmp[1]) {
                    case 'i': {
                        ss >> trainID;
                        break;
                    }
                    case 'd': {
                        ss >> date;
                        d.dx=(date[0]-'0')*10+date[1]-'0';
                        d.dy=(date[3]-'0')*10+date[4]-'0';
                        d.x=d.y=0;
                        break;
                    }
                }
            }
            ts.query_train(trainID, d);

        }
        else if(s=="query_ticket"){
            std::string tmp, date, from, to;
            bool _type = true;
            date_time d;
            while (ss >> tmp) {
                switch (tmp[1]) {
                    case 'd': {
                        ss >> date;
                        d.dx=(date[0]-'0')*10+date[1]-'0';
                        d.dy=(date[3]-'0')*10+date[4]-'0';
                        d.x=d.y=0;
                        break;
                    }
                    case 's': {
                        ss >> from;
                        break;
                    }
                    case 't': {
                        ss >> to;
                        break;
                    }
                    case 'p': {
                        ss >> tmp;
                        if (tmp == "time") {
                            _type = true;
                        }
                        else if (tmp == "cost") {
                            _type = false;
                        }
                        break;
                    }
                }
            }
            t.query_ticket(from, to, d, _type);
        }
        else if(s=="query_transfer"){
            std::string tmp, date, from, to;
            bool _type = true;
            date_time d;
            while (ss >> tmp) {
                switch (tmp[1]) {
                    case 'd': {
                        ss >> date;
                        d.dx=(date[0]-'0')*10+date[1]-'0';
                        d.dy=(date[3]-'0')*10+date[4]-'0';
                        d.x=d.y=0;
                        break;
                    }
                    case 's': {
                        ss >> from;
                        break;
                    }
                    case 't': {
                        ss >> to;
                        break;
                    }
                    case 'p': {
                        ss >> tmp;
                        if (tmp == "time") {
                            _type = true;
                        }
                        else if (tmp == "cost") {
                            _type = false;
                        }
                        break;
                    }
                }
            }
            t.query_transfer(from, to, d, _type);
        }
        else if(s=="buy_ticket"){
            std::string tmp, username, trainID, date, from, to;
            int num; bool que = false;
            date_time d;
            while (ss >> tmp) {
                switch (tmp[1]) {
                    case 'u': {
                        ss >> username;
                        break;
                    }
                    case 'i': {
                        ss >> trainID;
                        break;
                    }
                    case 'd': {
                        ss >> date;
                        d.dx=(date[0]-'0')*10+date[1]-'0';
                        d.dy=(date[3]-'0')*10+date[4]-'0';
                        d.x=d.y=0;
                        break;
                    }
                    case 'n': {
                        ss >> num;
                        break;
                    }
                    case 'f': {
                        ss >> from;
                        break;
                    }
                    case 't': {
                        ss >> to;
                        break;
                    }
                    case 'q': {
                        ss >> tmp;
                        if (tmp == "true") {
                            que = true;
                        }
                        else if (tmp == "false") {
                            que = false;
                        }
                        break;
                    }
                }
            }
            us.buy_ticket(username, trainID, d, num, from, to, que);
        }
        else if(s=="query_order"){
            std::string tmp, username;
            ss >> tmp >> username;
            us.query_order(username);
        }
        else if(s=="refund_ticket"){
            std::string tmp, username;
            int num;
            while (ss >> tmp) {
                switch (tmp[1]) {
                    case 'u': {
                        ss >> username;
                        break;
                    }
                    case 'n': {
                        ss >> num;
                        break;
                    }
                }
            }
            us.refund_ticket(username, num);
        }
    }
    return 0;
}
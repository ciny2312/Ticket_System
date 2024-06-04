#include <iostream>
#include "file.hpp"
struct date_time{
    int x,y;
    int dx,dy;
    void format(){
        if(y>=60){
            x+=y/60;
            y%=60;
        }
        if(x>=24){
            dy+=x/24;
            x%=24;
        }
        if(dx==6&&dy>30){
            dx++;
            dy-=30;
        }
        if(dx==7&&dy>31){
            dx++;
            dy-=31;
        }
        if(dx==8&&dy>31){
            dx++;
            dy-=31;
        }
    }
    bool operator < (const date_time &oth){
        if(dx!=oth.dx){
            return dx<oth.dx;
        }
        if(dy!=oth.dy){
            return dy<oth.dy;
        }
        if(x!=oth.x){
            return x<oth.x;
        }
        return y<oth.y;
    }
    bool operator > (const date_time &oth){
        if(dx!=oth.dx){
            return dx>oth.dx;
        }
        if(dy!=oth.dy){
            return dy>oth.dy;
        }
        if(x!=oth.x){
            return x>oth.x;
        }
        return y>oth.y;
    }
    bool operator <= (const date_time &oth){
        return !(*this>oth);
    }
    bool operator >= (const date_time &oth){
        return !(*this<oth);
    }
    date_time operator + (const date_time &a){
        date_time ans;
        ans.x=x+a.x;
        ans.y=y+a.y;
        ans.dx=dx+a.dx;
        ans.dy=dy+a.dy;
        ans.format();
        return ans;
    }
    friend std::ostream& operator << (std::ostream &o,const date_time &a){
        if(a.dx<10){
            o<<0;
        }
        o<<a.dx<<'-';
        if(a.dy<10){
            o<<0;
        }
        o<<a.dy<<' ';
        if(a.x<10){
            o<<0;
        }
        o<<a.x<<':';
        if(a.y<10){
            o<<0;
        }
        o<<a.y;
        return o;
    }
    friend int count_day(date_time x,date_time y){
        while(y.dx>x.dx){
            if(y.dx==9){
                y.dx--;
                y.dy+=31;
            }
            if(y.dx==8){
                y.dx--;
                y.dy+=31;
            }
            else if(y.dx==7){
                y.dx--;
                y.dy+=30;
            }
        }
        return y.dy-x.dy+1;
    }
    friend int ask_num(date_time x,date_time y){
        while(y.dx>x.dx){
            if(y.dx==8){
                y.dx--;
                y.dy+=31;
            }
            if(y.dx==7){
                y.dx--;
                y.dy+=30;
            }
        }
        if(y.dy>x.dy){
            y.x+=(y.dy-x.dy)*24;
            y.dy=x.dy;
        }
        if(y.x>x.x){
            y.y+=(y.x-x.x)*60;
            y.x=x.x;
        }
        return y.y-x.y;
    }
};
class road{
    String from,to;
    public:
    road(String _from,String _to):from(_from),to(_to){}
    bool operator < (const road &oth){
        if(from==oth.from){
            return from<oth.from;
        }
        return to<oth.to;
    }
    bool operator > (const road &oth){
        if(from==oth.from){
            return from<oth.from;
        }
        return to<oth.to;
    }
    bool operator <= (const road &oth){
        return !(*this>oth);
    }
    bool operator >= (const road &oth){
        return !(*this<oth);
    }

};
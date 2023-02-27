#ifndef CARD_H
#define CARD_H

#include "config.h"
#include "mypushbutton.h"
#include <QProgressBar>




class TowerCard
{
    friend class Showtable;
    enum Towertypes
    {
        magictower=0,
        armytower=1
    };
public:
    TowerCard(QString path,int type,int _x,int _y);
    ~TowerCard();
    void show();
    TowerCard(const TowerCard &t);
    void change_condition(int score);//刷新改变状态
    void reset(){time_count=0;}
    int get_type(){return type;}
private:
    QString path;
    mypushbutton * btn=nullptr;
    QProgressBar  qpr;
    //放置位置和类型
    int x;
    int y;
    int type;
    int state;//状态机的状态

    int time_count;
    int time_interval;

};

#endif // CARD_H

#include "card.h"

TowerCard::TowerCard(QString path,int type,int _x,int _y)
{
    x=_x;y=_y;
    this->type=type;
    this->path=path;


    btn=new mypushbutton(this->path);
    btn->setGeometry(x,y,50,50);

    qpr.setOrientation(Qt::Horizontal);  // 水平方向
    qpr.setMinimum(0);  // 最小值
    qpr.setMaximum(MAX_CD_TIME);  // 最大值
    qpr.setValue(MAX_CD_TIME);  // 当前进度
    qpr.setGeometry(x-10,y,50,50);

    time_interval=MAX_CD_TIME;
    time_count=MAX_CD_TIME;

}
TowerCard::TowerCard(const TowerCard &t)
{
    x=t.x;y=t.y;
    this->type=t.type;
    this->path=t.path;

    btn=new mypushbutton(this->path);
    btn->setGeometry(x,y,50,50);

    qpr.setOrientation(Qt::Horizontal);  // 水平方向
    qpr.setMinimum(0);  // 最小值
    qpr.setMaximum(MAX_CD_TIME);  // 最大值
    qpr.setValue(MAX_CD_TIME);  // 当前进度
    qpr.setGeometry(x-50,y,50,50);
}
void TowerCard::show()
{
    qpr.lower();
    btn->show();
    qpr.show();
}
TowerCard::~TowerCard()
{
   delete btn;
}
void TowerCard::change_condition(int score)
{
    int money=0;
    int increase;
    switch(type)
    {
        case magictower:
                money=100;
                increase=3;
                break;
        case armytower:
                money=70;
                increase=4;
                break;
    }


    if(score>=money&&time_count==time_interval)
        btn->setEnabled(true);
    else
        btn->setEnabled(false);

    if(time_count<time_interval)
    {
           if(time_count+increase>time_interval)
               time_count=time_interval;
           else
               time_count+=increase;
           double rate=(double)time_count/(double)time_interval;
           qpr.setValue(MAX_CD_TIME*rate);
           qpr.show();
    }

}

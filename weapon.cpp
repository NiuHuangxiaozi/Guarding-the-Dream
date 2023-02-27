#include "weapon.h"

weapon::weapon(int from_px,int from_py,\
       int ack,int speed)
{
    this->from_px=from_px;
    this->from_py=from_py;
    px=from_px;
    py=from_py;
    this->ack=ack;
    this->speed=speed;
}


hostle::hostle(int from_px,int from_py,\
       int ack,int speed,QWidget *parent):
    weapon(from_px+10,from_py+40,\
           ack,speed)
{
    //预存爆炸动画
    bost.setFileName(HOSTLE_BOOM_PATH);

    QPixmap pix;
    pix.load(HOSTLE_PATH);
    label.setPixmap(pix);
    label.setGeometry(from_py+40,from_px+10,20,20);
    label.setParent(parent);
    label.show();


}
void hostle::change_xy()
{
    double rate= (double)abs(px-to_px)/(double)abs(py-to_py);
    //qDebug()<<"px-topx"<<px<<"-"<<to_px;
    //qDebug()<<"py-topy"<<py<<"-"<<to_py;
    double inc_x=speed*rate;
    double inc_y=speed;
    //qDebug()<<"intx"<<inc_x;
    //qDebug()<<"inty"<<inc_y;
    if(to_px<=px&&to_py<=py)
    {
        if(px-inc_x<to_px)
              px=to_px;
        if(py-inc_y<to_py)
              py=to_py;
        else
        {
            px-=inc_x;
            py-=inc_y;
            //qDebug()<<"px "<<px<<"py "<<py;
        }
     }
    else if(to_px>=px&&to_py>=py)
    {
        if(px+inc_x>to_px)
              px=to_px;
        if(py+inc_y>to_py)
              py=to_py;
        else
        {
            px+=inc_x;
            py+=inc_y;
        }

    }
    else if(to_px>=px&&to_py<=py)
    {
        if(px+inc_x>to_px)
              px=to_px;
        if(py-inc_y<to_py)
              py=to_py;
        else
        {
            px+=inc_x;
            py-=inc_y;
        }

    }
    else if(to_px<=px&&to_py>=py)
    {
        if(px-inc_x<to_px)
              px=to_px;
        if(py+inc_y>to_py)
              py=to_py;
        else
        {
            px-=inc_x;
            py+=inc_y;
        }
    }
}
void hostle::running(int _px,int _py)
{
    if(px==-1)//失去目标立即爆炸
    {
        bost.start();
        label.setGeometry(py,px-30,50,70);
        label.setMovie(&bost);
        state=1;
    }
    if(px!=-1)
    {
         //初始化
         this->to_px=_px+50;
         this->to_py=_py+50;
    }
    //qDebug()<<"px :"<<px;
    //qDebug()<<"_px :"<<_px;
    //qDebug()<<"initial py :"<<py;
    //qDebug()<<"initial to_py :"<<to_py;
    if(px>=to_px-50&&px<=to_px&&py>=to_py-50&&py<=to_py)
    {
        //qDebug()<<"change 1 ";
        bost.start();
        //qDebug()<<"px :"<<to_px;
        //qDebug()<<"py :"<<to_py;
        label.setGeometry(to_py-30,to_px-30,50,70);
        label.setMovie(&bost);
        state=1;
    }
    else
    {
        change_xy();
        label.setGeometry(py,px,20,20);
    }
}
void hostle::showpic(int _px,int _py)
{
    if(state==0)//正常运行
    {
        running(_px,_py);
    }
    else if(state==1)//死亡动画
    {
        //qDebug()<<"die ";
      if(bost.currentFrameNumber()+1==bost.frameCount())
      {
             is_die=true;
             state=2;
      }
    }
    else if(state==2)//等待消亡
    {

    }
}

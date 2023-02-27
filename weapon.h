#ifndef WEAPON_H
#define WEAPON_H


#include"config.h"
#include <QLabel>
#include <QMovie>
class weapon
{
public:
    weapon(int from_px,int from_py,\
           int ack,int speed);
    virtual void showpic(int px, int py){};
    virtual ~weapon(){};
    bool whether_die(){return is_die;}
    int get_ack(){return ack;}
    //
    void set_group_ack()
    {
        is_group_ack=true;
    }
    void set_ice_ack()
    {
        is_ice_ack=true;
    }
    void set_blood_ack()
    {
        is_blood_ack=true;
    }
    //
    bool is_group(){return is_group_ack;}
    bool is_ice(){return is_ice_ack;}
    bool is_blood(){return is_blood_ack==true;}
protected:
    int from_px;
    int from_py;

    int to_px;
    int to_py;

    int px;
    int py;

    int ack;

    int speed;

    int state=0;//状态机编程


    bool is_die=false;

    bool is_group_ack=false;//判断是否群攻
    bool is_ice_ack=false;//判断是否有冰冻攻击
    bool is_blood_ack=false;//判断是否有流血攻击
};


class hostle:public weapon
{
private:
    QMovie bost;
    QLabel label;


    //状态机函数
    void running(int _px,int _py);
    void change_xy();


public:
     hostle(int from_px,int from_py,\
            int ack,int speed,QWidget *parent);
     void showpic(int px, int py);
};

#endif // WEAPON_H

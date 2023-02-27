#ifndef ENEMY_H
#define ENEMY_H

#include <QWidget>
#include <QMovie>
#include <QPixmap>
#include "config.h"
#include <QLabel>
#include <QPainter>
#include <QProgressBar>
#include <vector>

enum enemytype{
   boar=0
};
enum affixes
{
    fast=0,//加速的
    flash=1//闪现的
};


class Enemy
{
    friend class Showtable;
public:
    Enemy(int x,int y,int px,int py,int ack,
         int speed,int type,int max_hp,int dir,
         int route_num,int max_block_num,QWidget *parent = nullptr);
    virtual ~Enemy(){};
    void Get_position(int &block_num,int & route_index,int & x,int & y,int & px,int & py,int &dir,int &speed);
    void Set_position(int block_num,int x,int y,int px,int py,int dir);
    virtual void showenemy(QWidget *parent){};
    bool is_next(){return block_walk_num==100;}
    void reset_next(){ block_walk_num=0;}
    bool is_achieved(){return achieved;}
    virtual void painthp(QPainter & mypainter){};


    int get_block_num(){return block_num;}
    int get_block_num_walk(){return block_walk_num;}
    int get_max_block_num(){return max_block_num;}

    //获取逻辑坐标
    std::vector<int> get_logical_xy();
    //获取物理坐标
    std::vector<int> get_physical_xy();

    void get_acked(int value);//减hp

    bool whether_die(){return is_die==true;}//是否死亡

    void set_is_meet_tower(bool b){meet_tower=b;}//是否遇到植物

    virtual bool interval_increase(){return true;}//攻击cd

    //设置是否被寒冰攻击
    virtual void set_ice_acked(bool flag){};
    //设置收到毒箭攻击
    void set_blood_acked(bool flag){is_blood_acked=flag;}

    //下面是一些是否含有次项词缀的函数
    bool has_fast(){return affix_types[0]==1;}
    bool has_flash(){return affix_types[1]==1;}








    //获取塔防词缀
    int get_tower_affix(){return tower_affix_type;}

protected:
    enum dirtype{
       east=1,
       west=2,
       south=3,
       north=4
    };


   int x;
   int y;


   int px;
   int py;

   int ack;
   int speed;

   int type;


   int max_hp;

   int hp;

   int dir;

   int route_num;//表示走哪条路线

   int block_num;//表示目前在哪一个格子

   int max_block_num;//表示这一条线路有多少个格子

   int block_walk_num=0;//表示一个格子是否走完了

   bool is_change_dir=false;//表示方向是否发生了改变

   bool achieved=false;//判断是否到达了终点

   QProgressBar qpr;

   bool is_die=false;

   bool meet_tower=false;//判断是否遇到了塔防

   //敌人的词缀数组（代表的是没有词缀）
   std::vector<int> affix_types;

   QLabel affixshow;//以文字的形式展示类型

   int tower_affix_type;//塔防的词缀，用于死亡时掉落产生


   bool is_ice_acked=false;//判断是否遇到了寒冰攻击
   int ice_count;
   int ice_interval;

   bool is_blood_acked=false;//判断是否收到了毒箭攻击
   //流血显示的计时
   int blood_count;
   int blood_interval;


signals:


};



class  Boar:public Enemy
{


public:
    Boar(int x,int y,int px,int py,int ack,int speed,int type,
         int max_hp,int dir,int route_num,int max_block_num,QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);
    void showenemy(QWidget *parent);
    void painthp(QPainter & painter);
    bool interval_increase();
    void showimage();
    void set_ice_acked(bool flag);

private:
    //vars
    QMovie  movie_walk;
    QMovie  movie_walk_left;
    QMovie  movie_up;
    QMovie  movie_attack;
    QMovie  movie_left_attack;
    QMovie  movie_die;
    QMovie  movie_die_left;
    QLabel label;

    //QMovie  movie_walk_ice;
    //QMovie  movie_walk_left_ice;
    //QMovie  movie_up_ice;
    //QMovie  movie_attack_ice;
    //QMovie  movie_left_attack_ice;
    //QMovie  movie_die_ice;
    //QMovie  movie_die_left_ice;

    QLabel blood_label;
    QLabel ice_label;
    int walk_count;
    int walk_interval;

    int state=0;

    int attack_count;
    int attack_inteval;


    //关于词缀的一些变量
    int flash_interval;
    int flash_count;


     //流血的状态
    int blood_state=0;

    //冰冻的状态
    int ice_state=0;









    //func
    void move_head(QWidget *parent);
    void move_forward(int dis);//根据方向调整前进方向

    void change_direction(QWidget *parent);

    void change_hp();

    void skill_cd();//技能的一些cd更新
    void reset_flash();
    bool is_flash();

    void close_all_movie();//关闭所有gif

    void ice_time_manage();//被减速后持续的时间

    //改变图像的一些列函数
    void change_to_die();
    void change_to_attack();
    void check_death();//检查死亡动画是否播放完毕
    void change();//状态图像转化
    void change_to_walk();//变为行走

    void normal_action(QWidget *parent);//正常的行为
    void ice_action(QWidget * parent);//冰雪状态行为
    void blood_action(QWidget * parent);//不断流血的行为

};

#endif // ENEMY_H

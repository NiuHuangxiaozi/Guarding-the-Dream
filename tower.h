#ifndef TOWER_H
#define TOWER_H

#include "config.h"
#include <QLabel>
#include <QPainter>
#include <QMovie>

//塔的类型
enum Towertypes
{
    magictower=0,
    armytower=1
};

class Tower
{
    friend class Showtable;
    enum affixtype{
        blood=0,
        ice=1,
        rampage=2,
        groupinjury=3
    };
public:
    Tower(int x,int y,int type ,int px,int py,QWidget *father);
    virtual void show(){qDebug()<<"bb";};
    virtual ~Tower(){};
    bool is_in_range(int x,int y);
    virtual void interval_increase(){};
    bool is_fire();
    void set_fire();

    void get_acked(int value);
    int get_ack();//获得攻击力

    int get_state(){return state;};

    void set_state(int s){state=s;}

    bool isattacking(){return isattack;}
    void set_isattacking(bool b){isattack=b;}

    bool whether_die(){return is_die;}

    std::vector<int> get_affixes(){return affixes;}

    void showrange();
    void hiderange();

    virtual void levelup(){};//升级的函数
    virtual void showimage(){};//展示升级后的图像

    void set_affix(int index,int affix_index);//添加相应的词缀
    bool has_affix(int loc);//判断相应位置相应的词缀是否存在

    //判断词缀是否存在
    //狂暴词缀
    int rampage_num();
    int group_ack_num();
    int ice_num();
    int blood_num();

protected:
    int x;
    int y;
    int ack;
    int range;
    int type;

    int px;
    int py;


    int shooter_count;
    int shooter_interval;

    int has_fire=false;//判断是否发射

    int state;
    bool isattack;

    int hp;
    int max_hp;

    bool is_die;

    std::vector<int>affixes;//塔防拥有的词缀

    //塔防的范围
    QLabel round;

    //塔防的等级
    int level;
};


class magicTower :public Tower
{
public:
  magicTower(int x,int y,int type,int px,int py,QWidget *father);
  ~magicTower(){};
  void show();
  void interval_increase();
  void showimage();//展示升级后的图像
  void levelup();//升级的函数
private:
  QLabel label;
  QMovie movie;
  QMovie movie_level2;
  QMovie movie_level3;
  QPixmap pix;

};
class armyTower :public Tower
{
public:
  armyTower(int x,int y,int type,int px,int py,QWidget *father);
  ~armyTower(){};
  void show();
  void set_state(int s);
  void interval_increase();
  void levelup();//升级的函数
  void showimage();//展示升级后的图像
private:
  QLabel label;
  QMovie movie;
  QMovie movie_level2;
  QMovie movie_level3;
  QMovie attackmovie;
  QPixmap pix;


  void change_hp();//掉血函数


};

#endif // TOWER_H

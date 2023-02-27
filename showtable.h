#ifndef SHOWTABLE_H
#define SHOWTABLE_H

#include <QWidget>
#include "config.h"
#include <QPainter>
#include "mypushbutton.h"
#include <QTimer>
#include <QLabel>
#include <QMouseEvent>
#include "mymap.h"
#include "messagetips.h"
#include "card.h"
#include "tower.h"
#include "enemy.h"
#include "weapon.h"
#include <algorithm>
#include "affixlabel.h"
#include <QSoundEffect>

//塔的类型


namespace Ui {
class Showtable;
}

class Showtable : public QWidget
{
    Q_OBJECT
    enum types
    {
        empty=0,
        road=1,
        grass=2,
        platform=3,
        origin=4,
        final=5
    };
public:
    Showtable(QWidget *parent = nullptr);
    ~Showtable();
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *e);//游戏结束，任意键返回主界面;过程中按Esc
    void set();
    void reset();
private:

    //1初始化阶段
    //vars

    Ui::Showtable *ui;

    mypushbutton * backbtn;//返回的按钮。
    mypushbutton * begin;
    mypushbutton * output_map;
    mypushbutton * input_map;
    myMap mymap;//地图类

    //some signals
    bool is_initial_map=false;

    bool is_play_game=false;

    //mypush
    vector<mypushbutton*> build_maps;
    //展示数组
    vector<vector<QLabel *>> Blanks;


    //funcs
    void init_func_btn();//返回按钮的设置



    void init_the_map();//加载地图

    void show_map_blocks();//展示方格


    //2地图创建阶段
    //var
    QLabel * inditator_block=nullptr;
    int inditator_type=-1;
    mypushbutton * route_unconnected=nullptr;
    MessageTips * mMessageTips=nullptr;

    //func
    void Hide_block(int logical_x,int logical_y);//右击block消失
    bool change_block(int logical_x,int logical_y);//替换block
    void build_the_road();//地图创建的步骤
    void setblock(int logical_x,int logical_y,int indicater_type);
    void Give_up_inditator(int logical_x,int logical_y);




    //3游戏阶段
    //music
    QSoundEffect backmusic;

    //vars
    vector<TowerCard*> tower_labels;//每一个卡片的数据

    mypushbutton * reset_btn=nullptr;//返回主界面的按钮。
    mypushbutton * continue_btn=nullptr;

    //词缀的选择，显示与计数
    vector<mypushbutton*> affixes_select;//下标与enum一致
    vector<QLabel*> affixes_select_counts;//展示选中的词缀
    vector<int>affixes_numbers;//每一类词缀的数量

    //Tower数组
    vector<Tower*> towers;


    //Tower全局的指针
    QLabel * indicater_tower=nullptr;
    int tower_type=-1;


    //敌人数组
    vector<Enemy*> enemies;
    int enemy_create_count=0;
    int enemy_create_interval=1500;

    //

    QTimer * TotalTime=nullptr;

    //游戏结束标志的判断
    int life;
    QLabel * lifeshow=nullptr;
    QLabel * loseshow=nullptr;


   //武器数组
   vector<weapon*> weapons;
   vector<int>target_index;
   vector<QSoundEffect*> weapons_sound;

   int myflag;



   //掉落词缀数组
   vector<Affixlabel*> affixes;
   vector<mypushbutton*> affixes_show;



   //词缀的卸载和安装
   vector<QLabel*> tower_affix_show;

   mypushbutton * dismantle;
   mypushbutton * levelupbtn;

   //全局词缀的展示
   QLabel  *global_affix;
   int global_affix_index;



   //点击建筑物，全局建筑物选择
   int global_tower_x;

   int global_tower_y;


   //全局的分数
   QLabel  score_show;
   int score=400;

    //funcs
    void play_game();//主要处理阶段
    void hidden_build_map_btn();//隐藏上一个阶段的选项按钮
    void init_tower_labels();//初始化塔防的选择
    void show_play_btns();//构建游戏阶段的硬件设施
    void init_game_btns();//设置游戏的开始与结束暂停键

    void Add_Tower(int tower_type,int x,int y);//添加塔防，做一些检查
    void Add_Affix(int logical_x,int logical_y);//添加词缀

    bool has_tower(int x,int y);

    void reset_game();//在游戏阶段重新开始

    void init_game_timer();//初始化时钟

    void Dealwithgame();//时钟所关联的函数，主要更新数据

    void Give_up_tower_inditator(int logical_x,int logical_y);

    void Give_up_affix_inditator();//放弃词缀

    void Show_affix_slot(int ,int);//展示安装词缀的槽

    void Delete_Tower();//删除塔防
    void Level_Tower();//升级塔防

    void hiderange();//隐藏范围
    void hide_affixset();//隐藏词缀安装

    void set_global_affix(int index);//设置全局词缀
    void reset_global_affix();//隐藏全局词缀

    void delete_all_globals();//删除所有目前的全局指向

    void change_Affix(int index);//更改词缀显示

    void Delete_affix(int x,int y);//卸载词缀

    void levelbtndeal();//管理升级按钮




    //时钟函数
    void create_enemy();//敌人生成
    void enemy_action();//敌人的行为
    void game_over();//判断游戏是否结束
    void tower_action();//塔防的行为，攻击拆除
    void weapon_action();//攻击物的行为
    void affixes_action();//词缀的行为
    void cd_action();//cd的行为
    void score_action();//分数行为

    void magictower_action(Tower*t);//魔法塔的行为
    void armytower_action(Tower *t);//近战塔的行为

    //在enemy_action里面调用
    void create_tower_affix(int type,std::vector<int>);//敌人死亡，创造词缀

    //在weaponaction里面调用
    void play_bang();//播放爆炸声

signals:
    void back_to_mainwindow();
    void game_to_mainwindow();
    void over();
};

#endif // SHOWTABLE_H

#include "showtable.h"
#include "ui_showtable.h"

Showtable::Showtable(QWidget *parent) :
    QWidget{parent}
{
    myflag=100;


    qDebug()<<"aaa";
    this->setFixedSize(PLAY_WIDTH,PLAY_HEIGHT);

    //地图构建阶段
    init_func_btn();//功能按钮硬件初始化
    init_the_map();//地图硬件初始化
    build_the_road();//地图要素块初始化


    //游戏开始阶段
    init_tower_labels();


    init_game_btns();
    this->setMouseTracking(true);

    init_game_timer();//初始化时钟

    //初始化音乐
    backmusic.setParent(this);
    backmusic.setSource(QUrl::fromLocalFile(BACK_MUSIC));
    backmusic.setLoopCount(20000);
    backmusic.setVolume(0.3);
    //设置音乐
    for(int i=0;i<20;i++){
        QSoundEffect*Plaing_Sound = new QSoundEffect(this);
        Plaing_Sound->setSource(QUrl::fromLocalFile(BANG_MUSIC));
        Plaing_Sound->setLoopCount(1);
        Plaing_Sound->setVolume(0.5);
        weapons_sound.push_back(Plaing_Sound);
    }



}

Showtable::~Showtable()
{
    delete ui;
    delete backbtn;
    delete begin;
    delete output_map;
    delete input_map;

    for(int i=0;i<build_maps.size();i++)
    {
        delete build_maps[i];
    }

    delete inditator_block;

    for(int i=0;i<Blanks.size();i++)
    {
        for(int j=0;j<Blanks[0].size();j++)
        {
            delete Blanks[i][j];
        }
    }

    delete route_unconnected;
    delete mMessageTips;


    //vars
    for(int i=0;i<tower_labels.size();i++)
    {
        delete tower_labels[i];
        tower_labels[i]=nullptr;
        tower_labels.erase(tower_labels.begin()+i);
    }

    delete reset_btn;//返回主界面的按钮。
    delete continue_btn;

    //词缀的选择，显示与计数
    for(int i=0;i<affixes_select.size();i++)
    {
        delete affixes_select[i];
        affixes_select[i]=nullptr;
        affixes_select.erase(affixes_select.begin()+i);
    }

    for(int i=0;i<affixes_select_counts.size();i++)
    {
        delete affixes_select_counts[i];
        affixes_select_counts[i]=nullptr;
        affixes_select_counts.erase(affixes_select_counts.begin()+i);
    }

    //Tower数组
    for(int i=0;i<towers.size();i++)
    {
        delete towers[i];
        towers[i]=nullptr;
        towers.erase(towers.begin()+i);
    }


    //Tower全局的指针
    delete indicater_tower;


    //敌人数组
    for(int i=0;i<enemies.size();i++)
    {
        delete enemies[i];
        enemies[i]=nullptr;
        enemies.erase(enemies.begin()+i);
    }

    //
    delete TotalTime;

    //游戏结束标志的判断
    delete lifeshow;
    delete loseshow;


   //武器数组
    for(int i=0;i<weapons.size();i++)
    {
        delete weapons[i];
        weapons[i]=nullptr;
        weapons.erase(weapons.begin()+i);
    }


   //掉落词缀数组
    for(int i=0;i<affixes.size();i++)
    {
        delete affixes[i];
        affixes[i]=nullptr;
        affixes.erase(affixes.begin()+i);
    }
    for(int i=0;i<affixes_show.size();i++)
    {
        delete affixes_show[i];
        affixes_show[i]=nullptr;
        affixes_show.erase(affixes_show.begin()+i);
    }



   //词缀的卸载和安装
    for(int i=0;i<tower_affix_show.size();i++)
    {
        delete tower_affix_show[i];
        tower_affix_show[i]=nullptr;
        tower_affix_show.erase(tower_affix_show.begin()+i);
    }

   delete dismantle;

   delete levelupbtn;



}
//绘制背景
void Showtable::paintEvent(QPaintEvent *){

    QPainter mypainter(this);   QPixmap pix;

    pix.load(PLAY_BACKGROUND_PATH);
    mypainter.drawPixmap(0,0,this->width(),this->height(),pix);



}
void Showtable::set()
{
    is_initial_map=true;//开启地图构建模式

    is_play_game=false;//关闭游戏开始模式
    //读取内存地图
    mymap.input_map();

    //更换地图
    show_map_blocks();

    //展示构建的初始化
    backbtn->show();
    begin->show();
    output_map->show();
    input_map->show();
    for(int i=1;i<build_maps.size();i++)
    {
      build_maps[i]->show();
    }

    life=LIFE_VALUE;

}
void Showtable::reset()
{

}


void Showtable::init_func_btn()
{
    //0、多个核心的报错


    //1、初始化返回按钮。
    backbtn=new mypushbutton(BACK_BTN_PATH);
    backbtn->setParent(this);
    backbtn->move(1200,750);

    connect(backbtn,&QPushButton::clicked,this,[=](){

        backbtn->downBounce();//下弹
        backbtn->upBounce();//上弹
        QTimer::singleShot(100,this,[=](){

                  emit this->back_to_mainwindow();
           });

       });

    //2 begin按钮的初始化
    begin=new mypushbutton(START_BTN_PATH);
    begin->setParent(this);
    begin->move(1100,750);

    connect(begin,&QPushButton::clicked,this,[=](){
        begin->downBounce();//下弹
        begin->upBounce();//上弹
        try
        {
            mymap.check_and_create_route();
        }

        catch(int a)
        {
            if(a==1)//多个终点
            {
                if(mMessageTips==nullptr)
                     mMessageTips=new MessageTips("只能有一个核心！",this,1300,900);
                else
                     mMessageTips->set_text("只能有一个核心");
                mMessageTips->setOpacityValue(2.5);
                mMessageTips->show();
            }
            else if(a==2)//线路不通
            {

                route_unconnected->raise();
                route_unconnected->move(500,300);
                route_unconnected->show();
            }
            else if(a==3)//必须要有一个核心
            {
                if(mMessageTips==nullptr)
                     mMessageTips=new MessageTips("必须有一个核心！",this,1300,900);
                else
                     mMessageTips->set_text("必须有一个核心");
                mMessageTips->setOpacityValue(2.5);
                mMessageTips->show();
            }
            else if(a==4)//至少有一个出怪口
            {
                if(mMessageTips==nullptr)
                     mMessageTips=new MessageTips("至少有一个出怪口！",this,1300,900);
                else
                     mMessageTips->set_text("至少有一个出怪口");
                mMessageTips->setOpacityValue(2.5);
                mMessageTips->show();
            }
            return ;
        }

        //进入游戏阶段。

        play_game();
       });

    //3 input 地图
    output_map=new mypushbutton(INPUT_BTN_PATH);
    output_map->setParent(this);
    output_map->move(1100,800);

    connect(output_map,&QPushButton::clicked,this,[=](){
        output_map->downBounce();//下弹
        output_map->upBounce();//上弹
        mymap.input_map();
        show_map_blocks();
       });



    //4 output 地图
    input_map=new mypushbutton(OUTPUT_BTN_PATH);
    input_map->setParent(this);
    input_map->move(1200,800);

    connect(input_map,&QPushButton::clicked,this,[=](){
        input_map->downBounce();//下弹
        input_map->upBounce();//上弹
        mymap.output_map();
       });

    //5显示没有通路的按钮
    route_unconnected=new mypushbutton(ROUTE_UNCONNECTED_PATH);
    route_unconnected->setParent(this);
    route_unconnected->hide();

    connect(route_unconnected,&QPushButton::clicked,this,[=](){
        route_unconnected->downBounce();//下弹
        route_unconnected->upBounce();//上弹
        route_unconnected->hide();
       });




}


//mymaps
void Showtable::init_the_map()
{
    int row=mymap.get_row();
    int col=mymap.get_col();

    QLabel *pt=nullptr;
    for(int i=0;i<row;i++)
    {
        vector<QLabel*>temp;
        for(int j=0;j<col;j++)
        {
            pt=new QLabel;
            pt->setParent(this);
            pt->hide();
            temp.push_back(pt);
        }
        Blanks.push_back(temp);
    }
}
void Showtable::show_map_blocks()
{

    for(int i=0;i<mymap.get_row();i++){
        for(int j=0;j<mymap.get_col();j++)
        {
             int type=mymap.get_type(i,j);
             QPixmap pixMap1(ROAD_PATH);
             QPixmap pixMap2(GRASS_PATH);
             QPixmap pixMap3(PLAT_FORM_PATH);
             QPixmap pixMap4(ORIGIN_PATH);
             QPixmap pixMap5(FINAL_PATH);
             switch(type)
             {
                 assert(i<Blanks.size());
                 assert(j<Blanks[0].size());
                 case empty:
                    Blanks[i][j]->setGeometry(mymap.get_pix_y(i,j),mymap.get_pix_x(i,j),99,99);
                    Blanks[i][j]->hide();
                    Blanks[i][j]->lower();
                    break;
                 case road:
                    Blanks[i][j]->setPixmap(pixMap1);
                    Blanks[i][j]->setGeometry(mymap.get_pix_y(i,j),mymap.get_pix_x(i,j),99,99);//前两个参数表示label左上角位置后面分别是宽和高
                    Blanks[i][j]->show();
                    Blanks[i][j]->lower();
                    break;
                 case grass:
                     Blanks[i][j]->setPixmap(pixMap2);
                     Blanks[i][j]->setGeometry(mymap.get_pix_y(i,j),mymap.get_pix_x(i,j),99,99);//前
                     Blanks[i][j]->show();
                     Blanks[i][j]->lower();
                     break;
                 case platform:
                      Blanks[i][j]->setPixmap(pixMap3);
                      Blanks[i][j]->setGeometry(mymap.get_pix_y(i,j),mymap.get_pix_x(i,j),99,99);//前
                      Blanks[i][j]->show();
                      Blanks[i][j]->lower();
                     break;
                 case origin:
                      Blanks[i][j]->setPixmap(pixMap4);
                      Blanks[i][j]->setGeometry(mymap.get_pix_y(i,j),mymap.get_pix_x(i,j),99,99);//前
                      Blanks[i][j]->show();
                      Blanks[i][j]->lower();
                     break;
                 case final:
                     Blanks[i][j]->setPixmap(pixMap5);
                     Blanks[i][j]->setGeometry(mymap.get_pix_y(i,j),mymap.get_pix_x(i,j),99,99);//前
                     Blanks[i][j]->show();
                     Blanks[i][j]->lower();
                    break;
             }
        }
     }

}

//地图创建阶段
 void Showtable::build_the_road()
 {
     //初始化
     mypushbutton * select_road=new mypushbutton(ROAD_PATH);
     select_road->setMouseTracking(true);
     mypushbutton * select_grass=new mypushbutton(GRASS_PATH);
     mypushbutton * select_platform=new mypushbutton(PLAT_FORM_PATH);

     mypushbutton * select_origin=new mypushbutton(ORIGIN_PATH);
     mypushbutton * select_final=new mypushbutton(FINAL_PATH);
     build_maps.push_back(nullptr);//注意第一个
     build_maps.push_back(select_road);
     build_maps.push_back(select_grass);
     build_maps.push_back(select_platform);
     build_maps.push_back(select_origin);
     build_maps.push_back(select_final);


    //加载路面。
    assert(1<build_maps.size());

    build_maps[1]->setParent(this);
    build_maps[1]->move(150,750);
    build_maps[1]->show();
    connect(build_maps[1],&QPushButton::clicked,this,[=]{
        build_maps[1]->downBounce();//下弹
        build_maps[1]->upBounce();//上弹
        if(inditator_block==nullptr){
                inditator_type=road;
                QPixmap light_road_pic(LIGHT_ROAD_PATH);
                inditator_block=new QLabel();
                inditator_block->setMouseTracking(true);
                inditator_block->setParent(this);
                inditator_block->setPixmap(light_road_pic);
        }
    });

    //加载路面。
    assert(2<build_maps.size());

    build_maps[2]->setParent(this);
    build_maps[2]->move(350,750);
    build_maps[2]->show();
    connect(build_maps[2],&QPushButton::clicked,this,[=]{
        build_maps[2]->downBounce();//下弹
        build_maps[2]->upBounce();//上弹
        if(inditator_block!=nullptr){
            delete inditator_block;
            }
        inditator_type=grass;
        QPixmap pic(GRASS_PATH);
        inditator_block=new QLabel();
        inditator_block->setMouseTracking(true);
        inditator_block->setParent(this);
        inditator_block->setPixmap(pic);
        }
    );

    assert(3<build_maps.size());

    build_maps[3]->setParent(this);
    build_maps[3]->move(500,750);
    build_maps[3]->show();
    connect(build_maps[3],&QPushButton::clicked,this,[=]{
        build_maps[3]->downBounce();//下弹
        build_maps[3]->upBounce();//上弹
        if(inditator_block!=nullptr){
            delete inditator_block;
            }
        inditator_type=platform;
        QPixmap pic(PLAT_FORM_PATH);
        inditator_block=new QLabel();
        inditator_block->setMouseTracking(true);
        inditator_block->setParent(this);
        inditator_block->setPixmap(pic);

    });

    assert(4<build_maps.size());

    build_maps[4]->setParent(this);
    build_maps[4]->move(650,750);
    build_maps[4]->show();
    connect(build_maps[4],&QPushButton::clicked,this,[=]{
        build_maps[4]->downBounce();//下弹
        build_maps[4]->upBounce();//上弹
        if(inditator_block!=nullptr){
            delete inditator_block;
            }
        inditator_type=origin;
        QPixmap pic(ORIGIN_PATH);
        inditator_block=new QLabel();
        inditator_block->setMouseTracking(true);
        inditator_block->setParent(this);
        inditator_block->setPixmap(pic);

    });

    assert(5<build_maps.size());

    build_maps[5]->setParent(this);
    build_maps[5]->move(800,750);
    build_maps[5]->show();
    connect(build_maps[5],&QPushButton::clicked,this,[=]{
        build_maps[5]->downBounce();//下弹
        build_maps[5]->upBounce();//上弹
        if(inditator_block!=nullptr){
            delete inditator_block;
            }
        inditator_type=final;
        QPixmap pic(FINAL_PATH);
        inditator_block=new QLabel();
        inditator_block->setMouseTracking(true);
        inditator_block->setParent(this);
        inditator_block->setPixmap(pic);

    });
}


void Showtable::Hide_block(int logical_x,int logical_y)
{
    int row=mymap.get_row();
    int col=mymap.get_col();
    if(logical_x<0||logical_x>=row||logical_y<0||logical_y>=col)
        return;
    //隐藏
    assert(logical_x<Blanks.size());
    assert(logical_y<Blanks[0].size());

    Blanks[logical_x][logical_y]->hide();
    //改变内存
    mymap.set_type(empty,logical_x,logical_y);
}

bool Showtable::change_block(int logical_x,int logical_y)
{
    int row=mymap.get_row();
    int col=mymap.get_col();
    if(logical_x<0||logical_x>=row||logical_y<0||logical_y>=col)
        return false;
    if(mymap.get_type(logical_x,logical_y)==empty)
    {
        mymap.set_type(inditator_type,logical_x,logical_y);
        setblock(logical_x,logical_y,inditator_type);
        return true;
    }
    else
    {
        return false;
    }
}
void Showtable::setblock(int logical_x,int logical_y,int inditator_type)
{
    assert(logical_x<Blanks.size());
    assert(logical_y<Blanks[0].size());
    QPixmap pix;
    switch(inditator_type)
    {
        case empty:
                Blanks[logical_x][logical_y]->hide();
                break;
        case road:
               pix.load(ROAD_PATH);
               Blanks[logical_x][logical_y]->setPixmap(pix);
               Blanks[logical_x][logical_y]->show();
               break;
        case grass:
               pix.load(GRASS_PATH);
               Blanks[logical_x][logical_y]->setPixmap(pix);
               Blanks[logical_x][logical_y]->show();
               break;
        case platform:
               pix.load(PLAT_FORM_PATH);
               Blanks[logical_x][logical_y]->setPixmap(pix);
               Blanks[logical_x][logical_y]->show();
               break;
        case origin:
               pix.load(ORIGIN_PATH);
               Blanks[logical_x][logical_y]->setPixmap(pix);
               Blanks[logical_x][logical_y]->show();
               break;
        case final:
               pix.load(FINAL_PATH);
               Blanks[logical_x][logical_y]->setPixmap(pix);
               Blanks[logical_x][logical_y]->show();
               break;
    }


}

void Showtable::Give_up_inditator(int logical_x,int logical_y)
{
    if(logical_x>=7)
    {
        delete inditator_block;
        inditator_block=nullptr;
        inditator_type=-1;
    }
}
void Showtable::Give_up_tower_inditator(int logical_x,int logical_y)
{

    if(logical_x>=7&&indicater_tower!=nullptr)
    {
        delete indicater_tower;
        indicater_tower=nullptr;
        tower_type=-1;
    }
}



 //游戏阶段
void Showtable::Give_up_affix_inditator()
{
    if(global_affix!=nullptr){
        delete global_affix;
        global_affix=nullptr;
        global_affix_index=-1;
    }
}

void Showtable::play_game()//主要处理阶段
{
    qDebug()<<"Begin play game!";
    //阶段标志的转换
    is_play_game=true;
    is_initial_map=false;

    //布局的改变
    hidden_build_map_btn();

    //布置新的按钮和构建
    show_play_btns();

    backmusic.play();


}
void Showtable::hidden_build_map_btn()
{
    backbtn->hide();
    begin->hide();
    output_map->hide();
    input_map->hide();
    for(int i=1;i<build_maps.size();i++)
    {
      build_maps[i]->hide();
    }

}
void Showtable::init_tower_labels()
{
       //0号塔：魔法塔。
        TowerCard * magic=new TowerCard(TOWER0_PATH,magictower,100,750);
        TowerCard * army=new TowerCard(ARMY_LABEL_PATH,armytower,200,750);





       //加入一个vector中
        tower_labels.push_back(magic);
        tower_labels.push_back(army);




        //完成父亲的设置
        for(int i=0;i<tower_labels.size();i++)
        {

            tower_labels[i]->btn->setParent(this);
            tower_labels[i]->qpr.setParent(this);
            tower_labels[i]->btn->hide();
            tower_labels[i]->qpr.hide();
        }


        //完成相应的绑定
        //魔法塔
        assert(0<tower_labels.size());
        connect(tower_labels[0]->btn,&QPushButton::clicked,this,[=](){
                tower_labels[0]->btn->downBounce();
                tower_labels[0]->btn->upBounce();

                delete_all_globals();
                indicater_tower=new QLabel();
                tower_type=magictower;
                QPixmap s(MAGIC_INDACATER_PATH);
                indicater_tower->setPixmap(s);
                qDebug()<<"This is magic tower!";
        });


        //近战塔
        assert(1<tower_labels.size());
        connect(tower_labels[1]->btn,&QPushButton::clicked,this,[=](){
                tower_labels[1]->btn->downBounce();
                tower_labels[1]->btn->upBounce();

                delete_all_globals();
                indicater_tower=new QLabel();
                tower_type=armytower;
                QPixmap s(ARMY_LABEL_PATH);
                indicater_tower->setPixmap(s);
                qDebug()<<"This is army tower!";
});




}


void Showtable::show_play_btns()
{
    for(int i=0;i<tower_labels.size();i++){
        tower_labels[i]->show();
    }
    reset_btn->show();
    continue_btn->show();


    //展示词缀选择
    for(int i=0;i<affixes_select.size();i++){
        affixes_select[i]->show();
        assert(i<affixes_numbers.size());
        affixes_select_counts[i]->setNum(affixes_numbers[i]);
        affixes_select_counts[i]->show();
    }

    lifeshow->show();
    score_show.show();
}

void Showtable::init_game_btns()
{
    //1暂停返回按钮
    reset_btn=new mypushbutton(RESET_BTN_PATH);//返回按钮
    continue_btn=new mypushbutton(CONTINUE_PATH);//暂停按钮

    reset_btn->setParent(this);
    continue_btn->setParent(this);

    reset_btn->hide();
    continue_btn->hide();

    reset_btn->setGeometry(1300,720,80,80);
    continue_btn->setGeometry(1200,720,80,80);

    connect(reset_btn,&QPushButton::clicked,this,[=](){
        reset_btn->downBounce();
        reset_btn->upBounce();
        //
        reset_game();
        emit this->game_to_mainwindow();

    });

    connect(continue_btn,&QPushButton::clicked,this,[=](){
        continue_btn->downBounce();
        continue_btn->upBounce();

        if(TotalTime->isActive())
        {
            TotalTime->stop();
        }
        else
        {
            TotalTime->start();
        }
    });
    //2游戏生命值的计数
    life=LIFE_VALUE;
    lifeshow=new QLabel();
    QString showstr="目前生命值为:"+QString::number(life);
    QFont ft;
    ft.setPointSize(20);
    lifeshow->setFont(ft);
    lifeshow->setText(showstr);
    lifeshow->move(950,750);
    lifeshow->setParent(this);
    lifeshow->hide();


    //3游戏失败的标签
    QPixmap pix(LOSS_PATH);
    loseshow=new QLabel();
    loseshow->setPixmap(pix);
    loseshow->move(500,300);
    loseshow->setParent(this);
    loseshow->hide();

    //4游戏词缀的收集和选择(目前四个词词缀）
    affixes_numbers.resize(TOTAL_AFFIX_NUM,0);
    for(int i=0;i<TOTAL_AFFIX_NUM;i++)
    {
        mypushbutton *pt=nullptr;
        QLabel *qpt=nullptr;
        switch(i)
        {
            case blood:
                pt=new mypushbutton(BLOOD_LABEL_PATH);
                break;
            case ice:
                pt=new mypushbutton(ICE_LABEL_PATH);
                break;
            case rampage:
                pt=new mypushbutton(RAMPAGE_LABEL_PATH);
                break;
            case groupinjury:
                pt=new mypushbutton(GROUP_LABEL_PATH);
                break;
        }
        pt->setGeometry(300+i*75,750,50,50);
        pt->setParent(this);
        pt->hide();

        qpt=new QLabel();
        assert(i<affixes_numbers.size());
        qpt->setNum(affixes_numbers[i]);
        qpt->setGeometry(300+i*75+20,800,20,20);
        qpt->setParent(this);
        qpt->hide();


        affixes_select.push_back(pt);
        affixes_select_counts.push_back(qpt);

        //按钮的绑定
        connect(pt,&QPushButton::clicked,[=](){
            pt->downBounce();
            pt->upBounce();
            for(int i=0;i<affixes_select.size();i++)
            {
              if(affixes_select[i]==pt)
              {
                    set_global_affix(i);
                    break;
              }
            }
        });

    }


    //5词缀安装的设置。
    for(int i=0;i<TOWER_AFFIX_NUM;i++)
    {
        QLabel * pt=new QLabel();
        QPixmap pix;
        pix.load(AFFIX_LOAD_PATH);
        pt->setPixmap(pix);
        pt->setGeometry(625+100*i,750,50,50);
        pt->setParent(this);
        pt->hide();
        tower_affix_show.push_back(pt);
    }
    dismantle=new mypushbutton(DISMENTLE_TOWER_PATH);
    dismantle->setGeometry(650,820,100,34);
    dismantle->setParent(this);
    dismantle->hide();
    connect(dismantle,&QPushButton::clicked,[=](){
        if(global_tower_x!=-1&&global_tower_y!=-1)
            Delete_Tower();
    });

    //升级按钮的设置
    levelupbtn=new mypushbutton(LEVELUP_PATH);
    levelupbtn->setGeometry(800,800,90,60);
    levelupbtn->setParent(this);
    levelupbtn->hide();
    connect(levelupbtn,&QPushButton::clicked,[=](){
        levelupbtn->downBounce();
        levelupbtn->upBounce();
        if(global_tower_x!=-1&&global_tower_y!=-1)
        {

            Level_Tower();
            levelupbtn->setEnabled(false);
            //钱不够了
            score-=100;
            if(score<100)
                levelupbtn->setEnabled(false);
        }
    });






    //全局建筑物坐标设置
    global_tower_x=-1;
    global_tower_y=-1;


    //全局词缀安放的设置
    global_affix=nullptr;
    global_affix_index=-1;


    //设置全局的分数
    score_show.setText("得分："+QString::number(score));
    QFont ft1;
    ft1.setPointSize(20);
    score_show.setFont(ft1);

    score_show.setParent(this);
    score_show.setGeometry(950,800,200,50);
    score_show.show();
}
void Showtable::set_global_affix(int index)
{
    //1清除已有的指针
    delete_all_globals();

   //2设置
   global_affix_index=index;
   QPixmap pix;
   global_affix=new QLabel();
   global_affix->setParent(this);
   switch(index)
   {
       case blood:
            pix.load(BLOOD_LABEL_PATH);
            global_affix->setGeometry(300,750,50,50);
            break;
       case ice:
            pix.load(ICE_LABEL_PATH);
            global_affix->setGeometry(375,750,50,50);
            break;
       case rampage:
            pix.load(RAMPAGE_LABEL_PATH);
            global_affix->setGeometry(450,750,50,50);
            break;
       case groupinjury:
            pix.load(GROUP_LABEL_PATH);
            global_affix->setGeometry(525,750,50,50);
            break;
   }
   global_affix->setPixmap(pix);
   global_affix->show();

}
void Showtable::reset_global_affix()
{
    delete  global_affix;
    global_affix=nullptr;
    global_affix_index=-1;
}
void Showtable::delete_all_globals()
{
    if(global_affix!=nullptr)
     {
         delete  global_affix;
         global_affix=nullptr;
         global_affix_index=-1;
     }
    if(indicater_tower!=nullptr)
     {
         delete indicater_tower;
         indicater_tower=nullptr;
         tower_type=-1;
     }
}

void Showtable::Delete_Tower()
{
    for(int i=0;i<towers.size();i++)
    {
        if(towers[i]->x==global_tower_x&&\
           towers[i]->y==global_tower_y)
        {
            delete towers[i];
            towers[i]=nullptr;
            towers.erase(towers.begin()+i);
            break;
        }
    }

    hide_affixset();
}
void Showtable::Level_Tower()
{
    for(int i=0;i<towers.size();i++)
    {
        if(towers[i]->x==global_tower_x&&\
           towers[i]->y==global_tower_y)
        {
            towers[i]->levelup();
            towers[i]->showimage();

            if(towers[i]->level==3)
                 levelupbtn->setEnabled(false);
            break;
        }
    }
}
bool Showtable::has_tower(int x,int y)
{
    for(int i=0;i<towers.size();i++)
    {
        if(towers[i]->x==x&&towers[i]->y==y)
            return true;
    }
    return false;
}


void Showtable::Add_Tower(int tower_type,int x,int y)//添加塔防，做一些检查
{
     if(x<0||x>=7||y<0||y>=14)
         return ;
     if(has_tower(x,y))
          return;

     //是近战塔
     if(tower_type==armytower)
     {
         if(mymap.get_type(x,y)==road)
         {
            Tower * t=nullptr;
            switch(tower_type)
            {
               case armytower:
                    t=new armyTower(x,y,armytower,mymap.get_pix_x(x,y),mymap.get_pix_y(x,y),this);
                    break;
            }
            towers.push_back(t);

            //设置cd
            for(int i=0;i<tower_labels.size();i++)
            {

                if(tower_labels[i]->get_type()==tower_type){
                            qDebug()<<"rrrr";
                           tower_labels[i]->reset();
                           break;
                }
            }

            switch(tower_type)
            {
                    case magictower:
                            score-=100;
                            break;
                    case armytower:
                            score-=70;
                            break;
            }
            throw 0;
         }
         else
         {
            return;
         }
     }



     //远程塔
     if(mymap.get_type(x,y)==platform)
     {
         Tower * t=nullptr;
         switch(tower_type)
         {
            case magictower:
                 t=new magicTower(x,y,magictower,mymap.get_pix_x(x,y),mymap.get_pix_y(x,y),this);
                 break;
         }
         towers.push_back(t);

         //设置cd
         for(int i=0;i<tower_labels.size();i++)
         {
             if(tower_labels[i]->get_type()==tower_type){
                        tower_labels[i]->reset();
                        break;
             }
         }
         switch(tower_type)
         {
                 case magictower:
                         score-=100;
                         break;
                 case armytower:
                         score-=70;
                         break;
         }
         throw 0;
     }
}

void Showtable::reset_game()
{
    //1清空塔防
    for(int i=0;i<towers.size();i++)
    {
        delete towers[i];
    }
    towers.clear();
    //2清空僵尸
    for(int i=0;i<enemies.size();i++)
    {
        delete enemies[i];
    }
    enemies.clear();
    //清空武器
    for(int i=0;i<weapons.size();i++)
    {
        delete weapons[i];
    }
    weapons.clear();
    target_index.clear();


    //3隐藏功能按钮。
    reset_btn->hide();//返回主界面的按钮。
    continue_btn->hide();//暂停按钮

    //4卡片全部隐藏
    for(int i=0;i<tower_labels.size();i++)
    {
        tower_labels[i]->btn->hide();
        tower_labels[i]->qpr.hide();
    }

    //5全局tower重置
    indicater_tower=nullptr;
    tower_type=-1;

    //6时钟停止
    TotalTime->stop();

    //7隐藏生命值
    lifeshow->hide();

    //8清空全局指针
    indicater_tower=nullptr;
    tower_type=-1;

    myflag=100;

    //9清空所有掉落的词缀
    for(int i=0;i<affixes.size();i++)
    {
        delete affixes[i];
        delete affixes_show[i];
    }
    affixes.clear();
    affixes_show.clear();

    //10隐藏词缀选择
    for(int i=0;i<affixes_select.size();i++)
    {
        affixes_select[i]->hide();
        affixes_select_counts[i]->hide();
    }
    affixes_numbers.clear();
    affixes_numbers.resize(TOTAL_AFFIX_NUM,0);

    //11隐藏词缀的安装与设置
    for(int j=0;j<tower_affix_show.size();j++)
    {
        tower_affix_show[j]->hide();
    }
    dismantle->hide();
    levelupbtn->hide();

    //12
    delete global_affix;
    global_affix=nullptr;
    global_affix_index=-1;

    //13
    delete indicater_tower;
    indicater_tower=nullptr;
    tower_type=-1;

    //关闭音乐
    backmusic.stop();

    //隐藏得分表示
    score_show.hide();
    score=400;
    score_show.setText("得分:"+QString::number(score));


}


void Showtable::init_game_timer()//初始化时钟
{

    TotalTime = new QTimer(this);

    TotalTime->stop();
    TotalTime->setInterval(1);
    connect(TotalTime,&QTimer::timeout,this,[=](){
        Dealwithgame();
    });//全局时间槽
}



void Showtable::Dealwithgame()
{
    //qDebug()<<myflag;
    //产生敌人
    if(myflag>=0){
        create_enemy();
    }
    //敌人的行为

    enemy_action();

    //塔防的行为
    tower_action();

    //武器的行为
    weapon_action();

    //删除掉落的词缀
    affixes_action();

    //塔防的cd，词缀的数量
    cd_action();

    //全局分数
    score_action();
    //判断游戏是否结束
    game_over();
}
//以下均为每一轮时钟都要执行的函数
void Showtable::score_action()
{
    score_show.setText("得分："+QString::number(score));
}

void Showtable::cd_action()
{
    //更新词缀数量
    for(int i=0;i<affixes_numbers.size();i++)
    {
        affixes_select_counts[i]->setNum(affixes_numbers[i]);
    }

    //更新塔防cd
    for(int j=0;j<tower_labels.size();j++)
    {
        tower_labels[j]->change_condition(score);
    }

    for(int i=0;i<affixes_numbers.size();i++)
    {
        if(affixes_numbers[i]==0)
        {
            assert(i<affixes_select.size());
            affixes_select[i]->setEnabled(false);
        }
        else
        {
            assert(i<affixes_select.size());
            affixes_select[i]->setEnabled(true);
        }
    }

    //更新升级的cd
    if(global_tower_x!=-1&&global_tower_y!=-1)
    {
        if(score<100)
        {
            levelupbtn->setEnabled(false);
        }
        else if(score>=100)
        {
            levelbtndeal();
        }
    }
}
void Showtable::levelbtndeal()
{
   for(int i=0;i<towers.size();i++)
   {
       if(towers[i]->x==global_tower_x &&
          towers[i]->y==global_tower_y)
       {
           if(towers[i]->level==3)
               levelupbtn->setEnabled(false);
           else
               levelupbtn->setEnabled(true);
           break;
       }
   }
}

void Showtable::affixes_action()
{
   for(int i=0;i<affixes.size();i++)
   {
        if(affixes[i]->whether_die())
        {
            //获得类型，增加词缀数量
            int type=affixes[i]->get_type();

            switch(type)
            {
                case blood:
                    affixes_numbers[blood]++;
                    break;
                case ice:
                    affixes_numbers[ice]++;
                    break;
                case rampage:
                    affixes_numbers[rampage]++;
                    break;
                case groupinjury:
                    affixes_numbers[groupinjury]++;
                    break;
            }
            //删除词缀
            assert(i<affixes.size());

            delete affixes[i];
            affixes[i]=nullptr;
            affixes.erase(affixes.begin()+i);

            delete affixes_show[i];
            affixes_show[i]=nullptr;
            affixes_show.erase(affixes_show.begin()+i);

            i--;
        }
   }
}
void Showtable::create_enemy()
{
    if(enemy_create_count==enemy_create_interval)
    {
        myflag--;
        srand((int)time(0));  // 产生随机种子  把0换成NULL也行

        int  num=rand()%1+1;//获得产生数量
        qDebug()<<"The num is "<<num;
        //确定产生出口
        Enemy *enemy=nullptr;
        for(int i=0;i<num;i++)
        {

          int type=rand()%1;//产生敌人类型

          //获得生成地
          int entrynum=mymap.Get_entry_num();
          int select_entry=rand()%entrynum;
          int x;int y;
          mymap.Get_entry_location(select_entry,x,y);
          int dir=mymap.Get_direction(select_entry,0);

          int route_max_num=mymap.get_block_nums(select_entry);
          switch(type)
          {
            case boar:
                qDebug()<<"This is boar !";
                enemy=new Boar(x,y,mymap.get_pix_x(x,y),mymap.get_pix_y(x,y),\
                               10,3,boar,100,dir,select_entry,route_max_num,\
                               this);
                break;

          }
          enemies.push_back(enemy);
        }
        enemy_create_count=0;
    }
    else
    {
        enemy_create_count++;
    }
}
void Showtable::create_tower_affix(int type,std::vector<int> loc)//敌人死亡，创造词缀、
{
   Affixlabel *pt=nullptr;
   mypushbutton *btn=nullptr;
   switch(type)
   {
       case blood:
           pt=new Affixlabel(blood,loc[0],loc[1],this);
           btn=new mypushbutton(BLOOD_PATH);
           break;
       case ice:
           pt=new Affixlabel(ice,loc[0],loc[1],this);
           btn=new mypushbutton(ICE_PATH);
           break;
       case rampage:
           pt=new Affixlabel(rampage,loc[0],loc[1],this);
           btn=new mypushbutton(RAMPAGE_PATH);
           break;
       case groupinjury:
           pt=new Affixlabel(groupinjury,loc[0],loc[1],this);
           btn=new mypushbutton(GROUP_PATH);
           break;
   }
   affixes.push_back(pt);
   affixes_show.push_back(btn);
   connect(btn,&QPushButton::clicked,[=](){

        pt->is_die=true;
   });

   //掉落词缀展示
   srand((int)time(0));
   int offset=(rand()+loc[0]+loc[1])%100;
   btn->setGeometry(loc[1]+offset,loc[0]+75,25,25);
   btn->setParent(this);
   btn->show();
}

void Showtable::enemy_action()
{

    //敌人参数的调整
    int route_index;int x;int y;int px;int py;
    int dir;int speed;int block_num;


    //敌人的行为
    for(int index=0;index<enemies.size();index++)
    {
        assert(index>=0);
        if(enemies[index]->whether_die())
        {
            //消除导弹追踪
            for (int it =0; it<target_index.size();it++)
            {
                    if(target_index[it]==index)
                    {
                        target_index[it]=-1;
                    }
                    else if(target_index[it]>index)
                    {
                        target_index[it]=target_index[it]-1;
                    }
             }

            //删除敌人,死之前产生词缀
            create_tower_affix(enemies[index]->get_tower_affix(),\
                               enemies[index]->get_physical_xy());

            //删除敌人,死之前增加分数
            score=score+50;
            //delete相应的敌人
            qDebug()<<"enemy delele1";

            delete enemies[index];
            enemies[index]=nullptr;
            enemies.erase(enemies.begin()+index);
            index--;
            continue;
        }
        if(enemies[index]->is_next())
        {
            //获取敌人的坐标
            enemies[index]->Get_position(block_num,route_index,x,y,px,py,dir,speed);
            mymap.change_location(route_index,block_num,x,y,px,py,dir,speed);
            enemies[index]->Set_position(block_num,x,y,px,py,dir);
            enemies[index]->reset_next();
        }

        //qDebug()<<"enemy action";
        bool is_meet=false;
        for(int i=0;i<towers.size();i++)
        {
            assert(i>=0);
            if(towers[i]->x==enemies[index]->x&&\
               towers[i]->y==enemies[index]->y)
            {
                //对塔攻击
                if(enemies[index]->interval_increase())
                    towers[i]->get_acked(enemies[index]->ack);
                //设置遇到了塔
                enemies[index]->set_is_meet_tower(true);
                is_meet=true;
                break;
            }
        }
        if(!is_meet)
            enemies[index]->set_is_meet_tower(false);

        enemies[index]->showenemy(this);
    }




    //删除某些敌人，HP<0或者到达了终点
    for(int index=0;index<enemies.size();index++)
    {
        assert(index>=0);
        if(enemies[index]->is_achieved())
        {
          qDebug()<<"enemy delele2";
          for (int it =0; it<target_index.size();it++)
          {
                  if(target_index[it]==index)
                  {
                      target_index[it]=-1;
                  }
                  else if(target_index[it]>index)
                  {
                      target_index[it]=target_index[it]-1;
                  }
           }


          delete enemies[index];//删除僵尸
          enemies.erase(enemies.begin()+index);
          index--;
          life--;
      }
    }


}
bool comp(Enemy * t1,Enemy * t2)
{
    int rest_block1=t1->get_max_block_num()-t1->get_block_num();
    int rest_block2=t2->get_max_block_num()-t2->get_block_num();
    if(rest_block1==rest_block2)
    {
       return t1->get_block_num_walk()>t2->get_block_num_walk();
    }
    else
    {
       return  rest_block1 <rest_block2;
    }
}
void Showtable::tower_action()//塔防的行为，攻击拆除
{
    sort(enemies.begin(),enemies.end(),comp);
    for(int i=0;i<towers.size();i++)
    {
        //判断塔的消亡
        if(towers[i]->whether_die())
        {
            delete towers[i];
            towers[i]=nullptr;
            towers.erase(towers.begin()+i);
            i--;
            continue;
        }
        //塔的行为
        switch(towers[i]->type)
        {
            case magictower:
                magictower_action(towers[i]);
                break;
            case armytower:
                armytower_action(towers[i]);
                break;

        }
    }
}

void Showtable::magictower_action(Tower*t)
{
    t->interval_increase();
    t->show();
    if(t->is_fire()){
       for(int j=0;j<enemies.size();j++){//遍历敌人
            vector<int>loc=enemies[j]->get_logical_xy();
            if(t->is_in_range(loc[0],loc[1])){
                t->set_fire();
                weapon* pt=new hostle(t->px,t->py,t->get_ack(),40,this);
                weapons.push_back(pt);

                //有群攻属性,设置为群攻
                if(t->group_ack_num()>0)
                {
                    pt->set_group_ack();
                }
                if(t->ice_num()>0)
                {
                    pt->set_ice_ack();
                }
                if(t->blood_num()>0)
                {
                    pt->set_blood_ack();
                }
                target_index.push_back(j);

                break;
            }
        }
    }

}


void Showtable::armytower_action(Tower *t)
{
    t->interval_increase();
    if(t->is_fire())
    {
        bool flag=false;
        for(int j=0;j<enemies.size();j++){//遍历敌人
                 vector<int>loc=enemies[j]->get_logical_xy();
                 if(t->is_in_range(loc[0],loc[1])){
                        //不是群攻
                        if(t->group_ack_num()==0){
                            enemies[j]->get_acked(t->ack);
                            if(t->ice_num()>0)
                                enemies[j]->set_ice_acked(true);
                        }
                        else
                        {
                            int tar_x=enemies[j]->x;
                            int tar_y=enemies[j]->y;
                            for(int i=0;i<enemies.size();i++)
                            {
                               if(enemies[i]->x==tar_x&&enemies[i]->y==tar_y)
                               {
                                  enemies[i]->get_acked(t->ack);
                                  if(t->ice_num()>0)
                                    enemies[i]->set_ice_acked(true);
                               }
                            }

                        }

                        t->set_isattacking(true);
                        t->set_fire();
                        flag=true;
                        break;
                 }
             }
        if(!flag)
            t->set_isattacking(false);
    }


    t->show();
}






void Showtable::game_over()
{
    QString showstr="目前生命值为:"+QString::number(life);
    lifeshow->setText(showstr);
    if(life<=0)
    {
            TotalTime->stop();
            loseshow->show();
            reset_game();

    }
}

void Showtable::weapon_action()
{

    for(int i=0;i<weapons.size();i++)
    {
        //删除weapon
        if(weapons[i]->whether_die())
        {
            //对敌人造成伤害
            assert(i<target_index.size());
            if(target_index[i]!=-1&&(!weapons[i]->is_group()))
            {
                assert(target_index[i]>=0);
                assert(target_index[i]<enemies.size());
                enemies[target_index[i]]->get_acked(weapons[i]->get_ack());

                //减速
                if(weapons[i]->is_ice()){
                    enemies[target_index[i]]->set_ice_acked(true);
                }
                if(weapons[i]->is_blood())
                {
                    enemies[target_index[i]]->set_blood_acked(true);
                }
            }
            //判断是否是群攻
            if(weapons[i]->is_group()&&target_index[i]!=-1)
            {
                //得到敌人的逻辑坐标
                assert(target_index[i]>=0);
                assert(target_index[i]<enemies.size());
                int tar_x=enemies[target_index[i]]->x;
                int tar_y=enemies[target_index[i]]->y;
                for(int j=0;j<enemies.size();j++)
                {
                    if(enemies[j]->x==tar_x&&enemies[j]->y==tar_y)
                    {
                       enemies[j]->get_acked(weapons[i]->get_ack());
                       if(weapons[i]->is_ice())
                            enemies[j]->set_ice_acked(true);
                       if(weapons[i]->is_blood())
                            enemies[j]->set_blood_acked(true);
                    }
                }
            }
            //播放音乐
            play_bang();
            //删除weapon
            delete weapons[i];
            weapons[i]=nullptr;
            weapons.erase(weapons.begin()+i);

            target_index.erase(target_index.begin()+i);//删除map对应键值

            i--;
            continue;
        }

        //运行weapon
        if(target_index[i]==-1)
            weapons[i]->showpic(-1,-1);
        else
        {
            assert(target_index[i]>=0);
            assert(target_index[i]<enemies.size());
            vector<int>loc=enemies[target_index[i]]->get_logical_xy();

            int px=mymap.get_pix_x(loc[0],loc[1]);
            int py=mymap.get_pix_y(loc[0],loc[1]);
            weapons[i]->showpic(px,py);

        }
    }
}
void Showtable::play_bang(){
    assert(weapons_sound.size()==20);
    for(int i=0;i<20;i++)
    {
        if(!weapons_sound[i]->isPlaying())
        {
            weapons_sound[i]->play();
            break;
        }
    }
}

void Showtable::hiderange()
{
    if(global_tower_x!=-1&&global_tower_y!=-1)
    {
        for(int i=0;i<towers.size();i++)
        {
            if(towers[i]->x==global_tower_x &&\
               towers[i]->y==global_tower_y)
            {
                towers[i]->hiderange();
                break;
            }
        }
    }
}

void Showtable::hide_affixset()
{
    dismantle->hide();
    levelupbtn->hide();
    for(int i=0;i<tower_affix_show.size();i++)
    {
        tower_affix_show[i]->hide();
    }

}
void Showtable::Show_affix_slot(int lx,int ly)//展示安装词缀的槽
{

    //检查是否有塔
    if(lx<0||lx>=7||ly<0||ly>=14)
        return ;
    if(!has_tower(lx,ly))
         return;

    //寻找塔防下标
    int tower_index=-1;
    for(int i=0;i<towers.size();i++)
    {
        if(towers[i]->x==lx &&towers[i]->y==ly)
        {
            tower_index=i;
            break;
        }
    }
    assert(tower_index>=0);
    assert(tower_index<towers.size());
    std::vector<int>cur_affixes=towers[tower_index]->get_affixes();
    QPixmap pix;
    for(int i=0;i<cur_affixes.size();i++)
    {
        switch(cur_affixes[i])
        {
            case -1:
                pix.load(AFFIX_LOAD_PATH);
                break;
            case blood:
                pix.load(BLOOD_LABEL_PATH);
                break;
            case ice:
                pix.load(ICE_LABEL_PATH);
                break;
            case rampage:
                pix.load(RAMPAGE_LABEL_PATH);
                break;
            case groupinjury:
                pix.load(GROUP_LABEL_PATH);
                break;
        }
        tower_affix_show[i]->setPixmap(pix);
        tower_affix_show[i]->show();
    }


    dismantle->show();

    levelupbtn->show();
    assert(tower_index>=0);
    assert(tower_index<towers.size());
    if(towers[tower_index]->level==3)
        levelupbtn->setEnabled(false);

    if(score<100)
        levelupbtn->setEnabled(false);

    hiderange();//隐藏上一个范围
    towers[tower_index]->showrange();
    global_tower_x=lx;
    global_tower_y=ly;
}

void Showtable::change_Affix(int index)
{
    QPixmap pix;
    switch(global_affix_index)
    {
        case -1:
            pix.load(AFFIX_LOAD_PATH);
            break;
        case blood:
            pix.load(BLOOD_LABEL_PATH);
            break;
        case ice:
            pix.load(ICE_LABEL_PATH);
            break;
        case rampage:
            pix.load(RAMPAGE_LABEL_PATH);
            break;
        case groupinjury:
            pix.load(GROUP_LABEL_PATH);
            break;
    }
    tower_affix_show[index]->setPixmap(pix);
}
void Showtable::Add_Affix(int x,int y)
{
  //找寻对应塔防
  int loc=-1;
  for(int i=0;i<towers.size();i++)
  {
    if(towers[i]->x==global_tower_x&&\
       towers[i]->y==global_tower_y)
    {
        loc=i;break;
    }

  }
  bool is_change=false;
  assert(loc>=0);
  assert(loc<towers.size());
 //点击第一个词缀
  if(x>=625&&x<=625+50&&y>=750&&y<=750+50)
  {
     if(!towers[loc]->has_affix(0))
     {
         towers[loc]->set_affix(0,global_affix_index);
         change_Affix(0);
         is_change=true;
     }
  }
  else if(x>=725&&x<=725+50&&y>=750&&y<=750+50)
 //点击第二个词缀
  {
    if(!towers[loc]->has_affix(1))
    {
        towers[loc]->set_affix(1,global_affix_index);
        change_Affix(1);
        is_change=true;
    }
  }


  if(is_change)
  {
    assert(global_affix_index<affixes_numbers.size());
    assert(global_affix_index>=0);
    //消耗了一个词缀
    affixes_numbers[global_affix_index]--;

    //删除全局指针
    delete global_affix;
    global_affix=nullptr;
    global_affix_index=-1;
  }

}

void Showtable::Delete_affix(int x,int y)
{
    //找寻对应塔防
    int loc=-1;
    for(int i=0;i<towers.size();i++)
    {
      if(towers[i]->x==global_tower_x&&\
         towers[i]->y==global_tower_y)
      {
          loc=i;break;
      }
    }
    assert(loc>=0);
    assert(loc<towers.size());
   //点击第一个词缀
    if(x>=625&&x<=625+50&&y>=750&&y<=750+50)
    {
       if(towers[loc]->has_affix(0))
       {
           towers[loc]->set_affix(0,-1);
           change_Affix(0);
       }
    }
    else if(x>=725&&x<=725+50&&y>=750&&y<=750+50)
   //点击第二个词缀
    {
      if(towers[loc]->has_affix(1))
      {
          towers[loc]->set_affix(1,-1);
          change_Affix(1);
      }
    }
}






























































 //监听事件
 void Showtable::mousePressEvent(QMouseEvent *e)
 {
     int x=e->x();int y=e->y();
     int logical_x=mymap.translate_px2lx(y);
     int logical_y=mymap.translate_py2ly(x);




     if(e->button()==Qt::LeftButton)
     {
         if(is_initial_map)
         {
             if(inditator_block!=nullptr){
                    bool success=change_block(logical_x,logical_y);
             }
         }
         if(is_play_game)
         {
             //1安装塔防
             if(indicater_tower!=nullptr)
             {

                 try{
                 Add_Tower(tower_type,mymap.translate_px2lx(y),mymap.translate_px2lx(x));
                 }
                 catch(int a)
                 {
                      if(a==0)
                      {
                        delete indicater_tower;
                        indicater_tower=nullptr;
                        tower_type=-1;
                      }
                 }
                 return;
             }

             else if(global_affix!=nullptr)
             {

                Add_Affix(x,y);
             }
             else
             {
                 //2点击塔防，显示词缀信息。
               Show_affix_slot(logical_x,logical_y);
             }
         }
     }


     else if(e->button()==Qt::RightButton)
     {
         if(is_initial_map)
         {
             Hide_block(logical_x,logical_y);
             Give_up_inditator(logical_x,logical_y);
         }
         if(is_play_game)
         {
            if(indicater_tower!=nullptr){
                Give_up_tower_inditator(logical_x,logical_y);
            }
            else if(global_affix!=nullptr){
                Give_up_affix_inditator();
            }
            Delete_affix(x,y);
            hiderange();

         }
     }
 }


void Showtable::mouseMoveEvent(QMouseEvent *event){
     int x=event->x();int y=event->y();
     if(is_initial_map)
     {
         if(inditator_block!=nullptr)
         {
                 inditator_block->setGeometry(x-BLOCK_WIDTH/3,y-BLOCK_HEIGHT/3,90,90);
                 inditator_block->show();
         }
     }

     if(is_play_game)
     {
        if(indicater_tower!=nullptr)
        {

                indicater_tower->setGeometry(x-40,y-70,80,70);
                indicater_tower->setParent(this);
                indicater_tower->show();
        }
        else if(global_affix!=nullptr)
        {
            global_affix->setGeometry(x-50,y-50,50,50);
            //indicater_tower->setParent(this);
            global_affix->show();
        }
     }
 }

void Showtable::keyPressEvent(QKeyEvent *e)
{//游戏结束，任意键返回主界面;过程中按Esc
    if(life<=0||e->key()==Qt::Key_Escape)
    {
            loseshow->hide();
            emit over();
    }

}





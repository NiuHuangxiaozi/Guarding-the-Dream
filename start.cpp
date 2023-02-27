#include "start.h"
#include "ui_start.h"

Start::Start(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Start)
{
    //1初始化界面
    ui->setupUi(this);
    this->setFixedSize(MAIN_WINDOW_WIDTH,MAIN_WINDOW_HEIGHT);
    this->setMouseTracking(true);

    showtable=new Showtable();

    //2初始化按钮
    init_start_btn();

     //3返回按钮
    init_back_btn();

}

Start::~Start()
{
    delete ui;
}

void Start::init_start_btn()
{
        //1、初始化按钮。
        mypushbutton * startbtn=new mypushbutton(MAIN_WINDOW_BTN_PATH);
        startbtn->setParent(this);
        startbtn->move(this->width()*0.5-startbtn->width()*0.5,this->height()*0.8);

        //2、开始按钮的行为
        connect(startbtn,&QPushButton::clicked,this,[=](){

                    startbtn->downBounce();//下弹
                    startbtn->upBounce();//上弹

                    //延时进入
                    QTimer::singleShot(600,this,[=](){
                        this->hide();
                        showtable->set();
                        showtable->show();
                            });
                });
}

void Start::init_back_btn()
{
    //返回到开始界面的信号
    connect(showtable,&Showtable::back_to_mainwindow,this,[=](){
                showtable->hide();
                this->show();
            });

    //返回到开始界面的信号
    connect(showtable,&Showtable::game_to_mainwindow,this,[=](){
                showtable->hide();
                this->show();
            });
    //游戏结束返回
    connect(showtable,&Showtable::over,this,[=](){
                showtable->hide();
                this->show();
            });
}

//绘制背景
void Start::paintEvent(QPaintEvent *){

        QPainter mypainter(this);
        QPixmap pix;
        pix.load(MAIN_WINDOW_BACKGROUND_PATH);
        mypainter.drawPixmap(0,0,this->width(),this->height(),pix);

        pix.load(MAIN_WINDOW_THEME_PATH);
        mypainter.drawPixmap(this->width()*0.5-pix.width()*0.5,this->height()*0.35,pix);
}


#ifndef START_H
#define START_H

#include <QMainWindow>
#include "config.h"
#include <QPainter>
#include "mypushbutton.h"
#include <QTimer>
#include "showtable.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Start; }
QT_END_NAMESPACE

class Start : public QMainWindow
{
    Q_OBJECT

public:
    Start(QWidget *parent = nullptr);
    ~Start();
    void paintEvent(QPaintEvent *);

    void init_start_btn();//初始化开始按钮
    void init_back_btn();//初始化返回按钮

private:
    Ui::Start *ui;
    Showtable *showtable;//展示的类
};
#endif // START_H

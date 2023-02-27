#ifndef AFFIXLABEL_H
#define AFFIXLABEL_H

#include "config.h"
#include <QLabel>
#include <QWidget>
#include  "mypushbutton.h"

enum affixtype{
    blood=0,
    ice=1,
    rampage=2,
    groupinjury=3
};

class Affixlabel
{
    friend class Showtable;
private:
    int px;
    int py;
    int type;
    bool is_die;

    int showtime;//展示的时间，如果一段时间用户不点就会消失
public:
    Affixlabel(QWidget * parent=nullptr);
    Affixlabel(int type,int px,int py,QWidget * parent);
    ~Affixlabel();


    int get_type();

    bool whether_die();

};

#endif // AFFIXLABEL_H

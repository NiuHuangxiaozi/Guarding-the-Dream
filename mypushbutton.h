#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QDebug>
#include <QPropertyAnimation>
class mypushbutton : public QPushButton
{
    Q_OBJECT
public:
        mypushbutton(QString normalimg,QString pressImg ="");
        void myset(QString normalimg,QString pressImg ="");
        QString normalimgpath;
        QString pressImgpath;
        void downBounce();
        void upBounce();
        void mousePressEvent(QMouseEvent *e);
        void mouseReleaseEvent(QMouseEvent *e);
signals:

};


#endif // MYPUSHBUTTON_H

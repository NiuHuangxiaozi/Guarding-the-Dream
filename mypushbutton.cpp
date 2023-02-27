#include "mypushbutton.h"

 void mypushbutton::myset(QString normalimg,QString pressImg)
 {
     this->normalimgpath=normalimg;
     this->pressImgpath=pressImg;
     QPixmap pix;
     bool ret=pix.load(normalimg);
     if(ret==false){
         qDebug()<<"图片加载失败";
         return;
     }
     this->setFixedSize(pix.width(),pix.height());
     this->setStyleSheet("QPushButton{border:0px;}");
     //设置图标
     this->setIcon(pix);
     this->setIconSize(pix.size());
 }
mypushbutton::mypushbutton(QString normalimg,QString pressImg){
    this->normalimgpath=normalimg;
    this->pressImgpath=pressImg;
    if(normalimg=="")
        return;
    QPixmap pix;
    bool ret=pix.load(normalimg);
    if(ret==false){
        qDebug()<<"图片加载失败";
        return;
    }
    this->setFixedSize(pix.width(),pix.height());
    this->setStyleSheet("QPushButton{border:0px;}");
    //设置图标
    this->setIcon(pix);
    this->setIconSize(pix.size());
}
void mypushbutton::downBounce(){
    // 1. 创建动态对象
    QPropertyAnimation* ani = new QPropertyAnimation(this, "geometry");

    // 2. 设置动画时间间隔
    ani->setDuration(200);

    // 3. 起始位置
    ani->setStartValue(QRect(this->x(), this->y(), this->width(), this->height()));

    // 4. 结束位置
    ani->setEndValue(QRect(this->x(), this->y()+10, this->width(), this->height()));

    // 5. 设置弹跳曲线
    ani->setEasingCurve(QEasingCurve::OutBounce);

    // 6. 执行动画
    ani->start();
}
void mypushbutton::upBounce(){
    // 1. 创建动态对象
    QPropertyAnimation* ani = new QPropertyAnimation(this, "geometry");

    // 2. 设置动画时间间隔
    ani->setDuration(200);

    // 3. 起始位置
    ani->setStartValue(QRect(this->x(), this->y()+10, this->width(), this->height()));

    // 4. 结束位置
    ani->setEndValue(QRect(this->x(), this->y(), this->width(), this->height()));

    // 5. 设置弹跳曲线
    ani->setEasingCurve(QEasingCurve::OutBounce);

    // 6. 执行动画
    ani->start();
}
void mypushbutton::mousePressEvent(QMouseEvent *e){
    if(this->pressImgpath != ""){
//        this->setStyleSheet("background: rgb(255,150,128)");
        QPixmap pix;
        bool res = pix.load(this->pressImgpath);
        if(!res){
            qDebug() << pressImgpath << "图片加载失败！";
            return;
        }

        //this->setFixedSize(pix.size());

        this->resize(pix.width(), pix.height());

        this->setStyleSheet("QPushButton{border:0px;}");

        this->setIcon(pix);

        this->setIconSize(pix.size());
    }

    return QPushButton::mousePressEvent(e);
}
void mypushbutton::mouseReleaseEvent(QMouseEvent *e){
    if(this->pressImgpath != ""){
//        this->setStyleSheet("background-color: rgb(0,0,255)");
        QPixmap pix;
        bool res = pix.load(this->normalimgpath);
        if(!res){
            qDebug() << normalimgpath << "图片加载失败！";
            return;
        }

        //this->setFixedSize(pix.size());

        this->resize(pix.width(), pix.height());

        this->setStyleSheet("QPushButton{border:0px;}");

        this->setIcon(pix);

        this->setIconSize(pix.size());
    }

    return QPushButton::mouseReleaseEvent(e);
}










#include "affixlabel.h"

Affixlabel::Affixlabel(QWidget * parent)
{

}
Affixlabel::Affixlabel(int type,int px,int py,QWidget * parent)
{
    this->type=type;
    this->px=px;
    this->py=py;
}

int Affixlabel::get_type()
{
     return type;
}

bool Affixlabel::whether_die()
{
   return is_die==true;
}
Affixlabel::~Affixlabel()
{
}

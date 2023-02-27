#include "tower.h"

Tower::Tower(int x,int y,int type,int px,int py,QWidget *father)
{
    this->x=x;
    this->y=y;
    this->type=type;
    this->px=px;
    this->py=py;

    isattack=false;

    is_die=false;//判断塔是否被摧毁

    affixes.resize(TOWER_AFFIX_NUM,-1);//初始没有任何词缀

    QPixmap pic;
    pic.load(RANGE_PATH);
    round.setPixmap(pic);
    round.setParent(father);
    round.hide();
    round.setScaledContents(true);

}
void  Tower::showrange()
{
    if(range==0)
    {
        round.setGeometry(py,px,100,100);
        round.show();
    }
    else if(range==1)
    {
        round.setGeometry(py-100,px-100,300,300);
        round.show();
    }
    else if(range==2)
    {
        round.setGeometry(py-200,px-200,500,500);
        round.show();
    }
    else if(range==3)
    {
        round.setGeometry(py-300,px-300,700,700);
        round.show();
    }
}
void Tower::hiderange()
{
    round.hide();
}
bool Tower::is_in_range(int x,int y)
{
    int low_x=this->x-range;
    int high_x=this->x+range;
    int low_y=this->y-range;
    int high_y=this->y+range;
    return  x>=low_x &&x<=high_x &&y>=low_y &&y<=high_y;
}
void Tower::set_affix(int index,int affix_index)//添加相应的词缀
{
   affixes[index]=affix_index;
}
bool Tower::has_affix(int loc)//判断相应位置相应的词缀是否存在
{
    assert(loc<TOWER_AFFIX_NUM);
    return affixes[loc]!=-1;
}


int Tower::rampage_num()
{
    int count=0;
    for(int i=0;i<affixes.size();i++)
    {
        if(affixes[i]==rampage)
              count++;
    }
    return count;
}
int Tower::group_ack_num()
{
    int count=0;
    for(int i=0;i<affixes.size();i++)
    {
        if(affixes[i]==groupinjury)
              count++;
    }
    return count;
}
int Tower::ice_num()
{
    int count=0;
    for(int i=0;i<affixes.size();i++)
    {
        if(affixes[i]==ice)
              count++;
    }
    return count;
}
int Tower::blood_num()
{
    int count=0;
    for(int i=0;i<affixes.size();i++)
    {
        if(affixes[i]==blood)
              count++;
    }
    return count;
}


bool Tower::is_fire()
{
    int rampagenum=rampage_num();
    int real_shooter_interval=shooter_interval;
    if(rampagenum!=0)
        real_shooter_interval=shooter_interval/(2*rampage_num());
    return shooter_count==real_shooter_interval;
}
void Tower::set_fire()
{
  has_fire=true;
}
void Tower::get_acked(int value)
{
  if(hp-value>=0)
      hp-=value;
  else
      hp=0;
}
int Tower::get_ack()
{
    return ack+rampage_num()*5;
}







//1 、magictower
magicTower::magicTower(int x,int y,int type,int px,int py,QWidget *father)
    :Tower(x,y,type,px,py,father)
{
  ack=10;
  range=1;

  level=1;

  movie.setFileName(MAGIC_TOWER_PATH);
  movie.start();
  label.setMovie(&movie);
  label.setGeometry(py+5,px+10,100 ,80);
  label.setParent(father);
  label.show();


  shooter_count=0;
  shooter_interval=1000;

  movie_level2.setFileName(MAGIC_TOWER1_PATH);
  movie_level3.setFileName(MAGIC_TOWER2_PATH);

}
void magicTower::showimage()
{
    if(level==2)
    {
        label.setMovie(&movie_level2);
        movie_level2.start();
    }
    else if(level==3)
    {
        label.setMovie(&movie_level3);
        movie_level3.start();
    }
}
void magicTower::show()
{
}
void magicTower::interval_increase()
{
    int rampagenum=rampage_num();
    int real_shooter_interval=shooter_interval;
    if(rampagenum!=0)
        real_shooter_interval=shooter_interval/(2*rampagenum);
    if(shooter_count>real_shooter_interval)
    {
        shooter_count=real_shooter_interval;
    }
    if(shooter_count<real_shooter_interval)
    {
         shooter_count++;
    }
    if(shooter_count==real_shooter_interval&&has_fire==true){
        has_fire=false;
        shooter_count=0;
    }
}
void magicTower::levelup()//升级的函数
{
    if(level==1)
    {
        level++;
        range++;
        ack+=5;
    }
    else if(level==2)
    {
        level++;
        range++;
        ack+=5;
    }
    showrange();
}




//2armtower
armyTower::armyTower(int x,int y,int type,int px,int py,QWidget *father)
    :Tower(x,y,type,px,py,father)
{
  //1
  ack=20;
  range=0;
  level=1;
  state=0;//初始状态

  max_hp=1000;
  hp=max_hp;


//2
  movie.setFileName(ARMY_PATH);
  movie.start();

  attackmovie.setFileName(ARMY_ATTACK_PATH);//攻击的动画


  label.setMovie(&movie);
  label.setGeometry(py+5,px+10,100 ,80);
  label.setParent(father);
  label.show();


  shooter_count=0;
  shooter_interval=1500;

  //升级的gif
  movie_level2.setFileName(ARMY1_PATH);
  movie_level3.setFileName(ARMY2_PATH);
}

void armyTower::show()
{
    change_hp();
    if(level==1){
        switch(state)
        {
            case 0:
                if(isattack){
                    qDebug()<<"attack";
                    movie.stop();
                    attackmovie.start();
                    label.setMovie(&attackmovie);
                    state=1;
                }
                break;
            case 1:
                if(!isattack)
                {
                    attackmovie.stop();
                    movie.start();
                    label.setMovie(&movie);
                    state=0;
                }
                break;
        }
    }
}
void armyTower::interval_increase()
{
    int rampagenum=rampage_num();
    int real_shooter_interval=shooter_interval;
    if(rampagenum!=0)
        real_shooter_interval=shooter_interval/(2*rampagenum);
    if(shooter_count>real_shooter_interval)
    {
        shooter_count=real_shooter_interval;
    }
    if(shooter_count<real_shooter_interval)
    {
         shooter_count++;
    }
    if(shooter_count==real_shooter_interval&&has_fire==true){
        has_fire=false;
        shooter_count=0;
    }
}
void armyTower::change_hp()
{
    if(hp==0)
    {
       is_die=true;
    }
}
void armyTower::levelup()//升级的函数
{
    if(level==1)
    {
        level++;
        ack+=10;
    }
    else if(level==2)
    {
        level++;
        ack+=10;
    }
}
void armyTower::showimage()
{
    if(level==2)
    {
        label.setMovie(&movie_level2);
        movie_level2.start();
    }
    else if(level==3)
    {
        label.setMovie(&movie_level3);
        movie_level3.start();
    }
}

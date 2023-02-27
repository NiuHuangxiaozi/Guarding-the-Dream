#include "enemy.h"

Enemy::Enemy(int x,int y,int px,int py,int ack,int speed,\
             int type,int max_hp,int dir,int route_num,int max_block_num,QWidget *parent)
{
    this->x=x;
    this->y=y;
    this->px=px;
    this->py=py;
    this->ack=ack;
    this->speed=speed;
    this->type=type;

    this->max_hp=max_hp;
    this->hp=max_hp;

    this->dir=dir;

    this->route_num=route_num;//表示走哪条路线

    this->block_num=0;

    this->max_block_num=max_block_num;//route一共的block数量，用于定位敌人（sort函数）



     //widget继承
     //this->setParent(parent);
     //this->show();

    qpr.setOrientation(Qt::Horizontal);  // 水平方向
    qpr.setMinimum(0);  // 最小值
    qpr.setMaximum(max_hp);  // 最大值
    qpr.setValue(hp);  // 当前进度
    qpr.setGeometry(py+10,px,80,5);
    qpr.setTextVisible(false);
    qpr.setParent(parent);
    qpr.show();



    //词缀的初始化
    QString affix_str;
    affix_types.resize(2,0);
    int not_normal=0;
    for(int index=0;index<affix_types.size();index++)
    {
        srand((int)time(0));
        affix_types[index]=(rand())%2;//是否拥有这个词缀特性
        if(affix_types[index]==1)
        {
            not_normal=1;
            switch(index)
            {
                case fast:
                    affix_str+="|Fast|\n";
                    break;
                case flash:
                    affix_str+="|Flash|\n";
                    break;
            }
        }
    }
    if(not_normal==1){
        affixshow.setText(affix_str);
        QFont ft;
        ft.setPointSize(6);
        affixshow.setFont(ft);
        affixshow.setGeometry(py,px,50,50);
        affixshow.setParent(parent);
        affixshow.raise();
        affixshow.show();
    }


    //塔防词缀的初始化
    tower_affix_type=rand()%4;
}

void Enemy::Get_position(int &block_num,int & route_index,int & x,int & y,int & px,int & py,int &dir,int &speed)
{
    x=this->x;
    y=this->y;
    px=this->px;
    py=this->py;
    dir=this->dir;
    speed=this->speed;
    route_index=this->route_num;
    block_num=this->block_num;
}
void Enemy::Set_position(int block_num,int x,int y,int px,int py,int dir)
{
    this->x=x;
    this->y=y;
    this->px=px;
    this->py=py;
    this->block_num=block_num;

    //如果方向发生了改变那么我们就要换gif
    if(dir!=this->dir)
        is_change_dir=true;
    this->dir=dir;

    if(this->dir==0)
    {
        achieved=true;
    }
}
std::vector<int> Enemy::get_logical_xy()
{
    std::vector<int> answer;
    answer.push_back(x);
    answer.push_back(y);
    return answer;
}
std::vector<int> Enemy::get_physical_xy()
{
    std::vector<int> answer;
    answer.push_back(px);
    answer.push_back(py);
    return answer;
}
void Enemy::get_acked(int value)
{
    if(hp-value>=0)
        hp-=value;
    else
        hp=0;
}






//
Boar::Boar(int x,int y,int px,int py,int ack,\
        int speed,int type,int max_hp,int dir,\
           int route_num,int max_block_num,QWidget *parent):
    Enemy(x,y,px,py,ack,speed,type,max_hp,dir,route_num,max_block_num,parent)
{
    //设置普通的图片
    movie_walk.setFileName(ENEMY_WALK);
    movie_walk_left.setFileName(ENEMY_LEFT_WALK);
    movie_up.setFileName(ENEMY_UP);
    movie_attack.setFileName(ENEMY_ATTACK);
    movie_left_attack.setFileName(ENEMY_LEFT_ATTACK);
    movie_die.setFileName(ENEMY_DIE);
    movie_die_left.setFileName(ENEMY_LEFT_DIE);

    //设置冰冻的图片
    //movie_walk_ice.setFileName(ENEMY_WALK_ICE);;
    //movie_walk_left_ice.setFileName(ENEMY_LEFT_WALK_ICE);;
    //movie_up_ice.setFileName(ENEMY_UP_ICE);;
    //movie_attack_ice.setFileName(ENEMY_ATTACK_ICE);
    //movie_left_attack_ice.setFileName(ENEMY_LEFT_ATTACK_ICE);
    //movie_die_ice.setFileName(ENEMY_DIE_ICE);;
    //movie_die_left_ice.setFileName(ENEMY_LEFT_DIE_ICE);
    ice_label.setParent(parent);

    //流血字体的设置
    blood_label.setParent(parent);
    blood_label.hide();
    blood_label.setGeometry(py+100,px+10,40,40);

    switch(dir)
    {
        case east:
               movie_walk.start();
               label.setMovie(&movie_walk);
               label.setGeometry(py+2,px+10,100 ,88);
               label.setParent(parent);
               label.show();
               break;
        case west:
                movie_walk_left.start();
                label.setMovie(&movie_walk_left);
                label.setGeometry(py+2,px+10,100 ,88);
                label.setParent(parent);
                label.show();
                break;
        case south:
                movie_up.start();
                label.setMovie(&movie_up);
                label.setGeometry(py+2,px+10,100 ,88);
                label.setParent(parent);
                label.show();
                break;
        case north:
                movie_up.start();
                label.setMovie(&movie_up);
                label.setGeometry(py+2,px+10,100 ,88);
                label.setParent(parent);
                label.show();
                break;
    }
    walk_count=0;
    walk_interval=45;

    //攻击间隔
    attack_count=1000;
    attack_inteval=1000;

    //闪现间隔
    flash_interval=2000;
    flash_count=0;


    ice_count=0;
    ice_interval=3000;

    blood_count=0;
    blood_interval=500;
}


void Boar::skill_cd()
{
    if(flash_count<flash_interval)
        flash_count++;
}
bool Boar::is_flash()
{
    return flash_count==flash_interval;
}
void Boar::reset_flash()
{
    flash_count=0;
}


void Boar::move_forward(int dis)
{
    switch(dir)
    {
        case east:
           py+=dis;
           break;
        case west:
           py-=dis;
           break;
        case south:
           px+=dis;
           break;
        case north:
           px-=dis;
           break;
    }
}
void Boar::showimage()
{
    label.setGeometry(py,px,100 ,88);
    label.show();
    qpr.setGeometry(py+10,px,80,5);
    qpr.show();
    affixshow.setGeometry(py,px,50,50);

}
void Boar::move_head(QWidget *parent)
{

    if(walk_count==walk_interval)
    {
        int increase_walk_num=0;
        int current_speed=speed;
        //判断是否拥有fast词缀
        if(has_fast())
            current_speed=speed*2;
        if(block_walk_num+current_speed>100)
              increase_walk_num=100-block_walk_num;
        else
              increase_walk_num=current_speed;

        move_forward(increase_walk_num);


        block_walk_num+=increase_walk_num;

        showimage();

        walk_count=0;
    }
    else
    {
        walk_count++;
    }
}
void Boar::close_all_movie()
{
    movie_walk.stop();
    movie_walk_left.stop();
    movie_up.stop();
    movie_attack.stop();
    movie_die.stop();
    movie_die_left.stop();

    //movie_walk_ice.stop();
    //movie_walk_left_ice.stop();
    //movie_up_ice.stop();
    //movie_attack_ice.stop();
    //movie_die_ice.stop();
    //movie_die_left_ice.stop();
}
void Boar::change_direction(QWidget *parent)
{
    if(is_change_dir)
    {
        switch(dir)
        {
            case east:
                   close_all_movie();
                   movie_walk.start();
                   label.setMovie(&movie_walk);
                   label.setGeometry(py,px,100 ,88);
                   //label.setParent(parent);
                   //label.show();
                   break;
            case west:
                    close_all_movie();
                    movie_walk_left.start();
                    label.setMovie(&movie_walk_left);
                    label.setGeometry(py,px,100 ,88);
                    //label.setParent(parent);
                    //label.show();
                    break;
            case south:
                    close_all_movie();
                    movie_up.start();
                    label.setMovie(&movie_up);
                    label.setGeometry(py,px,100 ,88);
                    //label.setParent(parent);
                    //label.show();
                    break;
            case north:
                    close_all_movie();
                    movie_up.start();
                    label.setMovie(&movie_up);
                    label.setGeometry(py,px,100 ,88);
                    //label.setParent(parent);
                    //label.show();
                    break;
        }
        is_change_dir=false;
    }
}
void Boar::change_hp()
{
    double rate=(double)hp/(double)max_hp;
    qpr.setValue(100*rate);
}
void Boar::showenemy(QWidget *parent)
{

   if(is_blood_acked==true){
      blood_action(parent);
   }
   if(is_ice_acked==true){
        ice_action(parent);
   }
   normal_action(parent);
}
void Boar::ice_time_manage()
{
    if(ice_count<ice_interval)
        ice_count++;
    else
    {
        ice_count=0;is_ice_acked=false;
    }
}
void Boar::change()
{
    switch(state)
    {
      case 0:
        change_to_walk();
        break;
      case 1:
        change_to_attack();
        break;

    }
}
void Boar::normal_action(QWidget *parent)
{
    skill_cd();//不管什么时候都要更新技能cd
    switch(state)
    {
       case 0://正常行走状态
            change_hp();
            if(hp==0)
            {
                close_all_movie();
                change_to_die();
                state=2;
                return;
            }
            change_direction(parent);
            move_head(parent);
            if(meet_tower==true)
            {
                  if(is_flash()&&has_flash())
                  {
                      block_walk_num=100;
                      move_forward(100);
                      showimage();
                      reset_flash();
                      return;
                  }
                  close_all_movie();
                  change_to_attack();
                  state=1;
            }
            break;
         case 1://遇到塔，攻击
            change_hp();
            if(hp==0)
            {
                close_all_movie();
                change_to_die();
                state=2;
                return;
            }
            if(meet_tower==false)
            {
                  change_direction(parent);
                  state=0;
            }
            break;

         case 2://播放死亡
              check_death();
              break;
    }
}
void Boar::ice_action(QWidget * parent)
{
    QPixmap pix;
    switch(ice_state)
    {
        case 0:
            pix.load(ICE);
            ice_label.setPixmap(pix);
            ice_label.setGeometry(py,px+50,68,50);
            ice_label.raise();
            ice_label.show();

            walk_count=0;
            walk_interval*=2;

            ice_state=1;
            break;
        case 1:
            if(ice_count<ice_interval)
            {
               ice_label.setGeometry(py,px+50,68,50);
               ice_label.raise();
               ice_label.show();
               ice_count++;
            }
            else
            {
                ice_count=0;
                ice_label.hide();

                walk_count=0;
                walk_interval=walk_interval/2;

                ice_state=0;
                is_ice_acked=false;
            }
            break;
    }

}
void Boar::blood_action(QWidget * parent)
{
   switch(blood_state)
   {
        case 0://准备显示图片
           if(blood_count<blood_interval){
               blood_count++;
           }
           else
           {
                blood_label.setText("-30");
                blood_label.setGeometry(py+100,px+10,40,40);
                blood_label.show();
                get_acked(30);

                blood_count=0;
                blood_state=1;
           }
            break;
        case 1://第一次流血
            if(blood_count<blood_interval){
                blood_label.setGeometry(py+100,px+10,40,40);
                blood_label.show();
                blood_count++;
            }
            else
            {
                blood_count=0;
                blood_label.hide();
                blood_state=2;
            }
            break;
        case 2://空闲状态
           if(blood_count<blood_interval)
               blood_count++;
           else
           {
               blood_count=0;
               blood_label.setText("-15");
               blood_label.show();
               get_acked(15);
               blood_state=3;
           }
            break;
        case 3://恢复原始状态
               if(blood_count<blood_interval){
                   blood_label.setGeometry(py+100,px+10,40,40);
                   blood_label.show();
                   blood_count++;
               }
               else
               {
                   blood_count=0;
                   blood_label.hide();
                   blood_state=4;
               }
               break;
         case 4://
               if(blood_count<blood_interval){
                   blood_count++;
               }
               else
               {
                   blood_count=0;
                   blood_label.setText("-5");
                   blood_label.show();
                   get_acked(5);
                   blood_state=5;
                }
                break;
          case 5:
               if(blood_count<blood_interval)
               {
                   blood_label.setGeometry(py+100,px+10,40,40);
                   blood_label.show();
                   blood_count++;
               }
               else
               {
                   blood_count=0;
                   blood_label.hide();
                   blood_state=0;
                   is_blood_acked=false;
               }
               break;
   }
}
bool Boar::interval_increase()
{
            if(attack_count<attack_inteval){
                 attack_count++;
                 return false;
            }
            if(attack_count==attack_inteval){
                attack_count=0;
                return true;
            }
            return true;
}


void Boar::set_ice_acked(bool flag)
{
    ice_count=0;
    is_ice_acked=flag;

    walk_interval=10+walk_interval;//减速

    close_all_movie();
    change();
}

//改变图像的一些列函数
void Boar::change_to_die()
{
    label.setGeometry(py,px,100 ,88);
    if(dir==east)
    {
           movie_die.start();
           label.setMovie(&movie_die);
    }
    else
    {
           movie_die_left.start();
           label.setMovie(&movie_die_left);
    }
}
void Boar::change_to_attack()
{
    if(dir==east)
    {
           movie_attack.start();
           label.setMovie(&movie_attack);
    }
    else if(dir==west)
    {

        movie_left_attack.start();
        label.setMovie(&movie_left_attack);
    }
}
void Boar::check_death()
{
    if(dir==east)
    {
                is_die=true;
    }
    else
    {
                is_die=true;
    }
}
void Boar::change_to_walk()
{
    if(dir==east){
           movie_walk.start();
           label.setMovie(&movie_walk);
    }
    else if(dir==west)
    {
               movie_walk_left.start();
               label.setMovie(&movie_walk_left);
    }
}









void Boar::painthp(QPainter & painter)
{
    QBrush brush(QColor(255, 255, 255), Qt::Dense1Pattern);
    painter.setPen(QPen(Qt::white,4));
    brush.setColor(Qt::darkGreen);
    painter.setBrush(brush);
    painter.drawRect(py,px-10,52,4);
    painter.setPen(QPen(Qt::lightGray,3));
    painter.drawRect(py+1,px-9,50,2);
    painter.setPen(QPen(Qt::red,3));
    painter.drawRect(px+1,y-9,100,2);
}

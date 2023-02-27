#ifndef MYMAP_H
#define MYMAP_H
#include <QWidget>
#include <QDebug>
#include "config.h"
#include <string>
#include<vector>
#include<fstream>
#include <QPainter>
#include "mypushbutton.h"
#include<queue>
using namespace std;



enum types
{
    empty=0,
    road=1,
    grass=2,
    platform=3,
    origin=4,
    final=5
};


class Block//物理
{
    friend class myMap;
private:
    int is_used;
    int type;
    int pix_x;
    int pix_y;
    int pix_mid_x;
    int pix_mid_y;
public:
     int  get_type() const { return type;}
};

class Node//逻辑
{
    friend class myMap;
private:
    int x;
    int y;
    int direction;
public:
    Node() {};
    Node(int _x, int _y,int dir=0);
    void set_dir(int d);

};


class Dij
{

private:
    bool is_road_or_final(int x, int y, const vector<vector<Block>>& blocks);
    void  find(int x, int beginnum, vector<vector<int>>& answer);
    int pairtoint(int x, int y) { return x * 14 + y; }

    vector<int> intopair(int a) {
        vector<int>answer;
        answer.push_back(a / 14);
        answer.push_back(a % 14);
        return answer;
    };
public:
    vector<vector<bool>> is_visited;//访问设置
    vector<int>dis;//记录距离的容器
    vector<int>path;//记录路径的容器

    void dealwith(int newnum, int originx, int originy, const vector<vector<Block>>& blocks);

    void  create_map_path(vector<int>finalpairs, int originx, int originy, vector<vector<int>>& answer);
    bool is_connected(int finalx, int finaly);
};







class myMap : public QWidget
{
    Q_OBJECT
    enum dirtype{
       east=1,
       west=2,
       south=3,
       north=4
    };
private:
     string map_path="../tower/map.txt";
     vector<vector<Block>> blocks;

     int row_length;
     int col_length;

     int block_height;
     int block_width;

     vector<int>final_index;//终点的坐标们

     vector<vector<int>>begin_indexs;//起始点的坐标

     vector<vector<Node>> routes;//多条地图

     Dij dij;

     //funcs
     void init_map();
     void encode_map();

     bool is_final(int x,int y);

public:
        myMap();

        void output_map();
        void input_map();

        int get_type(int i,int j);

        int get_row(){return row_length;}
        int get_col(){return col_length;}


        int get_pix_x(int i,int j){return blocks[i][j].pix_x; };
        int get_pix_y(int i,int j){return blocks[i][j].pix_y;};
        int get_pix_mid_y(int i,int j){return blocks[i][j].pix_mid_y;};
        int get_pix_mid_x(int i,int j){return blocks[i][j].pix_mid_x;};

        int translate_px2lx(int x){return x/100;};
        int translate_py2ly(int y){return y/100;};

        void set_type(int empty,int x,int y){blocks[x][y].type=empty;}

        bool check_and_create_route();

        vector<Node> make_route(vector<vector<int>> s);//生成一条地图

        //关于敌人对于地图类的访问。
        int Get_entry_num();//获得敌人的出口数量

        void Get_entry_location(int index,int &x,int &y);//获得出口的坐标

        int Get_direction(int route_number,int num_index);//
        void change_location(int route_index,int &block_num,int &x,int &y,int &px,int &py,int &dir,int speed);

        int get_block_nums(int select_entry);//获得某一路线的网格数
};

















#endif // MYMAP_H

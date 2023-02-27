#include "mymap.h"


//node
void Node::set_dir(int d)
{
    direction = d;
}
Node::Node(int _x, int _y, int dir)
{
    x = _x;
    y = _y;
    direction = dir;
}




//block
myMap::myMap()
{
   block_height=BLOCK_HEIGHT;
   block_width=BLOCK_WIDTH;


   init_map();

   encode_map();
}
void myMap::encode_map()
{

    for(int row=0;row<row_length;row++){
        for(int col=0;col<col_length;col++){
            blocks[row][col].pix_x=row*block_width;
            blocks[row][col].pix_y=col*block_height;


            blocks[row][col].pix_mid_x=row*block_width+25;
            blocks[row][col].pix_mid_y=col*block_height+50;
        }
    }
}
void myMap::init_map()
{
   fstream File;
   File.open(map_path,ios::in);
   if(!File.is_open())
           qDebug()<<"map file not open";

   //读入地图大小
   File>>row_length>>col_length;

   blocks.resize(row_length);
   for(int row=0;row<row_length;row++)
   {
        blocks[row].resize(col_length);
   }

   //读入类型
   for(int row=0;row<row_length;row++){
       for(int col=0;col<col_length;col++){
           File>>blocks[row][col].type;
       }
   }

   //建立坐标与数字标号之间的关系。


}


void myMap::input_map()
{
    fstream File;
    File.open(map_path,ios::in);
    if(!File.is_open())
            qDebug()<<"map file not open";

    //读入地图大小
    File>>row_length>>col_length;

    //读入类型
    for(int row=0;row<row_length;row++){
        for(int col=0;col<col_length;col++){
            File>>blocks[row][col].type;
        }
    }
}
void myMap::output_map()
{
    fstream File;
    File.open(map_path,ios::out);
    if(!File.is_open())
            qDebug()<<"map file not open";

    //读入地图大小
    File<<row_length<<" "<<col_length<<endl;

    //读入类型
    for(int row=0;row<row_length;row++){
        for(int col=0;col<col_length;col++){
            File<<blocks[row][col].type<<" ";
        }
        File<<endl;
    }
}
int myMap::get_type(int i,int j)
{
    return blocks[i][j].type;
}


bool myMap::check_and_create_route()//检查并且告诉用户
{
    //统计起点和终点
    final_index.clear();
    begin_indexs.clear();
    for(int row=0;row<row_length;row++){
        for(int col=0;col<col_length;col++){
            if(blocks[row][col].type==origin)
            {
                begin_indexs.push_back({row,col});
            }
            else if(blocks[row][col].type==final)
            {
                final_index.push_back(row);
                final_index.push_back(col);
            }
          }
     }
    //检查重点数量
    if(final_index.size()>2)
        throw 1;
    if(final_index.size()==0)
        throw 3;
    if(begin_indexs.size()==0)
        throw 4;


    //检查是否有通路
    dij.dealwith(row_length*col_length,final_index[0],final_index[1],blocks);
    for(int index=0;index<begin_indexs.size();index++)
    {
        bool flag=dij.is_connected(begin_indexs[index][0],begin_indexs[index][1]);
        if(!flag)
            throw 2;
    }

    //生成多条地图线路
    routes.clear();//每次生成前都要清除已有路线
    for(int index=0;index<begin_indexs.size();index++)
    {
        vector<vector<int>>one_route;
        dij.create_map_path(begin_indexs[index],final_index[0],final_index[1],one_route);
        routes.push_back(make_route(one_route));
        //for(int i=0;i<routes[index].size();i++)
        //{
          //  qDebug()<<"Node "<<routes[index][i].x<<" "<<routes[index][i].y<<" dir "<<routes[index][i].direction;
       // }
    }
    return true;
}

vector<Node> myMap::make_route(vector<vector<int>> s)
{
    vector<Node>answer;
    for (int i = 0; i < s.size(); i++)
    {

        Node pt(s[i][0],s[i][1]);
        answer.push_back(pt);
        if (i != 0)
        {
            if (s[i][0] == s[i - 1][0] && s[i][1] - 1 == s[i - 1][1])//east
            {
                answer[i - 1].set_dir(1);
            }
            else if (s[i][0] == s[i - 1][0] && s[i][1] +1 == s[i - 1][1])//west
            {
                answer[i - 1].set_dir(2);
            }
            else if (s[i][1] == s[i - 1][1] && s[i][0] - 1 == s[i - 1][0])//sourth
            {
                answer[i - 1].set_dir(3);
            }
            else //north
            {
                answer[i - 1].set_dir(4);
            }
        }
    }
    return answer;
}

int myMap::Get_entry_num()//获得敌人的出口数量
{
    return begin_indexs.size();
}

void myMap::Get_entry_location(int index,int &x,int &y)//获得出口的坐标
{
    x=begin_indexs[index][0];
    y=begin_indexs[index][1];
}
int  myMap::Get_direction(int route_number,int num_index)
{
      return routes[route_number][num_index].direction;
}
void myMap::change_location(int route_index,int &block_num,int &x,int &y,int &px,int &py,int &dir,int speed)
{
   switch(dir)
   {
     case east:
          block_num++;
          y++;
          dir=routes[route_index][block_num].direction;
          break;
     case west:
         block_num++;
         y--;
         dir=routes[route_index][block_num].direction;
         break;
     case south:
          block_num++;
          x++;
          dir=routes[route_index][block_num].direction;
          break;
     case north:
          block_num++;
          x--;
          dir=routes[route_index][block_num].direction;
          break;
   }
}
int myMap::get_block_nums(int select_entry)
{
    return routes[select_entry].size();
}






















//dij

bool Dij::is_road_or_final(int x, int y, const vector<vector<Block>>& blocks)
{
    if (!is_visited[x][y] &&
        ((blocks[x][y].get_type() == road) ||
            (blocks[x][y].get_type() == origin))
        )
        return true;
    else
        return false;

}
void Dij::dealwith(int newnum, int originx, int originy, const vector<vector<Block>>& blocks)
{

    dis.clear();
    path.clear();
    is_visited.clear();
    int num = newnum;//点的数目
    int origin_num = pairtoint(originx, originy);//得到原点的代号


    dis.resize(num);//距离
    path.resize(num);//路径


    //三个数据结构的初始化
    is_visited.resize(blocks.size());
    for (int i = 0; i < is_visited.size(); i++)
    {
        is_visited[i].resize(blocks[0].size(), false);
    }
    dis.assign(dis.size(), -1);//距离容器中除了源点本身，其他顶点初始化为-1
    dis[origin_num] = 0;

    path.assign(path.size(), -1);//路径全部初始化为-1



    queue<int>q;
    q.push(origin_num);

    while (!q.empty())
    {
        int w = q.front();//删掉队列头部顶点并把该顶点的未被访问过的邻接点放入队列
        q.pop();

        vector<int>pairs;
        pairs = intopair(w);
        int x = pairs[0];
        int y = pairs[1];


        is_visited[x][y] = true;
        if (blocks[x][y].get_type() == origin)
            continue;


        int i = 0;
        if (x >= 1)
        {
            if (is_road_or_final(x-1,y,blocks))
            {
                i = pairtoint(x - 1, y);
                dis[i] = dis[w] + 1;
                path[i] = w;
                q.push(i);
            }
        }
        if (x + 1 <= 6)
        {
            if (is_road_or_final(x + 1, y, blocks))
            {
                    i = pairtoint(x + 1, y);
                    dis[i] = dis[w] + 1;
                    path[i] = w;
                    q.push(i);
            }
        }
        if (y >= 1)
        {
            if(is_road_or_final(x, y-1, blocks))
            {
                i = pairtoint(x, y - 1);
                dis[i] = dis[w] + 1;
                path[i] = w;
                q.push(i);
            }
        }
        if (y + 1 <= 13)
        {
            if (is_road_or_final(x, y + 1, blocks))
            {
                i = pairtoint(x, y + 1);
                dis[i] = dis[w] + 1;
                path[i] = w;
                q.push(i);
            }
        }
    }

}

void Dij::find(int finalnum,int beginnum, vector<vector<int>>& answer)									//递归输出最短路径
{
    vector<int>temp = intopair(beginnum);
    answer.push_back(temp);
    if (path[beginnum] == finalnum) {
        vector<int>s = intopair(finalnum);
        answer.push_back(s);
    }
    else {
        find(finalnum,path[beginnum], answer);
    }
    return;
}
void  Dij::create_map_path(vector<int>originpairs, int finalx, int finaly, vector<vector<int>>& answer)
{
    int finalnum = pairtoint(finalx, finaly);
    int beginnum = pairtoint(originpairs[0], originpairs[1]);
    find(finalnum, beginnum,answer);
}
bool Dij::is_connected(int finalx, int finaly)
{
    return dis[pairtoint(finalx, finaly)]!=-1;
}






























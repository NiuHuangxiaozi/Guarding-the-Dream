#include "dij.h"

void Dij::dealwith(int newnum,int originx,int originy,const vector<vector<)
{
    int num=newnum;
    dis.resize(num);
    path.resize(num);
    dis.assign(dis.size(),-1);//距离容器中除了源点本身，其他顶点初始化为-1
    dis[v] = 0;

}

void Unweighted(int originx,int originy)
{

    int v;
    cin >> v;
    v--;
    dis.assign(dis.size(),-1);//距离容器中除了源点本身，其他顶点初始化为-1
    dis[v] = 0;
    path.assign(path.size(), -1);//路径全部初始化为-1
    queue<int>q;//用队列进行广度优先搜索
    q.push(v);
    while (!q.empty())
    {
        int w = q.front();//删掉队列头部顶点并把该顶点的未被访问过的邻接点放入队列
        q.pop();
        for (unsigned int i = 0; i < map.size(); i++)
        {
            if (map[w][i] == 1 && dis[i] == -1)//dis[i]为-1说明未被访问
            {
                dis[i] = dis[w] + 1;
                path[i] = w + 1;
                q.push(i);
            }
        }
    }
}

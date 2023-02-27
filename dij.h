#ifndef DIJ_H
#define DIJ_H
#include<vector>
using namespace std;

class Dij
{
public:
    vector<vector<int>>map;//邻接矩阵
    vector<int>dis;//记录距离的容器
    vector<int>path;//记录路径的容器

    void dealwith(int,int originx,int originy);
    //void Unweighted();
    int pairtoint(int x,int y);
    int intopair(int a);
};

#endif // DIJ_H

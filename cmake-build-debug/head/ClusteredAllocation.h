//
// Created by JXY on 2024/5/30.
//

#ifndef FILESYSTEM_CLUSTEREDALLOCATION_H
#define FILESYSTEM_CLUSTEREDALLOCATION_H
#include<iostream>
#include<vector>
#include"head.h"
using namespace std;

class ClusterList;
class Cluster;


class Cluster {
public:
    int N;
    int FreeBLoc[10];        // 数据域

    // 默认构造函数
    Cluster() = default;
};



// 定义 ClusteredList 类
class ClusteredList {
public:
    Cluster head;   // 头节点(头结点信息存储在超级块中，程序运行后放到这里
    Cluster* pointer;

private:
    void RefreshCluster();
    void appendCluster();
    void freeCluster();
    // 构造函数

};

ClusteredList ClusterList;
#endif //FILESYSTEM_CLUSTEREDALLOCATION_H

#pragma once
#ifndef BACKENDPROCESSOR_H
#define BACKENDPROCESSOR_H

#include "config.h"
#include "objects/box.h"
#include "objects/linkroute.h"
using namespace box;
//专门用于处理地图后端计算分析的类
class BackendProcessor:public QObject
{
    Q_OBJECT
private:
    type** data;
    int wScale;
    int hScale;

    BoxMap* linkBoxes;//前端

    bool isLegal(const QPoint& pt) const;//判断数组行列是否合法（不越界）
    bool isGeneralizedLegal(const QPoint& pt) const;//判断数组行列是否广义合法（合法+周围一圈）
    type dataAt(int x,int y)const;
    type dataAt(const QPoint& pt) const;

public:
    BackendProcessor();
    BackendProcessor(const Record& record);//用于测试，从record中初始化backendProcessor对象
    BackendProcessor(BoxMap* lkBoxes);
    ~BackendProcessor(){}

    void init2DArray();//用于测试，分配二维数组动态内存
    void load(type** d,int w,int h);//加载二维数组
    void delete2DArray();//用于测试，回收之前分配的内存，需要外部手动调用

    void generateFromArray(const QJsonArray& jArray);//从JsonArray中生成箱子
    void generateFromVector(const QVector<box::type> map);//从QVector中生成箱子
    bool randomSwapOnce();//随机交换两个箱子，返回交换后是否有变化
    void shuffle(int times);//随机洗牌
    void shuffle();//随机洗牌

    //接口

//    //判断是否可以消除的函数（已不需要）
//        //判断是否可以0次转折消除
//    bool checkLinkTwistZero(const QPoint& p1,const QPoint& p2) const;
//        //判断是否可以1次转折消除
//    bool checkLinkTwistOnce(const QPoint& p1,const QPoint& p2) const;
//        //判断是否可以2次转折消除
//    bool checkLinkTwistTwice(const QPoint& p1,const QPoint& p2) const;

    //返回位于p1,p2的箱子能否消除，如果不能，将bestRoute置nullptr,若能，bestRoute指向堆空间的动态对象，表示最短路径
    bool checkLink(const QPoint& p1, const QPoint& p2, LinkRoute *&bestRoute) const;
    //选择一个出发点（StartPt)，返回一个可连接的结束点(EndPt)，若不存在返回(-1,-1)
    QPoint hintFrom(const QPoint& startPt) const;
    QVector<QPoint> hint() const;//提示功能
signals:
};

#endif // BACKENDPROCESSOR_H

#pragma once
#ifndef BACKENDPROCESSOR_H
#define BACKENDPROCESSOR_H

#include "config.h"
#include "objects/box.h"
#include "objects/linkroute.h"
using namespace box;
/*
 * BackendProcessor
 * 处理后端计算、分析的类，只存储箱子的种类，不存储实体信息
 */

class BackendProcessor:public QObject
{
    Q_OBJECT
private:
    type** data;
    int wScale;
    int hScale;

    BoxMap* linkBoxes;//前端

    bool isGeneralizedLegal(const QPoint& pt) const;//判断数组行列是否广义合法（合法+周围一圈）
    type dataAt(int x,int y)const;
    type dataAt(const QPoint& pt) const;

public:
    BackendProcessor(const Record& record);//用于测试，从record中初始化backendProcessor对象
    BackendProcessor(BoxMap* lkBoxes);
    ~BackendProcessor(){}

    bool isLegal(const QPoint& pt) const;//判断数组行列是否合法（不越界）

    void init2DArray();//用于测试，分配二维数组动态内存
    void load(type** d,int w,int h);//加载二维数组
    void delete2DArray();//用于测试，回收之前分配的内存，需要外部手动调用

    void generateFromArray(const QJsonArray& jArray);//从JsonArray中生成箱子
    void generateFromVector(const QVector<box::type> map);//从QVector中生成箱子

    //shuffle相关
    bool randomSwapOnce();//随机交换两个箱子，返回交换后是否有变化
    void shuffle(int times);//随机洗牌
    void shuffle();//随机洗牌

    //返回位于p1,p2的箱子能否消除，并传入bestRoute指针，如果可以连接，bestRoute作为最佳路径传出，否则bestRoute置为nullptr
    bool checkLink(const QPoint& p1, const QPoint& p2, LinkRoute *&bestRoute) const;
    bool checkLink(const QPoint& p1, const QPoint& p2) const;//overload checkLink for test

    //提示与可解性
    QVector<QPoint> hintFrom(const QPoint& startPt) const;//选择一个出发点（startPt)，返回所有可以连线的方块坐标的容器
    QVector<QPoint> hint() const;//给出全局提示之一
    QVector<QPoint> hint(const QPoint& standPt) const;//给出对于standPt可达的局部提示之一
    bool isSolvable() const;//判断当前地图是否有解（全局有解）

    //选择一个出发点（StartPt)，返回所有可以到达(reach)的方块坐标的容器
    QVector<QPoint> reachableFrom(const QPoint& startPt) const;
protected:
    static void shuffleVector(QVector<QPoint>& targetVec);//随机打乱targetVec
signals:
};

#endif // BACKENDPROCESSOR_H

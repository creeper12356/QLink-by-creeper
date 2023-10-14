#pragma once
#ifndef LINKROUTE_H
#define LINKROUTE_H
#include "entity.h"
#include "config.h"
/*
 * LinkRoute: 箱子消除的连线类
 */

class BoxMap;

struct LinkRouteNode//节点类
{
    QPoint pos;//行列坐标
    linkRoute::nodeDir dir;//节点取向
    LinkRouteNode(){}
    LinkRouteNode(const QPoint& p,linkRoute::nodeDir d)
        :pos(p),dir(d){}
};

class LinkRoute
{
private:
    QVector<LinkRouteNode> nodes;//路径上的各节点
    QMap<linkRoute::nodeDir,QPixmap> nodePic;//不同节点方向对应的图片
    int _turn = 0;//转折数
private:
    //要求j = i + 1，且i,j位于0到size - 1之间,返回从节点i到j的运动方向
    entity::dir dirBetweenNodes(int i,int j);
    entity::dir dirBeforeNode(int i);//节点i之前的运动方向
    entity::dir dirAfterNode(int i);//节点i之后的运动方向
public:
    LinkRoute();
    ~LinkRoute();
private:
    void initNodePic();//初始化nodePic
public:
    void appendNode(const QPoint& pos,linkRoute::nodeDir dir);//在后面加入节点
    void removeFirst();//删除最前面的节点
    void removeLast();//删除最后面的节点
    /*
     * 如果当前长度<=maxSize，不作处理；
     * 当前长度>maxSize,删除多余的元素
     */
    void limitSize(int maxSize);

    int size() const;
    int& rturn() {return _turn;}
    int turn() const {return _turn;}
    void updateDir();//根据路径上节点走向更新每个节点的取向
    void print() const;
public:
    void draw(QPainter& painter,BoxMap* linkBoxes);
};

#endif // LINKROUTE_H

#pragma once
#ifndef BOXMAP_H
#define BOXMAP_H
#include "config.h"
#include "plainbox.h"
#include "propbox.h"
//存储箱子组织方式的类
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class BoxMap:public QObject
{
    Q_OBJECT
protected:
    //basic data
    int wScale;//宽度规模
    int hScale;//高度规模
    QPointF corner;//左上角点（相对于map的位置)
    QSizeF size;//箱子的大小
    QSizeF dist;//箱子之间的间距（包括横向、纵向间距两个分量）
    //array data
    Box* **ptrData;//指向实体箱子的二维数组
    box::type ** data;//后台二维数组
    //backend data
    BackendProcessor* processor;//后台处理器
    Settings*& settings;//游戏设置
    //archive
    QVector<Box*> boxes;//箱子档案
    //assistant data
    QVector<QPoint> nullBoxes;//无方块坐标
    QVector<QPoint> plainBoxes;//普通箱子坐标
    QVector<QPoint> propBoxes;//道具箱子坐标

public:
    BoxMap(Settings*& sts,const Record& record);
    ~BoxMap();

    //初始化函数
private:
    void initBasicData(const Record& record);
    void init2DArray();//初始化二维数组，分配空间
    void initProcessor(const Record& record);
    void initBoxes();

public:
    //实体交互
    void generateEntities(GameMain* parent);//在窗口parent中根据boxMap参数生成实体
    bool addBoxAt(const QPoint& pt, box::type type);//在pt处加入箱子（type） ，返回是否加入成功
    bool addBoxAt(int x,int y,box::type type);//重载
    bool removeBoxAt(QPoint pt);//删除pt处的箱子，返回是否删除成功
    bool removeBoxAt(int x,int y);//重载
    bool swapEntityBox(const QPoint& p1,const QPoint& p2);
    QPointF boxPosAtData(int i,int j);//计算data[i][j]对应箱子实体的坐标
    QRectF rectAt(int i,int j);//返回boxMap位于data[i][j]对应的矩形

    QPoint posToDataCoord(const QPointF& pos);//如果不区分返回值(-1,-1) 是否为缝隙，可以使用此重载
    QPoint posToDataCoord(const QPointF& pos,bool* ok);//实体点坐标所在的boxMap箱子的行列坐标（不一定合法），ok传出是否有对应坐标
private:
    //to be renamed...
    qreal smallerEdge(const qreal& pos,const qreal& corner,const qreal& size,const qreal& dist) const;
    qreal largerEdge(const qreal& pos,const qreal& corner,const qreal& size,const qreal& dist) const;

    int calculateIndex(const qreal& pos,const qreal& corner,const qreal& size,const qreal& dist,bool *ok) const;//posToDataCoord调用的抽象函数
    //给定一维下的参数，以及从min到max的范围（要求min < max），判断该范围覆盖的方块index，并返回
    QVector<int> coverIndex(const qreal& corner,const qreal& size,const qreal& dist,qreal min,qreal max);

public:
    //返回矩形entityBox在boxMap中覆盖到的所有箱子行列坐标的容器
    QVector<QPoint> coverDataCoords(const QRectF& entityBox);
public:

    //接口函数
    int getWScale() const{return wScale;}
    int getHScale() const{return hScale;}
    QPointF getCorner() const{return corner;}
    QSizeF getSize() const{return size;}
    QSizeF getDist() const{return dist;}

    box::type** getData() {return data;}
    box::type& getDataAt(int x,int y){return data[x][y];}
    box::type& getDataAt(QPoint pt){return data[pt.x()][pt.y()];}
    Box* ** getPtrData() {return ptrData;}
    Box*& getPtrDataAt(int x,int y){return ptrData[x][y];}
    Box*& getPtrDataAt(QPoint pt){return ptrData[pt.x()][pt.y()];}
    BackendProcessor* getProcessor(){return processor;}

    QVector<Box*>& getBoxes() {return boxes;}

    const QVector<QPoint>& getNullBoxes() const{return nullBoxes;}
    const QVector<QPoint>& getPlainBoxes() const{return plainBoxes;}
    const QVector<QPoint>& getPropBoxes() const{return propBoxes;}

public:
    void draw(QPainter& painter,bool isDebugMode);    
};

#endif // BOXMAP_H

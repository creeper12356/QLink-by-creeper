#pragma once
#ifndef MAP_H
#define MAP_H
#include <QColor>
#include <QPixmap>
#include <QPoint>

class Block//地图中的固定方块
{
    friend class Map;
private:
    QPixmap pic;//方块的贴图

    qreal speedFactor;//速度因子，影响玩家移动速度
public:
    Block(){};

    //接口函数
    const QPixmap& getPic() const{return pic;}//目前暂时不允许修改pic
    qreal getSpeedFactor() const{return speedFactor;}
};

class Map
{
private:
    int wScale;//宽度规模
    int hScale;//高度规模
    int blockSize;//每个方块的大小
    QPoint corner;//左上角点（相对于Widget)
    int** data;//存储宽*高区域的方块信息，先用方块的id表示

    std::vector<Block*> blocks;//方块档案

public:
    Map(int w = 16,int h = 15,QPoint p = QPoint(10,100),int bs = 50);
    ~ Map();
    void initBlocks();//初始化所有方块信息
    void addBlock(Block* newBlock);

    //接口函数
    int getWScale() const{return wScale;}
    int getHScale() const{return hScale;}
    int getBlockSize() const{return blockSize;}
    QPoint getCorner() const{return corner;}
    int** getData() const{return data;}

    std::vector<Block*>& getBlocks() {return blocks;}

};

#endif // MAP_H

#pragma once
#ifndef BOX_H
#define BOX_H
#include "entity.h"
#include "config.h"
/*
 * 箱子类
 */
class Box:public Entity
{
protected:
    box::type boxType;//箱子外观:apple,beet_root,...
    box::division boxDivision;//箱子功能:plain_box,prop_box
    QPixmap* head = nullptr;
    QSoundEffect *breakSound = nullptr;//破坏音效
    bool isEntity;//是否为实体

    int breakScore = 0;//破坏方块得到的加分
public:
    bool isActivated;//是否被激活
    bool isLocked;//是否上锁，上锁的箱子无法被激活
    bool isHighlighted;//是否高亮显示
public:
    Box();
    virtual ~Box();

    //设置函数(setters)
    virtual void setBoxType(box::type t);//设置箱子类型
    void setBoxDivision(box::division d);//设置箱子功能类型
    void setIsEntity(bool i);//如果设置为非实体，分配部分堆空间（只能调用1次）
    bool setHead(const QString& path);//加载head图片，返回是否加载成功
    void setHead(QPixmap *pix);//设置head指向
    void copyStyle(const Box* other);//赋值另一个箱子的样式（大小、图片），可移动性
    bool setBreakSound(const QString& path);//加载破坏音效,返回path是否有效
    void setBreakSoundVolume(qreal v);//设置破坏音效音量
    void setBreakScore(int breakScore);//设置方块破坏对应的分数

    //接口函数(getters)
    box::type getBoxType() const{return boxType;}
    box::division getBoxDivision() const{return boxDivision;}
    bool getIsEntity() const {return isEntity;}
    virtual const QPixmap & getHead() const override{return *head;}
    int getBreakScore() const{return breakScore;}
    //绘制函数
    virtual void draw(QPainter &painter,bool isDebugMode = false) override;

    static box::division typeToDivision(box::type t);//根据箱子种类判断箱子功能
public:
    void playBreakSound() const;//播放破坏音效
};

#endif // BOX_H

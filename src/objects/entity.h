#pragma once
#ifndef ENTITY_H
#define ENTITY_H
#include "config.h"
/*
 * 可以移动、碰撞的物体，包括玩家和箱子
 */
class Entity
{
protected:
    QRectF entityBox;//碰撞箱
    entity::type entityType;//Role or Box
public:
    QString id;//用于唯一确定实体
    //bool标记
public:
    bool isIdVisible;//是否显示实体id
    bool isMovable;//是否可移动
public:
    Entity();
    virtual ~Entity(){};

    //设置函数(setters)
    void setEntityBox(const QRectF& r);//设置碰撞箱（包括位置、大小等信息）
    void setPos(qreal x,qreal y);//调整实体左上角点位置
    void setPos(QPointF p);//调整实体左上角点位置
    void setSize(qreal w,qreal h);
    void setSize(QSizeF sz);
    void setWidth(qreal w);//设置实体宽度
    void setHeight(qreal h);//设置实体高度

    //接口函数(getters)
    entity::type getEntityType() const{return entityType;}
    QPointF getPos() const{return entityBox.topLeft();}
    qreal getWidth() const{return entityBox.width();}
    qreal getHeight() const{return entityBox.height();}
    virtual const QPixmap& getHead() const = 0;//获取头像图片
    const QRectF& getEntityBox() const{return entityBox;}

    //与移动相关的函数
protected:
    qreal boundarySpace(const Map* m,int dir) const;//this实体与地图边界的剩余空间（间距）
    qreal space(const Entity* other,int dir) const;//this与other在dir方向上的剩余空间（间距）
    void pureMove(qreal singleMove,int dir);//不考虑边界和碰撞的移动
    static bool interSectionX(const Entity* e1,const Entity *e2);//两实体x坐标范围有无交集
    static bool interSectionY(const Entity* e1,const Entity* e2);//两实体y坐标范围有无交集
    bool interSection(const Entity* other,qreal singleMove,int dir) const;//判断在dir方向上，other和this有无交集

    virtual qreal move(GameMain* parent,qreal singleMove,int dir);//实体移动函数，返回实际移动的像素数

public:
    virtual void draw(QPainter& painter, bool isDebugMode = false);
};

#endif // ENTITY_H

#pragma once
#ifndef ROLE_H
#define ROLE_H
#include <QWidget>
#include <QKeyEvent>
#include <QMap>
#include <QVector>
#include <QTimer>

#include "config.h"
#include "entity.h"

class Role:public QObject,public Entity
{
    Q_OBJECT
protected:
    //角色属性
    qreal speed;//移动速度（绝对速度）单位：像素/秒
    //偏好属性
    QMap<entity::dir,QPixmap*> moveImages;//与运动相关的贴图
    QMap<entity::dir,int> moveKeys;//运动方向按键
    //后台属性
    entity::dir dftFaceDir;//默认面部朝向，在faceDir容器为空时使用
    QVector<entity::dir> faceDir;//存储面部朝向的栈，栈顶为当前状态
    QTimer monitor;//运动监视器

    QTimer dizzyTimer;//迷惑状态计时器
    QTimer freezeTimer;//冻结状态计时器

    //后台参数
    //dizzy相关
    int dizzyTime;//单次dizzy的时间（ms）
    int dizzyRemainingTime;//dizzyTimer剩余时间
    //freeze相关
    int freezeTime;//单次冻结时长(ms)
    int freezeRemainingTime;//冻结剩余时间

public:
    Role();
    ~Role();
public:
    //设置函数
    void setSpeed(qreal s);//设置速度
    void insertMoveKeys(entity::dir dir,int key);//向moveKeys中插入键值对
    void insertMoveImages(entity::dir d,QString path);//向moveImages中插入键值对（图片以路径传入）
    void setOverrideFaceDir(entity::dir d);//将新的面部方向d入栈
    void restoreOverrideFaceDir(entity::dir d);//将面部方向d从栈中抽出
    void setDefaultFaceDir(entity::dir d);//设置默认面朝方向

    //设置dizzyTime参数
    void setDizzyTime(int dt);
    //设置freezeTime
    void setFreezeTime(int ft);

    //接口函数（包括读、写接口）
    entity::dir getDefaultFaceDir() const;//默认面朝方向
    entity::dir getFaceDir() const;
    virtual const QPixmap & getHead() const override;
    entity::dir getDirFromKey(int key) const ;//从按下的键key获取玩家对应的方向，没有对应方向返回entity::null
    QMap<entity::dir,QPixmap*>& rmoveImages(){return moveImages;}//提供moveImages的读写接口
    QVector<entity::dir>& rfaceDir() {return faceDir;}//提供faceDir的读写接口
    QTimer& rmonitor() {return monitor;}//运动监听器的读写接口

    int getDizzyTime() const {return dizzyTime;}//dizzyTime读接口
    //dizzyRemainingTime读接口,仅在暂停状态下有意义
    int getDizzyRemainingTime () const{return dizzyRemainingTime;}

    int getFreezeTime() const{return freezeTime;}
    //仅在暂停状态下有意义
    int getFreezeRemainingTime()const{return freezeRemainingTime;}

    //移动、实体相关
    qreal calculateSingleMove(GameMain* parent,int interval) const;
    virtual qreal move(GameMain *parent, qreal singleMove, int dir) override;
    QPoint findTarget(GameMain *parent);//计算被激活方块在linkBox中的行、列

public:
    virtual void draw(QPainter &painter, bool isDebugMode = false) override;
protected:
    void swapDirKeys();//交换上下、左右方向按键
public:
    //dizzy相关
    void initDizzyTimer();
    bool isDizzy() const{return dizzyTimer.isActive();}//是否处于dizzy状态
    void dizzy();//被dizzy技能函数调用
    void pauseDizzyTimer();//暂停dizzyTimer计时器
    void startDizzyTimer();//开始dizzyTimer计时器
    //freeze相关
    void initFreezeTimer();
    void freeze();//被freeze技能函数调用
    void pauseFreezeTimer();
    void startFreezeTimer();

signals:
    void monitorTimeout(Role* player);//monitor计时器timeout信号
    void dizzyEnd();//dizzy状态结束信号
    void freezeEnd();//freeze状态结束信号
protected slots:
    void emitMonitorTimeout();//作为monitor timeout的槽函数，发出monitorTimeout信号
};

#endif // ROLE_H

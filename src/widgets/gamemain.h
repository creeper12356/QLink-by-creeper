#pragma once
#ifndef GAMEMAIN_H
#define GAMEMAIN_H

#include <QWidget>
#include <QString>
#include <QTimer>
#include <QDebug>
#include <vector>
#include <QtMath>
#include <QStack>
#include <QPushButton>

#include "config.h"
#include "objects/map.h"
#include "objects/role.h"
#include "objects/boxmap.h"
#include "backendprocessor.h"
#include "objects/linkroute.h"
#include "objects/statebar.h"

#include "widgets/settings.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

using namespace gameMain;

class GameMain : public QWidget
{
    Q_OBJECT
//data
protected:
    //主要容器和字段
    QVector<Role*> players;
    QVector<Box*> boxes;
    QVector<Entity*> entities;//存储所有实体
    BoxMap* linkBoxes;//连连看的箱子地图
    BackendProcessor* processor;//后端处理器
    Map* background;//背景地图

    QVector<LinkRoute*> routes;//连线容器
    QMap<Role*,QVector<QPoint>> activatedBoxes;//从玩家指针 映射到 玩家激活箱子对应的行列坐标
    QVector<QPoint> hintBoxes;//处于提示状态（高亮状态）箱子对应的坐标 容器
    QMap<Role*,int> scores;//玩家分数

    Clock* gameClk;

    //标签
    gameMode mode;

    //游戏参数（仅在初始化时修改才有效）
    qreal dizzyTime = 10;//迷惑时间（s）
    qreal freezeTime = 3;//冻结时间（s）
    qreal hintTime = 10;//提示时间(s)
    int monitorInterval = 5;//玩家运动监听器监听周期(ms)
    qreal routeLifeSpan = 0.3;//路径存在时间（s）

    //hint相关
    QTimer hintTimer;//游戏提示计时器
    int hintRemainingTime;//提示剩余时间（ms）

    //游戏设置
    Settings*& settings;
    //游戏存档
    Record* record = nullptr;

private:
    Ui::Widget *ui;

//functions
public:
//    GameMain(QWidget* parent,Settings*& s,gameMode mode = singleMode);
    GameMain(QWidget* parent, Settings*& s, Record &record);
    ~GameMain();

protected:
    //初始化函数
    void initPlayerMoveKeys(Role* player,int playerNum);//初始化玩家方向键，被initPlayer调用
    Role* initPlayer(const QString& id,int playerNum);//通过id初始化玩家，成功返回玩家指针，失败返回nullptr
//    void initPlayers();//初始化玩家属性
    void initPlayers(const Record& record);
//    void initBoxes();//初始化箱子
    void initLinkBoxes();//初始化连连看箱子地图
    void initLinkBoxes(const Record& record);
    void initProcessor();//初始化后端处理器,必须在initLinkBoxes之后调用
    void initGameClk();//初始化倒计时
    void initGameClk(const Record& record);
    void initHintTimer();//初始化提示计时器

public:
    //接口函数
    Map* getBgd() const{return background;}
    QVector<Entity*>& getEntities(){return entities;}
    BoxMap* getLinkBoxes(){return linkBoxes;}
    gameMain::gameMode getGameMode() const{return mode;}
    Record& getRecord(){return *record;}

    //维护游戏对象函数
        //添加
    void addEntity(Entity* e);//加入实体
    void addPlayer(Role* r);//加入玩家
    void addBox(Box* b);//加入箱子
        //删除
    void removeEntity(Entity* e);//删除实体
    void removePlayer(Role* r);//删除玩家
    void removeBox(Box* b);//删除箱子

    //与游戏状态有关
public:
    void start();//开始游戏
    void pause();//暂停游戏

protected:
    void deleteBoxAt(const QPoint& pt);//从游戏中完全删除位于pt的箱子，被tryLink函数调用
    void updateScore(Role* player,const LinkRoute* route);
    bool tryActivate(const QPoint &target, Box*& entityTarget, Role* player);//尝试激活方块
    bool tryDeactivate(const QPoint& target,Box*& entityTarget,Role* player);//尝试取消激活方块,返回是否更新isActivated字段
    void addRawRoute(LinkRoute*& route);//加入由updateScore生成的路径
    bool tryLink(Role* player);//检查某个玩家的连线情况，返回是否可以消除
    bool isWin() const;//判断游戏是否结束
    //消除方块函数
    void processPlainBoxTarget(const QPoint& target,Box*& entityTarget,Role* player);//处理普通箱子对象的选中
    void processPropBoxTarget(const QPoint& target,Box*& entityTarget,Role* player);//处理道具箱子对象的选中
    //hint相关
    bool isHint() const;//判断是否处于提示状态
    void startHintTimer();
    void pauseHintTimer();
    void clearHint();//清空所有提示记录
    void updateHint();//更新提示
private slots:
    void movePlayer(Role* player);//与moveVector::monitor绑定，处理玩家的移动
    void clockTimeOutSlot();
    void repaintSlot();//重画槽函数

protected:
    //QWidget事件虚函数（交互事件）
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void closeEvent(QCloseEvent *event) override;

    //工具函数
protected:
    //返回相对于player来说的另一个玩家，不存在返回Null
    Role* other(Role* player);

    //道具箱子技能函数
public:
    void addTime(Role* player);//延长剩余时间
    void shuffle(Role* player);//重排所有方块位置
    void hint(Role* player);//提示
    //仅双人
    void freeze(Role* player);//对手3s无法移动
    void dizzy(Role* player);//对手10s内移动方向颠倒
    //存档相关
public:
    void saveGameToRecord();
protected:
    //测试参数
    QTimer testTimer;//测试计时器
    bool isDebugMode = false;//测试模式
private slots:
    //仅用于测试，实时打印一些信息。
    void statePrinter();
    void on_shuffle_button_clicked();//按下shuffle按钮的槽函数
    void on_hint_button_clicked();//按下hint按钮的槽函数

signals:
    void gameMainClosed();//游戏窗口被关闭信号，提示主菜单窗口显示
    void gamePaused(const QString& info);//游戏暂停信号
};
#endif // GAMEMAIN_H

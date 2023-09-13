#include "role.h"
#include "widgets/gamemain.h"
#include "boxmap.h"

using namespace std;

Role::Role():Entity()
{
    entityType = entity::role;
    dftFaceDir = entity::down;//默认面朝方向：向下
    connect(&monitor,&QTimer::timeout,this,&Role::emitMonitorTimeout);

    initDizzyTimer();
    initFreezeTimer();
}
Role::~Role()
{
    qDebug() << "~role" ;
    for(auto i = moveImages.begin();i != moveImages.end();++i)
    {
        delete i.value();
    }
}
void Role::setSpeed(qreal s)
{
    speed = s;
}

void Role::insertMoveKeys(entity::dir dir, int key)
{
    moveKeys.insert(dir,key);
}
void Role::insertMoveImages(entity::dir d, const QString &path)
{
    QPixmap* handle = new QPixmap(path);
    moveImages.insert(d,handle);
}
void Role::setOverrideFaceDir(entity::dir d)
{
    faceDir.append(d);
}
void Role::restoreOverrideFaceDir(entity::dir d)
{
    if(faceDir.size() == 1)//只有一个键被按下
    {
        dftFaceDir = d;//更新默认方向
    }
    faceDir.removeAll(d);
}

void Role::setDefaultFaceDir(entity::dir d)
{
    dftFaceDir = d;
}
void Role::setDizzyTime(int dt)
{
    dizzyTime = dt;
    dizzyRemainingTime = dt;
}
void Role::setFreezeTime(int ft)
{
    freezeTime = ft;
    freezeRemainingTime = ft;
}

entity::dir Role::getDefaultFaceDir() const
{
    return dftFaceDir;
}
entity::dir Role::getFaceDir() const
{
    if(faceDir.empty())
        return getDefaultFaceDir();
    else return faceDir.back();
}
const QPixmap& Role::getHead() const
{
    return (*moveImages.value(getFaceDir()));
}
entity::dir Role::getDirFromKey(int key) const
{
    return moveKeys.key(key,entity::no_dir);
}
qreal Role::calculateSingleMove(GameMain* parent,int interval) const
{
    QPointF center = entityBox.center();
    int blockSize = parent->getBgd()->getBlockSize();
    QPoint indexPoint = QPoint(center.x() / blockSize,center.y() / blockSize);
    qreal singleMove =
            speed *
            parent->getBgd()->getBlocks()
                [
                    parent->getBgd()->getData()
                        [indexPoint.y()][indexPoint.x()]
                ]
            ->getSpeedFactor();//在该种方块上的真实绝对速度

    singleMove *= interval / 1000.0;//转换为monitor单次取样移动的像素数
    return singleMove;
}
qreal Role::move(GameMain *parent, qreal singleMove, int dir)
{
    return Entity::move(parent,singleMove,dir);
}
QPoint Role::findTarget(GameMain *parent)
{
    entity::dir faceDir = getFaceDir();//玩家面部朝向
    const QRectF& entityBox = getEntityBox();
    BoxMap* linkBoxes = parent->getLinkBoxes();
    QPointF digPt;//从玩家的中心开始，朝dir方向挖掘至方块内部的点坐标
    qreal step;
    if(faceDir <= entity::down){
        step = getHeight() + linkBoxes->getSize().height()/ 2 + EPS;
    }
    else{
        step = getWidth() / 2 + EPS;
    }
    digPt = entity::pureMove(entityBox.center(),faceDir,step);
    QPoint target = linkBoxes->posToDataCoord(digPt);
    if(linkBoxes->getProcessor()->isLegal(target)){
        return target;
    }
    else return QPoint(-1,-1);
}
void Role::emitMonitorTimeout()
{
    emit monitorTimeout(this);
}
void Role::draw(QPainter &painter, bool isDebugMode)
{
    Entity::draw(painter,isDebugMode);
    if(!isMovable)//冻结状态
    {
        painter.drawPixmap(entityBox.x() - entityBox.width() / 10,
                           entityBox.y() - entityBox.height() / 10,
                           entityBox.width() * 1.2,
                           entityBox.height() * 1.2,
                           QPixmap(":/images/effects/freeze.png"));

    }
    if(isDizzy())//迷惑状态
    {
        painter.save();
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(100,0,100,90));
        painter.drawRect(entityBox.toRect());
        painter.restore();
    }
}
void Role::swapDirKeys()
{
    //交换上下左右按键
    qSwap(moveKeys[entity::up],moveKeys[entity::down]);
    qSwap(moveKeys[entity::left],moveKeys[entity::right]);
    dftFaceDir = entity::opsiteDir(getFaceDir());
    faceDir.clear();
}
void Role::initDizzyTimer()
{
    dizzyTimer.setSingleShot(true);
    connect(&dizzyTimer,&QTimer::timeout,this,[this]
    {
        qDebug() << "dizzy end";
        this->swapDirKeys();
        emit dizzyEnd();
    });
}
void Role::dizzy()
{
    if(isDizzy())//已经处于dizzy状态
    {
        dizzyTimer.stop();//停止
    }
    else{
        swapDirKeys();
    }
    startDizzyTimer();
}
void Role::pauseDizzyTimer()
{
    dizzyRemainingTime = dizzyTimer.remainingTime();
    qDebug() << "dizzyRemainingTime: " << dizzyRemainingTime;
    qDebug() << "pause for a while.";
    dizzyTimer.stop();
}
void Role::startDizzyTimer()
{
    dizzyTimer.start(dizzyRemainingTime);
    dizzyRemainingTime = getDizzyTime();//重置为dizzyTime
}

void Role::initFreezeTimer()
{
    freezeTimer.setSingleShot(true);
    connect(&freezeTimer,&QTimer::timeout,this,[this]
    {
        qDebug() << "freeze end";
        //还原函数
        this->isMovable = true;
        emit freezeEnd();
    });
}
void Role::freeze()
{
    if(!isMovable)//已经处于freeze状态
    {
        freezeTimer.stop();
    }
    else{
        //技能函数
        isMovable = false;
    }
    startFreezeTimer();
}
void Role::pauseFreezeTimer()
{
    freezeRemainingTime = freezeTimer.remainingTime();
    freezeTimer.stop();
}
void Role::startFreezeTimer()
{
    freezeTimer.start(freezeRemainingTime);
    freezeRemainingTime = getFreezeTime();//重置
}

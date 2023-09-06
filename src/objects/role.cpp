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
void Role::insertMoveImages(entity::dir d, QString path)
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
    //player
    entity::dir faceDir = getFaceDir();//玩家面部朝向
    const QRectF& entityBox = getEntityBox();
    //linkBoxes
    BoxMap* linkBoxes = parent->getLinkBoxes();
    int wScale = linkBoxes->getWScale();
    int hScale = linkBoxes->getHScale();
    QSizeF size = linkBoxes->getSize();
    QPointF corner = linkBoxes->getCorner();
    QSizeF dist = linkBoxes->getDist();

    QPoint target(-1,-1);//被激活箱子在linkBoxes中的行、列
    if(faceDir == entity::up)//向上移动
    {
        int i = (entityBox.top()
                 - size.height()
                 - corner.y()
                 - dist.height() / 2)
                / (dist.height() + size.height());

        qreal num = entityBox.center().x()
                - corner.x()
                - dist.width() / 2;
        qreal den = dist.width() + size.width();

        int j1 = qFloor(num / den) , j2 = qFloor((num - size.width()) / den);


        if(j1 != j2)
        {
            if(i >= 0 && i <= hScale - 1
                    &&
                j1 >= 0 && j1 <= wScale - 1)
            {
                target.rx() = i;
                target.ry() = j1;
            }
        }
    }
    else if(faceDir == entity::down)
    {
        int i = (entityBox.bottom()
                 - corner.y()
                 - dist.height() / 2)
                / (dist.height() + size.height());

        qreal num = entityBox.center().x()
                - corner.x()
                - dist.width() / 2;
        qreal den = dist.width() + size.width();

        int j1 = qFloor(num / den) , j2 = qFloor((num - size.width()) / den);


        if(j1 != j2)
        {
            if(i >= 0 && i <= hScale - 1
                    &&
                j1 >= 0 && j1 <= wScale - 1)
            {
                target.rx() = i;
                target.ry() = j1;
            }
        }
    }
    else if(faceDir == entity::left)
    {
        int j = (entityBox.left()
                 - size.width()
                 - corner.x()
                 - dist.width() / 2)
                / (dist.width() + size.width());

        qreal num = entityBox.center().y()
                - corner.y()
                - dist.height() / 2;
        qreal den = dist.height() + size.height();

        int i1 = qFloor(num / den) , i2 = qFloor((num - size.height()) / den);

        if(i1 != i2)
        {
            if(j >= 0 && j <= wScale - 1
                    &&
                i1 >= 0 && i1 <= hScale - 1)
            {
                target.rx() = i1;
                target.ry() = j;
            }
        }
    }
    else if(faceDir == entity::right)
    {
        int j = (entityBox.right()
                 - corner.x()
                 - dist.width() / 2)
                / (dist.width() + size.width());

        qreal num = entityBox.center().y()
                - corner.y()
                - dist.height() / 2;
        qreal den = dist.height() + size.height();

        int i1 = qFloor(num / den) , i2 = qFloor((num - size.height()) / den);

        if(i1 != i2)
        {
            if(j >= 0 && j <= wScale - 1
                    &&
                i1 >= 0 && i1 <= hScale - 1)
            {
                target.rx() = i1;
                target.ry() = j;
            }
        }
    }
    return target;
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

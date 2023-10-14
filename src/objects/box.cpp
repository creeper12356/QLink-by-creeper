#include "box.h"
Box::Box():Entity()
{
    entityType = entity::box;
    isMovable = false;//默认箱子不可移动
    isActivated = false;//默认箱子未激活
    isLocked = false;//默认箱子未上锁
    isHighlighted = false;//默认不高亮显示
    isEntity = true;//默认为实体箱子
}
Box::~Box()
{
    qDebug() << "~box" << "boxType" << boxType;
    if(!isEntity)
    {
        delete head;
        delete breakSound;
    }
}

void Box::setBoxType(box::type t)
{
    boxType = t;
}
void Box::setBoxDivision(box::division d)
{
    boxDivision = d;
}
void Box::setIsEntity(bool i)
{
    isEntity = i;
    if(!isEntity)
    {
        head = new QPixmap;
        breakSound = new QSoundEffect;
    }
}
bool Box::setHead(const QString &path)
{
    return head->load(path);
}
void Box::setHead(QPixmap* pix)
{
    head = pix;
}
void Box::copyStyle(const Box *other)
{
    this->head = other->head;
    this->breakSound = other->breakSound;
    this->breakScore = other->breakScore;
    this->entityBox.setSize(other->entityBox.size());
    this->isMovable = other->isMovable;
}
bool Box::setBreakSound(const QString &path)
{
    if(!QFile(path).exists())
    {
        return false;
    }
    breakSound->setSource(QUrl::fromLocalFile(path));
    return true;
}
void Box::setBreakSoundVolume(qreal v)
{
    breakSound->setVolume(v);
}

void Box::setBreakScore(int bs)
{
    breakScore = bs;
}
void Box::draw(QPainter &painter,bool isDebugMode)
{
    Entity::draw(painter,isDebugMode);
    if(isHighlighted)
    {
        painter.drawPixmap(entityBox.x() - entityBox.width() * 0.1,
                           entityBox.y() - entityBox.height() * 0.1,
                           entityBox.width() * 1.2,
                           entityBox.height() * 1.2,
                           QPixmap(":/images/effects/highlight.png"));
    }
    if(isActivated)
    {
        painter.drawPixmap(entityBox.x() - entityBox.width() * 0.1,
                           entityBox.y() - entityBox.height() * 0.1,
                           entityBox.width() * 1.2,
                           entityBox.height() * 1.2,
                           QPixmap(":/images/effects/activate.png"));
    }
}

box::division Box::typeToDivision(box::type t)
{
    if(t < box::clock)
    {
        return box::plain_box;
    }
    else
    {
        return box::prop_box;
    }
}
void Box::playBreakSound() const
{
    if(breakSound->isLoaded())
    {
        breakSound->play();
    }
}

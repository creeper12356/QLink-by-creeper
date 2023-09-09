#include "entity.h"
#include "widgets/gamemain.h"

#define EPS 1e-8//比较浮点数时的精度

entity::dir entity::opsiteDir(entity::dir d)
{
    if(d == entity::up)
    {
        return entity::down;
    }
    else if(d == entity::down)
    {
        return entity::up;
    }
    else if(d == entity::left)
    {
        return entity::right;
    }
    else if(d == entity::right)
    {
        return entity::left;
    }
    else
    {
        return entity::no_dir;
    }
}
QPoint entity::unitDisplacement(const QPoint& staPt,entity::dir d)
{
    QPoint terPt = staPt;
    if(d == up)
    {
        --terPt.rx();
    }
    else if(d == down)
    {
        ++terPt.rx();
    }
    else if(d == left)
    {
        --terPt.ry();
    }
    else if(d == right)
    {
        ++terPt.ry();
    }
    return terPt;
}
Entity::Entity()
{
    isIdVisible = false;//默认不显示id
    isMovable = true;//默认允许移动
}

void Entity::setEntityBox(const QRectF &r)
{
    entityBox = r;
}
void Entity::setPos(qreal x, qreal y)
{
    entityBox.moveTopLeft(QPointF(x,y));
}
void Entity::setPos(QPointF p)
{
    entityBox.moveTopLeft(p);
}
void Entity::setSize(qreal w, qreal h)
{
    entityBox.setSize(QSizeF(w,h));
}
void Entity::setSize(QSizeF sz)
{
    entityBox.setSize(sz);
}
void Entity::setWidth(qreal w)
{
    entityBox.setSize(QSizeF(w,entityBox.height()));
}
void Entity::setHeight(qreal h)
{
    entityBox.setSize(QSizeF(entityBox.width(),h));
}

qreal Entity::boundarySpace(const Map *m, int dir) const
{
    switch (dir) {
    case 0:return entityBox.top();break;
    case 1:return m->getHScale() * m->getBlockSize() - entityBox.bottom();break;
    case 2:return entityBox.left();break;
    case 3:return m->getWScale() * m->getBlockSize() - entityBox.right();break;
    default:return 0.0;break;
    }
}
qreal Entity::space(const Entity *other, int dir) const
{
    switch (dir) {
    case 0:return (this->entityBox.top() - other->entityBox.bottom());break;
    case 1:return (other->entityBox.top() - this->entityBox.bottom());break;
    case 2:return (this->entityBox.left() - other->entityBox.right());break;
    case 3:return (other->entityBox.left() - this->entityBox.right());break;
    default:return 0.0;
    }
}
void Entity::pureMove(qreal singleMove, int dir)
{
    switch (dir) {
    case 0:entityBox.adjust(0,-singleMove,0,-singleMove);break;
    case 1:entityBox.adjust(0,singleMove,0,singleMove);break;
    case 2:entityBox.adjust(-singleMove,0,-singleMove,0);break;
    case 3:entityBox.adjust(singleMove,0,singleMove,0);break;
    }
}
bool Entity::interSectionX(const Entity *e1, const Entity *e2)
{
    if((e1->entityBox.left() - e2->entityBox.right())
        *(e1->entityBox.right() - e2->entityBox.left())
        < 0)//x方向（水平方向有交集）
        return true;
    else return false;
}
bool Entity::interSectionY(const Entity *e1, const Entity *e2)
{
    if((e1->entityBox.top() - e2->entityBox.bottom())
        * (e1->entityBox.bottom() - e2->entityBox.top())
        < 0)//y方向（竖直方向有交集）
        return true;
    else return false;
}
bool Entity::interSection(const Entity* other, qreal singleMove,int dir) const
{
    if(dir <= 1)//上下运动
    {
        if(!interSectionX(this,other))
            return false;
    }
    else //左右运动
    {
        if(!interSectionY(this,other))
            return false;
    }
    if(space(other,dir) >= -EPS && space(other,dir) - singleMove < EPS)
        return true;
    else return false;
}
qreal Entity::move(GameMain* parent,qreal singleMove,int dir)
{
    if(!isMovable)//无法移动
        return 0.0;

    QVector<Entity*> neighbors;//存储可能碰撞实体的地址
    QVector<Entity*>& entities = parent->getEntities();
    int entitySize = parent->getEntities().size();//所有实体数量
    for(int i = 0;i <= entitySize - 1;++i)//遍历地图中所有实体
    {
        if(entities[i] == this)
            continue;
        if(interSection(entities[i],singleMove,dir) && entities[i]->entityType == entity::box)
            //邻近实体为箱子且可能碰撞
        {
            neighbors.push_back(entities[i]);
        }
    }

    if(neighbors.empty())//无邻近箱子
    {
        qreal realSingleMove = qMin(singleMove,
                                    boundarySpace(parent->getBgd(),dir));
        pureMove(realSingleMove,dir);
        return realSingleMove;
    }
    else
    {
        int neighborSize = neighbors.size();
        qreal minSingleMove = Q_INFINITY;//最小步长
        for(int i = 0;i <= neighborSize - 1;++i)//遍历所有邻近实体判断是否可以移动
        {
            qreal step =
                singleMove - space(neighbors[i],dir);
            qreal realSingleMove =
                    neighbors[i]->move(parent,step,dir) - step + singleMove;
            minSingleMove = qMin(realSingleMove,minSingleMove);
        }
        pureMove(minSingleMove,dir);
        return minSingleMove;
    }
}

void Entity::draw(QPainter &painter,bool isDebugMode)
{
    if(isIdVisible)//绘制实体id
    {
        painter.drawText(getPos(),id);
    }

    if(isDebugMode)//调试模式
    {
        painter.save();
        painter.setPen(QColor(0,0,0));
        painter.setBrush(QColor(180,20,180));
        painter.drawRect(getEntityBox().toRect());
        painter.restore();

        QString posInfo = "("
                + QString::number(getEntityBox().x())
                + ","
                + QString::number(getEntityBox().y())
                + ")";
        QPointF posCoord(getEntityBox().x() + id.length() * 10,
                         getEntityBox().y() + 2);
        painter.drawText(posCoord,posInfo);
    }

    painter.drawPixmap(getEntityBox().toRect()
                        ,getHead());//绘制实体本身
}

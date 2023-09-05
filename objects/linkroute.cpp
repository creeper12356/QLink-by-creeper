#include "linkroute.h"
#include "boxmap.h"

entity::dir LinkRoute::dirBetweenNodes(int i, int j)
{
    //保证安全
    if(i < 0 || j >= size())
    {
        return entity::no_dir;
    }
    if(j != i + 1)
    {
        return entity::no_dir;
    }

    if(nodes[i].pos.x() > nodes[j].pos.x())
    {
        return entity::up;
    }
    if(nodes[i].pos.x() < nodes[j].pos.x())
    {
        return entity::down;
    }
    if(nodes[i].pos.y() > nodes[j].pos.y())
    {
        return entity::left;
    }
    if(nodes[i].pos.y() < nodes[j].pos.y())
    {
        return entity::right;
    }
    return entity::no_dir;
}
entity::dir LinkRoute::dirBeforeNode(int i)
{
    return dirBetweenNodes(i - 1,i);
}
entity::dir LinkRoute::dirAfterNode(int i)
{
    return dirBetweenNodes(i,i + 1);
}

LinkRoute::LinkRoute()
{
    initNodePic();
}
LinkRoute::~LinkRoute()
{
    qDebug() << "~LinkRoute";
}
void LinkRoute::initNodePic()
{
    nodePic.insert(linkRoute::up_down,QPixmap(":/images/link_route/up_down.png"));
    nodePic.insert(linkRoute::left_right,QPixmap(":/images/link_route/left_right.png"));
    nodePic.insert(linkRoute::up_left,QPixmap(":/images/link_route/up_left.png"));
    nodePic.insert(linkRoute::up_right,QPixmap(":/images/link_route/up_right.png"));
    nodePic.insert(linkRoute::down_left,QPixmap(":/images/link_route/down_left.png"));
    nodePic.insert(linkRoute::down_right,QPixmap(":/images/link_route/down_right.png"));
}
void LinkRoute::appendNode(const QPoint &pos, linkRoute::nodeDir dir)
{
    nodes.append(LinkRouteNode(pos,dir));
}
void LinkRoute::removeFirst()
{
    nodes.removeFirst();
}
void LinkRoute::removeLast()
{
    nodes.removeLast();
}
void LinkRoute::limitSize(int maxSize)
{
    if(nodes.size() <= maxSize)
    {
        return ;
    }
    nodes.resize(maxSize);
}
int LinkRoute::size() const
{
    return nodes.size();
}

void LinkRoute::updateDir()
{
    int nodeSize = size();
    entity::dir before = entity::no_dir;
    entity::dir after = entity::no_dir;
    for(int i = 0;i <= nodeSize - 1;++i)
    {
        after = dirAfterNode(i);
        if(i == 0)
        {
            nodes[i].dir = linkRoute::nodeDir(after / 2);
        }
        else if(i == nodeSize - 1)
        {
            nodes[i].dir = linkRoute::nodeDir(before / 2);
        }
        else
        {
            if((before == entity::right && after == entity::up)
                    || (before == entity::down && after == entity::left))
            {
                nodes[i].dir = linkRoute::up_left;
            }
            else if((before == entity::left && after == entity::up)
                    || (before == entity::down && after == entity::right))
            {
                nodes[i].dir = linkRoute::up_right;
            }
            else if((before == entity::right && after == entity::down)
                    || (before == entity::up && after == entity::left))
            {
                nodes[i].dir = linkRoute::down_left;
            }
            else if((before == entity::left && after == entity::down)
                    || (before == entity::up && after == entity::right))
            {
                nodes[i].dir = linkRoute::down_right;
            }
            else if(before == entity::left || before == entity::right)
            {
                nodes[i].dir = linkRoute::left_right;
            }
            else if(before == entity::up || before == entity::down)
            {
                nodes[i].dir = linkRoute::up_down;
            }
        }
        before = after;
    }
}

void LinkRoute::print() const
{
    qDebug() << "linkRoute: ";
    for(auto& node:nodes)
    {
        qDebug() << '\t' << node.pos;
    }
    qDebug() << "linkRoute end.";
}
void LinkRoute::draw(QPainter &painter, BoxMap *linkBoxes)
{
    for(auto& node:nodes)
    {
        QRectF rect = linkBoxes->rectAt(node.pos.x(),node.pos.y());
        painter.drawPixmap(rect.toRect(),nodePic[node.dir]);
    }
}

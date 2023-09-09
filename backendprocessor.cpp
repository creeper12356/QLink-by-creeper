#include "backendprocessor.h"
#include "objects/boxmap.h"
#include "record.h"
using namespace box;
BackendProcessor::BackendProcessor():data(nullptr)
{
}

BackendProcessor::BackendProcessor(const Record& record)
    :
      wScale(record.getBasic().wScale),
      hScale(record.getBasic().hScale)
{
    init2DArray();
    int index = 0;
    for(int i = 0;i <= wScale - 1;++i){
        for(int j = 0;j <= hScale - 1;++j){
            data[i][j] = record.getMap()[index++];
        }
    }
}
BackendProcessor::BackendProcessor(BoxMap* lkBoxes)
    :data(lkBoxes->getData()),wScale(lkBoxes->getWScale()),hScale(lkBoxes->getHScale())
{
    linkBoxes = lkBoxes;
}

void BackendProcessor::init2DArray()
{
    data = new box::type* [hScale];
    for(int i = 0;i <= hScale - 1;++i)
    {
        data[i] = new box::type[wScale];
    }
}

bool BackendProcessor::isLegal(const QPoint &pt) const
{
    if(pt.x() < 0 || pt.x() > hScale - 1)
        return false;
    if(pt.y() < 0 || pt.y() > wScale - 1)
    {
        return false;
    }
    return true;
}
bool BackendProcessor::isGeneralizedLegal(const QPoint &pt) const
{
    if(pt.x() < -1 || pt.x() > hScale)
    {
        return false;
    }
    if(pt.y() < -1 || pt.y() > wScale)
    {
        return false;
    }
    return true;
}
type BackendProcessor::dataAt(int x, int y) const
{
    if(!isLegal(QPoint(x,y)))
    {
        return null;
    }
    return data[x][y];
}
type BackendProcessor::dataAt(const QPoint &pt) const
{
    if(!isLegal(pt))
        return null;
    return data[pt.x()][pt.y()];
}
void BackendProcessor::load(type **d, int w, int h)
{
    data = d;
    wScale = w;
    hScale = h;
}

void BackendProcessor::delete2DArray()
{
    for(int i = 0;i <= hScale - 1;++i)
    {
        delete [] data[i];
    }
    delete [] data;
}

void BackendProcessor::generateFromArray(const QJsonArray &jArray)
{
    int index = 0;
    for(int i = 0;i <= hScale - 1;++i)
    {
        for(int j = 0;j <= wScale - 1;++j)
        {
            data[i][j] = box::type(jArray.at(index).toInt());
            ++index;
        }
    }
}

void BackendProcessor::generateFromVector(const QVector<type> map)
{
    int index = 0;
    for(int i = 0;i <= hScale - 1;++i)
    {
        for(int j = 0;j <= wScale - 1;++j)
        {
            data[i][j] = map[index];
            ++index;
        }
    }
}
bool BackendProcessor::randomSwapOnce()
{
    int r1 = QRandomGenerator::global()->bounded(hScale),
        c1 = QRandomGenerator::global()->bounded(wScale),
        r2 = QRandomGenerator::global()->bounded(hScale),
        c2 = QRandomGenerator::global()->bounded(wScale);
    if(data[r1][c1] == null || data[r2][c2] == null)//有空箱子
    {
        return false;
    }
    if(r1 == r2 && c1 == c2)//选中同一个实体
    {
        return false;
    }
    if(data[r1][c1] == data[r2][c2])//选中箱子种类相同
    {
        return false;
    }

    //前端允许交换才执行交换函数
    if(linkBoxes->swapEntityBox(QPoint(r1,c1),QPoint(r2,c2)))
    {
        qSwap(data[r1][c1],data[r2][c2]);
        return true;
    }
    else
    {
        return false;
    }
}
void BackendProcessor::shuffle(int times)
{
    for(int i = 1;i <= times;++i)
    {
        randomSwapOnce();
    }
}
void BackendProcessor::shuffle()
{
    shuffle(wScale * hScale);
}
/*checkLink过时算法
bool BackendProcessor::checkLinkTwistZero(const QPoint &p1, const QPoint &p2) const
{
    if(p1.x() == p2.x())//同一行
    {
        int minY = qMin(p1.y(),p2.y());
        int maxY = qMax(p1.y(),p2.y());
        for(int j = minY + 1;j <= maxY - 1;++j)
        {
            if(dataAt(p1.x(),j) != null)//有阻碍方块
            {
                return false;
            }
        }
        return true;
    }
    else if(p1.y() == p2.y())//同一列
    {
        int minX = qMin(p1.x(),p2.x());
        int maxX = qMax(p1.x(),p2.x());
        for(int i = minX + 1;i <= maxX - 1;++i)
        {
            if(dataAt(i,p1.y()) != null)//有阻碍方块
            {
                return false;
            }
        }
        return true;
    }
    else return false;//既不在同一行也不再同一列
}
bool BackendProcessor::checkLinkTwistOnce(const QPoint &p1, const QPoint &p2) const
{
    if(p1.x() == p2.x() || p1.y() == p2.y())//同一行或同一列
    {
        return false;
    }
    QPoint twist;//转折点
    //case 1
    twist.rx() = p1.x();
    twist.ry() = p2.y();
    if(dataAt(twist) == null
            && checkLinkTwistZero(p1,twist)
            && checkLinkTwistZero(p2,twist))
    {
        return true;
    }
    //case 2
    twist.rx() = p2.x();
    twist.ry() = p1.y();
    if(dataAt(twist) == null
            && checkLinkTwistZero(p1,twist)
            && checkLinkTwistZero(p2,twist))
    {
        return true;
    }
    //default
    return false;

}
// 最复杂的两个转折连通情况:
// 两点A,B不在同一行也不在同一列，在A,B所在行、列所在四条直线找点C
// 如果存在C使得
// C与A行（或列）连通且C与B单拐点连通
// 或
// C与B行（或列）连通且C与A单拐点连通
//
// 则两点可以通过两个转折连通
bool BackendProcessor::checkLinkTwistTwice(const QPoint &p1, const QPoint &p2) const
{

    for(int i = -1;i <= hScale;++i)
    {
        for(int j = -1;j <= wScale;++j)
        {
            if(dataAt(i,j) != box::null)//不为空不能作为拐点
            {
                continue;
            }
            if(i != p1.x()
                    && i != p2.x()
                    && j != p1.y()
                    && j != p2.y())
                //不在搜索的四条直线上
            {
                continue;
            }
            if((i == p1.x() && j == p1.y())
                    || (i == p2.x() && j == p2.y())
                    || (i == p1.x() && j == p2.y())
                    || (i == p2.x() && j == p1.y()))
                //四条直线交点也要过滤掉
            {
                continue;
            }

            QPoint searchPt(i,j);
            if((checkLinkTwistZero(searchPt,p1) && checkLinkTwistOnce(searchPt,p2))
                    ||(checkLinkTwistZero(searchPt,p2) && checkLinkTwistOnce(searchPt,p1)))
            {
                return true;
            }

        }
    }
    return false;//找不到满足条件的点

}
*/
/*未使用深度优先遍历的checkLink函数
bool BackendProcessor::checkLink(const QPoint &p1, const QPoint &p2) const
{
    if(dataAt(p1) != dataAt(p2))//不是同一种
    {
        return false;
    }
    //相同种类方块
    if(checkLinkTwistZero(p1,p2))
    {
        return true;
    }
    if(checkLinkTwistOnce(p1,p2))
    {
        return true;
    }
    if(checkLinkTwistTwice(p1,p2))
    {
        return true;
    }
    return false;
}
*/

bool BackendProcessor::checkLink(const QPoint &p1, const QPoint &p2,LinkRoute*& bestRoute) const
{
    if(dataAt(p1) != dataAt(p2))//箱子种类不同
    {
        return false;
    }
    Tile startTile;
    startTile.pos = p1;
    startTile.depth = 0;

    QStack<Tile> sta;//深度优先遍历辅助栈
    LinkRoute route;//记录路径
    bestRoute = new LinkRoute;//最短路径，存储在堆上，便于发送指针
    int result = 0;//记录从p1到p2路径总数

    sta.push(startTile);

    while(!sta.empty())
    {
        Tile lastTile = sta.pop();
        route.limitSize(lastTile.depth);
        route.appendNode(lastTile.pos,linkRoute::no_dir);

        //遍历四个方向搜寻
        for(int i = 0;i <= 3;++i)
        {
            QPoint curPt = entity::unitDisplacement(lastTile.pos,entity::dir(i));
            if(!isGeneralizedLegal(curPt))//路径不满足广义合法
            {
                continue;
            }

            Tile curTile;
            curTile.pos = curPt;
            curTile.turn = lastTile.turn;
            curTile.dir = entity::dir(i);
            curTile.depth = lastTile.depth + 1;

            //上一个节点方向改变
            if(lastTile.dir != curTile.dir)
            {
                ++curTile.turn;
            }
            //检查是否回退
            if(entity::opsiteDir(lastTile.dir) == curTile.dir)
            {
                continue;
            }

            //超过两次转弯不合法
            if(curTile.turn > 2)
            {
                continue;
            }

            //找到相邻的空节点
            if(dataAt(curTile.pos) == null)
            {
                sta.push(curTile);
            }
            else if(curTile.pos == p2)//找到终点
            {
                //.test
                route.rturn() = curTile.turn;
                if(result == 0)
                {
                    *bestRoute = route;
                }
                else
                {
                    if(bestRoute->size() > route.size())
                    {
                        *bestRoute = route;
                    }
                }
                ++result;

            }

        }
    }
    if(result == 0)//没有可以连接的路径
    {
        delete bestRoute;
        bestRoute = nullptr;
        return false;
    }
    //result > 0
    bestRoute->appendNode(p2,linkRoute::no_dir);
    return true;
}

QPoint BackendProcessor::hintFrom(const QPoint &startPt) const
{
    Tile startTile;

    startTile.pos = startPt;
    startTile.depth = 0;

    QStack<Tile> sta;//深度优先遍历辅助栈
    LinkRoute route;//记录路径

    sta.push(startTile);

    while(!sta.empty())
    {
        Tile lastTile = sta.pop();
        route.limitSize(lastTile.depth);
        route.appendNode(lastTile.pos,linkRoute::no_dir);

        //遍历四个方向搜寻
        for(int i = 0;i <= 3;++i)
        {
            QPoint curPt = entity::unitDisplacement(lastTile.pos,entity::dir(i));
            if(!isGeneralizedLegal(curPt))//路径不满足广义合法
            {
                continue;
            }

            Tile curTile;
            curTile.pos = curPt;
            curTile.turn = lastTile.turn;
            curTile.dir = entity::dir(i);
            curTile.depth = lastTile.depth + 1;

            //上一个节点方向改变
            if(lastTile.dir != curTile.dir)
            {
                ++curTile.turn;
            }
            //检查是否回退
            if(entity::opsiteDir(lastTile.dir) == curTile.dir)
            {
                continue;
            }

            //超过两次转弯不合法
            if(curTile.turn > 2)
            {
                continue;
            }

            //找到相邻的空节点
            if(dataAt(curTile.pos) == null)
            {
                sta.push(curTile);
            }
            else if(dataAt(curTile.pos) == dataAt(startTile.pos) //找到可以连接的方块
                    && !linkBoxes->getPtrDataAt(curTile.pos)->isLocked)//且未上锁
            {
                return curTile.pos;
            }
        }
    }
    //no endPt
    return QPoint(-1,-1);
}
QVector<QPoint> BackendProcessor::hint() const
{
    QVector<QPoint> ans;

    QVector<QPoint> traversePos;
    for(int i = 0;i <= hScale - 1;++i)
    {
        for(int j = 0;j <= wScale - 1;++j)
        {
            if(dataAt(i,j) != null //all entity boxes
            && !linkBoxes->getPtrDataAt(i,j)->isLocked //all plain_box except locked one
            && Box::typeToDivision(dataAt(i,j)) != prop_box //all plain_box and null
            ){
                traversePos.push_back(QPoint(i,j));
            }
        }
    }
    //shuffle for times
    for(int i = 0;i <= traversePos.size() - 1;++i){
        int i1 = QRandomGenerator::global()->bounded(traversePos.size()),
            i2 = QRandomGenerator::global()->bounded(traversePos.size());
        qSwap(traversePos[i1],traversePos[i2]);
    }
    qDebug() << "traverPos" << traversePos;

    for(int i = 0;i <= traversePos.size() - 1;++i){
        QPoint endPt = hintFrom(traversePos[i]);
        if(endPt.x() != -1){
            ans.push_back(traversePos[i]);
            ans.push_back(endPt);
            return ans;
        }
    }
    //no solve
    return ans;
}

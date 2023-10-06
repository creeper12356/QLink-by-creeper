#include "backendprocessor.h"
#include "objects/boxmap.h"
#include "record.h"
using namespace box;

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
    :data(lkBoxes->getData()),
      wScale(lkBoxes->getWScale()),
      hScale(lkBoxes->getHScale()),
      linkBoxes(lkBoxes)
{

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
    bestRoute = new LinkRoute;//最短路径，存储在堆上
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

bool BackendProcessor::checkLink(const QPoint &p1, const QPoint &p2) const
{
    LinkRoute* route = nullptr;
    bool result = checkLink(p1,p2,route);
    delete route;
    return result;
}

QVector<QPoint> BackendProcessor::hintFrom(const QPoint &startPt) const
{
    QVector<QPoint> ans;
    Tile startTile;
    startTile.pos = startPt;
    startTile.depth = 0;
    QStack<Tile> sta;//深度优先遍历辅助栈
    LinkRoute route;//记录路径

    sta.push(startTile);
    while(!sta.empty()){
        Tile lastTile = sta.pop();
        route.limitSize(lastTile.depth);
        route.appendNode(lastTile.pos,linkRoute::no_dir);

        //遍历四个方向搜寻
        for(int i = 0;i <= 3;++i){
            QPoint curPt = entity::unitDisplacement(lastTile.pos,entity::dir(i));
            if(!isGeneralizedLegal(curPt)){//不满足广义合法
                continue;
            }
            Tile curTile;
            curTile.pos = curPt;
            curTile.turn = lastTile.turn;
            curTile.dir = entity::dir(i);
            curTile.depth = lastTile.depth + 1;
            //上一个节点方向改变
            if(lastTile.dir != curTile.dir){
                ++curTile.turn;
            }
            //检查是否回退
            if(entity::opsiteDir(lastTile.dir) == curTile.dir){
                continue;
            }
            //超过两次转弯不合法
            if(curTile.turn > 2){
                continue;
            }
            //找到相邻的空节点
            if(dataAt(curTile.pos) == null){
                sta.push(curTile);
            }
            else if(dataAt(curTile.pos) == dataAt(startTile.pos) //找到可以连接的方块
                    && !linkBoxes->getPtrDataAt(curTile.pos)->isLocked){//且未上锁
                if(!ans.contains(curTile.pos)){
                    ans.push_back(curTile.pos);
                }
            }
        }
    }
    return ans;
}

void BackendProcessor::shuffleVector(QVector<QPoint> &targetVec) const
{
    int size = targetVec.size();
    qreal factor = QRandomGenerator::global()->bounded(3.0) + 1;//1-3随机实数
    qDebug() << "factor " << factor;
    qDebug() << "shuffle for " << factor * size << " times.";
    //随机交换factor * size 次
    int times = factor * size;
    for(int i = 1;i <= times ;++i){
        int i1 = QRandomGenerator::global()->bounded(size),
            i2 = QRandomGenerator::global()->bounded(size);
        qSwap(targetVec[i1],targetVec[i2]);
    }
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
    shuffleVector(traversePos);

    for(const QPoint& startPt:traversePos){
        QVector<QPoint> endPts = hintFrom(startPt);
        if(!endPts.isEmpty()){
            ans.push_back(startPt);
            ans.push_back(endPts[0]);
            break;
        }
    }
    return ans;
}

QVector<QPoint> BackendProcessor::hint(const QPoint &standPt) const
{
    QVector<QPoint> ans;

    QVector<QPoint> reachablePts = reachableFrom(standPt);

    //find solution
    for(const QPoint& startPt:reachablePts){
        QVector<QPoint> endPts = hintFrom(startPt);
        for(const QPoint& endPt:endPts){
            if(reachablePts.contains(endPt)){
                ans.push_back(startPt);
                ans.push_back(endPt);
                return ans;
            }
        }
    }
    return ans;
}

QVector<QPoint> BackendProcessor::reachableFrom(const QPoint &startPt) const
{
    //边界情况，重定向到(-1,-1)
    if(!isGeneralizedLegal(startPt)){
        return reachableFrom(QPoint(-1,-1));
    }
    //using bfs traverse
    QVector<QPoint> ans;
    QQueue<QPoint> qu;
    QVector<QPoint> visited;
    qu.push_back(startPt);
    QPoint cur;
    while(!qu.empty()){
        cur = qu.front();
        qu.pop_front();
        visited.push_back(cur);
        //遍历四个方向
        for(int i = 0;i <= 3;++i){
            QPoint tar = entity::unitDisplacement(cur,entity::dir(i));
            if(!isGeneralizedLegal(tar)){
                continue;
            }
            if(visited.contains(tar)){
                continue;
            }
            if(dataAt(tar) == null){
                if(!qu.contains(tar)){
                    qu.push_back(tar);
                }
            }
            else{
                    ans.push_back(tar);
            }
        }
    }
    return ans;
}

bool BackendProcessor::isSolvable() const
{
    //只要道具存在就返回true
    if(!linkBoxes->getPropBoxes().empty()){
        return true;
    }
    //不存在道具
    return (hint().size() == 2);
}

void BackendProcessor::reorganize(int w, int h)
{
    for(int i = 0;i <= h - 1;++i){
        for(int j = 0;j <= w - 1;++j){
            qDebug() << "i = " << i;
            qDebug() << "j = " << j;
            data[i][j] = box::apple;
        }
    }
}

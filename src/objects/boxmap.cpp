#include "boxmap.h"
#include "widgets/gamemain.h"
#include "backendprocessor.h"
#include "widgets/settings.h"
#include "ui_settings.h"
#include "record.h"

QVector<box::type> BoxMap::boxData;
BoxMap::BoxMap(Settings*& sts,const Record& record)
    :settings(sts)
{
    initBasicData(record);
    initBoxes();
    init2DArray();
    initProcessor(record);
}

BoxMap::~BoxMap()
{
    //释放二维数组空间
    for(int i = 0;i <= hScale - 1;++i)
    {
        delete [] data[i];
        delete [] ptrData[i];
    }
    delete [] data;
    delete [] ptrData;

    delete processor;

    //释放档案所占空间
    qDebug() << "delete box archive...";
    for(auto ptr:boxes)
    {

        delete ptr;
    }
}

void BoxMap::initBasicData(const Record &record)
{
    auto& basic = record.getBasic();
    wScale = basic.wScale;
    hScale = basic.hScale;
    corner = basic.corner;
    size = basic.size;
    dist = basic.dist;
}
void BoxMap::init2DArray()
{
    ptrData = new Box**[hScale];
    data = new box::type* [hScale];
    for(int i = 0;i <= hScale - 1;++i)
    {
        ptrData[i] = new Box*[wScale];
        data[i] = new box::type[wScale];
        for(int j = 0;j <= wScale - 1;++j)
        {
            ptrData[i][j] = nullptr;//初始化为null
        }
    }
}

void BoxMap::initProcessor(const Record &record)
{
    processor = new BackendProcessor(this);
    processor->generateFromVector(record.getMap());
}
void BoxMap::initBoxes()
{
    Box* handle;
    for(auto box:settings->getBoxes()){
        QString id = box->value("id").toString();
        handle = new Box;
        handle->setIsEntity(false);
        handle->setHead(":/images/boxes/" + id + ".png");

        if(!handle->setBreakSound(":/audios/" + id + ".wav"))//如果没有找到对应的音效
        {
            handle->setBreakSound(":/audios/block.wav");//使用默认音效
        }
        handle->setBreakScore(box->value("breakScore").toInt());
        handle->setBreakSoundVolume(settings->getUi()->entity_audio_slider->value() / 100.0);
        handle->setSize(size);

        boxes.push_back(handle);
    }
}
void BoxMap::generateEntities(GameMain *parent)
{
    for(int i = 0;i <= hScale - 1;++i)
    {
        for(int j = 0;j <= wScale - 1;++j)
        {
            if(data[i][j] == null)
            {
                nullBoxes.append(QPoint(i,j));
                continue;
            }
            else if(Box::typeToDivision(data[i][j]) == box::plain_box)
            {
                ptrData[i][j] = new PlainBox;
                plainBoxes.append(QPoint(i,j));
            }
            else if(Box::typeToDivision(data[i][j]) == box::prop_box)
            {
                ptrData[i][j] = new PropBox;
                propBoxes.append(QPoint(i,j));
            }
            //init box entity data
            ptrData[i][j]->setBoxType(data[i][j]);
            ptrData[i][j]->setPos(this->boxPosAtData(i,j));
            ptrData[i][j]->copyStyle(boxes[data[i][j]]);
            parent->addBox(ptrData[i][j]);
        }
    }
}

bool BoxMap::addBoxAt(const QPoint &pt, type type)
{
    return addBoxAt(pt.x(),pt.y(),type);
}

bool BoxMap::addBoxAt(int x, int y, type type)
{
    if(data[x][y] != null){
        qDebug() << "cannot override existing box.";
        return false;
    }
    if(type == null){
        return false;
    }

    data[x][y] = type;
    if(Box::typeToDivision(type) == plain_box){
        ptrData[x][y] = new PlainBox;
        plainBoxes.append(QPoint(x,y));
    }
    else{
        ptrData[x][y] = new PropBox;
        propBoxes.append(QPoint(x,y));
    }
    nullBoxes.removeOne(QPoint(x,y));
    Box* newBox = ptrData[x][y];
    newBox->setBoxType(type);
    newBox->setPos(this->boxPosAtData(x,y));
    newBox->copyStyle(boxes[type]);
    return true;
}

bool BoxMap::removeBoxAt(QPoint pt)
{
    return removeBoxAt(pt.x(),pt.y());
}
bool BoxMap::removeBoxAt(int x, int y)
{
    if(data[x][y] == null){
        return false;
    }
    if(Box::typeToDivision(data[x][y]) == plain_box){
        plainBoxes.removeOne(QPoint(x,y));
    }
    else{//prop_box
        propBoxes.removeOne(QPoint(x,y));
    }
    nullBoxes.append(QPoint(x,y));
    delete ptrData[x][y];
    ptrData[x][y] = nullptr;
    data[x][y] = null;
    return true;
}
QPointF BoxMap::boxPosAtData(int i, int j)
{
    return QPointF(corner.x() + dist.width() / 2 + (dist.width() + size.width()) * j,
                   corner.y() + dist.height() / 2 + (dist.height() + size.height()) * i);
}
QRectF BoxMap::rectAt(int i, int j)
{
    QRectF rect;
    rect.moveTo(corner.x() + (dist.width() + size.width()) * j,
            corner.y() + (dist.height() + size.height()) * i);
    rect.setWidth(dist.width() + size.width());
    rect.setHeight(dist.height() + size.height());
    return rect;
}

QPoint BoxMap::posToDataCoord(const QPointF &pos)
{
    bool ok;
    return posToDataCoord(pos,&ok);
}

QPoint BoxMap::posToDataCoord(const QPointF &pos, bool *ok)
{
    QPoint coord(-1,-1);
    //determine j
    coord.ry() = calculateIndex(pos.x(),corner.x(),size.width(),dist.width(),ok);
    if(!*ok){
        return QPoint(-1,-1);
    }
    //determine i
    coord.rx() = calculateIndex(pos.y(),corner.y(),size.height(),dist.height(),ok);
    if(!*ok){
        return QPoint(-1,-1);
    }

    return coord;
}

qreal BoxMap::smallerEdge(const qreal &pos, const qreal &corner, const qreal &size, const qreal &dist) const
{
    return (pos - corner - dist / 2 - size) / (dist + size);
}

qreal BoxMap::largerEdge(const qreal &pos, const qreal &corner, const qreal &size, const qreal &dist) const
{
    return (pos - corner - dist / 2) / (dist + size);
}

int BoxMap::calculateIndex(const qreal &pos, const qreal &corner, const qreal &size, const qreal &dist, bool *ok) const
{
    //i1 >= i2
    int i1 = qFloor(largerEdge(pos,corner,size,dist)),
        i2 = qFloor(smallerEdge(pos,corner,size,dist));
    if(i1 != i2)
    {
        *ok = true;
        return i1;
    }
    else{
        *ok = false;
        return -1;
    }
}

QVector<int> BoxMap::coverIndex(const qreal &corner, const qreal &size, const qreal &dist, qreal min, qreal max)
{
    Q_ASSERT(min < max);

    //min增大方向第一个smallerEdge
    int CeilIncreaseSmallerEdge = qCeil(smallerEdge(min,corner,size,dist) + EPS);
    //max减小方向第一个largerEdge
    int FloorDecreaseLargerEdge = qFloor(largerEdge(max,corner,size,dist) - EPS);

    //result
    QVector<int> indexes;
    indexes.push_back(CeilIncreaseSmallerEdge);
    if(CeilIncreaseSmallerEdge != FloorDecreaseLargerEdge){
        indexes.push_back(FloorDecreaseLargerEdge);
    }
    return indexes;
}

QVector<QPoint> BoxMap::coverDataCoords(const QRectF &entityBox)
{
    //行坐标集
    QVector<int> rowIndexes = coverIndex(corner.y(),size.height(),dist.height(),entityBox.top(),entityBox.bottom());
    //纵坐标集
    QVector<int> colIndexes = coverIndex(corner.x(),size.width(),dist.width(),entityBox.left(),entityBox.right());
    //组合
    QVector<QPoint> coverCoords;
    for(int r:rowIndexes){
        for(int c:colIndexes){
            coverCoords.push_back(QPoint(r,c));
        }
    }
    return coverCoords;
}

void BoxMap::initBoxData()
{
    //内嵌，后续修改
    boxData = {
        apple,
        beetroot,
        beetroot_soup,
        carrot,
        golden_apple,
        mushroom_stew,
        watermelon,
        wheat,
        box::clock,
        ender_pearl,
        snow_bucket,
        potion,
        book
    };
}


void BoxMap::draw(QPainter &painter, bool isDebugMode)
{
    if(!isDebugMode)
    {
        return ;
    }
    painter.save();
    painter.setBrush(QColor(255,255,255,100));
    painter.setPen(QPen(QColor(128,128,128,180),5));
    for(int i = 0;i <= hScale - 1;++i)
    {
        for(int j = 0;j <= wScale - 1;++j)
        {
            painter.drawRect(rectAt(i,j));
        }
    }
    painter.restore();
}
bool BoxMap::swapEntityBox(const QPoint& p1,const QPoint& p2)
{
    Box*& box1 = getPtrDataAt(p1),*&box2 = getPtrDataAt(p2);
    if(box1 == nullptr || box2 == nullptr)
    {
        return false;
    }
    if(box1->isLocked || box2->isLocked)
    {
        return false;
    }

    //交换指针指向
    qSwap(box1,box2);
    //交换实体位置
    QPointF tmpPos = box1->getPos();
    box1->setPos(box2->getPos());
    box2->setPos(tmpPos);

    return true;
}

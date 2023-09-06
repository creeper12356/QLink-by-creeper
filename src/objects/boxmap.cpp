#include "boxmap.h"
#include "widgets/gamemain.h"
#include "backendprocessor.h"
#include "widgets/settings.h"
#include "ui_settings.h"
#include "record.h"

//BoxMap::BoxMap(Settings*& sts)
//    :settings(sts)
//{
//    initBasicData();
//    initBoxes();
//    init2DArray();
//    initProcessor();
//}
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
//void BoxMap::initBasicData()
//{
//    QJsonObject* basic = settings->getLevels().value("single").at(0);
//    wScale = basic->value("hScale").toInt();
//    hScale = basic->value("hScale").toInt();
//    corner = QPointF(
//                basic->value("corner").toArray().at(0).toDouble(),
//                basic->value("corner").toArray().at(1).toDouble());
//    size = QSizeF(
//                basic->value("size").toArray().at(0).toDouble(),
//                basic->value("size").toArray().at(1).toDouble());
//    dist = QSizeF(
//                basic->value("dist").toArray().at(0).toDouble(),
//                basic->value("dist").toArray().at(1).toDouble());
//}

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
//void BoxMap::initProcessor()
//{
//    processor = new BackendProcessor(this);
//    processor->generateFromArray(settings->getLevels().value("single").at(0)->value("map").toArray());//生成箱子
//    processor->shuffle();//洗牌
//}

void BoxMap::initProcessor(const Record &record)
{
    processor = new BackendProcessor(this);
    processor->generateFromVector(record.getMap());
}
void BoxMap::initBoxes()
{
    Box* handle;
    for(const QString& id:settings->getBoxes())
    {
        handle = new Box;
        handle->setIsEntity(false);
        handle->setHead(":/images/boxes/" + id + ".png");

        if(!handle->setBreakSound(":/audios/" + id + ".wav"))
        {
            handle->setBreakSound(":/audios/block.wav");
        }
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
                continue;
            }
            else if(Box::typeToDivision(data[i][j]) == box::plain_box)
            {
                ptrData[i][j] = new PlainBox;
            }
            else if(Box::typeToDivision(data[i][j]) == box::prop_box)
            {
                ptrData[i][j] = new PropBox;
            }
            ptrData[i][j]->setBoxType(data[i][j]);
            ptrData[i][j]->setPos(this->boxPosAtData(i,j));
            ptrData[i][j]->copyStyle(boxes[data[i][j]]);
            parent->addBox(ptrData[i][j]);
        }
    }
}

void BoxMap::removeBoxAt(QPoint pt)
{
    delete ptrData[pt.x()][pt.y()];
    ptrData[pt.x()][pt.y()] = nullptr;
    data[pt.x()][pt.y()] = null;
}
void BoxMap::removeBoxAt(int x, int y)
{
    delete ptrData[x][y];
    ptrData[x][y] = nullptr;
    data[x][y] = null;
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
//    QFont font("Calibri");
//    font.setPointSize(20);
//    painter.setFont(font);
//    painter.setPen(QColor(0,0,0,180));
//    painter.translate(0,size.height() / 2);
//    for(int i = 0;i <= hScale - 1;++i)
//    {
//        for(int j = 0;j <= wScale - 1;++j)
//        {
//            //显示箱子坐标
//            painter.drawText(rectAt(i,j),"(" + QString::number(i) + "," + QString::number(j) + ")");
//        }
//    }
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

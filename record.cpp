#include "record.h"
#include "widgets/settings.h"
#include "objects/boxmap.h"

void Basic::readFromJsonObject(const QJsonObject &obj)
{
    if(obj.contains("wScale"))//更新固定参数
    {
        wScale = obj.value("wScale").toInt();
        hScale = obj.value("hScale").toInt();
        corner.setX(obj.value("corner").toArray().at(0).toDouble());
        corner.setY(obj.value("corner").toArray().at(1).toDouble());
        size.setWidth(obj.value("size").toArray().at(0).toDouble());
        size.setHeight(obj.value("size").toArray().at(1).toDouble());
        dist.setWidth(obj.value("dist").toArray().at(0).toDouble());
        dist.setHeight(obj.value("dist").toArray().at(1).toDouble());
    }
    gameTime = obj.value("gameTime").toInt();
}

QJsonObject Basic::writeToJsonObject() const
{
    QJsonObject obj;
    obj.insert("wScale",wScale);
    obj.insert("hScale",hScale);
    obj.insert("corner",QJsonArray({corner.x(),corner.y()}));
    obj.insert("size",QJsonArray({size.width(),size.height()}));
    obj.insert("dist",QJsonArray({dist.width(),dist.height()}));
    obj.insert("gameTime",gameTime);
    return obj;
}

void PlayerInfo::readFromJsonObject(const QJsonObject &obj)
{

    id = obj.value("id").toString();
    pos.setX(obj.value("pos").toArray().at(0).toDouble());
    pos.setY(obj.value("pos").toArray().at(1).toDouble());
    faceDir = entity::dir(obj.value("faceDir").toInt());
    if(obj.contains("score")){
        score = obj.value("score").toInt();
    }
    else{
            score = 0;
    }
}

QJsonObject PlayerInfo::writeToJsonObject() const
{
    QJsonObject obj;
    obj.insert("id",id);
    obj.insert("pos",QJsonArray({pos.x(),pos.y()}));
    obj.insert("faceDir",faceDir);
    obj.insert("score",score);
    return obj;
}
Record::Record(const QString &recordFilePath)
{
    if(QFile::exists(recordFilePath))
    {
        this->readFromFile(recordFilePath);
    }
    this->recordFilePath = recordFilePath;
}

Record::~Record()
{
    if(!isDeleted)//没有被删除
    {
        this->writeToFile(recordFilePath);
    }
}

box::type &Record::dataAt(const QPoint &pt)
{
    return dataAt(pt.x(),pt.y());
}

box::type &Record::dataAt(int i, int j)
{
    return map[i * basic.wScale + j];
}

void Record::setCurLevel(int level)
{
    curLevel = level;
}

void Record::setIsDeleted(bool flag)
{
    isDeleted = flag;
    if(flag)
    {
        QFile::remove(recordFilePath);
    }
}

void Record::reorganize(QPoint scale)
{
    qDebug() << "reorgan.";
    const int& n_wScale = scale.rx();
    const int& n_hScale = scale.ry();
    int total = n_wScale * n_hScale;

    auto& boxData = BoxMap::getBoxData();
    //init plain boxes
    QVector<box::type> plainBoxes;
    for(auto boxType:boxData){
        if(Box::typeToDivision(boxType) == box::plain_box){
            plainBoxes.push_back(boxType);
        }
    }
    //generate boxes
    int count = 0;
    for(int i = 0;i <= n_wScale - 1;++i){
        for(int j = 0;j <= n_hScale - 1;++j){
            if(count == total - 1 && (n_wScale % 2) && (n_hScale % 2)){//last box with odd index -> remain null.
                dataAt(i,j) = box::null;
                break;
            }
            dataAt(i,j) = box::type((count / 2) % plainBoxes.size());
            ++count;
        }
    }
    int r1,c1,r2,c2;
    for(int i = 1;i <= total;++i)//shuffle for times
    {
        r1 = QRandomGenerator::global()->bounded(n_wScale);
        c1 = QRandomGenerator::global()->bounded(n_hScale);
        r2 = QRandomGenerator::global()->bounded(n_wScale);
        c2 = QRandomGenerator::global()->bounded(n_hScale);

        qSwap(dataAt(r1,c1),dataAt(r2,c2));
    }
}

void Record::readFromFile(const QString &recordFile)
{
    qDebug() << "read record data from " << recordFile;
    QFile reader(recordFile);
    reader.open(QIODevice::ReadOnly);
    QJsonObject record = QJsonDocument::fromJson(reader.readAll()).object();
    reader.close();
    readFromJsonObject(record);
}

bool Record::readFromSettings(const Settings *settings, gameMain::gameMode mode, int level)
{
    QJsonObject* levelSettings;//mode和level对应的设置
    if(!isRandMode() && level == settings->getLevelsInMode(mode).size()){//非随机模式禁止访问最后一关
        qDebug() << "error: attempt to access last level without a randmode.";
        return false;
    }
    if(level > settings->getLevelsInMode(mode).size())//超出最后一关
    {
        qDebug() << "error: level overflow.";
        return false;
    }

    levelSettings = settings->getLevelsInMode(mode)[level - 1];
    this->mode = mode;
    this->curLevel = level;
    basic.readFromJsonObject(levelSettings->operator[]("basic").toObject());
    //update players
    players.clear();
    for(auto player:levelSettings->operator[]("playerInfos").toArray())
    {
        PlayerInfo playerInfo;
        playerInfo.readFromJsonObject(player.toObject());
        players.push_back(playerInfo);
    }
    //update map
    map.clear();
    for(auto box:levelSettings->operator[]("map").toArray())
    {
        map.push_back(box::type(box.toInt()));
    }
    isSaved = false;
    return true;
}

void Record::readFromJsonObject(const QJsonObject &obj)
{
    if(obj.contains("gameMode"))
    {
        mode = gameMain::gameMode(obj["gameMode"].toInt());
    }
    if(obj.contains("curLevel"))
    {
        curLevel = obj["curLevel"].toInt();
    }
    if(obj.contains("randModeArg")){
        randModeArg = QPoint(obj["randModeArg"].toArray()[0].toInt(),
                             obj["randModeArg"].toArray()[1].toInt());
    }
    if(obj.contains("isSaved")){
        isSaved = obj["isSaved"].toBool();
    }

    basic.readFromJsonObject(obj["basic"].toObject());

    players.clear();
    for(QJsonValue player:obj["playerInfos"].toArray())
    {
        PlayerInfo playerInfo;
        playerInfo.readFromJsonObject(player.toObject());
        players.push_back(playerInfo);
    }

    map.clear();
    for(auto box:obj["map"].toArray())
    {
        map.push_back(box::type(box.toInt()));
    }
}

void Record::writeToFile(const QString &recordFile)
{
    qDebug() << "write record data to " << recordFile;
    QFile writer(recordFile);
    writer.open(QIODevice::WriteOnly);

    QJsonObject record;
    record.insert("gameMode",mode);
    record.insert("curLevel",curLevel);
    record.insert("randModeArg",QJsonArray({randModeArg.x(),randModeArg.y()}));
    record.insert("isSaved",isSaved);
    record.insert("basic",basic.writeToJsonObject());

    QJsonArray playerArray;
    for(auto& player:players)
    {
        playerArray.append(player.writeToJsonObject());
    }
    record.insert("playerInfos",playerArray);

    QJsonArray mapArray;
    for(auto& box:map)
    {
        mapArray.append(box);
    }
    record.insert("map",mapArray);

    writer.write(QJsonDocument(record).toJson());
    writer.close();
}



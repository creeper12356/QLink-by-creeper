#ifndef RECORD_H
#define RECORD_H
#include "config.h"
#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QSizeF>
#include <QVector>
#include <QFile>
//存档类
struct Basic{
    int wScale;
    int hScale;
    QPointF corner;
    QSizeF size;
    QSizeF dist;
    int gameTime;
    void readFromJsonObject(const QJsonObject& obj);
    QJsonObject writeToJsonObject() const;
};
struct PlayerInfo{
    QString id;
    QPointF pos;
    entity::dir faceDir;
    void readFromJsonObject(const QJsonObject& obj);
    QJsonObject writeToJsonObject() const;
};

class Record
{
private:
    QString recordFilePath = "";
    gameMain::gameMode mode;
    int curLevel;
    Basic basic;
    QVector<PlayerInfo> players;
    QVector<box::type> map;

    //flags
    bool isDeleted = false;//若该字段为true，则析构时不写入文件
public:
    Record(const QString& recordFilePath);
    ~Record();
    //getters
    gameMain::gameMode getMode() const{return mode;}
    int getCurLevel() const{return curLevel;}
    const Basic& getBasic() const{return basic;}
    const QVector<PlayerInfo>& getPlayers() const{return players;}
    const QVector<box::type>& getMap() const{return map;}
    bool getIsDeleted() const{return isDeleted;}
    //setters
    void setCurLevel(int level);
    void setIsDeleted(bool flag);
private:
    void readFromFile(const QString& recordFilePath);//从文件recordFile中读取信息
    void writeToFile(const QString& recordFilePath);//将record保存至recordFile中
public:
    //从设置settings中读取模式mode下关卡level,返回是否读取成功
    bool readFromSettings(const Settings* settings,gameMain::gameMode mode,int level);
    void readFromJsonObject(const QJsonObject& obj);
signals:

};

#endif // RECORD_H

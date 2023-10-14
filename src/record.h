#pragma once
#ifndef RECORD_H
#define RECORD_H
#include "config.h"
/*
 * Record
 * 游戏的存档，保存所有必要信息
 */
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
    int score;
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
    QPoint randModeArg = QPoint(-1,-1);//随机模式参数： 格式(new_WScale,new_HScale)
    bool isSaved = false;//是否为存档（否则称新档）
public:
    Record(const QString& recordFilePath = "");
    ~Record();
public:
    Record& operator=(const Record& src);//operator= will change all members but recordFilePath.
    //getters
    gameMain::gameMode getMode() const{return mode;}
    int getCurLevel() const{return curLevel;}
    const Basic& getBasic() const{return basic;}
    const QVector<PlayerInfo>& getPlayers() const{return players;}
    const QVector<box::type>& getMap() const{return map;}
    bool getIsDeleted() const{return isDeleted;}

    bool fileExists() const;//返回路径是否存在
    bool getIsSaved() const{return isSaved;}//存档返回true，新档（从设置中读取的）返回false

    box::type& dataAt(const QPoint& pt);//返回地图对应pt坐标的map元素引用,不检查越界
    box::type& dataAt(int i,int j);//返回地图对应二维坐标[i][j]元素的引用，不检查越界

    //setters
    void setCurLevel(int level);
    void setIsDeleted(bool flag);
public:
    //randMode选项
    bool isRandMode() const{return randModeArg.x() != -1;}//是否为随机模式
    void markRandMode(){randModeArg.rx() = 1;}//标记为随机模式，此函数调用后必须设定randModeArg!!
    void setRandModeArg(const QPoint& arg){randModeArg = arg;}

    const QPoint& getRandModeArg() const{return randModeArg;}
    int getRandModeWScale() const{return randModeArg.x();}
    int getRandModeHScale() const{return randModeArg.y();}
    void reorganize(QPoint scale);//随机模式下打开新存档按(scale.x * scale.y) 重新组织

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

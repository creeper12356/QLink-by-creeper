#pragma once
#ifndef SETTINGS_H
#define SETTINGS_H
//设置窗口
#include <QWidget>
#include <QFile>
#include <QMap>
#include <QDir>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVector>
#include "config.h"

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT
private:
    QVector<QJsonObject*> roles;//玩家基本设置
    QStringList boxes;//箱子 基本设置
    QMap<QString,QVector<QJsonObject*>> levels;//关卡基本设置
public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

    const Ui::Settings* getUi() const{return ui;}
    const QVector<QJsonObject*>& getRoles()const{return roles;}
    const QStringList& getBoxes() const{return boxes;}
    const QMap<QString,QVector<QJsonObject*>>& getLevels() const{return levels;}

private slots:
    void on_ok_button_clicked();

private:
    Ui::Settings *ui;

    void readPlayerData();//读取玩家数据
    void readBoxData();//读取箱子数据
    void readLevelData();//读取关卡数据
    void readCtrlSettings();//读取控制信息
    void readAudioSettings();//读取声音设置
    void writeCtrlSettings();//写入控制信息
    void writeAudioSettings();//写入声音设置

};

#endif // SETTINGS_H

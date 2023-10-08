#pragma once
#ifndef SETTINGS_H
#define SETTINGS_H
//设置窗口
#include "config.h"

namespace Ui {
class Settings;
}
struct GameArg
{
    qreal dizzyTime;//迷惑时间（s）
    qreal freezeTime;//冻结时间（s）
    qreal hintTime;//提示时间(s)
    int monitorInterval;//玩家运动监听器监听周期(ms)
    qreal routeLifeSpan;//路径存在时间（s）
    bool readFromJsonObject(const QJsonObject& obj);
};

class Settings : public QWidget
{
    Q_OBJECT
private:
    QVector<QJsonObject*> roles;//玩家基本设置
    QVector<QJsonObject*> boxes;
    QMap<QString,QVector<QJsonObject*>> levels;//关卡基本设置
    GameArg args;//游戏参数设置
public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

    const Ui::Settings* getUi() const{return ui;}
    const QVector<QJsonObject*>& getRoles()const{return roles;}
    const QVector<QJsonObject*>& getBoxes() const{return boxes;}
    const QMap<QString,QVector<QJsonObject*>>& getLevels() const{return levels;}
    const QVector<QJsonObject *> getLevelsInMode(gameMain::gameMode mode) const;
    const GameArg& getArgs() const{return args;}
public:
    //setters
    void setEnableRandMode(bool flag);//设置randMode接口，会发送enableRandModeSetted信号
private slots:
    void on_ok_button_clicked();
    void on_entity_audio_slider_valueChanged(int value);


private:
    Ui::Settings *ui;
private:
    void readGameArgs();//读取游戏参数
    void readPlayerData();//读取玩家数据
    void readBoxData();//读取箱子数据
    void readLevelData();//读取关卡数据
    void readCtrlSettings();//读取控制信息
    void readAudioSettings();//读取声音设置
    void readAdvancedSettings();//读取高级设置
    void writeCtrlSettings();//写入控制信息
    void writeAudioSettings();//写入声音设置
    void writeAdvancedSettings();//写入高级设置
protected:
    void keyPressEvent(QKeyEvent *event) override;
signals:
    void enableRandModeSetted(bool flag);
};

#endif // SETTINGS_H

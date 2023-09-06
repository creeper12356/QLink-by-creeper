#pragma once
#ifndef WELCOMEPAGE_H
#define WELCOMEPAGE_H

#include <QWidget>
#include <QDebug>
#include <QPixmap>
#include <QPalette>

#include "gamemain.h"
#include "menupage.h"
#include "settings.h"
#include "recordbrowser.h"

namespace Ui {
class WelcomePage;
}

class WelcomePage : public QWidget
{
    Q_OBJECT

private:
    GameMain* gm = nullptr;//游戏窗口
    MenuPage* menu = nullptr;//菜单页面，需要时打开
    Settings* settings = nullptr;//设置页面
    RecordBrowser* browser = nullptr;//存档选择页面

public:
    explicit WelcomePage(QWidget* parent = 0);
    ~WelcomePage();

    //初始化函数
private:
    void initMenu();
    void initBrowser();
private:
//    void startNewGame(gameMain::gameMode mode);
    void startNewGame(Record& record);
private slots:
    void on_single_player_button_clicked();
    void on_multi_player_button_clicked();
    void on_exit_button_clicked();
    void on_set_button_clicked();

    void recordEnteredSlot(Record &record);
    void gameMainClosedSlot();
    void gamePausedSlot(const QString& info);
    void replayGame();
    void homeClickedSlot();
    void continueClickedSlot();
    void playNextLevel();
    void saveGameToRecord();

protected:
    void keyPressEvent(QKeyEvent *event) override;
private:
    Ui::WelcomePage *ui;
};

#endif // WELCOMEPAGE_H

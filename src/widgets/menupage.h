#pragma once
#ifndef MENUPAGE_H
#define MENUPAGE_H
/*
 * MenuPage:
 * 游戏菜单（暂停、超时、通关）
 */
#include "config.h"

namespace Ui {
class MenuPage;
}
using namespace menuPage;
class MenuPage : public QWidget
{
    Q_OBJECT
private:
    mode _mode = pause;//pause,timeout,win
public:
    explicit MenuPage(QWidget *parent = nullptr);
    ~MenuPage();

public:
    void switchPauseMode(const QString &info = "游戏暂停");
    void switchTimeoutMode(const QString& info = "倒计时结束");
    void switchWinMode(const QString& info = "恭喜通关");

    mode getMenuMode() const;

    QString info() const;
    void setInfo(const QString& str);

    //ui接口
    QPushButton* continueButton();
    QPushButton* nextButton();
    QPushButton* saveButton();
    ScoreBoard* winnerBoard();

protected:
    void keyPressEvent(QKeyEvent *event) override;
private slots:
    void on_replay_button_clicked();
    void on_home_button_clicked();
    void on_continue_button_clicked();
    void on_next_button_clicked();
    void on_save_button_clicked();

private:
    Ui::MenuPage *ui;
signals:
    //按钮按下信号，主窗口接收并作出响应
    void replayClicked();
    void homeClicked();
    void continueClicked();
    void nextClicked();
    void saveClicked();
};

#endif // MENUPAGE_H

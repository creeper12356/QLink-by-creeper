#pragma once
#ifndef MENUPAGE_H
#define MENUPAGE_H
//游戏菜单
#include "config.h"

namespace Ui {
class MenuPage;
}
using namespace menuPage;
class MenuPage : public QWidget
{
    Q_OBJECT
private:
    mode _mode = pause;
public:
    explicit MenuPage(QWidget *parent = nullptr);
    ~MenuPage();

public:
    void switchPauseMode();
    void switchTimeoutMode();
    void switchWinMode();

    mode getMenuMode() const;

    QString info() const;
    void setInfo(const QString& str);
    QPushButton* continueButton();
    QPushButton* nextButton();
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
    void replayClicked();
    void homeClicked();
    void continueClicked();
    void nextClicked();
    void saveClicked();
};

#endif // MENUPAGE_H

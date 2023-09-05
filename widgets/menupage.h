#pragma once
#ifndef MENUPAGE_H
#define MENUPAGE_H
//游戏暂停或结束菜单
#include <QWidget>
#include <QString>
#include <QPushButton>

namespace Ui {
class MenuPage;
}

class MenuPage : public QWidget
{
    Q_OBJECT

public:
    explicit MenuPage(QWidget *parent = nullptr);
    ~MenuPage();

    QString info() const;
    void setInfo(const QString& str);
    QPushButton* continueButton();
    QPushButton* replayButton();
    QPushButton* homeButton();
    QPushButton* helpButton();

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

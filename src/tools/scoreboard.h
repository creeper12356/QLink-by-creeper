#ifndef SCOREBOARD_H
#define SCOREBOARD_H
#include "config.h"
/*
 * ScoreBoard
 * 分数板类，用于展示玩家分数
 */
class ScoreBoard : public QWidget
{
    Q_OBJECT
private:
    Role* _player = nullptr;//绑定的玩家
    QLCDNumber displayer;//分数显示部件
    QLabel idBoard;//玩家id显示板
public:
    ScoreBoard(QWidget* parent = nullptr,Role* _player = nullptr);
    ~ScoreBoard();
    //overload functions
    ScoreBoard& operator=(const ScoreBoard& other);
    ScoreBoard& operator+=(int aScore);
    ScoreBoard& operator-=(int mScore);

    void add(int aScore);
    void minus(int mScore);

    //getters
    int score() const;
    const Role* player() const;
    //setters
    void setPlayer(Role* player);
    void setScore(int score);
protected:
    void paintEvent(QPaintEvent *event) override;

signals:

};

#endif // SCOREBOARD_H

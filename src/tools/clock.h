#ifndef CLOCK_H
#define CLOCK_H
#include "tools/indexbar.h"

class Clock: public IndexBar
{
    Q_OBJECT
private:
    int updateInterval;//update time interval
    QTimer* tm;
private slots:
    void updateTime();
public:
    Clock(QWidget* parent,int gameTime);
    ~Clock();

    int getCurTime() const;
    void setUpdateInterval(int interval);
    void start();
    void pause();
    void reset();
    void setTime(int time);
    void addTime(int time);
signals:
    void timeout();
};

#endif // CLOCK_H

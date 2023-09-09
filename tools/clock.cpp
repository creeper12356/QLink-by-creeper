#include "clock.h"

Clock::Clock(QWidget* parent,int gTime):IndexBar(parent,gTime,gTime)
{
    updateInterval = 10;
    tm = new QTimer;
    this->setBackgroundColor(QColor(255,255,0,180));
    this->setForegroundColor(QColor(255,0,10,200));

    connect(tm,&QTimer::timeout,this,&Clock::updateTime);
}
Clock::~Clock()
{
    delete tm;
}

int Clock::getCurTime() const
{
    return index();
}

void Clock::setUpdateInterval(int interval)
{
    this->updateInterval = interval;
}
void Clock::updateTime()
{
    rindex() -= updateInterval;
    if(index() <= 0)//超时
    {
        emit timeout();
    }
    update();
}
void Clock::start()
{
    tm->start(updateInterval);
}
void Clock::pause()
{
    tm->stop();
}
void Clock::reset()
{
    pause();
    rindex() = fullIndex();
}

void Clock::setTime(int time)
{
    if(time > index())//防止溢出
    {
        return ;
    }
    setIndex(time);
}
void Clock::addTime(int time)
{
    rindex() += time;
    if(index() > fullIndex())
    {
        setIndex(fullIndex());
    }
}


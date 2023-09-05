#include "statebar.h"
Clock::Clock(int gTime):gameTime(gTime),curTime(gTime)
{
    updateInterval = 10;
    tm = new QTimer;
    connect(tm,&QTimer::timeout,this,&Clock::updateTime);

    drawBox.moveTopLeft(QPointF(10,10));
    drawBox.setSize(QSizeF(800,40));
}
Clock::~Clock()
{
    delete tm;
}

int Clock::getCurTime() const
{
    return curTime;
}
void Clock::updateTime()
{
    curTime -= updateInterval;
    if(curTime <= 0)//超时
    {
        emit timeout();
    }
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
    curTime = gameTime;
}

void Clock::setTime(int time)
{
    if(time > curTime)//防止溢出
    {
        return ;
    }
    curTime = time;
}
void Clock::addTime(int time)
{
    curTime += time;
    if(curTime > gameTime)//防止溢出
    {
        curTime = gameTime;
    }
}
void Clock::draw(QPainter &painter) const
{
    painter.save();
    painter.setPen(QColor(0,0,0));
    painter.setBrush(QColor(255,255,180));
    painter.drawRect(drawBox.left() + 1,drawBox.top() + 1,drawBox.width() - 10 ,10);
    painter.setBrush(QColor(255,0,0));
    painter.drawRect(drawBox.left() + 1,drawBox.top() + 1,(drawBox.width() - 10) * curTime / gameTime ,10);
    painter.restore();
}

StateBar::StateBar():timer(10 * SECOND)
{
    this->setDrawBox(QRectF(5,5,800,40));
    timer.start();
}
StateBar::~StateBar()
{
    timer.pause();
}
void StateBar::setDrawBox(const QRectF &b)
{
    drawBox = b;
}
void StateBar::setPos(QPointF pt)
{
    drawBox.moveTopLeft(pt);
}
void StateBar::setPos(qreal x,qreal y)
{
    drawBox.moveTopLeft(QPointF(x,y));
}
void StateBar::draw(QPainter &painter)
{
    painter.drawText(drawBox.center(),QString::number(timer.curTime / SECOND));
}

//void StateBar::timeOutSlot()
//{
//    emit clockTimeOut();
//}

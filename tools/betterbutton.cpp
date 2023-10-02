#include "betterbutton.h"

BetterButton::BetterButton(QWidget *parent)
    :QPushButton(parent)
{
    setMouseTracking(true);
}

void BetterButton::mouseMoveEvent(QMouseEvent *event)
{
    QPushButton::mouseMoveEvent(event);
}
bool BetterButton::event(QEvent *event)
{
//    if(event->type() == QEvent::MouseMove){
//        qDebug() << "handled.";
//        return false;
//    }
    return QPushButton::event(event);
}

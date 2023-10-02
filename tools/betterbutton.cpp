#include "betterbutton.h"

BetterButton::BetterButton(QWidget *parent)
    :QPushButton(parent)
{
    QPropertyAnimation* jump_up = new QPropertyAnimation(this,
                                                         "geometry",
                                                         this);
    jump_up->setDuration(0.1 * SECOND);
    jump_up->setEasingCurve(QEasingCurve::OutQuad);

    QPropertyAnimation* jump_down = new QPropertyAnimation(this,
                                                           "geometry",
                                                           this);
    jump_down->setDuration(0.1 * SECOND);
    jump_down->setEasingCurve(QEasingCurve::InQuad);

    jump.addAnimation(jump_up);
    jump.addAnimation(jump_down);
}

BetterButton::~BetterButton()
{

}

bool BetterButton::event(QEvent *event)
{
    if(event->type() == QEvent::HoverEnter){
        jump.start();
    }
    return QPushButton::event(event);
}

void BetterButton::mousePressEvent(QMouseEvent *event)
{
    jump.start();
    QPushButton::mousePressEvent(event);
}

void BetterButton::resizeEvent(QResizeEvent *event)
{
    QRect geometry = this->geometry();
    QPropertyAnimation* jump_up,*jump_down;
    jump_up = dynamic_cast<QPropertyAnimation*>(jump.animationAt(0));//jump_up
    jump_down = dynamic_cast<QPropertyAnimation*>(jump.animationAt(1));//jump_down

    jump_up->setStartValue(geometry);
    jump_down->setEndValue(geometry);

    geometry.moveTop(geometry.top() - height() / 8.0);
    jump_up->setEndValue(geometry);
    jump_down->setStartValue(geometry);
}

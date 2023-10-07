#include "betterbutton.h"
int BetterButton::count = 0;
QSoundEffect* BetterButton::hoverSound = nullptr;
QSoundEffect* BetterButton::clickSound = nullptr;

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

    if(count == 0){
        hoverSound = new QSoundEffect;
        hoverSound->setSource(QUrl::fromLocalFile(":/audios/hover.wav"));

        clickSound = new QSoundEffect;
        clickSound->setSource(QUrl::fromLocalFile(":/audios/click.wav"));
    }
    ++count;
}

BetterButton::~BetterButton()
{
    --count;
    if(count == 0){
        delete hoverSound;
        delete clickSound;
    }
}

void BetterButton::setSoundVolume(qreal volume)
{
    hoverSound->setVolume(volume);
    clickSound->setVolume(volume);
}

bool BetterButton::event(QEvent *event)
{
    if(event->type() == QEvent::HoverEnter){
        hoverSound->play();
        jump.start();
    }
    else if(event->type() == QEvent::MouseButtonPress){
        clickSound->play();
        jump.start();
    }
    return QPushButton::event(event);
}

void BetterButton::mousePressEvent(QMouseEvent *event)
{
    clickSound->play();
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

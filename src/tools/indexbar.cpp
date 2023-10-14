#include "indexbar.h"

IndexBar::IndexBar(QWidget *parent) : QWidget(parent)
{
    b_color = QColor(255,255,255);//white bg
    f_color = QColor(0,255,0,190);//green fg
}

IndexBar::IndexBar(QWidget *parent, int fullIndex, int initIndex): QWidget(parent)
{
    _fullIndex = fullIndex;
    _index = initIndex;
    b_color = QColor(255,255,255);//white bg
    f_color = QColor(0,255,0,190);//green fg
}

void IndexBar::setFullIndex(int fi)
{
    _fullIndex = fi;
    update();
}

void IndexBar::setIndex(int i)
{
    _index = i;
    update();
}

void IndexBar::setBackgroundColor(const QColor &color)
{
    b_color = color;
    update();
}

void IndexBar::setForegroundColor(const QColor &color)
{
    f_color = color;
    update();
}

void IndexBar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setBrush(QColor(b_color));
    painter.drawRect(this->rect());

    painter.setBrush(QColor(f_color));
    painter.drawRect(rect().x(),
                     rect().y(),
                     rect().width() * (_index / (_fullIndex + 0.0)),
                     rect().height());
}

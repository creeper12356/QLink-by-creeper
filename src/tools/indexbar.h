#pragma once
#ifndef INDEXBAR_H
#define INDEXBAR_H

#include "config.h"
/*
 * IndexBar
 * 可视化的进度条类
 */

class IndexBar : public QWidget
{
    Q_OBJECT
private:
    int _fullIndex;
    int _index;
    QColor b_color;//background color
    QColor f_color;//foreground color

public:
    explicit IndexBar(QWidget *parent = nullptr);
    IndexBar(QWidget* parent,int fullIndex,int initIndex);
public:
    //setters
    void setFullIndex(int fi);
    void setIndex(int i);
    void setBackgroundColor(const QColor& color);
    void setForegroundColor(const QColor& color);
    //getters
    int fullIndex() const{return _fullIndex;}
    int index() const{return _index;}
    QColor backgroundColor() const {return b_color;}
    QColor foregroundColor() const {return f_color;}
    int &rindex() {return _index;}

protected:
    void paintEvent(QPaintEvent *event) override;
signals:

};

#endif // INDEXBAR_H

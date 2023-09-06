#pragma once
#ifndef KEYLINEEDIT_H
#define KEYLINEEDIT_H

#include <QLineEdit>
#include <QKeyEvent>
#include <QDebug>
//a subclass of QLineEdit only used for settings ui.

class KeyLineEdit:public QLineEdit
{
protected:
    void keyPressEvent(QKeyEvent *e) override;
private:
    int curKey = 0;
public:
    KeyLineEdit(QWidget* parent = nullptr);
    ~KeyLineEdit(){}
    int key() const{return curKey;}
    void setKey(int key);
};

#endif // KEYLINEEDIT_H

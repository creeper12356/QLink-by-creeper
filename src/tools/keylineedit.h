#pragma once
#ifndef KEYLINEEDIT_H
#define KEYLINEEDIT_H
#include "config.h"

/*
 * KeyLineEdit
 * 继承自QLineEdit,完善了方向键的显示，用于'设置'模块
 */

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

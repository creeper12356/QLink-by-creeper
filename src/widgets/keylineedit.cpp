#include "keylineedit.h"

KeyLineEdit::KeyLineEdit(QWidget* parent):QLineEdit(parent)
{
    this->setReadOnly(true);
}
void KeyLineEdit::keyPressEvent(QKeyEvent *e)
{
    this->setKey(e->key());
}
void KeyLineEdit::setKey(int key)
{
    //显示改变
    if(key == Qt::Key_Space)
    {
        this->setText("Space");
    }
    else if(isascii(key))
    {
        this->setText(QString(QChar(key)));
    }
    else if(key == Qt::Key_Up)
    {
        this->setText("↑");
    }
    else if(key == Qt::Key_Down)
    {
        this->setText("↓");
    }
    else if(key == Qt::Key_Left)
    {
        this->setText("←");
    }
    else if(key == Qt::Key_Right)
    {
        this->setText("→");
    }
    else//无效的key
    {
        return ;
    }
    //存储改变
    curKey = key;
}


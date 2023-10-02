#ifndef BETTERBUTTON_H
#define BETTERBUTTON_H
#include "config.h"

class BetterButton:public QPushButton
{
public:
    BetterButton(QWidget* parent = nullptr);
protected:
    void mouseMoveEvent(QMouseEvent *) override;
    bool event(QEvent *e) override;
};

#endif // BETTERBUTTON_H

#ifndef BETTERBUTTON_H
#define BETTERBUTTON_H
#include "config.h"

class BetterButton:public QPushButton
{
private:
    QSequentialAnimationGroup jump;
public:
    BetterButton(QWidget* parent = nullptr);
    ~BetterButton();
protected:
    bool event(QEvent* event) override;
    void mousePressEvent(QMouseEvent *)override;
    void resizeEvent(QResizeEvent*) override;
};

#endif // BETTERBUTTON_H

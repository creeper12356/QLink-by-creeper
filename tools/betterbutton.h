#ifndef BETTERBUTTON_H
#define BETTERBUTTON_H
#include "config.h"

class BetterButton:public QPushButton
{
private:
    QSequentialAnimationGroup jump;
    static QSoundEffect* hoverSound;
    static QSoundEffect* clickSound;
    static int count;//BetterButton对象数
public:
    BetterButton(QWidget* parent = nullptr);
    ~BetterButton();

    static void setSoundVolume(qreal volume);
protected:
    bool event(QEvent* event) override;
    void mousePressEvent(QMouseEvent *)override;
    void resizeEvent(QResizeEvent*) override;
};

#endif // BETTERBUTTON_H

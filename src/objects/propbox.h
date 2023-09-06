#pragma once
#ifndef PROPBOX_H
#define PROPBOX_H
#include "config.h"
#include "objects/box.h"
class PropBox:public Box
{
private:
    void (GameMain::*prop)(Role*);//技能函数
public:
    PropBox();
    virtual void setBoxType(box::type t) override;
    void bindProp();//绑定技能函数
    void execProp(GameMain* gm,Role* player);
};

#endif // PROPBOX_H

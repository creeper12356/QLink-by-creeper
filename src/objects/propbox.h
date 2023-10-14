#pragma once
#ifndef PROPBOX_H
#define PROPBOX_H
#include "config.h"
#include "objects/box.h"
/*
 * 道具箱子，无需两两连接，触碰即可消除，并激发道具效果（技能函数）
 */
class PropBox:public Box
{
private:
    void (GameMain::*prop)(Role*);//方块消除时调用的技能函数
public:
    PropBox();
    virtual void setBoxType(box::type t) override;
    void bindProp();//绑定技能函数
    void execProp(GameMain* gm,Role* player);//执行技能函数
};

#endif // PROPBOX_H

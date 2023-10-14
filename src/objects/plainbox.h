#pragma once
#ifndef PLAINBOX_H
#define PLAINBOX_H
#include "objects/box.h"
/*
 * 普通的箱子，玩家两两连接消除
 */
class PlainBox:public Box
{
public:
    PlainBox();
    virtual ~PlainBox();
};

#endif // PLAINBOX_H

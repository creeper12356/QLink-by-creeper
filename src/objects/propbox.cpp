#include "propbox.h"
#include "widgets/gamemain.h"
#include "objects/role.h"

PropBox::PropBox()
{
    setBoxDivision(box::prop_box);
    isLocked = true;//默认技能箱子上锁
}
void PropBox::setBoxType(box::type t)
{
    Box::setBoxType(t);
    bindProp();
}
void PropBox::bindProp()
{
    if(boxType == box::clock)
    {
        prop = &GameMain::addTime;
    }
    else if(boxType == box::ender_pearl)
    {
        prop = &GameMain::shuffle;
    }
    else if(boxType == box::snow_bucket)
    {
        prop = &GameMain::freeze;
    }
    else if(boxType == box::potion)
    {
        prop = &GameMain::dizzy;
    }
    else if(boxType == box::book)
    {
        prop = &GameMain::hint;
    }
}
void PropBox::execProp(GameMain* gm,Role *player)
{
    if(prop != nullptr)
    {
        (gm->*prop)(player);
    }
}

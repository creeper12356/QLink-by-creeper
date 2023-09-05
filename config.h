#pragma once
#ifndef CONFIG_H
#define CONFIG_H
#include <QPoint>
#define SECOND 1000

namespace gameMain {
    enum gameMode{singleMode = 0,multiMode};
}
namespace entity {
    enum type{box,role};
    enum dir{no_dir = -1,up = 0,down,left,right};//方向枚举类
    dir opsiteDir(dir d);//返回d的反方向
    QPoint unitDisplacement(const QPoint& staPt,dir d);//返回出发点staPt经过d方向移动单位长度后的坐标（行列坐标）
}
namespace box{
    enum type{null = -1,
              apple,
              beetroot,
              beetroot_soup,
              carrot,
              golden_apple,
              mushroom_stew,
              watermelon,
              wheat,
              clock,
              ender_pearl,
              snow_bucket,
              potion,
              book
             };
    enum division{plain_box = 0,
                  prop_box
                 };
}
namespace linkRoute {
    enum nodeDir{//路径中节点方向枚举类型
        no_dir = -1,
        up_down = 0,
        left_right = 1,
        up_left = 2,
        up_right = 3,
        down_left = 4,
        down_right = 5
    };
}
class GameMain;
class BackendProcessor;
class Settings;
class BoxMap;
class Map;
class Role;
class Record;
class Tile
{
public:
    QPoint pos;//坐标
    entity::dir dir;//当前查找方向
    int turn;//经过转弯数
    int depth;//查找深度

    Tile()
    {
        dir = entity::no_dir;
        turn = -1;
        depth = -1;
    }
};

#endif // CONFIG_H

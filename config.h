#pragma once
#ifndef CONFIG_H
#define CONFIG_H

#include <QSoundEffect>
#include <QSound>
#include <QMediaPlayer>

#include <QPoint>
#include <QPointF>
#include <QRectF>
#include <QSizeF>

#include <QUrl>
#include <QFile>
#include <QDir>

#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QVector>
#include <QMap>
#include <QString>
#include <QStack>
#include <QQueue>
#include <QList>
#include <QSet>

#include <QPalette>
#include <QPixmap>
#include <QPainter>
#include <QColor>

#include <QKeyEvent>
#include <QMouseEvent>

#include <QPushButton>
#include <QLCDNumber>
#include <QWidget>
#include <QListWidgetItem>
#include <QLineEdit>
#include <QInputDialog>
#include <QLabel>
#include <QMessageBox>

#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>

#include <QRandomGenerator>
#include <QTimer>
#include <QtMath>
#include <QDebug>

#define SECOND 1000
#define EPS 1e-8//比较浮点数时的精度
#define RANDOM_BETWEEN(__MIN__,__MAX__) \
    (QRandomGenerator::global()\
    ->bounded((__MAX__) - (__MIN__))\
    + __MIN__)

namespace gameMain {
    enum gameMode{singleMode = 0,multiMode};
}
namespace menuPage {
    enum mode{pause = 0,timeout = 1,win = 2};
}
namespace entity {
    enum type{box,role};
    enum dir{no_dir = -1,up = 0,down,left,right};//方向枚举类
    dir opsiteDir(dir d);//返回d的反方向
    QPoint unitDisplacement(const QPoint& staPt,dir d);//返回出发点staPt经过d方向移动单位格子后的坐标（行列坐标）
    QPointF pureMove(const QPointF& originPt,dir d,qreal step = 1.0);//originPt为像素坐标，返回dir移动step后的坐标
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
class ScoreBoard;
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

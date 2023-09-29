# QLink说明文档
## 1. 地图的生成
### 1.1	游戏的边界
地图是玩家自由移动的区域，同时是整个游戏的边界。
### 1.2 地图中的方块
地图中的方块用类Block实现
```
class Block//地图中的固定方块
{
    friend class Map;
private:
    QPixmap pic;//方块的贴图

    qreal speedFactor;//速度因子，影响玩家移动速度
public:
    Block(){};

    //接口函数
    const QPixmap& getPic() const{return pic;}//目前暂时不允许修改pic
    qreal getSpeedFactor() const{return speedFactor;}
};

```
### 1.2
## 2.地图上的实体
## 3.时间和分数
## 4.存档和回档
## 5.页面切换逻辑

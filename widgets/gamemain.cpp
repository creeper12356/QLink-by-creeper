#include "gamemain.h"
#include "ui_widget.h"
#include "ui_settings.h"
#include "record.h"

GameMain::GameMain(QWidget *parent, Settings *&s, Record &record)
    :QWidget(parent)
    ,ui(new Ui::Widget)
    ,settings(s)
{
    this->record = &record;
    this->mode = record.getMode();
    background = new Map();
    initUi();
    initPlayers(record);
    initLinkBoxes(record);
    initProcessor();
    initGameClk(record);
    initHintTimer();

    this->start();//开始游戏
    //测试计时器
    connect(this,&GameMain::boxDeleted,this,&GameMain::statePrinter);
    testTimer.start(100);
}

GameMain::~GameMain()
{
    delete background;
    delete linkBoxes;
    delete gameClk;

    //释放实体空间
    for(auto entity:entities)
    {
        delete entity;
    }
    emit gameMainDeleted();
}

void GameMain::initUi()
{
    ui->setupUi(this);
    ui->level_label->setText("第" + QString::number(record->getCurLevel()) +"关");
    this->setFocus();//保证按键不被按钮捕获
//    ui->shuffle_button->hide();
//    ui->hint_button->hide();
}
void GameMain::initPlayerMoveKeys(Role *player,int playerNum)
{
    if(playerNum == 1)//玩家1
    {
        player->insertMoveKeys(entity::up,settings->getUi()->p1_key_up->key());
        player->insertMoveKeys(entity::down,settings->getUi()->p1_key_down->key());
        player->insertMoveKeys(entity::left,settings->getUi()->p1_key_left->key());
        player->insertMoveKeys(entity::right,settings->getUi()->p1_key_right->key());
    }
    else if(playerNum == 2)//玩家2
    {
        player->insertMoveKeys(entity::up,settings->getUi()->p2_key_up->key());
        player->insertMoveKeys(entity::down,settings->getUi()->p2_key_down->key());
        player->insertMoveKeys(entity::left,settings->getUi()->p2_key_left->key());
        player->insertMoveKeys(entity::right,settings->getUi()->p2_key_right->key());
    }
    else//playerNum 不合法
    {
        qDebug() << "playerNum is not 1 or 2.";
    }
}
Role *GameMain::initPlayer(const QString &id, int playerNum)
{
    if(players.size() >= playerNum)
    {
        qDebug() << "player " << playerNum << " already exists.Return .";
        return nullptr;
    }
    Role* player;
    auto& playerSettings = settings->getRoles();
    QJsonObject* targetPlayer = nullptr;

    for(const auto& p:playerSettings)
    {
        if(p->value("id").toString() == id)//id匹配
        {
            targetPlayer = p;
            break;
        }
    }
    if(targetPlayer == nullptr)
    {
        qDebug() << "id " << id << " not valid.";
        return nullptr;
    }

    //基本设置
    player = new Role;
    player->id = targetPlayer->value("id").toString();
    player->isIdVisible = targetPlayer->value("isIdVisible").toBool();
    player->setSize(targetPlayer->value("size").toArray().at(0).toDouble(),
                    targetPlayer->value("size").toArray().at(1).toDouble());
    player->setSpeed(targetPlayer->value("speed").toDouble());

    //方向键设置
    initPlayerMoveKeys(player,playerNum);

    //设置运动贴图
    player->insertMoveImages(entity::up,targetPlayer->value("moveImages").toObject().value("up").toString());
    player->insertMoveImages(entity::down,targetPlayer->value("moveImages").toObject().value("down").toString());
    player->insertMoveImages(entity::left,targetPlayer->value("moveImages").toObject().value("left").toString());
    player->insertMoveImages(entity::right,targetPlayer->value("moveImages").toObject().value("right").toString());

    addPlayer(player);
    if(playerNum == 1){
        scoreBoards[player]->setGeometry(10,22,180,80);
    }
    else{
        scoreBoards[player]->setGeometry(640,22,180,80);
    }

    return player;
}

void GameMain::initPlayers(const Record &record)
{
    for(auto& player:record.getPlayers())
    {
        Role* handle = initPlayer(player.id,players.size() + 1);
        if(handle == nullptr)
        {
            continue;
        }
        handle->setDefaultFaceDir(player.faceDir);
        handle->setPos(player.pos);
        scoreBoards[handle]->setScore(player.score);
    }
}

void GameMain::initLinkBoxes(const Record &record)
{
    linkBoxes = new BoxMap(settings,record);
    linkBoxes->generateEntities(this);
}
void GameMain::initProcessor()
{
    processor = linkBoxes->getProcessor();
}


void GameMain::initGameClk(const Record &record)
{
    QString item = (mode == singleMode)?"single":"multi";
    gameClk = new Clock(this,
                settings->getLevels()[item][record.getCurLevel() - 1]
                ->operator[]("basic").toObject()
                ["gameTime"].toInt() * SECOND);//游戏总时间

    gameClk->setTime(record.getBasic().gameTime * SECOND);
    gameClk->setGeometry(10,0,950,25);
    connect(gameClk,&Clock::timeout,this,&GameMain::clockTimeOutSlot);
}

void GameMain::initHintTimer()
{
    hintTimer.setSingleShot(true);
    hintRemainingTime = hintTime * SECOND;
    connect(&hintTimer,&QTimer::timeout,this,[this]
    {
       qDebug() << "hint end";
       clearHint();
       repaintSlot();
    });
}
void GameMain::keyPressEvent(QKeyEvent *event)
{
    if(event->isAutoRepeat())//连按无效
    {
        return ;
    }
    int keyPress = event->key();//按下的键

    if(keyPress == Qt::Key_Escape)
    {
        this->pause();
        emit gamePaused();
    }
    else if(keyPress == Qt::Key_F3)
    {
        isDebugMode = !isDebugMode;//切换调试模式
        repaint();
    }
    for(Role* player:players)
    {
        if(player->getDirFromKey(keyPress) != entity::no_dir)
        {
            player->setOverrideFaceDir(player->getDirFromKey(keyPress));//更新面部朝向

            if(!player->rmonitor().isActive())
                player->rmonitor().start(monitorInterval);//如果计时器未开启，将计时器开启
        }
    }
}
void GameMain::keyReleaseEvent(QKeyEvent *event)
{
    if(event->isAutoRepeat())//连按无效
    {
        return ;
    }
    int keyRelease = event->key();
    for(Role* player:players)
    {
        if(player->getDirFromKey(keyRelease) != entity::no_dir)
        {
            player->restoreOverrideFaceDir(player->getDirFromKey(keyRelease));
            if(player->rfaceDir().empty())//玩家的四个方向键都没有被按下
            {
                player->rmonitor().stop();
            }
        }

    }
}
void GameMain::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    //绘制地图
    painter.translate(background->getCorner());//设置绘制地图原点（左上角点）
    painter.scale(1,1);
    int wScale = background->getWScale(),hScale = background->getHScale(),size = background->getBlockSize();
    for(int i = 0;i <= hScale - 1;++i)
    {
        for(int j = 0;j <= wScale - 1;++j)
        {
            painter.drawPixmap(j*size,i*size,size,size,background->getBlocks()[background->getData()[i][j]]->getPic());
        }
    }

    //绘制所有箱子
    for(auto box:boxes)
    {
        box->draw(painter,isDebugMode);
    }

    //绘制boxMap边框
    linkBoxes->draw(painter,isDebugMode);

    //绘制路径
    for(auto& route:routes)
    {
        route->draw(painter,linkBoxes);
    }

    //绘制所有玩家
    for(auto player:players)
    {
        player->draw(painter,isDebugMode);
    }
}
void GameMain::mousePressEvent(QMouseEvent *event)
{
    QPointF pos = event->pos() - background->getCorner();

    if(!isDebugMode)
        return ;
    Role* player = players[0];
    player->setPos(pos);
    for(auto pt:processor->hintFrom(linkBoxes->posToDataCoord(pos))){
        linkBoxes->getPtrDataAt(pt)->isHighlighted = true;
    }
    update();
}
void GameMain::mouseMoveEvent(QMouseEvent *event)
{
    mousePressEvent(event);

}
void GameMain::closeEvent(QCloseEvent *event)
{
    emit gameMainClosed();
}

Role *GameMain::other(Role *player)
{
    if(mode == gameMain::singleMode)//单人模式
    {
        return nullptr;
    }
    //双人模式
    Role* other = nullptr;//另一个玩家
    for(auto& p:players)
    {
        if(p != player)
        {
            other = p;
            break;
        }
    }
    return other;
}
void GameMain::addEntity(Entity *e)
{
    entities.push_back(e);
}
void GameMain::addPlayer(Role *r)
{
    players.push_back(r);
    addEntity(r);//同时放入实体指针集合中

    connect(r,&Role::monitorTimeout,this,&GameMain::movePlayer);//将玩家的monitorTimeout事件与对应的移动函数连接

    r->setDizzyTime(dizzyTime * SECOND);
    r->setFreezeTime(freezeTime * SECOND);
    connect(r,&Role::dizzyEnd,this,&GameMain::repaintSlot);
    connect(r,&Role::freezeEnd,this,&GameMain::repaintSlot);

    activatedBoxes.insert(r,QVector<QPoint>());//激活箱子Map中导入该玩家的键值对
//    register scoreBoard
    scoreBoards.insert(r,new ScoreBoard(this,r));
}
void GameMain::addBox(Box *b)
{
    boxes.push_back(b);
    addEntity(b);
}

void GameMain::removeEntity(Entity *e)
{
    entities.removeAll(e);
}
void GameMain::removePlayer(Role* r)
{
    players.removeAll(r);
    removeEntity(r);

    activatedBoxes.remove(r);
    scoreBoards.remove(r);
}
void GameMain::removeBox(Box *b)
{
    boxes.removeAll(b);
    removeEntity(b);
}
void GameMain::start()
{
    gameClk->start();

    if(hintRemainingTime != hintTime * SECOND)
    {
        this->startHintTimer();
        if(hintBoxes.empty())
        {
            on_hint_button_clicked();
        }
    }
    for(auto& player:players)
    {
        if(player->getDizzyRemainingTime() != player->getDizzyTime())
        {
            player->startDizzyTimer();
        }
        if(player->getFreezeRemainingTime() != player->getFreezeTime())
        {
            player->startFreezeTimer();
        }
    }
}

void GameMain::pause()
{
    gameClk->pause();
    for(auto& player:players)
    {
        if(player->isDizzy())
        {
            player->pauseDizzyTimer();
        }
        if(!player->isMovable)
        {
            player->pauseFreezeTimer();
        }
        player->rfaceDir().clear();
        player->rmonitor().stop();
    }
    if(this->isHint())
    {
        pauseHintTimer();
    }
}

void GameMain::deleteBoxAt(const QPoint &pt)
{
    //从gameMain中移除实体
    removeBox(linkBoxes->getPtrDataAt(pt));
    //从linkBoxes中删除实体
    linkBoxes->removeBoxAt(pt);
    emit boxDeleted();
}

void GameMain::updateScore(Role *player, const LinkRoute *route)
{
    int score = scoreBoards[player]->score();
    qDebug() << "size:" << route->size();
    qDebug() << "turn:" << route->turn();
    qDebug() << "add: " << (route->size() + 1) * (route->turn() + 1);
    score += (route->size() + 1) * (route->turn() + 1);//加分
    scoreBoards[player]->setScore(score);//update scoreboard
}

bool GameMain::tryActivate(const QPoint& target,Box *&entityTarget, Role *player)
{
    if(entityTarget->isLocked)//箱子已上锁
    {
        return false;
    }

    QVector<QPoint>& atvBoxes = activatedBoxes[player];
    if(atvBoxes.empty()//无激活箱子
            ||
        linkBoxes->getPtrDataAt(atvBoxes.back())
            != entityTarget)//激活箱子不重复
    {
        entityTarget->isActivated = true;
        atvBoxes.push_back(target);
        return true;
    }
    return false;
}

bool GameMain::tryDeactivate(const QPoint &target, Box *&entityTarget, Role *player)
{
    activatedBoxes[player].removeAll(target);
    auto other = this->other(player);
    if(other != nullptr)
    {
        if(activatedBoxes[other].contains(target))
        {
            return false;
        }
    }
    entityTarget->isActivated = false;
    return true;
}

void GameMain::addRawRoute(LinkRoute *&route)
{
    route->updateDir();
    route->removeFirst();
    route->removeLast();
    routes.push_back(route);
    QTimer::singleShot(routeLifeSpan * SECOND,this,[this,route]//定时消除路径
    {
        routes.removeAll(route);
        delete route;
        update();
    });
}
void GameMain::movePlayer(Role* player)
{
    qreal singleMove = player->calculateSingleMove(this,player->rmonitor().interval());
    //player
    entity::dir faceDir = player->getFaceDir();//玩家面部朝向

    if(player->isMovable && player->move(this,singleMove,faceDir) == 0.0)//检测到没有移动
    {
        QPoint target = player->findTarget(this);//查找激活箱子的行列坐标
        if(target != QPoint(-1,-1)
                && linkBoxes->getDataAt(target) != box::null)//存在激活的箱子
        {
            Box*& entityTarget = linkBoxes->getPtrDataAt(target);//实体目标
            if(entityTarget->getBoxDivision() == box::plain_box)
            {
                processPlainBoxTarget(target,entityTarget,player);
            }
            else if(entityTarget->getBoxDivision() == box::prop_box)
            {
                processPropBoxTarget(target,entityTarget,player);
            }
        }
    }
    repaint();//后期改成局部更新!!
}

void GameMain::clockTimeOutSlot()
{
    gameClk->pause();
    emit gameTimeout();
    this->hide();
}
void GameMain::repaintSlot()
{
    repaint();
}
bool GameMain::tryLink(Role *player)
{
    if(activatedBoxes[player].size() != 2){
        return false;
    }
    QPoint frontPt = activatedBoxes[player].front();
    QPoint backPt = activatedBoxes[player].back();
    Box*& frontBox = linkBoxes->getPtrDataAt(frontPt);
    Box*& backBox = linkBoxes->getPtrDataAt(backPt);
    LinkRoute* route = nullptr;

    if(!processor->checkLink(frontPt,backPt,route))//选中不匹配方块
    {
        qDebug() << "not match";
        tryDeactivate(frontPt,frontBox,player);
        tryDeactivate(backPt,backBox,player);
        return false;
    }
    //方块可消除
    frontBox->playBreakSound();
    frontBox->isLocked = true;
    backBox->isLocked = true;
    addRawRoute(route);
    updateScore(player,route);
    for(auto& p:players)
    {
        activatedBoxes[p].removeAll(frontPt);
        activatedBoxes[p].removeAll(backPt);
    }
    QTimer::singleShot(routeLifeSpan * SECOND,this,[this,frontPt,backPt]//定时消除方块
    {
        // 删除箱子
        deleteBoxAt(frontPt);
        deleteBoxAt(backPt);
        update();
        //更新提示
        if(hintBoxes.contains(frontPt) || hintBoxes.contains(backPt))//提示箱子被消除
        {
            updateHint();
        }
    });
    return true;
}
bool GameMain::isWin() const
{
    if(boxes.size() == 0)//没有方块
    {
        qDebug() << "win!";
        return true;
    }
    return false;
}
void GameMain::processPlainBoxTarget(const QPoint& target,Box*& entityTarget,Role* player)
{
    if(tryActivate(target,entityTarget,player))
    {
        tryLink(player);
    }
}
void GameMain::processPropBoxTarget(const QPoint &target,Box*& entityTarget, Role *player)
{
    PropBox* propBox = dynamic_cast<PropBox*>(entityTarget);

    //播放音效
    entityTarget->playBreakSound();
    //技能函数
    propBox->execProp(this,player);
    //update score
    scoreBoards[player]->setScore(scoreBoards[player]->score() + QRandomGenerator::global()->bounded(10));
    //删除箱子
    deleteBoxAt(target);
}

bool GameMain::isHint() const
{
    return hintTimer.isActive();
}

void GameMain::startHintTimer()
{
    hintTimer.start(hintRemainingTime);
    hintRemainingTime = hintTime * SECOND;//重置剩余时间
}
void GameMain::pauseHintTimer()
{
    hintRemainingTime = hintTimer.remainingTime();
    hintTimer.stop();
}

QVector<QPoint> GameMain::clearHint()
{
    QVector<QPoint> cleared(hintBoxes);
    for(auto& pt:hintBoxes)
    {
        if(linkBoxes->getPtrDataAt(pt) != nullptr)
        {
            linkBoxes->getPtrDataAt(pt)->isHighlighted = false;
        }
    }
    hintBoxes.clear();
    return cleared;
}

void GameMain::updateHint()
{
    QVector<QPoint> cleared = clearHint();
    //新的提示
    if(isHint())
    {
        if(!cleared.empty()){
            localHint(cleared[0]);
        }
    }
}

void GameMain::localHint(const QPoint &standPt)
{
    hintBoxes = processor->hint(standPt);
    if(hintBoxes.size() != 2){
        return ;
    }

    //change highlight state
    linkBoxes->getPtrDataAt(hintBoxes.at(0))->isHighlighted = true;
    linkBoxes->getPtrDataAt(hintBoxes.at(1))->isHighlighted = true;
    update();
}

void GameMain::addTime(Role *player)
{
    qDebug() << "addTime";
    gameClk->addTime(30* SECOND);
}
void GameMain::shuffle(Role* player)
{
    qDebug() << "shuffle";
    on_shuffle_button_clicked();

}

void GameMain::hint(Role *player)
{
    qDebug() << "hint";
    if(isHint())//已经处于提示状态
    {
        hintTimer.stop();
    }
    else{
        localHint(linkBoxes->coverDataCoords(player->getEntityBox())[0]);
    }
    startHintTimer();
}
void GameMain::freeze(Role *player)
{
    qDebug() << "freeze";
    if(mode == gameMain::singleMode)
    {
        return ;
    }
    Role* other = this->other(player);//另一个玩家
    other->freeze();
}
void GameMain::dizzy(Role *player)
{
    qDebug() << "dizzy";
    if(mode == gameMain::singleMode)
    {
        return ;
    }
    Role* other = this->other(player);//另一个玩家
    other->dizzy();
}

void GameMain::saveGameToRecord()
{
    QJsonObject basic;
    basic.insert("gameTime",gameClk->getCurTime() / SECOND);

    QJsonArray playerInfos;
    QJsonObject playerInfo;
    for(auto& player:players)
    {
        playerInfo.insert("id",player->id);
        QJsonArray pos;
        pos.append(player->getPos().x());
        pos.append(player->getPos().y());
        playerInfo.insert("pos",pos);
        playerInfo.insert("faceDir",player->getFaceDir());
        playerInfo.insert("score",scoreBoards[player]->score());
        playerInfos.append(playerInfo);
    }

    QJsonArray map;
    for(int i = 0;i <= linkBoxes->getHScale() - 1;++i)
    {
        for(int j = 0;j <= linkBoxes->getWScale() - 1;++j)
        {
            map.append(linkBoxes->getDataAt(i,j));
        }
    }

    QJsonObject obj;
    obj.insert("basic",basic);
    obj.insert("playerInfos",playerInfos);
    obj.insert("map",map);

    record->readFromJsonObject(obj);
    qDebug() << "complete save.";
}
//测试更新函数
void GameMain::statePrinter()
{
    if(this->isWin())
    {
        this->pause();
        ScoreBoard* winScoreBoard = nullptr;
        for(auto& sb:scoreBoards){
            if(winScoreBoard == nullptr || winScoreBoard->score() < sb->score()){
                winScoreBoard = sb;
            }
        }
        emit gameWin(winScoreBoard);
        return ;
    }
    if(!processor->isSolvable()){
        emit gameTimeout("游戏无解");
    }
}


void GameMain::on_shuffle_button_clicked()
{
    //清除所有玩家的箱子选中记录
    for(auto player:players)
    {
        for(auto pt:activatedBoxes[player])
        {
            linkBoxes->getPtrDataAt(pt)->isActivated = false;
        }
        activatedBoxes[player].clear();
    }
    //如果原来有提示，则重新刷新提示
    if(!hintBoxes.isEmpty())
    {
        clearHint();
    }
    //后端洗牌
    processor->shuffle((linkBoxes->getWScale() * linkBoxes->getHScale() - boxes.size() + 1) * 10);

    //如果处于提示状态，更新洗牌后的提示
    if(isHint())
    {
        on_hint_button_clicked();
    }
    //重画
    update();

    this->setFocus();
}

void GameMain::on_hint_button_clicked()
{
    this->setFocus();
    //寻找有效提示（size == 2)
    hintBoxes = processor->hint();
    if(hintBoxes.size() != 2){
        return ;
    }

    //change highlight state
    linkBoxes->getPtrDataAt(hintBoxes.at(0))->isHighlighted = true;
    linkBoxes->getPtrDataAt(hintBoxes.at(1))->isHighlighted = true;
    update();
}

void GameMain::on_clear_button_clicked()
{
    //clear all effects of all boxes
    for(auto& box:boxes){
        if(box->isLocked && Box::typeToDivision(box->getBoxType()) == box::plain_box){
            box->isLocked = false;
        }
        if(box->isActivated){
            box->isActivated = false;
        }
        if(box->isHighlighted){
            box->isHighlighted = false;
        }
    }
    for(auto& player:players){
        activatedBoxes[player].clear();
    }
    update();
}

void GameMain::on_win_button_clicked()
{
    for(int i = 0;i <= linkBoxes->getHScale() - 1;++i){
        for(int j = 0;j <= linkBoxes->getWScale() - 1;++j){
            if(linkBoxes->getDataAt(i,j) != null){
                removeBox(linkBoxes->getPtrDataAt(i,j));
                linkBoxes->removeBoxAt(i,j);
            }
        }
    }
    emit boxDeleted();
}

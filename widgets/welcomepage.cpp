#include "welcomepage.h"
#include "ui_welcomepage.h"
#include "record.h"

WelcomePage::WelcomePage(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::WelcomePage)
{
    ui->setupUi(this);
    initMenu();
    settings = new Settings();
    initBrowser();

    QPalette palette;
    palette.setBrush(QPalette::Background, QColor(0,200,180,255));
    this->setPalette(palette);
}

WelcomePage::~WelcomePage()
{
    delete ui;
    delete menu;
    delete settings;
    delete browser;
}

void WelcomePage::initMenu()
{
    menu = new MenuPage();
    connect(menu,&MenuPage::continueClicked,this,&WelcomePage::continueClickedSlot);
    connect(menu,&MenuPage::replayClicked,this,&WelcomePage::replayGame);
    connect(menu,&MenuPage::homeClicked,this,&WelcomePage::homeClickedSlot);
    connect(menu,&MenuPage::nextClicked,this,&WelcomePage::playNextLevel);
    connect(menu,&MenuPage::saveClicked,this,&WelcomePage::saveGameToRecord);
}

void WelcomePage::initBrowser()
{
    browser = new RecordBrowser(nullptr,settings);
    connect(browser,&RecordBrowser::recordEntered,this,&WelcomePage::recordEnteredSlot);
    connect(browser,&RecordBrowser::browserClosed,this,&WelcomePage::show);
}

void WelcomePage::loadNewGame(Record &record)
{
    gm = new GameMain(nullptr,settings,record);
    connect(gm,&GameMain::gameMainDeleted,this,&WelcomePage::gameMainDeletedSlot,Qt::QueuedConnection);
    connect(gm,&GameMain::gameMainClosed,this,&WelcomePage::gameMainClosedSlot,Qt::QueuedConnection);
    connect(gm,&GameMain::gamePaused,this,&WelcomePage::gamePausedSlot,Qt::QueuedConnection);
    connect(gm,&GameMain::gameTimeout,this,&WelcomePage::gameTimeoutSlot,Qt::QueuedConnection);
    connect(gm,&GameMain::gameWin,this,&WelcomePage::gameWinSlot,Qt::QueuedConnection);
}

void WelcomePage::startNewGame(Record &record)
{
    loadNewGame(record);
    gm->show();
    this->close();
}
void WelcomePage::on_single_player_button_clicked()//单人模式
{
    browser->filterRecord(singleMode);
    browser->show();
    this->hide();
}

void WelcomePage::on_multi_player_button_clicked()//双人模式
{
    browser->filterRecord(multiMode);
    browser->show();
    this->hide();
}
void WelcomePage::gameMainClosedSlot()
{
    delete gm;
    gm = nullptr;//将gameMain设置为nullptr
    this->show();
}
void WelcomePage::gamePausedSlot()
{
    menu->switchPauseMode();
    menu->show();
    gm->hide();
}

void WelcomePage::gameTimeoutSlot()
{
    menu->switchTimeoutMode();
    menu->show();
    gm->hide();
}

void WelcomePage::gameWinSlot(ScoreBoard* scoreBoard)
{
    menu->switchWinMode();
    menu->show();
    *menu->winnerBoard() = *scoreBoard;
    gm->hide();

    //reach last level
    if(gm->getRecord().getCurLevel() == settings->getLevelsInMode(gm->getGameMode()).size())
    {
        menu->nextButton()->hide();
    }
}
void WelcomePage::continueClickedSlot()
{
    gm->show();
    menu->close();
    gm->start();
}

void WelcomePage::playNextLevel()
{
    Record& record = gm->getRecord();
    if(!record.readFromSettings(settings,gm->getGameMode(),record.getCurLevel() + 1))
    {
        qDebug() << "no more levels to be played.";
        homeClickedSlot();
    }
    delete gm;
    startNewGame(record);
    menu->close();
    this->close();
}

void WelcomePage::saveGameToRecord()
{
    gm->saveGameToRecord();
}
void WelcomePage::replayGame()
{
    Record& record = gm->getRecord();
    record.readFromSettings(settings,gm->getGameMode(),record.getCurLevel());
    delete gm;
    startNewGame(record);
    menu->close();
    this->close();
}
void WelcomePage::homeClickedSlot()
{
    if(menu->getMenuMode() == menuPage::win)//load next level
    {
        Record& record = gm->getRecord();
        record.readFromSettings(settings,gm->getGameMode(),record.getCurLevel() + 1);
    }
    gm->close();
    menu->close();
}
void WelcomePage::on_exit_button_clicked()
{
    this->close();
}


void WelcomePage::on_set_button_clicked()
{
    settings->show();
}

void WelcomePage::recordEnteredSlot(Record &record)
{
    startNewGame(record);
}

void WelcomePage::gameMainDeletedSlot()
{
    menu->winnerBoard()->setPlayer(nullptr);
}

void WelcomePage::keyPressEvent(QKeyEvent *event)
{
    //退出程序
    if(event->key() == Qt::Key_Escape)
    {
        on_exit_button_clicked();
    }
}


#include "welcomepage.h"
#include "ui_welcomepage.h"
#include "record.h"
#include <QKeyEvent>

WelcomePage::WelcomePage(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::WelcomePage)
{
    ui->setupUi(this);
    initMenu();
    settings = new Settings();
    initBrowser();

    QPixmap pixmap("C:/Users/11814/Desktop/temp/SEP/SEP-QLink/testWidget/testWidget/background.jpg");
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(pixmap));
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

//void WelcomePage::startNewGame(gameMain::gameMode mode)
//{
//    gm = new GameMain(nullptr,settings,mode);
//    connect(gm,&GameMain::gameMainClosed,this,&WelcomePage::gameMainClosedSlot,Qt::QueuedConnection);
//    connect(gm,&GameMain::gamePaused,this,&WelcomePage::gamePausedSlot,Qt::QueuedConnection);
//    gm->show();
//    this->close();
//}

void WelcomePage::startNewGame(Record &record)
{
    gm = new GameMain(nullptr,settings,record);
    connect(gm,&GameMain::gameMainClosed,this,&WelcomePage::gameMainClosedSlot,Qt::QueuedConnection);
    connect(gm,&GameMain::gamePaused,this,&WelcomePage::gamePausedSlot,Qt::QueuedConnection);
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
void WelcomePage::gamePausedSlot(const QString& info)
{
    menu->setInfo(info);
    if(info == "倒计时结束")
    {
        menu->continueButton()->hide();
    }
    else
    {
        menu->continueButton()->show();
    }
    menu->show();
    gm->hide();
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

void WelcomePage::keyPressEvent(QKeyEvent *event)
{
    //退出程序
    if(event->key() == Qt::Key_Escape)
    {
        on_exit_button_clicked();
    }
}


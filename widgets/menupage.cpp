#include "menupage.h"
#include "ui_menupage.h"

MenuPage::MenuPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MenuPage)
{
    ui->setupUi(this);
    this->setFocus();
}

MenuPage::~MenuPage()
{
    delete ui;
}

void MenuPage::switchPauseMode(const QString& info)
{
    _mode = pause;
    ui->info_label->setText(info);
    ui->continue_button->show();
    ui->next_button->hide();
    ui->save_button->show();
}

void MenuPage::switchTimeoutMode(const QString& info)
{
    _mode = timeout;
    ui->info_label->setText(info);
    ui->continue_button->hide();
    ui->next_button->hide();
    ui->save_button->hide();
}

void MenuPage::switchWinMode(const QString& info)
{
    _mode = win;
    ui->info_label->setText(info);
    ui->continue_button->hide();
    ui->next_button->show();
    ui->save_button->hide();
}

mode MenuPage::getMenuMode() const
{
    return _mode;
}

QString MenuPage::info() const
{
    return ui->info_label->text();
}
void MenuPage::setInfo(const QString &str)
{
    ui->info_label->setText(str);
}

QPushButton* MenuPage::continueButton()
{
    return ui->continue_button;
}

QPushButton *MenuPage::nextButton()
{
    return ui->next_button;
}

ScoreBoard *MenuPage::winnerBoard()
{
    return ui->winner_board;
}

void MenuPage::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape
            && this->continueButton()->isVisible())
    {
        this->on_continue_button_clicked();//相当于按下继续游戏按钮
    }
    else if(event->key() == Qt::Key_R)//replay
    {
        this->on_replay_button_clicked();
    }
    else if(event->key() == Qt::Key_H)//home
    {
        this->on_home_button_clicked();
    }
}

void MenuPage::on_replay_button_clicked()
{
    this->setFocus();
    emit replayClicked();
}

void MenuPage::on_home_button_clicked()
{
    this->setFocus();
    emit homeClicked();
}

void MenuPage::on_continue_button_clicked()
{
    this->setFocus();
    emit continueClicked();
}

void MenuPage::on_next_button_clicked()
{
    emit nextClicked();
}

void MenuPage::on_save_button_clicked()
{
    this->setFocus();
    emit saveClicked();
}

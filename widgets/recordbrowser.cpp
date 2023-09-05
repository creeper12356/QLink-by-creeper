#include <QInputDialog>
#include "recordbrowser.h"
#include "ui_recordbrowser.h"
#include "gamemain.h"
#include "settings.h"

RecordBrowser::RecordBrowser(QWidget *parent, Settings *&sts) :
    QWidget(parent),
    settings(sts),
    ui(new Ui::RecordBrowser)
{
    ui->setupUi(this);
    QDir dir("records");
    for(QString recordName:dir.entryList())
    {
        if(recordName == ".." || recordName == ".")
        {
            continue;
        }
        ui->recordList->addItem(new RecordItem
                                (dir.path() + "/" + recordName,
                                QFileInfo(recordName).baseName()));
    }
}

RecordBrowser::~RecordBrowser()
{
    delete ui;
}

void RecordBrowser::filterRecord(gameMode mode)
{
    for(int i = 0;i <= ui->recordList->count() - 1;++i)
    {
        ui->recordList->item(i)->setHidden(
                    dynamic_cast<RecordItem*>
                    (ui->recordList->item(i))->getMode()!= mode);
    }
    ui->recordList->setCurrentRow(-1);
    this->filterMode = mode;
}

void RecordBrowser::updateText()
{
    int count = ui->recordList->count();
    for(int i = 0;i <= count - 1;++i)
    {
        dynamic_cast<RecordItem*>(ui->recordList->item(i))->updateText();
    }
}

void RecordBrowser::on_recordList_itemDoubleClicked(QListWidgetItem *item)
{
    emit recordEntered(dynamic_cast<RecordItem*>(item)->getRecord());//发出待加载存档的信号
    for(int i = 0;i <= ui->recordList->count() - 1;++i)
    {
        ui->recordList->item(i)->setBackground(QColor(0,200,100,180));
    }
    this->hide();//必须使用hide,若使用close会重新召回主页面
}

void RecordBrowser::on_recordList_currentRowChanged(int currentRow)
{
    for(int i = 0;i <= ui->recordList->count() - 1;++i)
    {
        if(i != currentRow)
        {
            ui->recordList->item(i)->setBackground(QColor(0,200,100,180));
        }
        else
        {
            ui->recordList->item(i)->setBackground(QColor(255,255,0));
        }
    }
}

void RecordBrowser::closeEvent(QCloseEvent *event)
{
    emit browserClosed();
}

void RecordBrowser::on_recordList_itemEntered(QListWidgetItem *item)
{
    ui->recordList->setCurrentItem(item);
}

void RecordBrowser::showEvent(QShowEvent *event)
{
    this->updateText();
}

void RecordBrowser::on_new_record_button_clicked()
{
    QInputDialog recordNameGetter(this);
    recordNameGetter.setWindowTitle("新游戏");
    recordNameGetter.setInputMode(QInputDialog::TextInput);
    recordNameGetter.setLabelText("输入存档名称");
    connect(&recordNameGetter,&QInputDialog::textValueSelected,this,&RecordBrowser::newRecord);
    recordNameGetter.exec();

}

void RecordBrowser::newRecord(const QString &recordName)
{
    RecordItem* newItem = new RecordItem("records/" + recordName + ".json",recordName);
    newItem->getRecord().readFromSettings(settings,getFilterMode(),1);//start from level 1
    ui->recordList->addItem(newItem);
    this->updateText();
}

void RecordBrowser::on_delete_record_button_clicked()
{
    RecordItem* delItem = dynamic_cast<RecordItem*>
            (ui->recordList->takeItem(ui->recordList->currentRow()));
    if(delItem == nullptr)
    {
        return ;
    }
    delItem->getRecord().setIsDeleted(true);
    delete delItem;
}

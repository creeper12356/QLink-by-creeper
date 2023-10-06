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

void RecordBrowser::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape){
        close();
    }
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
    if(ui->rand_mode->isChecked()){
        //随机模式
        Record* nRecord = newRecord("newrecord",settings->getLevelsInMode(getFilterMode()).size());
        if(!nRecord){
            return ;
        }
        //创建成功
        BasicSetDialog dialog;
        dialog.setMaxWScale(nRecord->getBasic().wScale);
        dialog.setMaxHScale(nRecord->getBasic().hScale);
        dialog.exec();
        if(!dialog.isSetted()){//canceled
            return ;
        }
        this->hide();
        nRecord->setRandModeArg(QPoint(dialog.wScale(),dialog.hScale()));
        emit recordEntered(*nRecord);
        return ;
    }
    QInputDialog recordNameGetter(this);
    recordNameGetter.setWindowTitle("新游戏");
    recordNameGetter.setInputMode(QInputDialog::TextInput);
    recordNameGetter.setLabelText("输入存档名称");
    connect(&recordNameGetter,SIGNAL(textValueSelected(QString)),this,SLOT(newRecord(QString)));
    recordNameGetter.exec();
}

Record* RecordBrowser::newRecord(QString recordName)
{
    return newRecord(recordName,1);
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

void RecordBrowser::on_recordList_itemActivated(QListWidgetItem *item)
{
    emit recordEntered(dynamic_cast<RecordItem*>(item)->getRecord());//发出待加载存档的信号
    for(int i = 0;i <= ui->recordList->count() - 1;++i)
    {
        ui->recordList->item(i)->setBackground(QColor(0,200,100,180));
    }
    this->hide();//必须使用hide,若使用close会重新召回主页面
}

void RecordBrowser::on_cancel_button_clicked()
{
    this->close();
}

Record* RecordBrowser::newRecord(QString recordName, int level)
{
    //avoid name repeat problem.
    for(int i = 0;i <= ui->recordList->count() - 1;++i){
        QString existName = dynamic_cast<RecordItem*>(ui->recordList->item(i))->getName();
        if(recordName == existName){
            QMessageBox::warning(this,"警告","存档名已存在，创建游戏失败。");
            return nullptr;
        }
    }

    RecordItem* newItem = new RecordItem("records/" + recordName + ".json",recordName);
    newItem->getRecord().readFromSettings(settings,getFilterMode(),level);//start from level 1
    ui->recordList->addItem(newItem);
    this->updateText();
    return &newItem->getRecord();
}

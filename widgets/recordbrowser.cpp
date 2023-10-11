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
        items.append(new RecordItem
                                (dir.path() + "/" + recordName,
                                QFileInfo(recordName).baseName()));
    }
}

RecordBrowser::~RecordBrowser()
{
    for(auto item:items){
        delete item;
    }
    delete ui;
}

void RecordBrowser::removeAllItems()
{
    while(ui->recordList->count() > 0){
        ui->recordList->takeItem(0);
    }
}

void RecordBrowser::filterRecord(gameMode mode)
{
    for(auto item:items){
        if(dynamic_cast<RecordItem*>(item)->getMode() == mode)//游戏模式匹配
        {
            ui->recordList->addItem(item);
        }
    }
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

void RecordBrowser::closeEvent(QCloseEvent *event)
{
    removeAllItems();
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
    for(int i = 0;i <= ui->recordList->count() - 1;++i)
       {
           if(i == ui->recordList->currentRow()){
               ui->recordList->item(i)->setBackground(CHOSEN_COLOR);
           }
           else if(ui->recordList->item(i) == item){
               item->setBackground(HOVER_COLOR);
           }
           else{
               ui->recordList->item(i)->setBackground(NORMAL_COLOR);
           }
    }
}

void RecordBrowser::on_recordList_itemActivated(QListWidgetItem *item)
{
    removeAllItems();
    emit recordEntered(dynamic_cast<RecordItem*>(item)->getRecord());//发出待加载存档的信号
    this->hide();//必须使用hide,若使用close会重新召回主页面
}

void RecordBrowser::showEvent(QShowEvent *event)
{
    this->updateText();
}

void RecordBrowser::on_new_record_button_clicked()
{
    if(ui->rand_mode->isChecked()){
        //随机模式
        Record* nRecord = newRecord("rand-" + QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss"),settings->getLevelsInMode(getFilterMode()).size(),true);
        if(!nRecord){
            qDebug() << "cannot create new rec.";
            return ;
        }
        //创建成功
        BasicSetDialog dialog;
        dialog.setMaxWScale(nRecord->getBasic().wScale);
        dialog.setMaxHScale(nRecord->getBasic().hScale);
        dialog.exec();
        if(!dialog.isSetted()){//canceled // actually will never happen.
            qDebug() << "rand mode game canceled";
            return ;
        }
        nRecord->setRandModeArg(QPoint(dialog.wScale(),dialog.hScale()));
        this->hide();
        removeAllItems();
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
    int currentRow = ui->recordList->currentRow();
    if(currentRow < 0)
    {
        return ;
    }
    //record chosen
    QString warningText = "你确定要删除这个存档吗？\n存档将会永久消失！（真的很久！）";
    auto button = QMessageBox::warning(this,"警告",warningText,QMessageBox::Yes|QMessageBox::No);
    if(button != QMessageBox::Yes){
        return ;
    }
    RecordItem* delItem = dynamic_cast<RecordItem*>(ui->recordList->takeItem(currentRow));
    items.removeOne(delItem);
    delItem->getRecord().setIsDeleted(true);
    delete delItem;
}


void RecordBrowser::on_cancel_button_clicked()
{
    this->close();
}

void RecordBrowser::on_recordList_itemClicked(QListWidgetItem *item)
{
    ui->recordList->setCurrentItem(item);
}

void RecordBrowser::on_recordList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if(current){
        current->setBackground(CHOSEN_COLOR);
    }
    if(previous){
        previous->setBackground(NORMAL_COLOR);
    }
}
Record* RecordBrowser::newRecord(QString recordName, int level, bool isRandMode)
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
    if(isRandMode) {//先提前标记以后会启用随机模式
        newItem->getRecord().markRandMode();
    }
    newItem->getRecord().readFromSettings(settings,getFilterMode(),level);
    ui->recordList->addItem(newItem);
    items.append(newItem);
    this->updateText();
    return &newItem->getRecord();
}
void RecordBrowser::setEnableRandMode(bool flag)
{
    if(!flag){//reset
        ui->rand_mode->setChecked(false);
    }
    ui->rand_mode->setVisible(flag);
}

void RecordBrowser::on_copy_record_button_clicked()
{
    int currentRow = ui->recordList->currentRow();
    if(currentRow < 0)//no item chosen
    {
        return ;
    }
    RecordItem* srcRecordItem = dynamic_cast<RecordItem*> (ui->recordList->item(currentRow));
    Record* copyRecord = newRecord(srcRecordItem->getName() + "-copy");
    if(!copyRecord){
        return ;
    }
    *copyRecord = srcRecordItem->getRecord();
    dynamic_cast<RecordItem*>(ui->recordList->item(ui->recordList->count() - 1))->updateText();
    qDebug() << "copy complete.";
}

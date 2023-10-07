#ifndef RECORDBROWSER_H
#define RECORDBROWSER_H
#include "config.h"
#include "recorditem.h"
#define NORMAL_COLOR QColor(0,190,190)
#define HOVER_COLOR QColor(0,210,210)
#define CHOSEN_COLOR QColor(0,255,255)
#include "basicsetdialog.h"

namespace Ui {
class RecordBrowser;
}

class RecordBrowser : public QWidget
{
    Q_OBJECT
private:
    Settings*& settings;
    QVector<RecordItem*> items;//存储所有的recordItem
    gameMain::gameMode filterMode = gameMain::singleMode;//当前过滤的游戏模式
public:
    RecordBrowser(QWidget *parent,Settings*& sts);
    ~RecordBrowser();
private:
    inline void removeAllItems();
public:
    void filterRecord(gameMain::gameMode mode);//只显示指定模式的存档
    void updateText();//更新所有条目的文字
    //getters
    gameMain::gameMode getFilterMode() const{return filterMode;}

private slots:
    void on_recordList_itemEntered(QListWidgetItem* item);
    void on_recordList_itemActivated(QListWidgetItem* item);
    void on_recordList_itemClicked(QListWidgetItem *item);
    void on_recordList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);


    void on_new_record_button_clicked();
    void on_delete_record_button_clicked();
    Record* newRecord(QString recordName);//default overload : level = 1
    void on_cancel_button_clicked();
private:
    Record* newRecord(QString recordName,int level);//新建存档名为recordName的存档，成功返回指针，失败返回Null
protected:
    void showEvent(QShowEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
private:
    Ui::RecordBrowser *ui;

public slots:
    //ui接口
    void setEnableRandMode(bool flag);
signals:
    void recordEntered(Record& record);//recordItem被双击发出此信号，发给welcomepage表示加载存档
    void browserClosed();//关闭信号
};

#endif // RECORDBROWSER_H

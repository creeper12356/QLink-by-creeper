#pragma once
#ifndef RECORDITEM_H
#define RECORDITEM_H
#include "config.h"
#include "record.h"
/*
 * RecordItem: 存档条目,作为存档的前端载体
 */
class RecordItem:public QListWidgetItem
{
private:
    Record record;
    QString name;

public:
    RecordItem(const QString& recordPath,const QString& recordName);
    ~RecordItem();

    //getters
    gameMain::gameMode getMode() const;
    Record& getRecord();
    const QString& getName() const;

public:
    void updateText();
protected:
};

#endif // RECORDITEM_H

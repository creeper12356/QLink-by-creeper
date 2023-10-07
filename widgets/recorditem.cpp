#include "recorditem.h"
#include "recordbrowser.h"
RecordItem::RecordItem(const QString &recordPath, const QString &recordName):record(recordPath)
{
    name = recordName;
    this->updateText();
    this->setBackground(NORMAL_COLOR);
}

RecordItem::~RecordItem()
{
    qDebug() << "~RecordItem";
}

gameMain::gameMode RecordItem::getMode() const
{
    return record.getMode();
}

Record &RecordItem::getRecord()
{
    return record;
}

const QString &RecordItem::getName() const
{
    return name;
}

void RecordItem::updateText()
{
    QString text;
    text.append(name + "\n");
    text.append((record.getMode() == gameMain::singleMode)?"singleplayer":"multiplayer");
    text.append("\nlevel" + QString::number(record.getCurLevel()));
    this->setText(text);
}


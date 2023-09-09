#include "recorditem.h"
RecordItem::RecordItem(const QString &recordPath, const QString &recordName):record(recordPath)
{
    name = recordName;
    this->updateText();
    this->setBackground(QColor(0,200,100,180));
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

void RecordItem::updateText()
{
    QString text;
    text.append(name + "\n");
    text.append((record.getMode() == gameMain::singleMode)?"singleplayer":"multiplayer");
    text.append("\nlevel" + QString::number(record.getCurLevel()));
    this->setText(text);
}

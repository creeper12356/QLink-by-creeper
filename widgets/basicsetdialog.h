#ifndef BASICSETDIALOG_H
#define BASICSETDIALOG_H

#include <QDialog>
#include <QPoint>
#include "config.h"

namespace Ui {
class BasicSetDialog;
}

class BasicSetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BasicSetDialog(QWidget *parent = nullptr);
    ~BasicSetDialog();

private:
    Ui::BasicSetDialog *ui;
private slots:
    void acceptSlot();
signals:
    void basicSetted(QPoint scale);
};

#endif // BASICSETDIALOG_H

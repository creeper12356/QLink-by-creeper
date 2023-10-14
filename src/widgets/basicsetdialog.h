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
private:
    int wScale_s = -1;
    int hScale_s = -1;
public:
    explicit BasicSetDialog(QWidget *parent = nullptr);
    ~BasicSetDialog();

private:
    Ui::BasicSetDialog *ui;
public:
    bool isSetted() const{return wScale_s != -1;}
    int wScale() const{return wScale_s;}
    int hScale() const{return hScale_s;}

    void setMaxWScale(int w);
    void setMaxHScale(int h);

protected:
    void closeEvent(QCloseEvent *) override;
private slots:
    void on_buttonBox_accepted();
};

#endif // BASICSETDIALOG_H

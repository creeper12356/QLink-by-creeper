#include "config.h"
#include "widgets/gamemain.h"
#include "widgets/welcomepage.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QApplication>
#include "tools/scoreboard.h"
int main(int argc, char *argv[])
{
     QDir::setCurrent("..");

    QApplication a(argc, argv);

    WelcomePage wp;

    wp.show();
    return a.exec();
}

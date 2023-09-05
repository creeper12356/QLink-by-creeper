#include "widgets/gamemain.h"
#include "widgets/welcomepage.h"

#include <QGraphicsScene>
#include <QGraphicsView>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    QGraphicsScene scene;
//    QGraphicsView view;

    WelcomePage wp;
//    scene.addWidget(&w);
//    view.setScene(&scene);
//    view.resize(w.size());
//    view.setForegroundBrush(QColor(0,0,150,100));
//    view.show();
    wp.show();
    return a.exec();
}

/*tips
 * comment main.cpp when running unit case.
 * */
#include "config.h"
#include "widgets/gamemain.h"
#include "widgets/welcomepage.h"

#include <QApplication>
#include "tools/scoreboard.h"
int main(int argc, char *argv[])
{
    //comment this line when building release exe file
     QDir::setCurrent("..");

<<<<<<< HEAD
    QApplication a(argc, argv);
    WelcomePage wp;
    wp.show();
    return a.exec();
}
=======
//    QApplication a(argc, argv);

//    WelcomePage wp;

//    wp.show();
//    return a.exec();
//}
>>>>>>> b4ad632eb8dcdadfeb51877b8d896197dfff2e83

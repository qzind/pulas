#include "maindialog.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    bool isHide = false;
    a.setApplicationName("Pulas");
    a.setApplicationDisplayName("Pulas");
    a.setApplicationVersion("0.2");

    //check the argument for hiding
    if(argc == 2) {
        QString arg(argv[1]);
        if(!arg.compare("hide") || !arg.compare("--hide"))
            isHide = true;
    }

    MainDialog w;
    if(!isHide)
        w.show();
    else
        w.showTray();

    return a.exec();
}

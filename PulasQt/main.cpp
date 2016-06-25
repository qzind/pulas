#include "maindialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("Pulas");
    a.setApplicationDisplayName("Pulas");
    a.setApplicationVersion("0.1");

    MainDialog w;
    w.show();

    return a.exec();
}

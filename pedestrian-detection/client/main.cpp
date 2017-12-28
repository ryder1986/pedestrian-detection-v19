#include "mainwindow.h"
#include <QApplication>
#include "tool1.h"
int main(int argc, char *argv[])
{
    Tool1::set_debug_level(Tool1::DEBUG_LEVEL::INFO);
    QApplication a(argc, argv);
    prt(info,"client start");
    MainWindow w;
    w.show();
    return a.exec();
}

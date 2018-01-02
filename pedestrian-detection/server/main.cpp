#include <QCoreApplication>
#include "tool1.h"
#include "server.h"
#include "camera.h"
int main(int argc, char *argv[])
{
    if(argc==2&&!strcmp(argv[1],"-trace")){
        Tool1::set_debug_level(Tool1::DEBUG_LEVEL::DEBUG);
        Tool1::set_label(Tool1::LABEL_SELECTION::BOTH);
    }
    else{
        Tool1::set_debug_level(Tool1::DEBUG_LEVEL::INFO);
        Tool1::set_label(Tool1::LABEL_SELECTION::NORMAL);
    }
    QCoreApplication a(argc, argv);
    prt(info,"sever start");
    Server s;
    ServerInfoReporter rpt;
    rpt.start();
    CameraManager &mgr=CameraManager::GetInstance();
    return a.exec();
}

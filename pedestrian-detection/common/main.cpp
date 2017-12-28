#include "tool1.h"
#include "videosrc.h"
#include "ui_form.h"

#include "videoprocessor.h"
#include <QWidget>
#include <thread>
typedef struct data{
    VideoSrc *src;
    VideoWidget *w;
    VideoProcessor p;
}data_t;
data_t d;
void test_fun(data_t *dt)
{
    bool ret=false;
    Mat mt;
//    while(1){
//        ret=dt->src->fetch_frame(mt);

//        if(ret){
//           // dt->w->update_mat(mt);
//           dt->p.work(mt);
//        }
//    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Tool1::set_label(Tool1::LABEL_SELECTION::BOTH);
    Tool1::set_debug_level(Tool1::DEBUG_LEVEL::DEBUG);
    prt(info,"test tool1  %d",111);
    VideoSrc src("test1.mp4");
    QWidget w;
   // Ui::Form *f=new Ui::Form();
   // f->setupUi(&w);
  //  w.show();
    d.src=&src;
  //  d.w=f->openGLWidget;
    thread *t=new thread(test_fun,&d);
    return a.exec();
}

#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H
#include <QtOpenGL/QtOpenGL>
#include "tool1.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/objdetect/objdetect.hpp>
using namespace cv;
class VideoWidget : public QOpenGLWidget
{
    Mat frame;
    int wi;
    int he;
    int tick;
    int pos_x;
    int pos_y;
    QStringList list;
    QList <QRect> rcts;
public:
    VideoWidget();
    VideoWidget(QWidget *w):QOpenGLWidget(w)
    {
        wi=640;
        he=480;
        tick=0;
        pos_x=0;
        pos_y=0;
        rcts.clear();
        rcts.append(QRect(333,333,444,444));
    }
    void set_rects(QByteArray rst)
    {
      // this->width()/640;
           //prt(info," pic width %d",this->width());
        QString str(rst.data());
        QStringList list=str.split(":");
        QStringList l;
        foreach (QString s, list) {
            l=s.split(',');
            QRect r;
            if(l.size()==4){
                r.setRect(l[0].toInt()*this->width()/wi*2,l[1].toInt()*this->height()/he*2,l[2].toInt()*this->width()/wi*2,l[3].toInt()*this->height()/he*2);
                rcts.append(r);
            }
        }
    }
    int update_mat(Mat frame_mat)
    {
        int size=frame_mat.total();

        if(size>0)
        {
            frame=frame_mat;
                prt(debug,"render set frame  %d %d",he=frame.rows,wi=frame.cols);

        }else{
            prt(info,"render set frame fail");
        }
        this->update();
        return 1;
    }
protected:
    void paintEvent(QPaintEvent *)
    {
        int len=frame.cols;
        if(len>0){
            tick++;
            QPainter painter(this);
            paint_layout1(painter);
            paint_layout2(painter);
        }else{
            //prt(info,"invalid frame");
        }
    }
    void  initializeGL()
    {

    }

    void paint_layout1(QPainter &painter){
#if 0
        painter.beginNativePainting();
        makeCurrent();
        CONVERT_YUV420PtoRGB24((unsigned char *)yuv_buf,(unsigned char *)rgb_buf,video_width,video_height);
        glPixelZoom(1.0f*this->width()/video_width, 1.0f*this->height()/video_height);
#if 0
        glDrawPixels(video_width, video_height,GL_RGB, GL_UNSIGNED_BYTE, rgb_buf);
#else
        QImage img((uchar *)rgb_buf,video_width,video_height,QImage::Format_RGB888);
        //  QImage img((uchar *)rgb_buf,video_width,video_height,QImage::Format_RGB888);
        //QImage  img = QImage((const uchar*)(frame.data),frame.cols,frame.rows,frame.cols*frame.channels(),QImage::Format_Indexed8);
        //   QImage  img = QImage((const uchar*)(frame.data),frame.cols,frame.rows,frame.cols*frame.channels(),QImage::Format_Grayscale8);

        painter.drawImage(QRect(0,0,this->width(),this->height()),img);
#endif
        painter.endNativePainting();
#else
        painter.beginNativePainting();
        makeCurrent();
        Mat bgr_frame=frame;
        Mat rgb_frame;
        // imwrite("test.jpg",rgb_frame);
        //  cvtColor(rgb_frame,yuv_frame,CV_RGB2GRAY);
        cvtColor(bgr_frame,rgb_frame,CV_BGR2RGB);
        QImage  img = QImage((const uchar*)(rgb_frame.data),
                             rgb_frame.cols,rgb_frame.rows,
                             rgb_frame.cols*rgb_frame.channels(),
                             QImage::Format_RGB888);
        painter.drawImage(QRect(0,0,this->width(),this->height()),img);
        painter.endNativePainting();

#endif
    }
    void paint_layout2(QPainter &painter){
        //  painter.beginNativePainting();
        // makeCurrent();
        QBrush red_brush_trans(QColor(0,0,200,100));
        painter.setBrush(red_brush_trans);
       // painter.drawEllipse(pos_x++%500,pos_y++%500,50,50);
        pos_x+=10;
        painter.drawEllipse(pos_x%300,0,50,50);
//        foreach (QRect r, rcts) {
//            painter.drawRect(r);
//        }

            if(rcts.size()>0)
            { painter.drawRect(rcts.first());
                 rcts.clear();
            }
        //  painter.drawEllipse(500,500,50,50);
        //    painter.endNativePainting();

    }
    void set_data(QByteArray ba)
    {
        QList<QByteArray> xy=ba.split(',');
        int x=xy[0].toInt();
        int y=xy[1].toInt();
        pos_x=x;
        pos_y=y;
    }
    void paint_layout3(QPainter &painter){
#if 0
        QBrush red_brush_trans(QColor(0,0,200,100));
        painter.setBrush(red_brush_trans);
        // pressed_x=0;
        // pressed_y=0;
        //  if(flg++>10)


        painter.drawEllipse(pressed_x+tick%300,pressed_y,100,100);
        QPen testpen(red_brush_trans,10);
        painter.setPen(testpen);

        for(int i=0;i<8;i++){
            painter.drawLine(pt[i*2],pt[i*2+1]);
        }


        for(int i=0;i<8;i++){
            painter.drawLine(pt[i*2],pt[i*2+1]);
        }

        for(int i=0;i<8;i++){
            painter.drawLine(pt_rear[i*2],pt_rear[i*2+1]);
        }
        painter.drawEllipse(p1,10,10);
        painter.drawEllipse(p2,10,10);
        painter.drawEllipse(p3,10,10);
#endif
    }

};

#endif // VIDEOWIDGET_H

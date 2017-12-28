#ifndef VIDEOSRC_H
#define VIDEOSRC_H
#include <QtCore>
#include <thread>
#include "tool1.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/objdetect/objdetect.hpp>

using namespace std;
using namespace cv;

class  VideoSrc{
    typedef struct thread_data{
        string url;
        CvCapture *p_cap;
        VideoCapture vcap;
         int width;
        int height;
        bool quit_flag;
        int frame_wait_time;
        deque <Mat> frame_list;//fast from ending
    }thread_data_t;

    thread_data_t d;
    thread *p_video_thread;
public:
    VideoSrc(const char *url)
    {
        d.url=url;
        d.quit_flag=false;
        p_video_thread=new thread(fun,&d);
    }
    ~VideoSrc()
    {
        d.quit_flag=true;
        p_video_thread->join();
        delete p_video_thread;
    }
    bool fetch_frame(Mat &mt)
    {
        bool ret=false;
        if(d.frame_list.size()>5){
            mt=*d.frame_list.begin();
            d.frame_list.pop_front();
            ret=true;
        }
        return ret;
    }

private:
    static  void fun(thread_data_t *data){
        data->p_cap=NULL;
        QString str(data->url.data());
        if(str.contains("rtsp")||str.contains("http")){
            data->frame_wait_time=0;
        }else{
            data->frame_wait_time=40;
        }
        Mat mat_rst;
        while(!data->quit_flag){
            if(data->p_cap){
                IplImage *img;
            //      prt(info,"get frame begin");
//                  bool rt=data->vcap.read(mat_rst);
//                     if(rt) {prt(info,"get   ok");}
//                     else {prt(info,"get   err");}
                  img=cvQueryFrame(data->p_cap);
                   //      prt(info,"get frame done");
                if(!img){
                    prt(info,"get frame error,retry 1 seconds later");
                    cvReleaseCapture(&data->p_cap);
                    this_thread::sleep_for(chrono::seconds(1));
                }else{
                    Mat(img).copyTo(mat_rst);
                    if(data->frame_list.size()<10){
                        data->frame_list.push_back(mat_rst);
                    //    prt(info,"get frame size %d",data->frame_list.size());
                    }
                    if(data->frame_wait_time)
                        this_thread::sleep_for(chrono::milliseconds(data->frame_wait_time));
                }
            }else{
                prt(info,"create video cap 1 sec later");
                //this_thread::sleep_for(chrono::seconds(1));
                data->p_cap=cvCreateFileCapture(data->url.data());  //create video source

                //   data->vcap=   VideoCapture(data->url.data());
                data->width=cvGetCaptureProperty(data->p_cap,CV_CAP_PROP_FRAME_WIDTH);
                data->height=cvGetCaptureProperty(data->p_cap,CV_CAP_PROP_FRAME_HEIGHT);
            }
        }
        if(data->p_cap)
            cvReleaseCapture(&data->p_cap);
        data->p_cap=NULL;

    }

    //    VideoSrc(QString path):p_cap(NULL)
    //    {
    //        memset(url,0,Tools::PATH_LENGTH);
    //        strcpy(url,path.toStdString().data());
    //        p_cap= cvCreateFileCapture(url);  //create video source
    //        width=cvGetCaptureProperty(p_cap,CV_CAP_PROP_FRAME_WIDTH);
    //        height=cvGetCaptureProperty(p_cap,CV_CAP_PROP_FRAME_HEIGHT);
    //        if(p_cap==NULL){
    //            prt(info,"video src start  %s err  ",url);
    //            video_connected_flag=false;
    //        }
    //        else {
    //            prt(info,"video src  start %s ok  ",url);
    //        }
    //    }
    //    ~VideoSrc()
    //    {
    //        cvReleaseCapture(&p_cap);
    //        p_cap=NULL;
    //    }
    //    Mat *get_frame()
    //    {
    //        int err=0;
    //        if(p_cap==NULL){
    //            video_connected_flag=false;
    //            err=1;
    //        }
    //        IplImage *ret_img;
    //        ret_img=cvQueryFrame(p_cap);
    //        Mat(ret_img).copyTo(mat_rst);
    //        if(ret_img==NULL){
    //            err=1;
    //        }else{
    //        }
    //        if(err)
    //            return NULL;
    //        else
    //            return &mat_rst;

    //    }
    //    char *get_url(){
    //        return url;
    //    }

    //private:
    //    CvCapture *p_cap;
    //    char url[Tools::PATH_LENGTH];
    //    int width;
    //    int height;
    //    Mat mat_rst;
};

#endif // VIDEOSRC_H

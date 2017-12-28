
unix{
#PROJECT_ROOT="/root/repo-github"
CVPATH="/root/repo-github/install-pedestrian-detection"
#COMMON_LIB_PATH="/root/repo-github/res/common/lib-linux32"
##DESTDIR=/root/repo-github/build/
}

win32{
#PROJECT_ROOT="Z:\\"
CVPATH="Z:\install-pedestrian-detection"
#COMMON_LIB_PATH="Z:\res\common\lib-win32"
##DESTDIR=Z:\build
}




unix{
DEFINES +=IS_UNIX
}
win32{
DEFINES +=IS_WIN32
}


unix{
    DEFINES += IS_UNIX
    QMAKE_CXXFLAGS+="-std=c++11"
    INCLUDEPATH +="$$CVPATH/cv/opencv-linux-x86/include"
    LIBS+=-L$$CVPATH/cv/opencv-linux-x86/lib -lopencv_core -lopencv_highgui \
       -lopencv_objdetect -lopencv_imgproc -lopencv_ml   -lX11
    QMAKE_LIBS+=-lpthread
}
win32{
    INCLUDEPATH +="$$CVPATH/cv/opencv-vs2013-x86/include"

    CONFIG(debug, debug|release){
        LIBS+=-L$$CVPATH\cv\opencv-vs2013-x86\lib -lopencv_core249d -lopencv_highgui249d\
        -lopencv_objdetect249d -lopencv_imgproc249d -lopencv_ml249d
    }else{
        LIBS+=-L$$CVPATH\cv\opencv-vs2013-x86\lib -lopencv_core249 -lopencv_highgui249 \
        -lopencv_objdetect249 -lopencv_imgproc249 -lopencv_ml249
    }
}


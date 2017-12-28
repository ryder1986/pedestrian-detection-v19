SOURCES+=\
$$PWD/tool1.cpp \
$$PWD/videosrc.cpp
HEADERS+=\
$$PWD/tool1.h \
$$PWD/config.h \
$$PWD/videosrc.h
unix{
DEFINES +=IS_UNIX
}
win32{
DEFINES +=IS_WIN32
}

INCLUDEPATH+=$$PWD
CONFIG+=c++11

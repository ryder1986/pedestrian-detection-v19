include(../pedestrian-detection.pri)
include(common.pri)
include(client.pri)
include(server.pri)
SOURCES += \
    main.cpp \
    videosrc.cpp \
    pd.cpp

HEADERS += \
    videosrc.h \
    pd.h
config.h
CONFIG+=c++11
QT +=opengl

unix{
DEFINES +=IS_UNIX
}
win32{
DEFINES +=IS_WIN32
}

FORMS += \
    form.ui

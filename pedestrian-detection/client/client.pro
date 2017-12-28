#-------------------------------------------------
#
# Project created by QtCreator 2017-12-20T16:41:31
#
#-------------------------------------------------
include(../pedestrian-detection.pri)
include(../common/common.pri)
include(../common/client.pri)
QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    client.cpp \
    searchwidget.cpp

HEADERS  += mainwindow.h \
    client.h \
    searchwidget.h
unix{
message(build $$TARGET on unix)
INSTALL_PATH="/root/repo-github/install-pedestrian-detection"
install_setting.path=$$INSTALL_PATH
install_setting.files=$$OUT_PWD/$$TARGET
INSTALLS=install_setting
}else{
message(build $$TARGET on win32)
INSTALL_PATH="Z://install-pedestrian-detection"
install_setting.path=$$INSTALL_PATH
    debug{
        install_setting.files=$$OUT_PWD/debug/client.exe
    }
    release{
        install_setting.files=$$OUT_PWD/release/client.exe
    }
INSTALLS=install_setting
}

FORMS += \
    form.ui \
    searchwidget.ui


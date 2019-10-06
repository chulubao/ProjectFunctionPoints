#-------------------------------------------------
#
# Project created by QtCreator 2019-09-22T22:00:39
#
#-------------------------------------------------

QT       += core gui  xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = uiService
TEMPLATE = app
LIBS += -lcrypto

SOURCES += main.cpp\
    login.cpp \
    ../../common/commoninfo.cpp \
    ../../common/communication.cpp \
    ../../common/logdebug.cpp \
    xmltool.cpp

HEADERS  += \
    login.h \
    ../../common/communication.h \
    ../../common/logdebug.h \
    ../../common/socketpath.h \
    ../../common/commoninfo.h \
    common.h \
    xmltool.h

FORMS    += \
    login.ui

#-------------------------------------------------
#
# Project created by QtCreator 2019-09-22T22:00:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = uiService
TEMPLATE = app
LIBS += -lcrypto

SOURCES += main.cpp\
    login.cpp \
    ../../common/commoninfo.cpp \
    ../../common/communication.cpp \
    ../../common/logdebug.cpp

HEADERS  += \
    login.h \
    communication.h \
    ../../common/communication.h \
    ../../common/logdebug.h \
    ../../common/socketpath.h \
    ../../rapidXml/rapidxml.hpp \
    ../../rapidXml/rapidxml_iterators.hpp \
    ../../rapidXml/rapidxml_print.hpp \
    ../../rapidXml/rapidxml_utils.hpp \
    ../../common/commoninfo.h

FORMS    += \
    login.ui

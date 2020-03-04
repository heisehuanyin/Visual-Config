#-------------------------------------------------
#
# Project created by QtCreator 2020-02-26T19:45:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ParserConfig
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

ICON = icon.icns

SOURCES += \
        boolindicator.cpp \
        colorinput.cpp \
        fontconfig.cpp \
        itemsgroup.cpp \
        main.cpp \
        numberinput.cpp \
        optionalgroup.cpp \
        pathinput.cpp \
        selectlist.cpp \
        setstack.cpp \
        textinput.cpp \
        widget.cpp

HEADERS += \
        boolindicator.h \
        colorinput.h \
        fontconfig.h \
        itemsgroup.h \
        numberinput.h \
        optionalgroup.h \
        pathinput.h \
        selectlist.h \
        setstack.h \
        textinput.h \
        widget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ConfigFoundation/release/ -lConfigFoundation
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ConfigFoundation/debug/ -lConfigFoundation
else:unix: LIBS += -L$$OUT_PWD/../ConfigFoundation/ -lConfigFoundation

INCLUDEPATH += $$PWD/../ConfigFoundation
DEPENDPATH += $$PWD/../ConfigFoundation

DISTFILES += \
    icon.icns
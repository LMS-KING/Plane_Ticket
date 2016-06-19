#-------------------------------------------------
#
# Project created by QtCreator 2015-07-03T13:33:52
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Air_Ticket
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialog.cpp \
    control.cpp \
    userregist.cpp \
    peoplemessage.cpp \
    ticketmessage.cpp \
    schedule.cpp \
    record.cpp \
    password.cpp \
    xinshiye.cpp

HEADERS  += mainwindow.h \
    dialog.h \
    control.h \
    userregist.h \
    peoplemessage.h \
    ticketmessage.h \
    schedule.h \
    record.h \
    password.h \
    xinshiye.h

FORMS    += mainwindow.ui \
    dialog.ui \
    control.ui \
    userregist.ui \
    peoplemessage.ui \
    ticketmessage.ui \
    schedule.ui \
    record.ui \
    password.ui \
    xinshiye.ui

RESOURCES += \
    qtresource.qrc

RC_FILE += logo.rc

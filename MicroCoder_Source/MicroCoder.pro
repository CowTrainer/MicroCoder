#-------------------------------------------------
#
# Project created by QtCreator 2014-03-04T15:07:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MicroCoder
TEMPLATE = app


SOURCES += main.cpp\
    draggableheaderview.cpp \
    generatedlg.cpp \
        mainwindow.cpp \
    configurationdlg.cpp \
    programinstructionlist.cpp \
    programtablemodel.cpp \
    signalconfigdlg.cpp \
    stringcombodelegate.cpp \
    stringspindelegate.cpp \
    ucsignal.cpp \
    signallistmodel.cpp \
    mimedatawrapper.cpp \
    binaryspindelegate.cpp \
    mystringlistmodel.cpp \
    configuration.cpp \
    microinstruction.cpp \
    tokenizer.cpp \
    microprograminstruction.cpp

HEADERS  += mainwindow.h \
    configurationdlg.h \
    draggableheaderview.h \
    generatedlg.h \
    programinstructionlist.h \
    programtablemodel.h \
    signalconfigdlg.h \
    stringcombodelegate.h \
    stringspindelegate.h \
    ucsignal.h \
    signallistmodel.h \
    mimedatawrapper.h \
    binaryspindelegate.h \
    mystringlistmodel.h \
    configuration.h \
    microinstruction.h \
    tokenizer.h \
    microprograminstruction.h

FORMS    += mainwindow.ui \
    configurationdlg.ui \
    generatedlg.ui \
    signalconfigdlg.ui

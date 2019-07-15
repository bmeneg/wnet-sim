#-------------------------------------------------
#
# Project created by QtCreator 2019-07-06T21:31:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = wnet-sim-ui
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += \
	QT_DEPRECATED_WARNINGS \
	DEBUG

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++14

SOURCES += \
        ../core/core.cpp \
        ../core/network_graph.cpp \
        edge_ui.cpp \
        main.cpp \
        mainwindow.cpp \
        vertex_ui.cpp

HEADERS += \
        ../core/core.hpp \
        ../core/network_graph.hpp \
        edge_ui.hpp \
        mainwindow.hpp \
        vertex_ui.hpp

LIBS += \
     -lboost_graph \
     -lboost_program_options

INCLUDEPATH += /usr/lib64/boost/ \
	   ../core/

DISTFILES += \
	../core/graph.txt

# Default rules for deployment.
!isEmpty(target.path): INSTALLS += target

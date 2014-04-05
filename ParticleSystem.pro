TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

unix:!macx: LIBS += -lOgreMain -lOIS

SOURCES += main.cpp \
    myapplication.cpp \
    myframelistener.cpp

HEADERS += \
    myapplication.h \
    myframelistener.h


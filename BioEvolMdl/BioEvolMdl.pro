QT += core
QT -= gui

CONFIG += c++11

TARGET = BioEvolMdl
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += ../main.cpp \
../src/*.cpp \
../src/*.h




unix|win32: LIBS += -lsfml-system

unix|win32: LIBS += -lsfml-window

unix|win32: LIBS += -lsfml-graphics

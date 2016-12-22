TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    testing.cpp

include(deployment.pri)
qtcAddDeployment()

#DEFINES += _GLIBCXX_DEBUG

QMAKE_CXXFLAGS_DEBUG += -fsanitize=address,undefined
QMAKE_LFLAGS_DEBUG += -fsanitize=address,undefined

QMAKE_CXXFLAGS += -std=c++11

HEADERS += \
    testing.h

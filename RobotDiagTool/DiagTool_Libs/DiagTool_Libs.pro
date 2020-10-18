QT -= gui
QT += serialport

TARGET = DiagTool_Libs
TEMPLATE = lib

DEFINES += DIAGTOOL_LIBS_LIBRARY

CONFIG += c++14
CONFIG += staticlib

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        communication.cpp \
        communicationserialport.cpp \
        inifilehandler.cpp \
        robotproxy.cpp

HEADERS += \
    communication.h \
    communicationserialport.h \
    inifilehandler.h \
    robotproxy.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target




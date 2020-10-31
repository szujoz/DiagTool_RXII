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
        commandpacker.cpp \
        communication.cpp \
        communicationserialport.cpp \
        encoder.cpp \
        escapeencoder.cpp \
        inifilehandler.cpp \
        robotcommand.cpp \
        robotproxy.cpp

HEADERS += \
    binaryencoder.h \
    commandidconfig.h \
    commandpacker.h \
    communication.h \
    communicationserialport.h \
    encoder.h \
    escapeencoder.h \
    icommandpacker.h \
    inifilehandler.h \
    irobotcommand.h \
    robotcommand.h \
    robotproxy.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target




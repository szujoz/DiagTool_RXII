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
        common/BinaryEncoder/EscapeEncoder.cpp \
        common/CRC/crc8.cpp \
        robot_modules/encoder.cpp \
        commandpacker.cpp \
        communication.cpp \
        communicationserialport.cpp \
        inifilehandler.cpp \
        robotcommand.cpp \
        robotproxy.cpp

HEADERS += \
    common/BinaryEncoder/BinaryEncoder.h \
    common/BinaryEncoder/EscapeEncoder.h \
    common/CRC/crc8.h \
    robot_modules/encoder.h \
    commandidconfig.h \
    commandpacker.h \
    communication.h \
    communicationserialport.h \
    icommandpacker.h \
    inifilehandler.h \
    irobotcommand.h \
    robotcommand.h \
    robotproxy.h

INCLUDEPATH += \
    common/CRC/ \
    common/BinaryEncoder/ \
    robot_modules/

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target




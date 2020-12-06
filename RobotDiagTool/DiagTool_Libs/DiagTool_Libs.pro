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
        robot_commands/commandfactory.cpp \
        robot_commands/robotcommandbuilder.cpp \
        robot_modules/encoder.cpp \
        robot_commands/commandpacker.cpp \
        robot_commands/robotcommand.cpp \
        robot_commands/commanddirector.cpp \
        communication.cpp \
        communicationserialport.cpp \
        inifilehandler.cpp \
        robot_modules/remote.cpp \
    robot_modules/sevensegui.cpp \
        robotproxy.cpp \

HEADERS += \
    common/BinaryEncoder/BinaryEncoder.h \
    common/BinaryEncoder/EscapeEncoder.h \
    common/CRC/crc8.h \
    robot_commands/commandfactory.h \
    robot_commands/icommandbuilder.h \
    robot_commands/robotcommandbuilder.h \
    robot_modules/encoder.h \
    robot_commands/commandidconfig.h \
    robot_commands/commandpacker.h \
    robot_commands/irobotcommand.h \
    robot_commands/robotcommand.h \
    robot_commands/icommandpacker.h \
    robot_commands/commanddirector.h \
    communication.h \
    communicationserialport.h \
    robot_modules/irobotmodule.h \
    inifilehandler.h \
    robot_modules/remote.h \
    robot_modules/sevensegui.h \
    robotproxy.h \

INCLUDEPATH += \
    common/CRC/ \
    common/BinaryEncoder/ \
    robot_modules/ \
    robot_commands/

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target




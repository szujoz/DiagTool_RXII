QT += core gui serialport qml quick widgets
QT += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14
TARGET = DiagTool

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    diagtoolappcontrol.cpp \
    main.cpp \
    mainwindow.cpp \
    serialconnectionworker.cpp \
    serialsettingsdialog.cpp

HEADERS += \
    diagtoolappcontrol.h \
    mainwindow.h \
    serialconnectionworker.h \
    serialsettingsdialog.h

FORMS += \
    mainwindow.ui \
    serialsettingsdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += \
        $$PWD/../DiagTool_Libs \
        $$PWD/../DiagTool_Libs/robot_modules \
        $$PWD/../DiagTool_Libs/robot_commands \
        $$PWD/../DiagTool_Libs/common/CRC \
        $$PWD/../DiagTool_Libs/common/BinaryEncoder \

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../DiagTool_Libs/release/ -lDiagTool_Libs
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../DiagTool_Libs/debug/ -lDiagTool_Libs

DISTFILES +=

RESOURCES += \
    resource.qrc

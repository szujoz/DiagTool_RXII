QT += testlib
QT -= gui
QT += serialport

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle
CONFIG += testcase

TEMPLATE = app

HEADERS += \
    tst_communicationserialport.h \
    tst_robotproxy.h

SOURCES +=  tst_defaulttest.cpp \
    tst_communicationserialport.cpp \
    tst_main.cpp \
    tst_robotproxy.cpp

INCLUDEPATH += $$PWD/../DiagTool_Libs

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../DiagTool_Libs/release/ -lDiagTool_Libs
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../DiagTool_Libs/debug/ -lDiagTool_Libs



TEMPLATE = subdirs

SUBDIRS += \
    DiagTool \
    DiagTool_Libs \
    DiagTool_Test

CONFIG += ordered

CarDiagnosticsTool.depends = CarDiagnosticsTool_Lib
CarDiagnosticsTool_Test.depends = CarDiagnosticsTool_Lib

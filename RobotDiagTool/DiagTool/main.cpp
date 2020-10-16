#include "mainwindow.h"
#include "diagtoolapp.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    DiagToolApp app;
    app.ThreadTest();
    app.ThreadTest();
    app.ThreadTest();
    app.ThreadTest();
    app.ThreadTest();

    w.on_widget_NewDataToDisplay(2);

    while(0)
    {
        //app.ThreadTest();
    }
    return a.exec();
}

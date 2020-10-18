#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void DisplaySerialState(bool const connected);

signals:
    void SerialDialogNeeded();
    void SerialConnectionRequest();
    void SerialDisconnectionRequest();

public slots:
    void on_widget_NewDataToDisplay(int i);

private slots:
    void on_pushButton_2_clicked();

    void on_actionSettings_triggered();

    void on_btn_ToolBarConnectSerial_clicked();

    void on_btn_ToolBarDisconnectSerial_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

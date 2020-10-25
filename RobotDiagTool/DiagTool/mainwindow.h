#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>

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
    void DisplaySerialTerminalData(QString const str);

    void ScopeInit();
    void DisplayScopeData(QVector<QPointF>& points);

signals:
    void SerialDialogNeeded();
    void SerialConnectionRequest();
    void SerialDisconnectionRequest();
    void SerialDataReady(QString const message);

private slots:
    void on_pushButton_2_clicked();

    void on_actionSettings_triggered();

    void on_btn_ToolBarConnectSerial_clicked();
    void on_btn_ToolBarDisconnectSerial_clicked();

    void on_pushButton_clicked();

    void on_btn_ScopeSignalSelectorToggle_clicked();

    void on_btn_TerminalSend_clicked();

private:
    Ui::MainWindow *ui;

    QLineSeries* lineSeries;
};
#endif // MAINWINDOW_H

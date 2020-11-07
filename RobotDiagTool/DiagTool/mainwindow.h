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

    void DisplayTraceInQuickTab(QString const text);

    bool IsScopeTabSelected();

signals:
    void SerialDialogNeeded();
    void SerialConnectionRequest();
    void SerialDisconnectionRequest();
    void SerialDataReady(QString const message);
    void SerialClearScope();

private slots:
    void on_btn_QuickTabToggle_clicked();
    void on_btn_QuickTabCleanTrace_clicked();

    void on_actionSettings_triggered();

    void on_btn_ToolBarConnectSerial_clicked();
    void on_btn_ToolBarDisconnectSerial_clicked();

    void on_pushButton_clicked();

    void on_btn_ScopeSignalSelectorToggle_clicked();
    void on_btn_ScopeClearData_clicked();

    void on_btn_TerminalSend_clicked();
    void on_btn_TerminalClearSerialTerminal_clicked();

private:
    Ui::MainWindow *ui;

    std::unique_ptr<QChartView> scopeChartView;
    // The chartview owns the following 3 pointers.
    QLineSeries* lineSeries;
    QValueAxis*  scopeAxisX;
    QValueAxis*  scopeAxisY;
    uint32_t nextDataIndexToBeChecked;

    void ScopeDynamicResizeIfNeeded(QVector<QPointF>& points);
};
#endif // MAINWINDOW_H

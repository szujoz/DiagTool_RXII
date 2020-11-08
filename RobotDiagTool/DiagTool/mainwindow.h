#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QChartView_;

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
    void ScopeAllowAutoScaling(bool on);

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

    void on_btn_ScopeSignalSelectorToggle_clicked();
    void on_btn_ScopeResetZoom_clicked();
    void on_btn_ScopeToggleDrawing_clicked();
    void on_btn_ScopeClearData_clicked();

    void on_btn_TerminalSend_clicked();
    void on_btn_TerminalClearSerialTerminal_clicked();

private:
    Ui::MainWindow *ui;

    std::unique_ptr<QChartView_> scopeChartView;
    // The chartview owns the following 3 pointers.
    QLineSeries* lineSeries;
    QValueAxis*  scopeAxisX;
    QValueAxis*  scopeAxisY;
    uint32_t nextDataIndexToBeChecked;
    //

    bool allowedToDrawChart;
    bool autoScalingOn;

    void ScopeDynamicResizeIfNeeded(QVector<QPointF>& points);
};

class QChartView_ : public QChartView
{
  Q_OBJECT
 public:
    QChartView_(MainWindow* parent);
    ~QChartView_() {}
protected:
    virtual void wheelEvent(QWheelEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

private:
    MainWindow* parentWindow;

    QPoint lastMousePos;
};
#endif // MAINWINDOW_H

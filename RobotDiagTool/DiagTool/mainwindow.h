#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>
#include <QMap>

#include "debugtrace.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QChartView_;
class ScopeSignalSelector;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    std::unique_ptr<ScopeSignalSelector> scopeSignalSelector;

    void DisplayDebugTrace(QString const trace);

    void DisplaySerialState(bool const connected);
    void DisplaySerialTerminalData(QString const str);

    void ScopeInit();

    void DisplayTraceInQuickTab(QString const text);
    void DisplayDistanceInQuickTab(int32_t const dist);

    bool IsGeneralTabSelected();
    bool IsScopeTabSelected();
    bool IsControllerTabSelected();

    void DisplayQickTabSpeed(float const speed);
    void DisplayRemoteChData(uint8_t const ch1, uint8_t const ch2, uint8_t const ch3);
    void DisplayEncoderData(int32_t const speed, int32_t const counter);
    void Display7SegNumber(uint8_t const number);
    void DisplayDistanceFront(int32_t const frontDistance);
    void DisplaySpeedCntrlSetPoint(int32_t const setPoint);
    void DisplaySpeedCntrlControlValue(int32_t const controlValue);
    void DisplaySpeedCntrlProcessValue(int32_t const processValue);
    void DisplaySpeedCntrlP(int32_t const P);
    void DisplaySpeedCntrlI(int32_t const I);
    void DisplaySpeedCntrlD(int32_t const D);
    void DisplaySpeedCntrlIntegrateLimit(int32_t const integrateLimit);
    void DisplaySpeedCntrlIntegrate(int32_t const integrate);
    void DisplaySpeedCntrlDerivative(int32_t const derivative);
    void DisplaySpeedCntrlError(int32_t const error);


signals:
    void SerialDialogNeeded();
    void SerialConnectionRequest();
    void SerialDisconnectionRequest();
    void SerialDataReady(QString const message);
    void SerialClearScope();

    // Command TX
    void CmdTx_DummyData(int32_t const data);
    void CmdTx_7SegNum(uint8_t const number);
    void CmdTx_SpeedCntrl_SetPoint(int32_t const setPoint);
    void CmdTx_SpeedCntrl_P(int32_t const P);
    void CmdTx_SpeedCntrl_I(int32_t const I);
    void CmdTx_SpeedCntrl_D(int32_t const D);
    void CmdTx_SpeedCntrl_IntegrateLimit(int32_t const integrateLimit);

    // Command RX



private slots:
    void on_btn_QuickTabToggle_clicked();
    void on_btn_QuickTabCleanTrace_clicked();
    void on_btn_QuickTabDummyDataTx_clicked();

    void on_actionSettings_triggered();
    void on_actionSave_triggered();
    void on_actionLoad_triggered();

    void on_btn_ToolBarConnectSerial_clicked();
    void on_btn_ToolBarDisconnectSerial_clicked();

    void on_btn_ScopeSignalSelectorToggle_clicked();
    void on_btn_ScopeResetZoom_clicked();
    void on_btn_ScopeToggleDrawing_clicked();
    void on_btn_ScopeClearData_clicked();

    void on_btn_TerminalSend_clicked();
    void on_btn_TerminalClearSerialTerminal_clicked();

    void on_btn_ScopeYUpperBoundIncr_pressed();
    void on_btn_ScopeYUpperBoundDecr_pressed();
    void on_btn_ScopeYLowerBoundIncr_pressed();
    void on_btn_ScopeYLowerBoundDecr_pressed();
    void on_btn_ScopeXUpperBoundIncr_pressed();
    void on_btn_ScopeXUpperBoundDecr_pressed();
    void on_btn_ScopeXLowerBoundIncr_pressed();
    void on_btn_ScopeXLowerBoundDecr_pressed();

    void on_lineEdit_ScopeYUpperBound_editingFinished();
    void on_lineEdit_ScopeYLowerBound_editingFinished();
    void on_lineEdit_ScopeXUpperBound_editingFinished();
    void on_lineEdit_ScopeXLowerBound_editingFinished();

    void handle_AxisXRangeChange(qreal min, qreal max);
    void handle_AxisYRangeChange(qreal min, qreal max);

    void on_checkBox_GeneralUiBoardNumberForce_stateChanged(int newState);
    void on_lineEdit_GeneralUiBoard7SegOut_editingFinished();

    void on_btn_ControllersForce_SetPoint_clicked();
    void on_btn_ControllersForce_P_clicked();
    void on_btn_ControllersForce_I_clicked();
    void on_btn_ControllersForce_D_clicked();
    void on_btn_ControllersForce_IntegrateLimit_clicked();

private:
    Ui::MainWindow *ui;

    DebugTrace* debug;

    std::unique_ptr<QChartView_> scopeChartView;
    // The chartview owns the following 3 pointers.
    //QLineSeries* lineSeries;

    QValueAxis*  scopeAxisX;
    QValueAxis*  scopeAxisY;
    uint32_t nextDataIndexToBeChecked;
    //

    bool autoScalingOn;

    int const serialTerminalBuffer_LongTerm_Size = 10000;
    int const serialTerminalBuffer_ShortTerm_Size = 100;
    QString serialTerminalBuffer_ShortTerm[100];
    int serialTerminalBuffer_LongTerm_WriteIndex;
    int serialTerminalBuffer_ShortTerm_WriteIndex;
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

typedef struct
{
    int               _chartViewSeriesID;
    QString           _name;
    QLineSeries*      _series;
    QVector<QPointF>* _points;
    int               _pointCount;
    bool              _visible;    

} SignalInfo;

class ScopeSignalSelector : public QObject // Bridge class for diagtoolapp
{
    Q_OBJECT
public:
    ScopeSignalSelector(Ui::MainWindow*  ui,
                        QChartView_* chartview,
                        QValueAxis*  axisX,
                        QValueAxis*  axisY);

    void RegisterLineSignal(QString const name);
    bool UpdateSignalPoints(QString const name, QVector<QPointF>& points);
    void ClearAllPoints();
    QVector<QVector<QPointF>> GetAllSelectedSingalPoints();
    QVector<QString> GetAllSelectedSignalNames();
    SignalInfo* GetSignalInfoByName(bool* found, QString const name);
    bool IsAllowedToDraw();
    void EnableDrawing();
    void DisableDrawing();
    void EnableAutoScale();
    void DisableAutoScale();

signals:
    void SignalToBeDisplayed(QString const name, bool const drawAllowed);

private:
    QList<SignalInfo> signalSeriesList;
    Ui::MainWindow*  ui;
    QChartView_* chartView;
    QValueAxis*  axisX;
    QValueAxis*  axisY;
    bool allowedToDrawChart;
    bool allowedAutoScaling;

    void AttachSignalToChartview(SignalInfo* signal);
    bool FindSignalByName(QString const name, SignalInfo** signal);
    QString GetUiItemNameBySignalName(QString const name);
    QString GetSignalNameByUiItemName(QString const itemName);
    void AddCheckboxToUi(QString const name);
    void ScopeDynamicResizeIfNeeded(QVector<QPointF>& points);

private slots:
    void StateChanged(int state);
};

#endif // MAINWINDOW_H

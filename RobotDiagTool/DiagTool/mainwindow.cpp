#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QChartView>
#include <QGraphicsView>
#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include <QtWidgets/QVBoxLayout>
#include <QtCharts/QChartGlobal>
#include <QChart>

using namespace QtCharts;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/images/asap-logo.png"));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete lineSeries;
    delete scopeAxisX;
    delete scopeAxisY;
}

void MainWindow::DisplaySerialState(const bool connected)
{
    if (connected == true)
    {
        ui->label_ToolBarSerialStatus->setText("Connected");
        //ui->label_ToolBarSerialStatus->setStyleSheet("QLabel { background-color : red; color : blue; }")
    }
    else
    {
        ui->label_ToolBarSerialStatus->setText("Disconnected");
    }
}

void MainWindow::DisplaySerialTerminalData(const QString str)
{
    ui->textEdit_TerminalSerialInput->append(str);
}

void MainWindow::ScopeInit()
{
    lineSeries = new QLineSeries();
    scopeAxisX = new QValueAxis();
    scopeAxisY = new QValueAxis();
    scopeChartView  = std::make_unique<QChartView_>(this);

    nextDataIndexToBeChecked = 0;

    scopeChartView->setMinimumSize(200, 100);

    scopeAxisX->setRange(0, 5);
    scopeAxisX->setLabelFormat("%g");
    scopeAxisX->setTitleText("Time");

    scopeAxisY->setRange(0, 5);
    scopeAxisY->setTitleText("Signal value");

    scopeChartView.get()->chart()->addAxis(scopeAxisX, Qt::AlignBottom);
    scopeChartView.get()->chart()->addAxis(scopeAxisY, Qt::AlignLeft);
    scopeChartView.get()->chart()->legend()->hide();
    scopeChartView.get()->chart()->setTitle("Measurement data from the robot.");
    scopeChartView.get()->chart()->addSeries(lineSeries);

    lineSeries->attachAxis(scopeAxisX);
    lineSeries->attachAxis(scopeAxisY);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(scopeChartView.get());

    ui->frame->setLayout(mainLayout);
    ui->frame->show();

    allowedToDrawChart = true;
    autoScalingOn = true;
}

void MainWindow::DisplayScopeData(QVector<QPointF>& points)
{
    if (allowedToDrawChart == true)
    {
        if (autoScalingOn == true)
        {
             ScopeDynamicResizeIfNeeded(points);
        }

        lineSeries->replace(points);
    }
}

void MainWindow::DisplayTraceInQuickTab(const QString text)
{
    ui->textEdit_quickTabTrace->append(text);
}

bool MainWindow::IsScopeTabSelected()
{
    bool scopeViewActive = false;

    if(ui->tabWidget->currentIndex() == 1)
    {
        scopeViewActive = true;
    }

    return scopeViewActive;
}

void MainWindow::ScopeAllowAutoScaling(bool on)
{
    autoScalingOn = on;
}

void MainWindow::on_btn_QuickTabToggle_clicked()
{
    if (ui->groupBox_QuickTab->isHidden())
    {
        ui->groupBox_QuickTab->show();
    }
    else
    {
        ui->groupBox_QuickTab->hide();
    }
}

void MainWindow::on_actionSettings_triggered()
{
    emit SerialDialogNeeded();
}

void MainWindow::on_btn_ToolBarConnectSerial_clicked()
{
    emit SerialConnectionRequest();
    DisplaySerialState(true);
}

void MainWindow::on_btn_ToolBarDisconnectSerial_clicked()
{
    emit SerialDisconnectionRequest();
    DisplaySerialState(false);
}

void MainWindow::on_pushButton_clicked()
{

}

void MainWindow::on_btn_ScopeSignalSelectorToggle_clicked()
{
    if (ui->label_ScopeSignalSelector->isHidden())
    {
        ui->label_ScopeSignalSelector->show();
        ui->scrollArea_ScopeSignalSelector->show();
        ui->btn_ScopeSignalSelectorToggle->setText("<");
    }
    else
    {
        ui->label_ScopeSignalSelector->hide();
        ui->scrollArea_ScopeSignalSelector->hide();
        ui->btn_ScopeSignalSelectorToggle->setText(">");
    }
}

void MainWindow::on_btn_TerminalSend_clicked()
{
    SerialDataReady(ui->lineEdit_TerminalSerialOutput->text());
}

void MainWindow::on_btn_QuickTabCleanTrace_clicked()
{
    ui->textEdit_quickTabTrace->clear();
}

void MainWindow::on_btn_TerminalClearSerialTerminal_clicked()
{
    ui->textEdit_TerminalSerialInput->clear();
}

void MainWindow::ScopeDynamicResizeIfNeeded(QVector<QPointF> &points)
{
    int   i = nextDataIndexToBeChecked;
    float maxX = scopeAxisX->max()*0.8;
    float minX = scopeAxisX->min()*0.8;
    float maxY = scopeAxisY->max()*0.8;
    float minY = scopeAxisY->min()*0.8;

    for(; i < points.size(); i++)
    {
        if(maxX < points[i].x())
        {
            maxX = points[i].x();
        }

        if(minX > points[i].x())
        {
            minX = points[i].x();
        }

        if(maxY < points[i].y())
        {
            maxY = points[i].y();
        }

        if(minY > points[i].y())
        {
            minY = points[i].y();
        }
    }
    nextDataIndexToBeChecked = i;

    scopeAxisX->setRange(minX*1.25,maxX*1.25);
    scopeAxisY->setRange(minY*1.25,maxY*1.25);
}

void MainWindow::on_btn_ScopeResetZoom_clicked()
{
    autoScalingOn = true;
    scopeChartView->chart()->zoomReset();
}

void MainWindow::on_btn_ScopeToggleDrawing_clicked()
{
    if (allowedToDrawChart == true)
    {
        allowedToDrawChart = false;
        ui->btn_ScopeToggleDrawing->setText("Start Drwaing");
    }
    else
    {
        allowedToDrawChart = true;
        ui->btn_ScopeToggleDrawing->setText("Stop Drwaing");
    }
}

void MainWindow::on_btn_ScopeClearData_clicked()
{
    nextDataIndexToBeChecked = 0;
    emit SerialClearScope();
}

QChartView_::QChartView_(MainWindow *parent) : parentWindow(parent)
{
    setRubberBand(QChartView::RectangleRubberBand);
}

void QChartView_::wheelEvent(QWheelEvent *event)
{
    if (parentWindow->IsScopeTabSelected() == true)
    {
        QPoint numDegrees = event->angleDelta() / 8;

        parentWindow->ScopeAllowAutoScaling(false);

        if(numDegrees.ry() > 0)
        {
            chart()->zoom(2.0);
        }
        else
        {
            chart()->zoom(0.5);
        }
    }

    QChartView::wheelEvent(event);
}

void QChartView_::mousePressEvent(QMouseEvent *event)
{
    if (parentWindow->IsScopeTabSelected() == true && event->buttons() & Qt::MiddleButton)
    {
        QApplication::setOverrideCursor(QCursor(Qt::SizeAllCursor));
        lastMousePos = event->pos();
        event->accept();

        parentWindow->ScopeAllowAutoScaling(false);
    }

    QChartView::mousePressEvent(event);
}

void QChartView_::mouseMoveEvent(QMouseEvent *event)
{
    if (parentWindow->IsScopeTabSelected() == true && event->buttons() & Qt::MiddleButton)
    {
        auto dPos = event->pos() - lastMousePos;
        chart()->scroll(-dPos.x(), dPos.y());
        lastMousePos = event->pos();
        event->accept();
    }

    QChartView::mouseMoveEvent(event);
}

void QChartView_::mouseReleaseEvent(QMouseEvent *event)
{
    if (parentWindow->IsScopeTabSelected() == true)
    {
        QApplication::restoreOverrideCursor();
    }

    QChartView::mouseReleaseEvent(event);
}

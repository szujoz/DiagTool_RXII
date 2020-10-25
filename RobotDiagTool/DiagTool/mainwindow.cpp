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
}

MainWindow::~MainWindow()
{
    delete ui;
    delete lineSeries;
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
    ui->textEdit_SerialTerminal->append(str);
}

void MainWindow::ScopeInit()
{
    lineSeries = new QLineSeries();
    QChart *m_chart = new QChart();
    QChartView *chartView = new QChartView(m_chart);
    chartView->setMinimumSize(200, 100);
    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, 20);
    axisX->setLabelFormat("%g");
    axisX->setTitleText("Time");
    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(-1, 5);
    axisY->setTitleText("Signal value");
    m_chart->addAxis(axisX, Qt::AlignBottom);
    m_chart->addAxis(axisY, Qt::AlignLeft);
    m_chart->legend()->hide();
    m_chart->setTitle("Measurement data from the robot.");
    m_chart->addSeries(lineSeries);
    lineSeries->attachAxis(axisX);
    lineSeries->attachAxis(axisY);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(chartView);

    ui->frame->setLayout(mainLayout);
    ui->frame->show();
}

void MainWindow::DisplayScopeData(QVector<QPointF>& points)
{
    // ToDo: Detect min, max changes
    float max = 0.0;
    for(int i = 0; i < points.size(); i++)
    {
        if(max < points[i].x())
        {
            max = points[i].x();
        }
    }

    lineSeries->replace(points);
}

void MainWindow::on_pushButton_2_clicked()
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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "serialsettingsdialog.h"
#include "inifilehandler.h"
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
    delete settingsWindow;
}


void MainWindow::on_widget_NewDataToDisplay(int i)
{
    QChart *m_chart = new QChart();
    QChartView *chartView = new QChartView(m_chart);
    chartView->setMinimumSize(200, 100);
    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, 2000);
    axisX->setLabelFormat("%g");
    axisX->setTitleText("Samples");
    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(-1, 5);
    axisY->setTitleText("Audio level");
    m_chart->addAxis(axisX, Qt::AlignBottom);
    m_chart->addAxis(axisY, Qt::AlignLeft);
    m_chart->legend()->hide();
    m_chart->setTitle("Data from the microphone ");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(chartView);

    ui->frame->setLayout(mainLayout);
    ui->frame->show();
}

void MainWindow::on_pushButton_2_clicked()
{
    if (ui->groupBox->isHidden())
    {
        ui->groupBox->show();
    }
    else
    {
        ui->groupBox->hide();
    }
}

void MainWindow::on_actionSettings_triggered()
{
    settingsWindow = new SerialSettingsDialog();
    settingsWindow->setAttribute(Qt::WA_QuitOnClose, false);
    settingsWindow->show();

    //connect(&settingsWindow, &SerialSettingsDialog::SettingsChanged,
    //        &IniFileHandler, &IniFileHandler::)
}

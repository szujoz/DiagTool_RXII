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
#include <QTextStream>

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

    connect(scopeAxisX, &QValueAxis::rangeChanged, this, &MainWindow::handle_AxisXRangeChange);
    connect(scopeAxisY, &QValueAxis::rangeChanged, this, &MainWindow::handle_AxisYRangeChange);
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
    lineSeries->setUseOpenGL(true);

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

void MainWindow::on_actionSave_triggered()
{
    QFileDialog dialog(this);
    QString     fileName;
    QStringList fileNames;
    auto measData = lineSeries->pointsVector();

    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilters({"Measurement files (*.txt)",
                           "Any files (*)"
                             });
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setWindowTitle("Save Measurement Log");

    if (dialog.exec())
        fileNames = dialog.selectedFiles();

    if(fileNames.isEmpty() != true)
    {
        fileName = fileNames.first();

        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

        QTextStream out(&file);
        out << "x,y\n";
        foreach(auto point, measData)
        {
            out << point.x() << "," << point.y() << "\n";
        }
    }
}

void MainWindow::on_actionLoad_triggered()
{
    QFileDialog dialog(this);
    QString     fileName;
    QStringList fileNames;
    QVector<QPointF> logData;

    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilters({"Measurement files (*.txt)",
                           "Any files (*)"
                             });
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setWindowTitle("Load Measurement Log");

    if (dialog.exec())
        fileNames = dialog.selectedFiles();

    if(fileNames.isEmpty() != true)
    {
        fileName = fileNames.first();

        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        QTextStream in(&file);
        auto head = in.readLine();
        while (!in.atEnd())
        {
            QString line = in.readLine();
            int comma_pos = line.indexOf(',');
            bool x_conv_ok = false;
            bool y_conv_ok = false;
            qreal x = line.mid(0,comma_pos-1).toDouble(&x_conv_ok);
            qreal y = line.mid(comma_pos+1,line.size()).toDouble(&y_conv_ok);

            if(x_conv_ok && y_conv_ok)
                logData.append(QPoint(x,y));
        }
        lineSeries->replace(logData);
    }
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

void MainWindow::on_btn_ScopeSignalSelectorToggle_clicked()
{
    if (ui->label_ScopeSignalSelector->isHidden())
    {
        ui->label_ScopeSignalSelector->show();
        ui->scrollArea_ScopeSignalSelector->show();
        ui->frame_ScopeCntrl->show();
        ui->btn_ScopeSignalSelectorToggle->setText("<");
    }
    else
    {
        ui->label_ScopeSignalSelector->hide();
        ui->scrollArea_ScopeSignalSelector->hide();
        ui->frame_ScopeCntrl->hide();
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
        ui->btn_ScopeToggleDrawing->setText("Cont");
    }
    else
    {
        allowedToDrawChart = true;
        ui->btn_ScopeToggleDrawing->setText("Stop");
    }
}

void MainWindow::on_btn_ScopeClearData_clicked()
{
    nextDataIndexToBeChecked = 0;
    lineSeries->clear();
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

void MainWindow::on_btn_ScopeYUpperBoundIncr_pressed()
{
    auto y_min = scopeAxisY->min();
    auto y_max = scopeAxisY->max();
    auto y_diff = y_max - y_min;

    scopeAxisY->setMax(y_max + 0.10*y_diff);
    ScopeAllowAutoScaling(false);
}

void MainWindow::on_btn_ScopeYUpperBoundDecr_pressed()
{
    auto y_min = scopeAxisY->min();
    auto y_max = scopeAxisY->max();
    auto y_diff = y_max - y_min;

    scopeAxisY->setMax(y_max - 0.10*y_diff);
    ScopeAllowAutoScaling(false);
}

void MainWindow::on_btn_ScopeYLowerBoundIncr_pressed()
{
    auto y_min = scopeAxisY->min();
    auto y_max = scopeAxisY->max();
    auto y_diff = y_max - y_min;

    scopeAxisY->setMin(y_min - 0.10*y_diff);
    ScopeAllowAutoScaling(false);
}

void MainWindow::on_btn_ScopeYLowerBoundDecr_pressed()
{
    auto y_min = scopeAxisY->min();
    auto y_max = scopeAxisY->max();
    auto y_diff = y_max - y_min;

    scopeAxisY->setMin(y_min + 0.10*y_diff);
    ScopeAllowAutoScaling(false);
}

void MainWindow::on_btn_ScopeXUpperBoundIncr_pressed()
{
    auto x_min = scopeAxisX->min();
    auto x_max = scopeAxisX->max();
    auto x_diff = x_max - x_min;

    scopeAxisX->setMax(x_max + 0.10*x_diff);
    ScopeAllowAutoScaling(false);
}

void MainWindow::on_btn_ScopeXUpperBoundDecr_pressed()
{
    auto x_min = scopeAxisX->min();
    auto x_max = scopeAxisX->max();
    auto x_diff = x_max - x_min;

    scopeAxisX->setMax(x_max - 0.10*x_diff);
    ScopeAllowAutoScaling(false);
}

void MainWindow::on_btn_ScopeXLowerBoundIncr_pressed()
{
    auto x_min = scopeAxisX->min();
    auto x_max = scopeAxisX->max();
    auto x_diff = x_max - x_min;

    scopeAxisX->setMin(x_min - 0.10*x_diff);
    ScopeAllowAutoScaling(false);
}

void MainWindow::on_btn_ScopeXLowerBoundDecr_pressed()
{
    auto x_min = scopeAxisX->min();
    auto x_max = scopeAxisX->max();
    auto x_diff = x_max - x_min;

    scopeAxisX->setMin(x_min + 0.10*x_diff);
    ScopeAllowAutoScaling(false);
}

void MainWindow::on_lineEdit_ScopeYUpperBound_editingFinished()
{
    bool convSuccess = false;
    qreal y_max = ui->lineEdit_ScopeYUpperBound->text().toFloat(&convSuccess);
    if(convSuccess == true)
    {
        scopeAxisY->setMax(y_max);
        ScopeAllowAutoScaling(false);
    }
}

void MainWindow::on_lineEdit_ScopeYLowerBound_editingFinished()
{
    bool convSuccess = false;
    qreal y_min = ui->lineEdit_ScopeYLowerBound->text().toFloat(&convSuccess);
    if(convSuccess == true)
    {
        scopeAxisY->setMin(y_min);
        ScopeAllowAutoScaling(false);
    }
}

void MainWindow::on_lineEdit_ScopeXUpperBound_editingFinished()
{
    bool convSuccess = false;
    qreal x_max = ui->lineEdit_ScopeXUpperBound->text().toFloat(&convSuccess);
    if(convSuccess == true)
    {
        scopeAxisX->setMax(x_max);
        ScopeAllowAutoScaling(false);
    }
}

void MainWindow::on_lineEdit_ScopeXLowerBound_editingFinished()
{
    bool convSuccess = false;
    qreal x_min = ui->lineEdit_ScopeXLowerBound->text().toFloat(&convSuccess);
    if(convSuccess == true)
    {
        scopeAxisX->setMin(x_min);
        ScopeAllowAutoScaling(false);
    }
}

void MainWindow::handle_AxisXRangeChange(qreal min, qreal max)
{
    ui->lineEdit_ScopeXUpperBound->setText(QString::number(max));
    ui->lineEdit_ScopeXLowerBound->setText(QString::number(min));
}

void MainWindow::handle_AxisYRangeChange(qreal min, qreal max)
{
    ui->lineEdit_ScopeYUpperBound->setText(QString::number(max));
    ui->lineEdit_ScopeYLowerBound->setText(QString::number(min));
}

void MainWindow::on_btn_QuickTabDummyDataTx_clicked()
{
    bool convSuccess = false;
    int32_t data = ui->lineEdit_QuickTabDummyDataTx->text().toLong(&convSuccess);

    emit CmdTx_DummyData(data);
}

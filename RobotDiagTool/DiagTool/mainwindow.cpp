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
#include <QDateTime>
#include <QDebug>

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

    lineSeries2 = new QLineSeries();
    scopeChartView->chart()->addSeries(lineSeries2);
    lineSeries2->attachAxis(scopeAxisX);
    lineSeries2->attachAxis(scopeAxisY);

    lineSeries3 = new QLineSeries();
    scopeChartView->chart()->addSeries(lineSeries3);
    lineSeries3->attachAxis(scopeAxisX);
    lineSeries3->attachAxis(scopeAxisY);

    lineSeries4 = new QLineSeries();
    scopeChartView->chart()->addSeries(lineSeries4);
    lineSeries4->attachAxis(scopeAxisX);
    lineSeries4->attachAxis(scopeAxisY);

    lineSeries5 = new QLineSeries();
    scopeChartView->chart()->addSeries(lineSeries5);
    lineSeries5->attachAxis(scopeAxisX);
    lineSeries5->attachAxis(scopeAxisY);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(scopeChartView.get());

    ui->frame->setLayout(mainLayout);
    ui->frame->show();
    ui->checkBox_ScopeDummyData->setChecked(true);

    allowedToDrawChart = true;
    autoScalingOn = true;

    RegisterLineSeries();
    CheckScopeCheckBoxes();

    scopeSignalSelector = std::make_unique<ScopeSignalSelector>(ui, scopeChartView.get(), scopeAxisX, scopeAxisY);

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

void MainWindow::DisplayRemoteChData(const uint8_t ch1, const uint8_t ch2, const uint8_t ch3)
{
    ui->lineEdit_GeneralRemoteCh1->setText(QString(ch1));
    ui->lineEdit_GeneralRemoteCh2->setText(QString(ch2));
    ui->lineEdit_GeneralRemoteCh3->setText(QString(ch3));
}

void MainWindow::DisplayEncoderData(const int32_t speed, const int32_t counter)
{
    ui->lineEdit_QuickTabSpeed->setText(QString(speed));
    ui->lineEdit_GeneralEncoderSpeed->setText(QString(speed));
    ui->lineEdit_GeneralEncoderCounter->setText(QString(counter));
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
    QString     date;
    QString     pref_name;
    auto measData = lineSeries->pointsVector();     // Scope points.

    // Default file name creation. Remove seconds (3 char) from iso time.
    date = QDateTime::currentDateTime().toString(Qt::DateFormat::ISODate);
    date.remove(date.size()-3,3);
    date.replace(':','-');
    pref_name = "meas_log_" + date + ".csv";

    // Configure the dialog.
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilters({"Measurement files (*.csv)",
                           "Any files (*)"
                             });
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setWindowTitle("Save Measurement Log");
    dialog.setDefaultSuffix(".csv");
    dialog.selectFile(pref_name);

    // Start the dialog and block untill it is closed.
    if (dialog.exec())
        fileNames = dialog.selectedFiles();

    // Check if a file was selected.
    if(fileNames.isEmpty() != true)
    {
        fileName = fileNames.first();   // Accept only the first file.

        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

        // Print the log data in csv format with header
        QTextStream out(&file);
        out << "x,y\n";
        foreach(auto point, measData)
        {
            out << point.x() << "," << point.y() << "\n";
        }

        file.close();
    }
    else
    {
        // Error: No file was selected.
    }
}

void MainWindow::on_actionLoad_triggered()
{
    QFileDialog dialog(this);
    QString     fileName;
    QStringList fileNames;
    QVector<QPointF> logData;

    // Configure the dialog.
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilters({"Measurement files (*.csv)",
                           "Any files (*)"
                             });
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setWindowTitle("Load Measurement Log");

    // Start the dialog and block untill it is closed.
    if (dialog.exec())
        fileNames = dialog.selectedFiles();

    // Check if a file was successfully selected.
    if(fileNames.isEmpty() != true)
    {
        fileName = fileNames.first();   // Accept only the first file.

        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;
        // Open the file to read the csv lines and fill up the vector.
        QTextStream in(&file);
        auto head = in.readLine();  // read the head, it cannot be processed.
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

        file.close();
    }
    else
    {
        // Error: No file was selected.
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

void MainWindow::RegisterLineSeries()
{
    int max_signal_number = 5;
    for(int i = 0; i < max_signal_number; i++)
    {
        lineSeriesList.append(new QLineSeries);
    }
}

void MainWindow::CheckScopeCheckBoxes()
{
    QObjectList children = ui->scrollAreaWidgetContents_ScopeSignalSelector->children();

    foreach(auto elem, children)
    {
        QCheckBox* box = qobject_cast<QCheckBox*>(elem);
        if (box != NULL)
        {
            box->setCheckState(Qt::Checked);
        }
    }
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

void MainWindow::on_checkBox_GeneralUiBoardNumberForce_stateChanged(int arg1)
{
    if (ui->checkBox_GeneralUiBoardNumberForce->checkState() == Qt::CheckState::Checked)
    {
        bool successful_conversion = false;
        uint16_t number = ui->lineEdit_GeneralUiBoard7SegOut->text().toUShort(&successful_conversion);
        if (successful_conversion == true)
        {
            if (number > 255u)
                number = 255u;

            emit CmdTx_7SegNum(static_cast<uint8_t>(number));
        }
        else
        {
            qDebug() << "Invalid 7segment number: " << ui->lineEdit_GeneralUiBoard7SegOut->text() << "\n";
        }
    }
}


void MainWindow::on_lineEdit_GeneralUiBoard7SegOut_editingFinished()
{
    if (ui->checkBox_GeneralUiBoardNumberForce->checkState() == Qt::CheckState::Checked)
    {
        bool successful_conversion = false;
        uint16_t number = ui->lineEdit_GeneralUiBoard7SegOut->text().toUShort(&successful_conversion);
        if (successful_conversion == true)
        {
            if (number > 255u)
                number = 255u;

            emit CmdTx_7SegNum(static_cast<uint8_t>(number));
        }
        else
        {
            qDebug() << "Invalid 7segment number: " << ui->lineEdit_GeneralUiBoard7SegOut->text() << "\n";
        }
    }
}

ScopeSignalSelector::ScopeSignalSelector(Ui::MainWindow* ui,
                                         QChartView_* chartview,
                                         QValueAxis* axisX,
                                         QValueAxis* axisY)
{
    this->ui = ui;
    this->chartView = chartview;
    this->axisX = axisX;
    this->axisY = axisY;
}

void ScopeSignalSelector::RegisterLineSignal(const QString name)
{
    SignalInfo item;
    item._name = name;
    item._series = new QLineSeries();
    item._points = new QVector<QPointF>();
    item._visible = false;
    item._chartViewSeriesID = chartView->chart()->series().count();
    signalSeries.append(item);

    SignalInfo* signal = &signalSeries.last();
    AttachSignalToChartview(signal);
    AddCheckboxToUi(name);
}

bool ScopeSignalSelector::UpdateSignalPoints(const QString name, QVector<QPointF>& points)
{
    bool updateSuccess = false;
    bool signalFound = false;
    SignalInfo* signal;

    signalFound = FindSignalByName(name, &signal);
    if (signalFound == true)
    {
        signal->_points = &points;
        signal->_series->replace(*(signal->_points));
        updateSuccess = true;
    }

    return updateSuccess;
}

void ScopeSignalSelector::AttachSignalToChartview(SignalInfo* signal)
{
    // if not NULL
    chartView->chart()->addSeries(signal->_series);
    signal->_series->attachAxis(axisX);
    signal->_series->attachAxis(axisY);
}

bool ScopeSignalSelector::FindSignalByName(QString const name, SignalInfo** signal)
{
    bool signalFound = false;

    for (int i = 0; i < signalSeries.count(); i++)
    {
        if(signalSeries[i]._name == name)
        {
            *signal = &signalSeries[i];
            signalFound = true;
            break;
        }
    }

    return signalFound;
}

QString ScopeSignalSelector::GetUiItemNameBySignalName(const QString name)
{
    return "checkBox_Scope" + name;
}

QString ScopeSignalSelector::GetSignalNameByUiItemName(const QString itemName)
{
    QString name = itemName.mid(14, itemName.size()-14);
    return name;
}

void ScopeSignalSelector::AddCheckboxToUi(const QString name)
{
    QCheckBox* box = new QCheckBox();
    box->setObjectName(GetUiItemNameBySignalName(name));
    box->setText(name);
    ui->scrollAreaWidgetContents_ScopeSignalSelector->layout()->addWidget(box);

    connect(box, &QCheckBox::stateChanged, this, &ScopeSignalSelector::StateChanged);
}

void ScopeSignalSelector::StateChanged(int state)
{
    bool drawAllowed = false;
    QString name = GetSignalNameByUiItemName(QObject::sender()->objectName());
    SignalInfo* signal;
    FindSignalByName(name, &signal);

    if (state == 0) // means unchecked
    {
        chartView->chart()->series().at(signal->_chartViewSeriesID)->hide();
    }
    else if (state == 2) // means checked
    {
        drawAllowed = true;
        chartView->chart()->series().at(signal->_chartViewSeriesID)->show();
    }
    else
    {
        // Error, invalid state.
    }

    signal->_visible = drawAllowed;
    emit SignalToBeDisplayed(name, drawAllowed);
}

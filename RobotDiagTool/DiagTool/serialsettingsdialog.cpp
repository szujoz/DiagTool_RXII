#include "serialsettingsdialog.h"
#include "ui_serialsettingsdialog.h"
#include "inifilehandler.h"
#include <QDebug>

SerialSettingsDialog::SerialSettingsDialog(QDialog *parent)
    : QDialog(parent)
    , ui(new Ui::SerialSettingsDialog)
{
    ui->setupUi(this);
}

SerialSettingsDialog::~SerialSettingsDialog()
{
    delete ui;
}

void SerialSettingsDialog::DisplaySerialSettings(const QString com, const QString baud, const QString dataBits, const QString stopBits)
{
    ui->lineEdit_ComChannel->insert(com);
    ui->lineEdit_Baud->insert(baud);
    ui->lineEdit_DataBits->insert(dataBits);
    ui->lineEdit_StopBits->insert(stopBits);
}

void SerialSettingsDialog::on_btn_SaveConfigSerialLineSettings_clicked()
{
    QMap<QString,QString> params;
    params.insert("COM",    ui->lineEdit_ComChannel->text());
    params.insert("baud",   ui->lineEdit_Baud->text());
    params.insert("dataBit",ui->lineEdit_DataBits->text());
    params.insert("stopBit",ui->lineEdit_StopBits->text());

    emit SettingsChanged(ui->lineEdit_ComChannel->text(),
                         ui->lineEdit_Baud->text(),
                         ui->lineEdit_DataBits->text(),
                         ui->lineEdit_StopBits->text());

    qDebug() << "Settings saved";
}

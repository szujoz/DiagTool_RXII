#ifndef SERIALSETTINGSDIALOG_H
#define SERIALSETTINGSDIALOG_H

#include <QDialog>
#include <QMap>

namespace Ui {
class SerialSettingsDialog;
}

class SerialSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SerialSettingsDialog(QDialog *parent = 0);
    ~SerialSettingsDialog();

    void DisplaySerialSettings(QString const com,
                               QString const baud,
                               QString const dataBits,
                               QString const stopBits);

signals:
    void SettingsChanged(QString const com,
                         QString const baud,
                         QString const dataBits,
                         QString const stopBits);

private slots:
    void on_btn_SaveConfigSerialLineSettings_clicked();

private:
    Ui::SerialSettingsDialog *ui;
};

#endif // SERIALSETTINGSDIALOG_H

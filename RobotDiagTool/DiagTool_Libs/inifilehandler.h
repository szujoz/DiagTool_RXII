#ifndef INIHANDLER_H
#define INIHANDLER_H

#include <QObject>
#include <memory>
#include <QSettings>
#include <QMap>

#include "debugtrace.h"

class IniFileHandler : public QObject
{
    Q_OBJECT

public:
    IniFileHandler(QString const path);
    ~IniFileHandler() = default;

    void SaveSettings();
    void LoadSettings();

    QMap<QString,QString> GetSettings();
    QString GetValue(QString const key);
    void SetValue(QString const key, QString const value);

public slots:
    void SettingsArrived(QMap<QString,QString> params);

private:
    std::unique_ptr<QSettings> settings;
    QString iniPath;

    DebugTrace* trace;
};

#endif // INIHANDLER_H

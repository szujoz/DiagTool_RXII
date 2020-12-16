#include "inifilehandler.h"

IniFileHandler::IniFileHandler(QString const path)
{
    settings = std::make_unique<QSettings>(path + "/serial_line.ini", QSettings::IniFormat);
    settings->sync();
    trace->GetInstance();
}

void IniFileHandler::SaveSettings()
{
    settings->sync();
    trace->Trace("Serial setting saved to ini file (/serial_line.ini)\n");
}

void IniFileHandler::LoadSettings()
{
    settings->sync();
    trace->Trace("Serial settings are loaded from ini file (/serial_line.ini)\n");
}

QMap<QString, QString> IniFileHandler::GetSettings()
{
    QMap<QString,QString> settMap;
    QStringList keyList;

    keyList = settings->allKeys();

    foreach(QString key, keyList)
    {
        settMap.insert(key,settings->value(key).toString());
    }

    return settMap;
}

QString IniFileHandler::GetValue(const QString key)
{
    return settings->value(key).toString();
}

void IniFileHandler::SetValue(const QString key, QString const value)
{
    settings->setValue(key, value);
}

void IniFileHandler::SettingsArrived(QMap<QString,QString> params)
{
    foreach(QString elem, params.keys())
    {
        SetValue(elem, params.value(elem));
    }
}


#ifndef ROBOTCOMMAND_H
#define ROBOTCOMMAND_H

#include <QObject>
#include "irobotcommand.h"
#include "commandidconfig.h"

class RobotCommand_Text : public IRobotCommand
{
    Q_OBJECT
public:
    RobotCommand_Text() {};

    virtual void RxProcessing(QByteArray const &message) override;

signals:
    void CmdArrived(QString const txt);
};

class RobotCommand_DummyData : public IRobotCommand
{
    Q_OBJECT
public:
    RobotCommand_DummyData() {};

    virtual void RxProcessing(QByteArray const &message) override;
    QByteArray CmdTxProcess(uint32_t const timestamp, int32_t const data);

signals:
    void CmdArrived(uint32_t const timestamp, int32_t const data);
    void ReadyToPack(CommandID const cmdID, QByteArray const &message);
};

#endif // ROBOTCOMMAND_H

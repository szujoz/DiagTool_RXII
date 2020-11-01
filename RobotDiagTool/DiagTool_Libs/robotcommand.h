#ifndef ROBOTCOMMAND_H
#define ROBOTCOMMAND_H

#include <QObject>
#include "irobotcommand.h"

class RobotCommand_Text : public IRobotCommand
{
    Q_OBJECT
public:
    RobotCommand_Text() {};

    virtual void operation(QByteArray const &message) override;

signals:
    void CmdArrived_Text(QString const txt);
};

class RobotCommand_DummyData : public IRobotCommand
{
    Q_OBJECT
public:
    RobotCommand_DummyData() {};

    virtual void operation(QByteArray const &message) override;

signals:

    void CmdArrived_DummyData(uint32_t const timestamp, uint32_t const data);
};

#endif // ROBOTCOMMAND_H

#ifndef IROBOTCOMMAND_H
#define IROBOTCOMMAND_H

#include <QObject>

#include "commandidconfig.h"

class IRobotCommand : public QObject
{
    Q_OBJECT
public:
    IRobotCommand() {};

    virtual CommandID GetCommandId() = 0;

    virtual bool IsIdMatch(QByteArray const &message) = 0;

    virtual void RxProcessing(QByteArray const &message) = 0;
};

#endif // IROBOTCOMMAND_H

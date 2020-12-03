#ifndef ICOMMANDPACKER_H
#define ICOMMANDPACKER_H

#include <QObject>
#include <QByteArray>

#include "commandidconfig.h"
#include "irobotcommand.h"

class ICommandPacker : public QObject
{
    Q_OBJECT
public:
    ICommandPacker() {};

    virtual bool RegisterCommand(IRobotCommand* command, QString const name) = 0;
    virtual IRobotCommand* GetCommand(bool* found, QString const name) = 0;
    virtual QByteArray Pack(QByteArray& message) = 0;
    virtual void Unpack(QByteArray& message) = 0;
};

#endif // ICOMMANDPACKER_H

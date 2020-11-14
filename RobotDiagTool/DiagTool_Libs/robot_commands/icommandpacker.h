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

    virtual void RegisterCommand(CommandID const id, IRobotCommand* cmd) = 0;
    virtual QByteArray Pack(QByteArray& message) = 0;
    virtual void Unpack(QByteArray& message) = 0;
};

#endif // ICOMMANDPACKER_H

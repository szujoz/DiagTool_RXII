#ifndef COMMANDPACKER_H
#define COMMANDPACKER_H

#include <QObject>
#include <memory>

#include "icommandpacker.h"
#include "irobotcommand.h"

#include "crc8.h"
#include "BinaryEncoder.h"
#include "commandfactory.h"

class CommandPacker : public ICommandPacker
{
    Q_OBJECT
public:
    static CommandPacker *GetInstance();

    virtual void RegisterCommand(CommandID const id, IRobotCommand* cmd) override;
    virtual QByteArray Pack(QByteArray& message) override;
    virtual void Unpack(QByteArray& message) override;

private:
    static CommandPacker* instance_;
    CommandFactory factory;
    std::unique_ptr<BinaryEncoder> coder;
    std::unique_ptr<Crc8> crc;

    CommandPacker();
};

#endif // COMMANDPACKER_H

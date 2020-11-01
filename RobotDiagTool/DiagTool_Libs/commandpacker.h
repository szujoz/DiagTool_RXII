#ifndef COMMANDPACKER_H
#define COMMANDPACKER_H

#include <QObject>
#include <memory>
#include <QMap>

#include "icommandpacker.h"
#include "irobotcommand.h"

#include "crc8.h"
#include "BinaryEncoder.h"

class CommandFactory
{
public:
    CommandFactory();

    void RegisterCommand(CommandID const id, IRobotCommand* cmd);

    IRobotCommand* CreateCommand(CommandID const id);

private:
    std::unique_ptr<QMap<CommandID,IRobotCommand*>> registeredCommands;
};

class CommandPacker : public ICommandPacker
{
    Q_OBJECT
public:
    static CommandPacker *GetInstance();

    virtual void RegisterCommand(CommandID const id, IRobotCommand* cmd) override;
    virtual QByteArray Pack(CommandID const cmdID, QByteArray& message) override;
    virtual void Unpack(QByteArray& message) override;

private:
    static CommandPacker* instance_;
    CommandFactory factory;
    std::unique_ptr<BinaryEncoder> coder;
    std::unique_ptr<Crc8> crc;

    CommandPacker();
};

#endif // COMMANDPACKER_H

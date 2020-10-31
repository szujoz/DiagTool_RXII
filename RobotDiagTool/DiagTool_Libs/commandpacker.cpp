#include "commandpacker.h"
#include "robotcommand.h"

CommandPacker* CommandPacker::instance_ = NULL;

CommandFactory::CommandFactory()
{
    registeredCommands = std::make_unique<QMap<CommandID,IRobotCommand*>>();
}

void CommandFactory::RegisterCommand(const CommandID id, IRobotCommand* cmd)
{
    registeredCommands.get()->insert(id, cmd);
}

IRobotCommand* CommandFactory::CreateCommand(const CommandID id)
{
    return registeredCommands.get()->value(id);
}

CommandPacker *CommandPacker::GetInstance()
{
    if(instance_ == NULL)
    {
        instance_ = new CommandPacker();
    }

    return instance_;
}

void CommandPacker::RegisterCommand(const CommandID id, IRobotCommand *cmd)
{
    factory.RegisterCommand(id, cmd);
}

QByteArray CommandPacker::Pack(const CommandID cmdID, QByteArray &message)
{
    IRobotCommand* cmd = factory.CreateCommand(cmdID);
    Q_UNUSED(message);
    Q_UNUSED(cmdID);
    //return cmd.
}

void CommandPacker::Unpack(QByteArray &message)
{
    // Extract the command ID and get the object from the factory.
    CommandID cmdId = static_cast<CommandID>(message[0] - 0x30);
    IRobotCommand* cmd = factory.CreateCommand(cmdId);

    // Command object was found. Remove the Command ID byte and process the bytes.
    if(cmd != NULL)
    {
        message.remove(0,1);
        cmd->operation(message);
    }
}

CommandPacker::CommandPacker() {}

CommandPacker::~CommandPacker() {}




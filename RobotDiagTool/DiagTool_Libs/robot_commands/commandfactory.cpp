#include "commandfactory.h"

CommandFactory::CommandFactory()
{
    registeredCommands = std::make_unique<QMap<CommandID,IRobotCommand*>>();
}

void CommandFactory::RegisterCommand(const CommandID id, IRobotCommand* cmd)
{
    registeredCommands->insert(id, cmd);
}

IRobotCommand* CommandFactory::CreateCommand(const CommandID id)
{
    return registeredCommands->value(id, NULL);
}

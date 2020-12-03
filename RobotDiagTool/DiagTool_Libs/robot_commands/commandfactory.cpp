#include "commandfactory.h"

CommandFactory::CommandFactory()
{
    registeredCommands.clear();
}

bool CommandFactory::RegisterCommand(IRobotCommand *command, const QString name)
{
    CommandInfo cInfo;
    bool isRegistered = false;
    bool isAlreadyIn  = false;

    FindCommandByNameInRegisteredCommands(&isAlreadyIn, name);

    if(isAlreadyIn == false)
    {
        cInfo.name    = name;
        cInfo.command = command;
        registeredCommands.push_back(cInfo);
        isRegistered = true;
    }

    return isRegistered;
}

IRobotCommand* CommandFactory::CreateCommand(bool *success, const QByteArray message)
{
    return FindCommandThatRecogniseTheMessageIDs(success, message);
}

IRobotCommand* CommandFactory::CreateCommand(bool* success, const QString name)
{
    return FindCommandByNameInRegisteredCommands(success, name);
}

IRobotCommand* CommandFactory::FindCommandByNameInRegisteredCommands(bool* found, const QString name)
{
    IRobotCommand* command = NULL;
    *found = false;

    for(size_t i = 0; i < registeredCommands.size(); i++)
    {
        if(registeredCommands[i].name == name)
        {
            *found = true;
            command = registeredCommands[i].command;
            break;
        }
    }

    return command;
}

IRobotCommand* CommandFactory::FindCommandThatRecogniseTheMessageIDs(bool *found, const QByteArray message)
{
    IRobotCommand* command = NULL;
    *found = false;

    for(size_t i = 0; i < registeredCommands.size(); i++)
    {
        if(registeredCommands[i].command->IsIdMatch(message))
        {
            *found = true;
            command = registeredCommands[i].command;
            break;
        }
    }

    return command;
}

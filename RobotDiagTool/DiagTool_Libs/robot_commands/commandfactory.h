#ifndef COMMANDFACTORY_H
#define COMMANDFACTORY_H

#include <memory>
#include <QMap>

#include "commandidconfig.h"
#include "irobotcommand.h"

class CommandFactory
{
public:
    CommandFactory();

    void RegisterCommand(CommandID const id, IRobotCommand* cmd);

    IRobotCommand* CreateCommand(CommandID const id);

private:
    std::unique_ptr<QMap<CommandID,IRobotCommand*>> registeredCommands;
};

#endif // COMMANDFACTORY_H

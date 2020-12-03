#ifndef COMMANDFACTORY_H
#define COMMANDFACTORY_H

#include "irobotcommand.h"

typedef struct
{
    QString        name;       // Name of the command, must be unique.
    IRobotCommand* command;
} CommandInfo;

class CommandFactory
{
public:
    CommandFactory();

    bool RegisterCommand(IRobotCommand* command, QString const name);

    IRobotCommand* CreateCommand(bool* success, QByteArray const message);
    IRobotCommand* CreateCommand(bool* success, QString    const name);

private:
    std::vector<CommandInfo> registeredCommands;

    IRobotCommand* FindCommandByNameInRegisteredCommands(bool* found, QString    const name);
    IRobotCommand* FindCommandThatRecogniseTheMessageIDs(bool* found, QByteArray const message);
};

#endif // COMMANDFACTORY_H

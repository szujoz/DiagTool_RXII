#include "commandpacker.h"
#include "robotcommand.h"
#include "EscapeEncoder.h"

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
    CommandID            cmdId = CommandID::eUnknown;
    IRobotCommand*       cmd;
    std::vector<uint8_t> coded_message;
    size_t               coded_size = 0;
    std::vector<uint8_t> decoded_message;
    size_t               decoded_size = 0;
    bool                 isCrcOk = false;
    QByteArray           messageToBeProcessed;

    // Determine array sizes
    coded_size = message.size();
    decoded_size = coded_size;      // The decoded bytes will be surely shorter.

    for (size_t i=0; i < coded_size; i++)
    {
        coded_message.push_back(static_cast<uint8_t>(message[i]));
    }
    decoded_message.reserve(decoded_size);

    // Decode the message.
    coder.get()->Decode(coded_message.data(), coded_size, decoded_message.data(), decoded_size);

    // Check CRC
    isCrcOk = crc.get()->CheckBlockCrc(decoded_message.data(), decoded_size);

    if(isCrcOk == true)
    {
        // Extract the command ID and get the object from the factory.
        cmdId = static_cast<CommandID>(message[0] - 0x00);
        cmd = factory.CreateCommand(cmdId);

        for(size_t i = 0; i < decoded_size; i++)
        {
           messageToBeProcessed.append(decoded_message[i]);
        }
        messageToBeProcessed.remove(0,1);
        messageToBeProcessed.remove(messageToBeProcessed.size()-1,1);

        // Command object was found. Remove the Command ID byte and process the bytes.
        if(cmd != NULL)
        {
            cmd->operation(messageToBeProcessed);
        }
    }
}

CommandPacker::CommandPacker()
{
    coder = std::make_unique<EscapeEncoder>();
    crc = std::make_unique<Crc8>();
}




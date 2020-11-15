#include "commandpacker.h"
#include "robotcommand.h"
#include "EscapeEncoder.h"
#include <QtDebug>

CommandPacker* CommandPacker::instance_ = NULL;

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

QByteArray CommandPacker::Pack(QByteArray &command_bytes)
{
    QByteArray           command = command_bytes;
    QByteArray           message;
    std::vector<uint8_t> commandBytes;
    std::vector<uint8_t> encodedBytes;
    uint8_t              crcByte;

    // Step 1: Add CRC
    commandBytes = Conv_QByteArray_To_U8Vector(command);
    crcByte = crc->CalculateCrC(commandBytes);
    commandBytes.push_back(crcByte);

    // Step 2: Encode
    encodedBytes = coder->Encode(commandBytes);

    // Step 3: append '\n'
    encodedBytes.push_back('\n');
    message = Conv_U8Vector_To_QByteArray(encodedBytes);

    return message;
}

void CommandPacker::Unpack(QByteArray &message)
{
    IRobotCommand*       cmdObj = NULL;
    QByteArray           codedMessage = message;
    QByteArray           decodedMessage;
    QByteArray           payload;
    std::vector<uint8_t> codedBytes;
    std::vector<uint8_t> decodedBytes;
    bool                 successfulExtraction = false;

    // Step 1: Decode
    codedBytes = Conv_QByteArray_To_U8Vector(codedMessage);
    decodedBytes = coder->Decode(codedBytes);
    decodedMessage = Conv_U8Vector_To_QByteArray(decodedBytes);

    // Step 2: Check CRC
    if(crc->IsCrcValid(decodedBytes) == true)
    {
        payload = ExtractPayload(&successfulExtraction, &cmdObj, decodedMessage);

        if (successfulExtraction == true)
        {
           cmdObj->RxProcessing(payload);
        }
    }
    else
    {
        qDebug() << "Message crc error: " << QString(decodedMessage.toHex(' '));
    }
}

CommandPacker::CommandPacker()
{
    coder = std::make_unique<EscapeEncoder_F>();
    crc = std::make_unique<CRC8_F>();
}

std::vector<uint8_t> CommandPacker::Conv_QByteArray_To_U8Vector(QByteArray& bytes)
{
    std::vector<uint8_t> vector;
    QByteArray byteArray = bytes;

    for (int i = 0; i < byteArray.size(); i++)
    {
        vector.push_back(static_cast<uint8_t>(byteArray.at(i)));
    }

    return vector;
}

QByteArray CommandPacker::Conv_U8Vector_To_QByteArray(std::vector<uint8_t>& vector)
{
    std::vector<uint8_t> _vector = vector;
    QByteArray byteArray;

    for(size_t i = 0; i < _vector.size(); i++)
    {
       byteArray.append(_vector[i]);
    }

    return byteArray;
}

QByteArray CommandPacker::ExtractPayload(bool* success, IRobotCommand** cmd, QByteArray const bytes)
{
    QByteArray     commandBytes = bytes;
    CommandID      commandId = CommandID::eUnknownCommand;

    // Extract the command ID and get the object from the factory.
    commandId = static_cast<CommandID>(commandBytes[0] - 0x00);
    *cmd = factory.CreateCommand(commandId);

    if(*cmd != NULL)
    {
        *success = true;

        // Remove the Command ID and the CRC bytes.
        commandBytes.remove(0,1);
        commandBytes.remove(commandBytes.size()-1,1);
    }
    else
    {
        *success = false;
        qDebug() << "Unknown Command ID: " << commandId;
    }

    return commandBytes;
}

CRC8_F::CRC8_F()
{
    crc = std::make_unique<Crc8>();
}

uint8_t CRC8_F::CalculateCrC(std::vector<uint8_t> const crc_bytes)
{
    uint8_t calculated_crc = 0;

    calculated_crc = crc->GetBlockCrc(crc_bytes.data(), crc_bytes.size());

    return calculated_crc;
}

bool CRC8_F::IsCrcValid(std::vector<uint8_t> const crc_bytes)
{
    bool isCrcOk = false;

    isCrcOk = crc->CheckBlockCrc(crc_bytes.data(), crc_bytes.size());

    return isCrcOk;
}

EscapeEncoder_F::EscapeEncoder_F()
{
    coder = std::make_unique<EscapeEncoder>();
}

std::vector<uint8_t> EscapeEncoder_F::Encode(const std::vector<uint8_t> bytes)
{
    std::vector<uint8_t> originalBytes = bytes;
    std::vector<uint8_t> encodedBytes;
    uint8_t encodedArray[originalBytes.size()*2]; // Reserve twice as many bytes to even store all as escaped characters.
    size_t encodedLenght = 0;

    coder->Encode(originalBytes.data(), originalBytes.size(), encodedArray, encodedLenght);

    encodedBytes = std::vector<uint8_t>(&encodedArray[0], &encodedArray[encodedLenght]);

    return encodedBytes;
}

std::vector<uint8_t> EscapeEncoder_F::Decode(const std::vector<uint8_t> encoded_bytes)
{
    std::vector<uint8_t> encodedBytes = encoded_bytes;
    std::vector<uint8_t> decodedBytes;
    uint8_t decodedArray[encodedBytes.size()];  // Reserve the same number of bytes, the decoded message less or equal in size.
    size_t decodedLenght = 0;

    coder->Decode(encodedBytes.data(), encodedBytes.size(), decodedArray, decodedLenght);

    decodedBytes = std::vector<uint8_t>(&decodedArray[0], &decodedArray[decodedLenght]);

    return decodedBytes;
}

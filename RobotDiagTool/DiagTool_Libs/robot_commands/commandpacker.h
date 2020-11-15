#ifndef COMMANDPACKER_H
#define COMMANDPACKER_H

#include <QObject>
#include <memory>

#include "icommandpacker.h"
#include "irobotcommand.h"

#include "crc8.h"
#include "BinaryEncoder.h"
#include "commandfactory.h"

class CRC8_F;
class EscapeEncoder_F;

class CommandPacker : public ICommandPacker
{
    Q_OBJECT
public:
    static CommandPacker *GetInstance();

    virtual void RegisterCommand(CommandID const id, IRobotCommand* cmd) override;
    virtual QByteArray Pack(QByteArray& command_bytes) override;
    virtual void Unpack(QByteArray& message) override;

private:
    static CommandPacker* instance_;
    CommandFactory factory;
    std::unique_ptr<EscapeEncoder_F> coder;
    std::unique_ptr<CRC8_F> crc;

    CommandPacker();

    std::vector<uint8_t> Conv_QByteArray_To_U8Vector(QByteArray& bytes);
    QByteArray Conv_U8Vector_To_QByteArray(std::vector<uint8_t>& vector);

    QByteArray ExtractPayload(bool* success, IRobotCommand** cmd, QByteArray const bytes);
};

class CRC8_F  // Facade for crc8.h funcitons.
{
public:
    CRC8_F();

    uint8_t CalculateCrC(std::vector<uint8_t> const crc_bytes);
    bool IsCrcValid(std::vector<uint8_t> const crc_bytes);

private:
    std::unique_ptr<Crc8> crc;
};

class EscapeEncoder_F // Facade for EscapeEncoder.h functions
{
public:
    EscapeEncoder_F();

    std::vector<uint8_t> Encode(std::vector<uint8_t> const bytes);
    std::vector<uint8_t> Decode(std::vector<uint8_t> const encoded_bytes);

private:
    std::unique_ptr<BinaryEncoder> coder;
};

#endif // COMMANDPACKER_H

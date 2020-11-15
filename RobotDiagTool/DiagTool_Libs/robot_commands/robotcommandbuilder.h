#ifndef ROBOTCOMMANDBUILDER_H
#define ROBOTCOMMANDBUILDER_H

#include <QDataStream>

#include "icommandbuilder.h"
#include "irobotcommand.h"

class RobotCommandBuilder : public ICommandBuilder
{
    Q_OBJECT
public:
    RobotCommandBuilder();

    virtual void set_uint8_data(uint8_t byte) override;
    virtual void set_uint32_data(uint32_t ulong) override;
    virtual void set_int32_data(int32_t _long) override;
    virtual void set_text_data(QString str) override;

    void reset();
    QByteArray getProduct();
    template<class T>
    QByteArray serializer(T data)
    {
        QByteArray  bytes;
        QDataStream stream(&bytes,QIODevice::ReadWrite);
        stream.setByteOrder(QDataStream::LittleEndian);
        stream << data;
        return bytes;
    }
private:
    IRobotCommand* command;
    QByteArray command_bytes;

};

#endif // ROBOTCOMMANDBUILDER_H

#include "robotcommand.h"
#include <QDataStream>

void RobotCommand_Text::operation(QByteArray const &message)
{
    QString str(message);
    emit CmdArrived_Text(str);
}

void RobotCommand_DummyData::operation(QByteArray const &message)
{
    uint32_t    ts = 0;
    QByteArray  ts_bytes = message.mid(0,4);
    QDataStream ts_stream(ts_bytes);
    ts_stream.setByteOrder(QDataStream::LittleEndian);
    ts_stream >> ts;

    int32_t     data = 0;
    QByteArray  data_bytes = message.mid(4,4);
    QDataStream data_stream(data_bytes);
    data_stream.setByteOrder(QDataStream::LittleEndian);
    data_stream >> data;

    emit CmdArrived_DummyData(ts, data);
}

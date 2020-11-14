#include "robotcommand.h"
#include <QDataStream>

void RobotCommand_Text::RxProcessing(QByteArray const &message)
{
    QString str(message);
    emit CmdArrived(str);
}

void RobotCommand_DummyData::RxProcessing(QByteArray const &message)
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

    emit CmdArrived(ts, data);
}

QByteArray RobotCommand_DummyData::CmdTxProcess(const uint32_t timestamp, const int32_t data)
{
    QByteArray  ts_bytes;
    QDataStream ts_stream(ts_bytes);
    ts_stream.setByteOrder(QDataStream::LittleEndian);
    ts_stream << timestamp;

    QByteArray  data_bytes;
    QDataStream data_stream(ts_bytes);
    data_stream.setByteOrder(QDataStream::LittleEndian);
    data_stream << data;

    emit ReadyToPack(CommandID::eDummyData, ts_bytes + data_bytes);
    return ts_bytes + data_bytes;
}


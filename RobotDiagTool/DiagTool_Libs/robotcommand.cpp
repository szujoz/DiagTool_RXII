#include "robotcommand.h"

void RobotCommand_Text::operation(QByteArray const &message)
{
    QString str(message);
    emit CmdArrived_Text(str);
}

void RobotCommand_DummyData::operation(QByteArray const &message)
{

}

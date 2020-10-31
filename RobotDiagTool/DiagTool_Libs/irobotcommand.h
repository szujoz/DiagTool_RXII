#ifndef IROBOTCOMMAND_H
#define IROBOTCOMMAND_H

#include <QObject>

class IRobotCommand : public QObject
{
    Q_OBJECT
public:
    IRobotCommand() {};

    virtual void operation(QByteArray const &message) = 0;
};

#endif // IROBOTCOMMAND_H

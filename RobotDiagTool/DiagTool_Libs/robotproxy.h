#ifndef ROBOTPROXY_H
#define ROBOTPROXY_H

#include <QObject>

#include "encoder.h"
#include "remote.h"

class RobotProxy : public QObject
{
    Q_OBJECT
public:
    RobotProxy();

signals:

private:
    Encoder encoder;
    Remote remote;

};

#endif // ROBOTPROXY_H

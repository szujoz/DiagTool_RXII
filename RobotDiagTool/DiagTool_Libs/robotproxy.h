#ifndef ROBOTPROXY_H
#define ROBOTPROXY_H

#include <QObject>

#include "encoder.h"
#include "remote.h"
#include "sevensegui.h"

class RobotProxy : public QObject
{
    Q_OBJECT
public:
    RobotProxy();

    Encoder encoder;
    Remote remote;
    SevenSegUi SevenSeg;

signals:
};

#endif // ROBOTPROXY_H

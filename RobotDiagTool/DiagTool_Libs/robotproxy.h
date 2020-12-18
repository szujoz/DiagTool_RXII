#ifndef ROBOTPROXY_H
#define ROBOTPROXY_H

#include <QObject>

#include "debugtrace.h"

#include "encoder.h"
#include "remote.h"
#include "sevensegui.h"
#include "distancesensor.h"
#include "speedcontroller.h"

class RobotProxy : public QObject
{
    Q_OBJECT
public:
    RobotProxy();

    DebugTrace* trace;

    Encoder encoder;
    Remote remote;
    SevenSegUi sevenSeg;
    DistanceSensor frontDist;
    SpeedController spdController;

    void EmptyAllBuffers();

signals:
};

#endif // ROBOTPROXY_H

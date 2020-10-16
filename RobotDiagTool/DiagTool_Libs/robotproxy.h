#ifndef ROBOTPROXY_H
#define ROBOTPROXY_H

#include <QObject>

class RobotProxy : public QObject
{
    Q_OBJECT
public:
    RobotProxy();

    void Cmd_Stop();
    void Cmd_DriveDistance(uint32_t const dist_milimeter);
    uint32_t Cmd_GetFrontDistance_mm();

signals:

};

#endif // ROBOTPROXY_H

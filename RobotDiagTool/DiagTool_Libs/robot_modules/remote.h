#ifndef REMOTE_H
#define REMOTE_H

#include "irobotmodule.h"

class Remote : public IRobotModule
{
    Q_OBJECT
public:
    Remote();

    virtual QList<QList<QPointF>> GetAllSeries() override;

private:
    QList<QPointF> ch1;
    QList<QPointF> ch2;
    QList<QPointF> ch3;
};

#endif // REMOTE_H

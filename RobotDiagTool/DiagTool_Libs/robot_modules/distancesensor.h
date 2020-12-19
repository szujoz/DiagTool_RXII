#ifndef DISTANCESENSOR_H
#define DISTANCESENSOR_H

#include "irobotmodule.h"

class DistanceSensor : public IRobotModule
{
    Q_OBJECT
public:
    virtual QList<QList<QPointF>> GetAllSeries() override;

    virtual void EmptyAllLists() override;

    void SetFrontDistance(uint32_t const timestamp, int32_t const frontDistance);

signals:

private:
    QList<QPointF> frontDistanceList;
};

#endif // DISTANCESENSOR_H

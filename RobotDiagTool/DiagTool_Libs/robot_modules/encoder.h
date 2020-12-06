#ifndef ENCODER_H
#define ENCODER_H

#include "irobotmodule.h"

class Encoder : public IRobotModule
{
public:
    Encoder();

    virtual QList<QList<QPointF>> GetAllSeries() override;

    virtual void EmptyAllLists() override;

    void SetSpeed(float const v, uint32_t const t);
    void SetDistance(float const s, uint32_t const t);

signals:
    void DataUpdated_Speed(QList<QPointF>& speeds);
    void DataUpdated_Distance(QList<QPointF>& dists);

private:
    QList<QPointF> speedList;
    QList<QPointF> distanceList;
};

#endif // ENCODER_H

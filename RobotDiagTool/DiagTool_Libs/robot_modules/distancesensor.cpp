#include "distancesensor.h"


QList<QList<QPointF> > DistanceSensor::GetAllSeries()
{
    QList<QList<QPointF>> series;
    series.append(frontDistanceList);

    return series;
}

void DistanceSensor::EmptyAllLists()
{
    frontDistanceList.clear();
}

void DistanceSensor::SetFrontDistance(const uint32_t timestamp, const int32_t frontDistance)
{
    frontDistanceList.push_back(QPointF(timestamp, frontDistance));
}

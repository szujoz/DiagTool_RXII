#include "encoder.h"

Encoder::Encoder()
{
    EmptyAllLists();
}

QList<QList<QPointF> > Encoder::GetAllSeries()
{
    QList<QList<QPointF>> series;
    series.append(speedList);
    series.append(distanceList);

    return series;
}

void Encoder::EmptyAllLists()
{
    speedList.clear();
    distanceList.clear();
}

void Encoder::SetSpeed(const float v, const uint32_t t)
{
    speedList.push_back(QPointF((float)t, v));
}

void Encoder::SetDistance(const float s, const uint32_t t)
{
    distanceList.push_back(QPointF((float)t, s));
}

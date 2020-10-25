#include "encoder.h"

Encoder::Encoder()
{
    speedList.clear();
    distanceList.clear();
}

void Encoder::SetSpeed(const float v, const uint32_t t)
{
    speedList.push_back(QPointF(v, (float)t));
}

void Encoder::SetDistance(const float s, const uint32_t t)
{
    distanceList.push_back(QPointF(s, (float)t));
}

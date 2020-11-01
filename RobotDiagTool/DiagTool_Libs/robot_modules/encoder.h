#ifndef ENCODER_H
#define ENCODER_H

#include <QObject>

#include <QPointF>

class Encoder
{
public:
    Encoder();

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

#include "remote.h"

Remote::Remote()
{
    ch1.clear();
    ch2.clear();
    ch3.clear();
}

QList<QList<QPointF> > Remote::GetAllSeries()
{
    QList<QList<QPointF>> series;
    series.append(ch1);
    series.append(ch2);
    series.append(ch3);

    return series;
}

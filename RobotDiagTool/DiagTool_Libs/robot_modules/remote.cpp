#include "remote.h"

Remote::Remote()
{
    EmptyAllLists();
}

QList<QList<QPointF> > Remote::GetAllSeries()
{
    QList<QList<QPointF>> series;
    series.append(ch1List);
    series.append(ch2List);
    series.append(ch3List);

    return series;
}

void Remote::EmptyAllLists()
{
    ch1List.clear();
    ch2List.clear();
    ch3List.clear();
}

void Remote::SetCh1(const uint32_t time, const int8_t ch1)
{
    ch1List.push_back(QPointF(time, ch1));
}

void Remote::SetCh2(const uint32_t time, const int8_t ch2)
{
    ch2List.push_back(QPointF(time, ch2));
}

void Remote::SetCh3(const uint32_t time, const int8_t ch3)
{
    ch3List.push_back(QPointF(time, ch3));
}

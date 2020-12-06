#include "sevensegui.h"

SevenSegUi::SevenSegUi()
{
    numberList.clear();
}

QList<QList<QPointF> > SevenSegUi::GetAllSeries()
{
    QList<QList<QPointF>> series;
    series.append(numberList);

    return series;
}

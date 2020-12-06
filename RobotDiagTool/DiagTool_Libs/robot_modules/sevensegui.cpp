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

void SevenSegUi::SetUiNumber(uint32_t const time, const uint8_t number)
{
    numberList.push_back(QPointF(time, number));
}

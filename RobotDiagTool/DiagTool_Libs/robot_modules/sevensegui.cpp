#include "sevensegui.h"

SevenSegUi::SevenSegUi()
{
    EmptyAllLists();
}

QList<QList<QPointF> > SevenSegUi::GetAllSeries()
{
    QList<QList<QPointF>> series;
    series.append(numberList);

    return series;
}

void SevenSegUi::EmptyAllLists()
{
    numberList.clear();
}

void SevenSegUi::SetUiNumber(uint32_t const time, const uint8_t number)
{
    numberList.push_back(QPointF(time, number));
}

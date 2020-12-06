#ifndef SEVENSEGUI_H
#define SEVENSEGUI_H

#include "irobotmodule.h"

class SevenSegUi : public IRobotModule
{
    Q_OBJECT
public:
    SevenSegUi();

    virtual QList<QList<QPointF>> GetAllSeries() override;

signals:

private:
    QList<QPointF> numberList;
};

#endif // SEVENSEGUI_H

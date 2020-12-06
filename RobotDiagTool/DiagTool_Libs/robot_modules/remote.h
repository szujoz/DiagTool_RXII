#ifndef REMOTE_H
#define REMOTE_H

#include "irobotmodule.h"

class Remote : public IRobotModule
{
    Q_OBJECT
public:
    Remote();

    virtual QList<QList<QPointF>> GetAllSeries() override;

    void SetCh1(uint32_t const time, int8_t const ch1);
    void SetCh2(uint32_t const time, int8_t const ch2);
    void SetCh3(uint32_t const time, int8_t const ch3);

private:
    QList<QPointF> ch1List;
    QList<QPointF> ch2List;
    QList<QPointF> ch3List;
};

#endif // REMOTE_H

#ifndef SPEEDCONTROLLER_H
#define SPEEDCONTROLLER_H

#include "irobotmodule.h"

class SpeedController : public IRobotModule
{
    Q_OBJECT
public:
    virtual QList<QList<QPointF>> GetAllSeries() override;

    virtual void EmptyAllLists() override;

    void SetSetPoint(uint32_t const timestamp, int32_t const setPoint);
    void SetControlValue(uint32_t const timestamp, int32_t const controlValue);
    void SetProcessValue(uint32_t const timestamp, int32_t const processValue);
    void SetP(uint32_t const timestamp, int32_t const P);
    void SetI(uint32_t const timestamp, int32_t const I);
    void SetD(uint32_t const timestamp, int32_t const D);
    void SetIntegrateLimit(uint32_t const timestamp, int32_t const integrateLimit);
    void SetIntegrate(uint32_t const timestamp, int32_t const integrate);
    void SetDerivative(uint32_t const timestamp, int32_t const derivative);
    void SetError(uint32_t const timestamp, int32_t const error);

signals:

private:
    QList<QPointF> setPointList;
    QList<QPointF> controlValueList;
    QList<QPointF> processValueList;
    QList<QPointF> pList;
    QList<QPointF> iList;
    QList<QPointF> dList;
    QList<QPointF> integrateLimitList;
    QList<QPointF> integrateList;
    QList<QPointF> derivativeList;
    QList<QPointF> errorList;
};

#endif // SPEEDCONTROLLER_H

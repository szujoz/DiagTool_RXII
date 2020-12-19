#include "speedcontroller.h"

QList<QList<QPointF> > SpeedController::GetAllSeries()
{
    QList<QList<QPointF>> series;
    series.append(setPointList);
    series.append(controlValueList);
    series.append(processValueList);
    series.append(pList);
    series.append(iList);
    series.append(dList);
    series.append(integrateLimitList);
    series.append(integrateList);
    series.append(derivativeList);
    series.append(errorList);

    return series;
}

void SpeedController::EmptyAllLists()
{
    setPointList.clear();
    controlValueList.clear();
    processValueList.clear();
    pList.clear();
    iList.clear();
    dList.clear();
    integrateLimitList.clear();
    integrateList.clear();
    derivativeList.clear();
    errorList.clear();
}

void SpeedController::SetSetPoint(const uint32_t timestamp, const int32_t setPoint)
{
    setPointList.push_back(QPointF(timestamp, setPoint));
}

void SpeedController::SetControlValue(const uint32_t timestamp, const int32_t controlValue)
{
    controlValueList.push_back(QPointF(timestamp, controlValue));
}

void SpeedController::SetProcessValue(const uint32_t timestamp, const int32_t processValue)
{
    processValueList.push_back(QPointF(timestamp, processValue));
}

void SpeedController::SetP(const uint32_t timestamp, const int32_t P)
{
    pList.push_back(QPointF(timestamp, P));
}

void SpeedController::SetI(const uint32_t timestamp, const int32_t I)
{
    iList.push_back(QPointF(timestamp, I));
}

void SpeedController::SetD(const uint32_t timestamp, const int32_t D)
{
    dList.push_back(QPointF(timestamp, D));
}

void SpeedController::SetIntegrateLimit(const uint32_t timestamp, const int32_t integrateLimit)
{
    integrateLimitList.push_back(QPointF(timestamp, integrateLimit));
}

void SpeedController::SetIntegrate(const uint32_t timestamp, const int32_t integrate)
{
    integrateList.push_back(QPointF(timestamp, integrate));
}

void SpeedController::SetDerivative(const uint32_t timestamp, const int32_t derivative)
{
    derivativeList.push_back(QPointF(timestamp, derivative));
}

void SpeedController::SetError(const uint32_t timestamp, const int32_t error)
{
    errorList.push_back(QPointF(timestamp, error));
}

#ifndef IROBOTMODULE_H
#define IROBOTMODULE_H

#include <QObject>
#include <QPointF>

class IRobotModule : public QObject
{
    Q_OBJECT
public:
    virtual QList<QList<QPointF>> GetAllSeries() = 0;

signals:

};

#endif // IROBOTMODULE_H

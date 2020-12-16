#ifndef DEBUGTRACE_H
#define DEBUGTRACE_H

#include <QObject>

class DebugTrace
{
public:
    static DebugTrace *GetInstance();

    void Trace(QString msg);
    QList<QString> *GetTraces();
    QString GetNotReadTraces();
    bool IsNewTraceAvailable();

private:
    static DebugTrace* instance_;

    static QList<QString>* traceList;

    static int readIndex;

    DebugTrace();
};

#endif // DEBUGTRACE_H

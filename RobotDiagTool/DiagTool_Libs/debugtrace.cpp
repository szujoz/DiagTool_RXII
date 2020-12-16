#include "debugtrace.h"

DebugTrace* DebugTrace::instance_ = NULL;
QList<QString>* DebugTrace::traceList = new QList<QString>();
int DebugTrace::readIndex = 0;

DebugTrace *DebugTrace::GetInstance()
{
    if(instance_ == NULL)
    {
        instance_ = new DebugTrace();
    }

    return instance_;
}

void DebugTrace::Trace(QString msg)
{
    traceList->push_back(msg);
}

QList<QString>* DebugTrace::GetTraces()
{
    return traceList;
}

QString DebugTrace::GetNotReadTraces()
{
    QString traces = "";
    int i;

    for(i = readIndex; i < traceList->count(); i++)
    {
        traces += traceList->at(i);
        traces += "\n";
    }
    readIndex = i;

    return traces;
}

bool DebugTrace::IsNewTraceAvailable()
{
    bool newAvailable = false;

    if (readIndex != traceList->count())
    {
        newAvailable = true;
    }

    return newAvailable;
}

DebugTrace::DebugTrace()
{
    traceList->clear();
}

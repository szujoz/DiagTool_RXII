#ifndef ICOMMANDBUILDER_H
#define ICOMMANDBUILDER_H

#include <QObject>

class ICommandBuilder : public QObject
{
    Q_OBJECT
public:
    virtual void set_uint8_data(uint8_t byte) = 0;
    virtual void set_uint32_data(uint32_t ulong) = 0;
    virtual void set_int32_data(int32_t _long) = 0;
    virtual void set_text_data(QString str) = 0;
};

#endif // ICOMMANDBUILDER_H

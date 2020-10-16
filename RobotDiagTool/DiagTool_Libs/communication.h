#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <memory>
#include <QDataStream>
#include <QDateTime>

class Communication : public QObject
{
    Q_OBJECT

public:
    Communication();
    virtual ~Communication();

    void connectToDevice(QIODevice *device);

    virtual bool isConnected() const = 0;

    QDataStream *getReceiveStream();

    template<typename T>
    void send(const T& toSendObject);

signals:
    void errorOccurred(const QString&);

    void dataReady(QDataStream& stream);

protected:
    QDataStream *receiveStream;
    QByteArray  sendBuffer;

    std::unique_ptr<QDataStream> getSendStream();

    virtual void sendBufferContent() = 0;

private:
    qint32 currentMessageSize;

protected slots:
    void dataReceived();
};

#endif // COMMUNICATION_H

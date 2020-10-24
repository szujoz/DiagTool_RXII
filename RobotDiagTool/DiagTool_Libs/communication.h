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
    void send(const T& toSendObject)
    {
        auto stream = getSendStream();

        // Elmentjük a jelenlegi stream pozíciót
        const qint64 startPos = stream->device()->size();
        qint32 msgSize = 0;
        // Ideiglenesen az elejére méretnek 0-t írunk. Majd
        //  ha már le tudjuk mérni az üzenet hosszát, visszajövünk ide és
        //  beírjuk a tényleges értéket.
        *stream << msgSize;
        // A tényleges adattartalom sorosítása
        *stream << toSendObject;
        const qint64 endPos = stream->device()->size();

        // Visszaugrunk és beírjuk a helyes üzenet méretet.
        stream->device()->seek(startPos);
        msgSize = endPos - startPos;
        *stream << msgSize;
        // Visszaugrunk az üzenet végére
        stream->device()->seek(endPos);

        // Ténylegesen elküldjük az üzenetet.
        //  (Ez absztrakt metódus, majd minden protokoll implementálja, ahogy kell.)
        sendBufferContent();
    }

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

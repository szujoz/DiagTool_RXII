#include "communication.h"

Communication::Communication()
{

}

Communication::~Communication()
{

}

void Communication::connectToDevice(QIODevice *device)
{
    Q_UNUSED(device);
}

QDataStream* Communication::getReceiveStream()
{
    return nullptr;
}

template<typename T>
void Communication::send(const T& toSendObject)
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

 std::unique_ptr<QDataStream> Communication::getSendStream()
 {
     return NULL;
 }

 void Communication::dataReceived()
 {
    qDebug("Data in rx buffer");
    auto msg = "Hello";
    send(msg);
 }

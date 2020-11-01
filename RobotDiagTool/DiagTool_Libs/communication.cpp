#include "communication.h"
#include <QDebug>
#include <QDataStream>

Communication::Communication()
{

}

Communication::~Communication()
{
    delete receiveStream;
}

void Communication::connectToDevice(QIODevice *device)
{
    receiveStream = new QDataStream(device);
}

QDataStream* Communication::getReceiveStream()
{
    return receiveStream;
}

 std::unique_ptr<QDataStream> Communication::getSendStream()
 {
     std::unique_ptr<QDataStream>stream = std::make_unique<QDataStream>(&sendBuffer, QIODevice::WriteOnly);
     return stream;
 }

 void Communication::dataReceived()
 {
     auto a = receiveStream->device()->bytesAvailable();
     char ch[a];

     receiveStream->device()->peek(ch,a);

     qDebug() << "Data in rx buffer: " << ch;

     for(int i = 0; i < a; i++)
     {
         if(ch[i] == '\n')
         {
             emit dataReady(*receiveStream);
         }
     }
 }

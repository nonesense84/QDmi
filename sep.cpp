#include "sep.h"

sep::sep()
{
    udpSocketPzb = new QUdpSocket(this);
    udpSocketPzb->bind(QHostAddress::Any, 10001);
    connect(udpSocketPzb, SIGNAL(readyRead()),this,SLOT(readingPendingPzb()));
    udpSocketLzb = new QUdpSocket(this);
    udpSocketLzb->bind(QHostAddress::Any, 10004);
    connect(udpSocketLzb, SIGNAL(readyRead()),this,SLOT(readingPendingLzb()));
    udpSocketSpeed = new QUdpSocket(this);
    udpSocketSpeed->bind(QHostAddress::Any, 10002);
    connect(udpSocketSpeed, SIGNAL(readyRead()),this,SLOT(readingPendingSpeed()));
    udpSocketMtd = new QUdpSocket(this);
    udpSocketMtd->bind(QHostAddress::Any, 10003);
    connect(udpSocketMtd, SIGNAL(readyRead()),this,SLOT(readingPendingMtd()));
}

void sep::readingPendingLzb(){
    QVector<quint8> lzbValuesToDecoder(7,0);
    QByteArray datagram;
    datagram.resize(udpSocketLzb->pendingDatagramSize());
    udpSocketLzb->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
    if(datagram.size() >= 7){
        for(int i = 0; i < 7; i = i + 1){
            lzbValuesToDecoder[i] = datagram[i];
        }
        emit newLzbValues(lzbValuesToDecoder);
    }
}

void sep::readingPendingPzb(){
    while (udpSocketPzb->hasPendingDatagrams()) {
        QVector<quint8> lmsToDecoder(18,0);
        QByteArray datagram;
        datagram.resize(udpSocketPzb->pendingDatagramSize());
        udpSocketPzb->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        if(datagram.size() >= 9){
            for(int i = 0; i < 9; i = i + 1){
                lmsToDecoder[i*2 +1] = (datagram[i]       & 0x0f);
                lmsToDecoder[i*2  ] = (datagram[i] >> 4)  & 0x0f;
            }
            emit newLzbIndicators(lmsToDecoder);
        }
    }
}
void sep::readingPendingSpeed(){
    while (udpSocketSpeed->hasPendingDatagrams()) {
        QVector<quint8> lmsToDecoder(16,0);
        QByteArray datagram;
        datagram.resize(udpSocketSpeed->pendingDatagramSize());
        udpSocketSpeed->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        if(datagram.size() >= 2){
            quint16 speed = static_cast<quint8>(datagram[0]) +
                           (static_cast<quint8>(datagram[1])<<8);
            emit newSpeed(static_cast<qreal>(speed));
            //qDebug() << "got new speed";
        }
    }
}

void sep::readingPendingMtd(){
    while (udpSocketMtd->hasPendingDatagrams()) {
        QVector<quint8> lmsToDecoder(7,0);
        QByteArray datagram;
        datagram.resize(udpSocketMtd->pendingDatagramSize());
        udpSocketMtd->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        if(datagram.size() >= 3){
            lmsToDecoder[0] = (datagram[0] >> 4) & 0x0f;    //IndDsd
            lmsToDecoder[1] =  datagram[0]       & 0x0f;    //IndDsdFb
            lmsToDecoder[2] = (datagram[1] >> 4) & 0x0f;    //IndDorTav  (Auch blinken)
            lmsToDecoder[3] =  datagram[1]       & 0x0f;    //IndPhantoUp
            lmsToDecoder[4] = (datagram[2] >> 4) & 0x0f;    //IndPhantoDown
            lmsToDecoder[5] =  datagram[2]       & 0x0f;    //IndMtMsO
            lmsToDecoder[6] = (datagram[3] >> 4) & 0x0f;    //IndMtHvtl
        }
        emit newMtdIndicators(lmsToDecoder);
    }
}

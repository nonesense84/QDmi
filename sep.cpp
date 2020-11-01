#include "sep.h"

sep::sep(){}

void sep::process(){
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
        QVector<quint8> lmsToDecoder(16,0); // FIXME: Den benutze ich doch nicht. Kann weg?
        QByteArray datagram;
        datagram.resize(udpSocketSpeed->pendingDatagramSize());
        udpSocketSpeed->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        if(datagram.size() >= 5){
            quint16 speed = static_cast<quint8>(datagram[0]) +
                           (static_cast<quint8>(datagram[1])<<8);
            emit newSpeed(speed);
            //qDebug() << "got new speed";
            qint16 forceRel = 0;
            quint8 brakingForceRel = static_cast<quint8>(datagram[3]);
            quint8 tractiveForceRel = static_cast<quint8>(datagram[4]);

            if(brakingForceRel != 0) forceRel = static_cast<qint16>(brakingForceRel * -1);
            if(tractiveForceRel != 0) forceRel = static_cast<qint16>(tractiveForceRel);
            qDebug() << forceRel;
            emit newPowerAbsolute(forceRel);
        }
    }
}

void sep::readingPendingMtd(){
    while (udpSocketMtd->hasPendingDatagrams()) {
        QVector<quint8> lmsToDecoder(13,0);
        QByteArray datagram;
        datagram.resize(udpSocketMtd->pendingDatagramSize());
        udpSocketMtd->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        if(datagram.size() >= 7){
            lmsToDecoder[1] =   (datagram[0] >> 4) & 0x0f;     //IndDsd
            lmsToDecoder[2] =   (datagram[0]       & 0x0f) * 2;//IndDsdAcu
            lmsToDecoder[2] =   (datagram[1] >> 4) & 0x0f;     //IndDsdFb
            lmsToDecoder[9] =    datagram[1]       & 0x0f;     //IndDorTav //Muss noch auf MTD-Seite angepasst werden
            lmsToDecoder[6] =   (datagram[2] >> 4) & 0x0f;     //IndPhantoUp
                                                               //IndPhantoDown
            lmsToDecoder[7] = !((datagram[3] >> 4) & 0x0f);    //IndMtMsO
            lmsToDecoder[8] =    datagram[3]       & 0x0f;     //IndMtHvtl
            lmsToDecoder[12]=   (datagram[4] >> 4) & 0x0f;     //tractionType     // Muss noch getestet werden!!!!!
            lmsToDecoder[0] =    datagram[4]       & 0x0f;     //Sifa faulty

        }
        emit newMtdIndicators(lmsToDecoder);
        emit newFzgVmaxTacho((static_cast<uint8_t>(datagram[5])) |   (static_cast<quint16>(datagram[6]) << 8));
    }
}

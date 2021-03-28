#include "sep.h"
#include <QDebug>
sep::sep(){}

void sep::process(){
    udpSocketSep = new QUdpSocket(this);
    udpSocketSep->bind(QHostAddress::Any, 10001);
    connect(udpSocketSep, SIGNAL(readyRead()),this,SLOT(readingPendingSep()));
    /*    udpSocketPzb = new QUdpSocket(this);
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
    connect(udpSocketMtd, SIGNAL(readyRead()),this,SLOT(readingPendingMtd()));*/
}
quint8 sep::upNib(qint8 value){return static_cast<quint8>(value >> 4);}
quint8 sep::lowNib(qint8 value){return static_cast<quint8>(value & 0x0f);}
quint16 sep::twoByteToquint16(qint8 lowByte, qint8 highByte){
    return static_cast<quint16>(static_cast<quint8>(lowByte) +
                              (static_cast<quint8>(highByte)<<8));
}

void sep::readingPendingSep(){
    #define pzbOffset  3
    #define lzbOffset  14
    #define MtdOffset  26
    #define vmfzgOffset  31
    #define dbcOffset  34
    #define timeOffset  36
    #define pressureOffset  40
    #define speedOffset  21
    #define forceOffset  24
    QVector<quint8> lzbValuesToDecoder(7,0);
    QVector<quint8> pzbLmsToDecoder(22,0);
    QVector<quint8> lmsToDecoder(13,0);
    QByteArray datagram;
    while (udpSocketSep->hasPendingDatagrams()) {
        datagram.resize(udpSocketSep->pendingDatagramSize());
        udpSocketSep->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        //qDebug() << datagram;
        if(datagram.size() >= 41){
            if (static_cast<quint8>(datagram[0]) != versionMajor)qDebug() << "TDF may by incompatible!";
            if (static_cast<quint8>(datagram[1]) != versionMinor)qDebug() << "May by there are new functions!";
            if (static_cast<quint8>(datagram[2]) != versionPatch)qDebug() << "May by there are hot fixes!";
            // ==================== Read PZB indicators ====================
            for(int i = 0; i < 11; i = i + 1){
                pzbLmsToDecoder[i*2 +1] = lowNib(datagram[pzbOffset + i]);
                pzbLmsToDecoder[i*2  ] =   upNib(datagram[pzbOffset + i]);
            }
            emit newLzbIndicators(pzbLmsToDecoder);
            // ==================== Read LZB values ====================
            for(int i = 0; i < 7; i = i + 1){
                lzbValuesToDecoder[i] = static_cast<quint8>(datagram[lzbOffset + i]);
            }
            emit newLzbValues(lzbValuesToDecoder);
            // ==================== Read Speed ====================
            quint16 speed =twoByteToquint16(datagram[speedOffset], datagram[speedOffset + 1]);
            emit newSpeed(speed);
            // ==================== Read Power ====================
            emit newPowerRelative(static_cast<qint16>(- datagram[forceOffset    ]
                                                      + datagram[forceOffset + 1]));
            // ==================== Read MTD indicators ====================
            lmsToDecoder[0] =    upNib(datagram[MtdOffset + 0]);            // Sifa faulty
            lmsToDecoder[1] =   lowNib(datagram[MtdOffset + 0]);            // IndDsd
            lmsToDecoder[2] =  ( upNib(datagram[MtdOffset + 1]) & 0x01) +   // IndDsdAcu +
                               (lowNib(datagram[MtdOffset + 1]) & 0x01);    // IndDsdFb (Enum übernommen aus Zusi Sifa-Hupe)
            switch (upNib(datagram[MtdOffset + 2])){                        // IndPanto (Enum übernommen aus Zusi Stromabnehmer)
                case 1: lmsToDecoder[6] = 0x01;break;
                case 2: lmsToDecoder[6] = 0x11;break;
                default:lmsToDecoder[6] = 0x00;
            }
            lmsToDecoder[7] = !(lowNib(datagram[MtdOffset + 2]));           // IndMtMsO
            lmsToDecoder[8] =    upNib(datagram[MtdOffset + 3]);            // IndMtHvtl
            lmsToDecoder[9] =   lowNib(datagram[MtdOffset + 3]);            // InfDoorSystem - Von Loksim nicht unterstützt -> Default 4 (TAV)
            lmsToDecoder[10] =   upNib(datagram[MtdOffset + 4]);            // InfDoorStatR
            lmsToDecoder[11] =  lowNib(datagram[MtdOffset + 4]);            // InfDoorStatL (Temporär IndDorTav)
            lmsToDecoder[12]=   lowNib(datagram[MtdOffset + 5]);            // tractionType
            emit newMtdIndicators(lmsToDecoder);
            emit newFzgVmaxTacho(twoByteToquint16(datagram[vmfzgOffset], datagram[vmfzgOffset + 1])+20);
            emit newAfbSoll(twoByteToquint16(datagram[dbcOffset], datagram[dbcOffset + 1]), upNib(datagram[MtdOffset + 5]) > 0);   // DbcSpeedSet, IndDbc
            simTime = static_cast<quint32>((static_cast<quint8>(datagram[timeOffset + 3]) << 24)
                                         + (static_cast<quint8>(datagram[timeOffset + 2]) << 16)
                                         + (static_cast<quint8>(datagram[timeOffset + 1]) <<  8)
                                         +  static_cast<quint8>(datagram[timeOffset + 0])      );
            if(simTime != simTimeOld){
                simTimeOld = simTime;
                QDateTime QSimTime = QDateTime::fromSecsSinceEpoch(simTime, Qt::UTC);
                emit newSimTime(QSimTime.time().toString());
            }
            emit newHll(static_cast<quint8>(datagram[pressureOffset + 0]));
            emit newBrz(static_cast<quint8>(datagram[pressureOffset + 1]));
            emit newHlb(static_cast<quint8>(datagram[pressureOffset + 2]));
        }
    }
}

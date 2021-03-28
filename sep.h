#ifndef SEP_H
#define SEP_H
#include <QUdpSocket>
#include <QDateTime>
#include <QObject>
#define  versionMajor  1
#define  versionMinor  0
#define  versionPatch  0

class sep : public QObject
{
    Q_OBJECT
public:
    sep();
public slots:
    void process();

signals:
    //void newLzbIndicators(quint8 lmsToDecoder[16]);
    void newLzbIndicators(QVector<quint8> lmsToDecoder);
    void newLzbValues(QVector<quint8> lmsToDecoder);
    void newSpeed(quint16 speed);
    void newHll(quint16 Hll);
    void newBrz(quint16 Brz);
    void newHlb(quint16 Hlb);
    void newPowerRelative(qint16 power);
    void newMtdIndicators(QVector<quint8> lmsToDecoder);
    void newFzgVmaxTacho(quint16 speed);
    void newAfbSoll(quint16 vSoll, bool visible);
    void newSimTime(QString simtime);

private:
    QUdpSocket *udpSocketSep;
    QUdpSocket *udpSocketPzb;
    QUdpSocket *udpSocketLzb;
    QUdpSocket *udpSocketMtd;
    QUdpSocket *udpSocketSpeed;
    QHostAddress sender;
    quint16 senderPort;
    quint32 simTime;
    quint32 simTimeOld;

private slots:
    void readingPendingSep();
    quint8 upNib(qint8 value);
    quint8 lowNib(qint8 value);
    quint16 twoByteToquint16(qint8 highByte, qint8 lowByte);
/*  bool upNibBlink(quint8 value);
    bool lowNibBlink(quint8 value);
    bool upNibOn(quint8 value);
    bool lowNibOn(quint8 value);*/
};


#endif // SEP_H

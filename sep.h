#ifndef SEP_H
#define SEP_H
#include <QUdpSocket>

#include <QObject>

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
    void newPowerAbsolute(qint16 power);
    void newMtdIndicators(QVector<quint8> lmsToDecoder);
    void newFzgVmaxTacho(quint16 speed);

private:
    QUdpSocket *udpSocketPzb;
    QUdpSocket *udpSocketLzb;
    QUdpSocket *udpSocketMtd;
    QUdpSocket *udpSocketSpeed;
    QHostAddress sender;
    quint16 senderPort;

private slots:
    void readingPendingPzb();
    void readingPendingLzb();
    void readingPendingMtd();
    void readingPendingSpeed();
};

#endif // SEP_H

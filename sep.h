#ifndef SEP_H
#define SEP_H
#include <QUdpSocket>

#include <QObject>

class sep : public QObject
{
    Q_OBJECT
public:
    sep();

signals:
    //void newLzbIndicators(quint8 lmsToDecoder[16]);
    void newLzbIndicators(QVector<quint8> lmsToDecoder);
    void newLzbValues(QVector<quint8> lmsToDecoder);
    void newSpeed(qreal speed);
    void newPowerAbsolute(qint16 power);
    void newMtdIndicators(QVector<quint8> lmsToDecoder);

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

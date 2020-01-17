#ifndef ZUSI3TCP_H
#define ZUSI3TCP_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QtMath>
#include "zusiindicator.h"
#include <QHostInfo>
#include "era.h"
#include "zusipower.h"

class zusi3Tcp: public QObject
{
    Q_OBJECT
private slots:
    void setLzbValue(float input, uint8_t pos);
    void setMtdIndicator(uint8_t value, uint8_t pos);

    bool checkHysterise(uint16_t *output, float input);
    bool checkHysterise(float *output, float input, bool isRelative = false);
    void clientReadReady();
    void checkClientConnection();
    void remooveTechMessage9();
    void remooveTechMessage10();
    void remooveTechMessage13();
    void resetVehicleBlocking();
    int32_t readIntegerInRawAtPos(int pos);
    int32_t readIntegerAtPos(int pos);
    uint16_t readIdAtPos(int pos);
    void setSammelschine();
public slots:
    void cutZusiTelegram();
    void zusiDecoder();
    qint8 setIpadress(QString address);
    void disconnectFromZusi();
public:
    zusi3Tcp();
    zusiIndicator *myIndicators;
    zusiPower *myPower;

private:
    bool istVMaxErstesFahrzeug = true;
    uint8_t ipHostPart = 1;
    uint16_t VIst=0;
    uint16_t VSoll=0;
    uint16_t VMFzg=0;
    //uint16_t VZiel=0;
    float ZugkraftAbs=0;
    float ZugkraftRel=0;
    quint16 ZugkraftAbsMax=0;
    quint16 ZugkraftAbsMaxFilter[7];
    QTcpSocket *client;
    QByteArray incommingData;
    QByteArray compZusiPacket;
    QString tuerSystem;
    bool lastWasNewNode = false;
    bool istReisezug = false, hauptschalter = false;
    QVector<quint8> mtdLmsToDecoder;
    QVector<quint8> lzbValuesToDecoder;
    union { //Datentyp zur Abfrage des Knotens
      int8_t chr[4];
      uint8_t byte[4];
      int integer;
    } DataLng;

    union {
      uint8_t byte[2];
      int8_t chr[2];
      int16_t smallInt;
      uint16_t word;
    } dataGroup;

    int16_t    layer = 0;
    uint16_t   nodeId1 = 0;
    uint16_t   nodeId2 = 0;
    uint16_t   nodeId3 = 0;
    uint16_t   nodeId4 = 0;
    uint16_t   nodeId5 = 0;
    uint16_t   nodeId6 = 0;
    uint16_t   atributeId = 0;
    QByteArray useDataComplex;

    union {
        int8_t chr[4];
        uint8_t byte[4];
        int32_t Integer;
        float Single;

    } useData4Byte;
    union {
        int8_t chr[2];
        uint8_t byte[2];
        int16_t smallInt;
        uint16_t Word;
    } useData2Byte;
    #define MAX_NUTZDATA 4
public: signals:
    void newLzbIndicators(QVector<quint8> lmsToDecoder);
    void newLzbValues(QVector<quint8> lmsToDecoder);
    void newSpeed(qreal speed);
    void newMtdIndicators(QVector<quint8> lmsToDecoder);
    void newTechnicalMessage(QString text, QColor forColor, QColor bgColor, quint8 id);
    void removeTechnicalMessage(quint8 id);
};

#endif // ZUSI3TCP_H

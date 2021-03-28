#ifndef ZUSI3TCP_H
#define ZUSI3TCP_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QtMath>
#include "zusiindicator.h"
#include <QHostInfo>
#include <qthread.h>
#include "era.h"
#include "zusipower.h"
#include <QDateTime>
class zusi3Tcp: public QObject
{
    Q_OBJECT
private slots:
    void setMtdIndicator(uint8_t value, uint8_t pos);

    float mPerSecToKmh(float input);
    bool checkHysterise(uint16_t *output, float input);
    bool checkHysterise(float *output, float input, bool isRelative = false);
    void clientReadReady();
    void checkClientConnection();
    void remooveTechMessage9();
    void remooveTechMessage10();
    void remooveTechMessage13();
    void resetVehicleBlocking();
    void guesTractionType();
    int32_t readIntegerInRawAtPos(int pos);
    int32_t readIntegerAtPos(int pos);
    uint16_t readIdAtPos(int pos);
    void setSammelschine();
    void setZugnummer(QString nummer, QString fromSystem);
    void transmitMtdIndicators();

public slots:
    void cutZusiTelegram();
    void zusiDecoderFahrpult();
    void zusiDecoderSecondaryInfos();
    void setIpadress(QString address);
    void disconnectFromZusi();
    void process();
    void reconnect();
    void setAutoReconnect(quint8 reconnect);
    void addKnotenAnfang(QVector<unsigned char> *vector, unsigned char knoten);
    void addKnotenEnde(QVector<unsigned char> *vector);
    void addAtribut(QVector<unsigned char> *vector, unsigned char id, unsigned char atribut);
    void addAtribut(QVector<unsigned char> *vector, unsigned char atribut);
    void addTextAtribut(QVector<unsigned char> *vector, quint16 id, QString text);
    void setUseManometer(bool use);
    void setTextUsing(quint8 useAutomText);

public:
    zusi3Tcp();
    zusiIndicator *myIndicators;
    zusiPower *myPower;

private:
    bool autoReconnect = true;
    QString ipAddress;
    QString zugnummer;
    bool useManometer = true;
    uint8_t forceTextmessages = 0;
    bool istVMaxErstesFahrzeug = true;
    bool trainHasBenMovedSinceLastNewTrainNumber = false;
    uint8_t ipHostPart = 1;
    uint16_t drHll=0;
    uint16_t drHlb=0;
    uint16_t drBrz=0;
    uint16_t VIst=0;
    uint16_t VSoll=0;
    uint16_t VMFzg=0;
    float fahrlSpng;
    uint8_t stromabnehmerLok;
    uint8_t stromabnehmerSteuerwagen;
    float zugkraftProAchs;
    float zugkraftProAchsSteuerwagen;
    //uint16_t VZiel=0;
    //float ZugkraftAbs=0;
    //float ZugkraftRel=0;
    //quint16 ZugkraftAbsMax=0;
    //quint16 ZugkraftAbsMaxFilter[7];
    QTcpSocket *client;
    QByteArray incommingData;
    QByteArray compZusiPacket;
    bool lastWasNewNode = false;
    bool istReisezug = false, hauptschalter = false;
    QVector<quint8> mtdLmsToDecoder;
    QVector<quint8> mtdLmsToDecoderOld;
    QTime simtime;
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
    uint16_t   nodeIds[6];
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
    void newSpeed(quint16 speed);
    void newHll(quint16 Hll);
    void newBrz(quint16 Brz);
    void newHlb(quint16 Hlb);
    void newSimTime(QString simtime);
    void newZugnummer(QString simtime);
    void newMtdIndicators(QVector<quint8> lmsToDecoder);
    void newTechnicalMessage(QString text, QColor forColor, QColor bgColor, quint8 id);
    void removeTechnicalMessage(quint8 id);
    void sendTcpConnectionFeedback(QString feedback);
    void sendDataSourceIsZusi(bool);
};

#endif // ZUSI3TCP_H

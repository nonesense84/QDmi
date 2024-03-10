#ifndef ZUSI3TCP_H
#define ZUSI3TCP_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QtMath>
#include "zusiindicator.h"
#include "zusi3etcs.h"
#include <QHostInfo>
#include <qthread.h>
#include "era.h"
#include "zusipower.h"
#include <QDateTime>

#define zusiMajor  3
#define zusiMinor  5
#define zusiPatch  3

class zusi3Tcp: public QObject
{
    Q_OBJECT
private slots:
    void setMtdIndicator(uint8_t value, uint8_t pos);

    float mPerSecToKmh(float input);
    bool checkHysterise(int32_t *output, float input);
    bool checkHysterise(uint16_t *output, float input);
    bool checkHysterise(float *output, float input, bool isRelative = false);
    void clientReadReady();
    void checkClientConnection(QAbstractSocket::SocketState state);
    void resetVehicleBlocking();
    void setTractionType();
    int32_t readIntegerInRawAtPos(int pos);
    int32_t readIntegerAtPos(int pos);
    uint16_t readIdAtPos(int pos);
    void setSammelschine();
    void setZugnummer(QString nummer, QString fromSystem);
    void transmitMtdIndicators();
    void removeTechniccalMessages();

public slots:
    void cutZusiTelegram();
    void zusiDecoderFahrpult();
    void zusiDecoderSecondaryInfos();
    void setIpadress(QString address);
    void disconnectFromZusi();
    void connectToZusi();
    //void handleConnectionError(QAbstractSocket::SocketError socketError);
    void subscribeZusiData();
    void process();
    void reconnect();
    void setAutoReconnect(quint8 reconnect);
    void addKnotenAnfang(QVector<unsigned char> *vector, quint16 knoten);
    void addKnotenEnde(QVector<unsigned char> *vector);
    void addAtribut(QVector<unsigned char> *vector, unsigned char atribut);
    void addByteAtribut(QVector<unsigned char> *vector, unsigned char id, quint8 atribut);
    void addAtribut(QVector<unsigned char> *vector, unsigned char id, quint16 atribut);
    void addAtribut(QVector<unsigned char> *vector, unsigned char id, QString atribut);
    void addTextAtribut(QVector<unsigned char> *vector, quint16 id, QString text);
    void setUseManometer(bool use);
    void setTextUsing(quint8 useAutomText);
    void setDriverId(QString driverID);
    void setTrainRunningNumber(QString trn);
    void setTrainDataLzb(quint16 BRA, quint16 BRH, quint16 ZL, quint16 VMZ, bool validated);
    void newTraindataEtcs(quint16 BRH, QString TCT, quint16 ZL, quint16 VMZ, QString AXL, QString AIT, QString LDG, bool validated);
    void sendEtcsAck();
    void sendEtcsOverride();
    void sendEtcsStart();
    void sendEtcsLevel(QString level);
    void sendEtcsIndexStm(quint16 index);
    void sendEtcsMode(quint16 mode);
    void sendEtcsTafMode(quint16 tafMode);
    void sendEtcsTestrun(quint8 testrun);

public:
    zusi3Tcp();
    zusiIndicator *myIndicators;
    zusiPower *myPower;
    zusi3etcs *myEtcs;

private:
    quint16 plainningCounter = 0;
    //bool DATA_OP_Tast_Zugbeeinf = false;
    uint8_t zusiTextMessagesPossible = false;
    bool autoReconnect = true;
    bool reconnectOnes = false;
    QString ipAddress;
    QHostAddress zusiPc;
    QString zugnummer;
    bool useManometer = true;
    uint8_t forceTextmessages = 0;
    bool istVMaxErstesFahrzeug = true;
    bool trainHasBenMovedSinceLastNewTrainNumber = false;
    uint8_t ipHostPart = 1;
    int32_t kilometer = 0;
    uint16_t drHll=0;
    uint16_t drHlb=0;
    uint16_t drBrz=0;
    uint16_t VIst=0;
    uint16_t VSoll=0;
    uint16_t VMFzg=0;
    bool fstAktiv;
    uint8_t tractionType;
    uint8_t currentType;
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
    bool dataContainsEtcs = false;
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
        uint32_t Cardinal;
        float Single;

    } useData4Byte;
    union {
        int8_t chr[2];
        uint8_t byte[2];
        int16_t smallInt;
        uint16_t Word;
    } useData2Byte;
    #define MAX_NUTZDATA 4
    void sendKeyboardCommand(quint16 mapping, quint8  command, quint16 action);
    void sendEtcsSettingsByte(quint8 ID, quint8  value);
    void sendEtcsSettingsWord(quint8 ID, quint16 value);
public: signals:
    void newKilometrierung(qint32 kilometrierung);
    void newLzbIndicators(QVector<quint8> lmsToDecoder);
    void newSpeed(quint16 speed);
    void newHll(quint16 Hll);
    void newBrz(quint16 Brz);
    void newHlb(quint16 Hlb);
    void newSimTime(QString simtime);
    void newZugnummer(QString trainNumber);
    void changedTrain();
    void newMtdIndicators(QVector<quint8> lmsToDecoder);
    void newTechnicalMessage(QStringList timeStamps, QStringList message);
    void removeTechnicalMessage(quint8 id);
    void sendTcpConnectionFeedback(QString feedback);
    void sendDataSourceIsZusi(bool);
    void newBra(quint16 BRA);
    void newBrh(quint16 BRH);
    void newZl(quint16 ZL);
    void newVmz(quint16 VMZ);
    void newTastaturkommando(quint16 tastaturkommando);
    void newCabActive(bool cabActive, bool standstill);
};

#endif // ZUSI3TCP_H

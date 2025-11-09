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
#include <QCoreApplication>
#include <QProcess>
#include <QNetworkInterface>

#define zusiMajor  3
#define zusiMinor  5
#define zusiPatch  9

class zusi3Tcp: public QObject
{
    Q_OBJECT
private slots:
    void setMtdIndicator(quint8 value, quint8 pos);

    float mPerSecToKmh(float input);
    quint16 mPerSecToIntKmh(float input);
    bool checkHysterise(qint32 *output, float input);
    bool checkHysterise(quint16 *output, float input);
    bool checkHysterise(float *output, float input, bool isRelative = false);
    void clientReadReady();
    void checkClientConnection(QAbstractSocket::SocketState state);
    void resetVehicleBlocking();
    void setTractionType();
    qint32 readIntegerInRawAtPos(int pos);
    qint32 readIntegerAtPos(int pos);
    quint16 readIdAtPos(int pos);
    void setSammelschine();
    void setZugnummer(QString nummer, QString fromSystem);
    void transmitMtdIndicators();
    void removeTechniccalMessages();
    QDateTime convertZusiTime(double daysSince2000);

public slots:
    void setNwSettingsOpen(bool open);
    void cutZusiTelegram();
    void zusiDecoderFahrpult();
    void zusiDecoderSecondaryInfos();
    void setIpadress(QString address);
    void disconnectFromZusi();
    void connectToZusi();
    //void handleConnectionError(QAbstractSocket::SocketError socketError);
    void subscribeZusiData();
    void initialize();
    void reconnect();
    void setAutoReconnect(quint8 reconnect);
    void addKnotenAnfang(QVector<unsigned char> *vector, quint16 knoten);
    void addKnotenEnde(QVector<unsigned char> *vector, quint8 numberNodes);
    void addAtribut(QVector<unsigned char> *vector, unsigned char atribut);
    void addByteAtribut(QVector<unsigned char> *vector, unsigned char id, quint8 atribut);
    void addAtribut(QVector<unsigned char> *vector, unsigned char id, quint16 atribut);
    void addAtribut32(QVector<unsigned char> *vector, unsigned char id, quint32 atribut);
    void addFloatAtribut(QVector<unsigned char> *vector, unsigned char id, float atribut);
    void addAtribut(QVector<unsigned char> *vector, unsigned char id, QString atribut);
    void addTextAtribut(QVector<unsigned char> *vector, quint16 id, QString text);
    void setUseManometer(bool use);
    void setTextUsing(quint8 useAutomText);
    quint8 zusiEnumToSepEnum(quint8 zusiEnum);
    quint8 zusiZugartToSep(quint8 zusiEnum);
    void composeLzbInfos();
    void setDriverId(QString driverID);
    void setTrainRunningNumber(QString trn);
    void setTrainDataLzb(quint16 BRA, quint16 BRH, quint16 ZL, quint16 VMZ, bool validated);
    void setTraindataEtcs(quint16 BRH, quint16 TCT, quint16 ZL, quint16 VMZ, quint16 AXL, QString AIT, QString LDG);
    void sendEtcsAck();
    void sendAckEtcsTextMessage(quint32 id);
    void sendEtcsOverride();
    void sendEtcsStart();
    void sendEtcsLevel(quint8 level);
    void sendEtcsIndexStm(quint16 index);
    void sendEtcsMode(quint16 mode);
    void sendEtcsTafMode(quint16 tafMode);
    void sendEtcsTestrun(quint8 testrun);
    void setEtcsBaseline();
public:
    zusi3Tcp();
    zusiIndicator *myIndicators;
    zusiPower *myPower;
    zusi3etcs *myEtcs;

private:
    QTcpSocket *client;
    QByteArray incommingData;
    QByteArray compZusiPacket;
    QByteArray useDataComplex;
    QVector<quint8> mtdLmsToDecoder;
    QVector<quint8> mtdLmsToDecoderOld;
    QList<EtcsTextMessage> messageListMenueInfo;
    QList<EtcsTextMessage> messageList;
    QDateTime date;
    QHostAddress zusiPc;
    QString zugnummer;
    QString ipAddress;
    QTime simtime;
    qint32 kilometer = 0;
    float zugkraftProAchs;
    float zugkraftProAchsSteuerwagen;
    quint16 drHll=0;
    quint16 drHlb=0;
    quint16 drBrz=0;
    quint16 VIst=0;
    quint16 VSoll=0;
    quint16 VMFzg=0;
    qint16  layer = 0;
    quint16 nodeIds[6];
    quint16 atributeId = 0;
    quint16 lzVZ=0, lzVS=0, lzZE=0;
    quint8 lmTH=0, lmFH=0, lm85=0, lm70=0, lm55=0, lm95=0, lm75=0, lm60=0, lmBF=0, lmG=0, lmH=0, lmE4=0, lmEn=0, lmB=0, lmUe=0, lmEl=0,
           lmV4=0, lmS=0, lmGn=0, lmGnUe=0, lmGnG=0, lmGnS=0, lmPzb=0,lmIndusi=0, lzSys=7, pzZa=0, pzb90=0;
    quint8 melderbild=0, endeVerf=0, ersAuf=0, falAuf=0, vorAuf=0, elAuf=0, fUeLHpB=0, noHa=0, ueAusfVz=0, ueAusf=0;
    quint8 brGr=0, sysStateLzb=3, plzbLAH=0, lzbStoersch=0, pzbStoersch=0, typAktZugDa=0, zstZugb=0, sysStatePzb=3, zustLzb=0;
    quint8 lzbShow=0;
    quint8 zusiKtp = 0;
    quint8 ipHostPart = 1;
    quint8 tractionType;
    quint8 currentType;
    quint8 stromabnehmerLok;
    quint8 stromabnehmerSteuerwagen;
    bool plzbDataWasReceived= 0;
    bool autoReconnect = true;
    bool reconnectOnes = false;
    bool useManometer = true;
    bool istVMaxErstesFahrzeug = true;
    bool trainHasBenMovedSinceLastNewTrainNumber = false;
    bool fstAktiv;
    bool lastWasNewNode = false;
    bool istReisezug = false, hauptschalter = false;
    bool dataContainsEtcs = false;
    //quint16 VZiel=0;
    //float ZugkraftAbs=0;
    //float ZugkraftRel=0;
    //quint16 ZugkraftAbsMax=0;
    //quint16 ZugkraftAbsMaxFilter[7];
    union { //Datentyp zur Abfrage des Knotens
      qint8 chr[4];
      quint8 byte[4];
      int integer;
    } DataLng;
    union {
      quint8 byte[2];
      qint8 chr[2];
      qint16 smallInt;
      quint16 word;
    } dataGroup;
    union {
        qint8 chr[4];
        quint8 byte[4];
        qint32 Integer;
        quint32 Cardinal;
        float Single;

    } useData4Byte;
    union {
        qint8 chr[2];
        quint8 byte[2];
        qint16 smallInt;
        quint16 Word;
    } useData2Byte;
    union {
        qint8 chr[8];
        double DoubleFloat;
    } useData8Byte;
    #define MAX_NUTZDATA 4
    void sendKeyboardCommand(quint16 mapping, quint8  command, qint16 position);
    void sendEtcsSettingsByte(quint8 ID, quint8  value);
    void sendEtcsSettingsWord(quint8 ID, quint16 value);

public: signals:
    void newKilometrierung(qint32 kilometrierung);
    void newLzbIndicators(QVector<quint8> lmsToDecoder);
    void newLzbValues(QVector<quint8> lmsToDecoder);
    void newSpeed(quint16 speed);
    void newHll(quint16 Hll);
    void newBrz(quint16 Brz);
    void newHlb(quint16 Hlb);
    void newSimTime(QString simtime);
    void newZugnummer(QString trainNumber);
    void changedTrain();
    void newMtdIndicators(QVector<quint8> lmsToDecoder);
    void removeTechnicalMessage(quint8 id);
    void sendTcpConnectionFeedback(QString feedback);
    void sendDataSourceIsZusi(bool);
    void newLzbBra(quint16 BRA);
    void newLzbBrh(quint16 BRH);
    void newLzbZl(quint16 ZL);
    void newLzbVmz(quint16 VMZ);
    void newEvcBrh(quint16 BRH);
    void newEvcZl(quint16 ZL);
    void newEvcVmz(quint16 VMZ);

    void newTastaturkommando(quint16 tastaturkommando);
    void newCabActive(bool cabActive, bool standstill);
    void newTextMessages(const QList<EtcsTextMessage>& messages);

};

#endif // ZUSI3TCP_H

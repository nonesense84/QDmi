#ifndef ZUSIINDICATOR_H
#define ZUSIINDICATOR_H

#include <QObject>
#include <QDebug>
#include <QtMath>
#include <QTimer>
#include <era.h>
#define vMAuffordZDE  50    // Maximum speed, till "Zugdaten eingeben" or 55/70/85 is blinking
//#define levelUndefined  10
#define levelPzbLzbNtc  6
# define indOff 0
# define indOn 1
# define indBlink 5
class zusiIndicator : public QObject
{
    Q_OBJECT
private:
    QString simTime;
    uint16_t afbSoll = 0, vIst = 0;
    uint8_t
    lmHauptschalter = 0, lmGetriebe = 0, lmSchleudern = 0, lmGleiten = 0,
    lmUhrzeitDigital = 0, StwgHauptschalter = 0, lmSifa = 0, SifaHupe = 0,
    SifaStoerschalter = 0, SifaLuftabsperrhahn = 0, plzbDevice=7,
    stromabn1Oben = 0, stromabn2Oben = 0, stromabn3Oben = 0, stromabn4Oben = 0, stromabn1Hebend = 0,
    stromabn2Hebend = 0,stromabn3Hebend = 0,stromabn4Hebend = 0,indusiDevice = 7;
    bool afbAn = false;


private slots:


public:
    explicit zusiIndicator(QObject *parent = nullptr);


signals:
    void newAfbSoll(quint16 vSoll, bool visible);
    void newFzgVmaxTacho(quint16 speed);


public slots:
    void clearData();
    void setLmHauptschalter(uint8_t value);
    void setStatusStromabnehmer(uint8_t value);
    void setLmSchleudern(uint8_t value);
    void setLmGleiten(uint8_t value);
    void setLmUhrzeitDigital(uint8_t value);
    void setAfbSoll(uint16_t value);
    void setAfbAn(bool value);
    void setFzgVMax(uint16_t value);
    quint8 checkPlzbType(QString value);
    void setDefaults();

    //====
};

#endif // ZUSIINDICATOR_H

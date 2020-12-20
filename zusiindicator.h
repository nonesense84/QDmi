#ifndef ZUSIINDICATOR_H
#define ZUSIINDICATOR_H

#include <QObject>
#include <QDebug>
#include <QtMath>
#include <QTimer>
#include <era.h>
#include "db.h"
class zusiIndicator : public QObject
{
    Q_OBJECT
private:
    uint8_t indicators[64];
    uint8_t melderbild = 0, melderbildOld = 0, lm1000Hz = 0, lm85 = 0, lm70 = 0, lm55 = 0,
    zugart = 0, klartextmeldungen = 0, ktp = 0, lmBefehl = 0, lm500Hz = 0, lmS = 0,
    lmSDelayed = 0, lmH = 0, lmE40 = 0, lmB = 0, lmUe = 0, lmG = 0, lmEl = 0, lmEnde = 0,
    lmHauptschalter = 0, lmGetriebe = 0, lmSchleudern = 0, lmGleiten = 0,
    lmUhrzeitDigital = 0, StwgHauptschalter = 0, lmSifa = 0, SifaHupe = 0,
    SifaStoerschalter = 0, SifaLuftabsperrhahn = 0, ErsatzdatenWirksam = 0, lzbZustand = 0,
    falschfahrauftrag = 0, vorsichtauftrag = 0, lzbNothalt = 0, lzbRechnerausfall = 0,
    lzbElAuftrag = 0, lmEL = 0, lmV40 = 0, lmPruefStoer = 0, stromabn1Oben = 0,
    stromabn2Oben = 0, stromabn3Oben = 0, stromabn4Oben = 0, stromabn1Hebend = 0,
    stromabn2Hebend = 0,stromabn3Hebend = 0,stromabn4Hebend = 0, ZwangsbremsungAktiv = 0,
    lmV40Roh = 0, lmGnt = 0, lmGnt_Ue = 0, lmGnt_G = 0, lmGnt_S = 0, endeverfahren = 0;
    uint16_t zustandZugsicherung = 0,  Uebertragungsausfall = 0, FahrtUeberLlzbHaltPerBefehl = 0, afbSoll = 0, grundZwangsbrmnsung = 0;
    float vZiel = 0, vIst = 0;
    quint8 lastLimitMessage;
    bool restriktiv = false;
    bool afbAn = false;
    bool ersatzauftrag = false, lzbHaltUeberfahren = false;
    bool lzbVorhanden;
    QVector<quint8> lmsToDecoderOld;
    QTimer *lmElBlinkTestTimer;
    QVector<quint8> lzbValuesToDecoder;

private slots:
    void setLzbValue(int16_t input, uint8_t pos);
    void calcPzbTextmessages();
    void sentSpetLimitMessage(quint8 limit);
    //void makeLzbAnalogDatagram();
    void remooveMessage58();
    void setLzbElAuftrag12();

public:
    explicit zusiIndicator(QObject *parent = nullptr);


signals:
    void newLzbIndicators(QVector<quint8> lmsToDecoder);
    void newTextMessage(QString text, QColor forColor, QColor bgColor, quint8 id);
    void removeMessage(quint8 id);
    void newTechnicalMessage(QString text, QColor forColor, QColor bgColor, quint8 id);
    void removeTechnicalMessage(quint8 id);
    void newLzbValues(QVector<quint8> lmsToDecoder);
    void newSpeed(qreal speed);
    void newMtdIndicators(QVector<quint8> lmsToDecoder);
    void newAfbSoll(quint16 vSoll, bool visible);
    void newFzgVmaxTacho(quint16 speed);

public slots:
    void setMelderbild(uint8_t value);
    void setGrundZwangsbrmnsung(uint16_t value);
    void setZugart(uint8_t value);
    void setKlartextmeldungen(uint8_t value);
    void setZustandZugsicherung(uint16_t value);
    void clearData();
    //====
    void setLmHauptschalter(uint8_t value);
    void setStatusStromabnehmer(uint8_t value);
    void setLmSchleudern(uint8_t value);
    void setLmGleiten(uint8_t value);
    void setLmUhrzeitDigital(uint8_t value);
    void setLm1000Hz(uint8_t value);
    void setErsatzdatenWirksam(uint8_t value);
    void setLm500Hz(uint8_t value);
    void setLmBefehl(uint8_t value);
    void setLzbStoerschalter(uint8_t value);
    void setLzbZustand(uint8_t value);
    void setEndeverfahren(uint8_t value);
    void setFalschfahrauftrag(uint8_t value);
    void setErsatzauftrag(uint8_t value);
    void setVorsichtsauftrag(uint8_t value);
    void setLzbNothalt(uint8_t value);
    void setLzbElAuftrag(uint8_t value);
    void setLmE40(uint8_t value);
    void setLmB(uint8_t value);
    void setLmUe(uint8_t value);
    void setLmG(uint8_t value);
    void setLmEl(uint8_t value);
    void setLmV40(uint8_t value);
    void setLmS(uint8_t value);
    void setLmGnt(uint8_t value);
    void setLmGntUe(uint8_t value);
    void setLmGntG(uint8_t value);
    void setLmGntS(uint8_t value);
    void setLmSDeleyed();
    void setLmPruefStoer(uint8_t value);
    void setFahrtUeberLlzbHaltPerBefehl(uint16_t value);
    void setUebertragungsausfall(uint16_t value);
    void setVIst(uint16_t value);
    void setVSoll(int16_t value);
    void setVZiel(int16_t value);
    void setZielweg(int16_t value);
    void setAfbSoll(uint16_t value);
    void setAfbAn(bool value);
    void setFzgVMax(uint16_t value);
    void setZugbeeinflussungssystem(QString value);
    void makeLzbLmDatagram();
    //====
};

#endif // ZUSIINDICATOR_H

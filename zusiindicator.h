#ifndef ZUSIINDICATOR_H
#define ZUSIINDICATOR_H

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <era.h>
#include "db.h"
class zusiIndicator : public QObject
{
    Q_OBJECT
private:
    uint8_t indicators[64];
    uint8_t melderbild = 0, melderbildOld = 0, lm1000Hz = 0, lm85 = 0, lm70 = 0, lm55 = 0, zugart = 0, klartextmeldungen = 0, ktp = 0, lmBefehl = 0, lm500Hz = 0, lmS = 0, lmSDelayed = 0, lmH = 0, lmE40 = 0, lmB = 0, lmUe = 0, lmG = 0, lmEl = 0, lmEnde = 0,
    lmHauptschalter = 0, lmGetriebe = 0, lmSchleudern = 0, lmGleiten = 0, lmMgBremse = 0, lmHBremse = 0, lmRBremse = 0, lmHochabbremsung = 0,
    lmSchnellbremsung = 0, lmUhrzeitDigital = 0, lmDrehzahlverstellung = 0, lmFahrtrichtungVor = 0, lmFahrtrichtungZurueck = 0,
    lmFahrtrichtungM = 0, Entgleisung = 0, StwgHauptschalter = 0, lmSifa = 0, SifaHupe = 0, SifaHauptschalter = 0, SifaStoerschalter = 0,
    SifaLuftabsperrhahn = 0, PzbHauptschalter = 0, PzbStoerschalter = 0, PzbLuftabsperrhahn = 0, lmZugartU = 0, lmZugartM = 0,
    lmZugartO = 0, ErsatzdatenWirksam = 0, PzbHupe = 0, lmZugartLinksSBahn = 0, lmZugartSBahn = 0, lmZugartRechtsSBahn = 0,
    LzbStoerschalter = 0, LzbZustand = 0, Falschfahrauftrag = 0, Vorsichtauftrag = 0, LzbNothalt = 0, LzbRechnerausfall = 0,
    LzbElAuftrag = 0, lmEL = 0, lmV40 = 0, lmPruefStoer = 0, stromabn1Oben = 0, stromabn2Oben = 0, stromabn3Oben = 0, stromabn4Oben = 0,
    stromabn1Hebend = 0,stromabn2Hebend = 0,stromabn3Hebend = 0,stromabn4Hebend = 0, CirEelkeModus = 0, MfaAnzeigemodusZugdaten = 0,FunktionspruefungLaeuft = 0, AlleMelderBlinken = 0,
    AnzeigeDerFuehrungsgroessen = 0, lmBAnUndUeAus = 0, ZwangsbremsungAktiv = 0, lmV40Roh = 0;
    uint16_t zustandZugsicherung = 0,  Uebertragungsausfall = 0, FahrtUeberLlzbHaltPerBefehl = 0, afbSoll = 0;
    float vZiel = 0, zielweg = 0, vSoll = 0, vIst = 0;
    quint8 lastLimitMessage;
    bool restriktiv = false;
    bool afbAn = false;
    bool ersatzauftrag = false, falschfahrauftrag = false, lzbNothalt = false, lzbHaltUeberfahren = false;
    QTimer *lmElBlinkTestTimer;

private slots:
    void calcLmBlau();
    void calcPzbTextmessages();
    void sentSpetLimitMessage(quint8 limit);
    //void makeLzbAnalogDatagram();
    void makeLzbLmDatagram();
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
    void setZustandZugsicherung(uint16_t value);
    void setGrundZwangsbrmnsung(uint16_t value);
    void setZugart(uint8_t value);
    void setKlartextmeldungen(uint8_t value);

    //====
    void setLmHauptschalter(uint8_t value);
    void setStatusStromabnehmer(uint8_t value);
    void setLmGetriebe(uint8_t value);
    void setLmSchleudern(uint8_t value);
    void setLmGleiten(uint8_t value);
    void setLmMgBremse(uint8_t value);
    void setLmHBremse(uint8_t value);
    void setLmRBremse(uint8_t value);
    void setLmHochabbremsung(uint8_t value);
    void setLmSchnellbremsung(uint8_t value);
    void setLmUhrzeitDigital(uint8_t value);
    void setLmDrehzahlverstellung(uint8_t value);
    void setLmFahrtrichtungVor(uint8_t value);
    void setLmFahrtrichtungZurueck(uint8_t value);
    void setLmFahrtrichtungM(uint8_t value);
    void setEntgleisung(uint8_t value);
    void setStwgHauptschalter (uint8_t value);
    void setLmSifa(uint8_t value);
    void setSifaHupe(uint8_t value);
    void setSifaHauptschalter(uint8_t value);
    void setSifaStoerschalter(uint8_t value);
    void setSifaLuftabsperrhahn(uint8_t value);
    void setPzbHauptschalter(uint8_t value);
    void setPzbStoerschalter(uint8_t value);
    void setPzbLuftabsperrhahn(uint8_t value);
    void setLm1000Hz(uint8_t value);
    void setLmZugartU(uint8_t value);
    void setLmZugartM(uint8_t value);
    void setLmZugartO(uint8_t value);
    void setErsatzdatenWirksam(uint8_t value);
    void setPzbHupe(uint8_t value);
    void setLm500Hz(uint8_t value);
    void setLmBefehl(uint8_t value);
    void setLmZugartLinksSBahn(uint8_t value);
    void setLmZugartSBahn(uint8_t value);
    void setLmZugartRechtsSBahn(uint8_t value);
    void setLzbStoerschalter(uint8_t value);
    void setLzbZustand(uint8_t value);
    void setEndeverfahren(uint8_t value);
    void setFalschfahrauftrag(uint8_t value);
    void setErsatzauftrag(uint8_t value);
    void setVorsichtsauftrag(uint8_t value);
    void setLzbNothalt(uint8_t value);
    void setLzbRechnerausfall(uint8_t value);
    void setLmEnde(uint8_t value);
    void setLzbElAuftrag(uint8_t value);
    void setLmH(uint8_t value);
    void setLmE40(uint8_t value);
    void setLmB(uint8_t value);
    void setLmUe(uint8_t value);
    void setLmG(uint8_t value);
    void setLmEl(uint8_t value);
    void setLmV40(uint8_t value);
    void setLmS(uint8_t value);
    void setLmSDeleyed();
    void setLmPruefStoer(uint8_t value);
    void setFahrtUeberLlzbHaltPerBefehl(uint16_t value);
    void setUebertragungsausfall(uint16_t value);
    void setVIst(float value);
    void setVSoll(float value);
    void setVZiel(float value);
    void setZielweg(float value);
    void setAfbSoll(uint16_t value);
    void setAfbAn(bool value);
    void setCirEelkeModus(uint8_t value);
    void setMfaAnzeigemodusZugdaten(uint8_t value);
    void setFunktionspruefungLaeuft(uint8_t value);
    void setAlleMelderBlinken(uint8_t value);
    void setAnzeigeDerFuehrungsgroessen(uint8_t value);
    void setLmBAnUndUeAus(uint8_t value);
    void setZwangsbremsungAktiv(uint8_t value);
    void setFzgVMax(uint16_t value);
    //====
};

#endif // ZUSIINDICATOR_H

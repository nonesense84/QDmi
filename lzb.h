#ifndef LZB_H
#define LZB_H

#include <QObject>
#include <QColor>
#include <QTimer>
#include "db.h"
class lzb : public QObject
{
    Q_OBJECT
public:
    lzb();


public slots:
    void setStates(QVector<quint8> states);
    void setAnalogValues(QVector<quint8> values);
    void setTextUsing(quint8 useAutomText);
    void setZusiAsDataSource(bool value);
    void setVAct(quint16 V);

private slots:
    void addOrRemoveMessage(quint8 message, bool add);
    void addIndicator(quint8 indId, quint8 blinking, bool invers);
    void removeIndicator(quint8 indId);
    void sentSpetLimitMessage(quint8 limit);
    void resetExpectWt();
    void resetPanto();
    void resetHs();

signals:
    void newTextMessage(QString text, QColor forColor, QColor bgColor, quint8 id);
    void removeMessage(quint8 id);
    void newIcon1(QString lmOn, QString lmOff);
    void newIcon2(QString lmOn, QString lmOff);
    void newIcon3(QString lmOn, QString lmOff);
    void newIcon4(QString lmOn, QString lmOff);
    void newIcon5(QString lmOn, QString lmOff);
    void newIcon6(QString lmOn, QString lmOff);
    void newIcon7(QString lmOn, QString lmOff);
    void newIconC9(QString lmOn, QString lmOff);
    void newIconG10(QString lmOn, QString lmOff);
    void newIconBehav1(bool anabled, quint8 freq, bool inverse);
    void newIconBehav2(bool anabled, quint8 freq, bool inverse);
    void newIconBehav3(bool anabled, quint8 freq, bool inverse);
    void newIconBehav4(bool anabled, quint8 freq, bool inverse);
    void newIconBehav5(bool anabled, quint8 freq, bool inverse);
    void newIconBehav6(bool anabled, quint8 freq, bool inverse);
    void newIconBehav7(bool anabled, quint8 freq, bool inverse);
    void newIconBehavC9(bool anabled, quint8 freq, bool inverse);
    void newIconBehavG10(bool anabled, quint8 freq, bool inverse);
    void newVTarget(quint16 vZiel, bool visible);
    void newVPermit(quint16 vSoll, bool visible);
    void newOverspeed(bool warning);
    void newIntervenation(bool intervenation);
    void newVMaxReducing(bool vMaxReducing);
    void newTarDist(quint16 zielEntf, bool visible);

private:
    #define Keine 0
    #define ZugartO 3
    #define ZugartM 2
    #define ZugartU 1
    #define ZugartOBlink 4
    #define ZugartMBlink 5
    #define ZugartUBlink 6
    #define Tausend 1
    #define Fuenfhundert 2
    #define Zwangsbremsung 3
    #define Stoerbetrieb 4
    bool useTxtMsgByLm = false;
    bool zusiIsDataSource = false;
    bool blauBlink = false;
    bool intervenation = false;
    bool overspeed = false;
    bool geschwWechsel = false;
    bool gegenGlAuftr = false;
    bool wachTastErw =  false;
    bool pantoSenken = false;
    bool pantoHeben = false;
    bool hsAus = false;
    bool hsAn = false;
    bool ueAusfall = false;
    bool pzb90 = false;
    quint8 stoerschalter;
    quint8 zugart = 2;
    quint8 zustLmBlau;
    quint8 magnLm;
    quint8 actState;
    quint8 lastLimitMessage = 255;
    quint8 indicatorField[7] = {255,255,255,255,255,255,255};
    quint8 indicatorFieldBehav[7] = {255,255,255,255,255,255,255};
    quint8 emittedMessages[69];
    quint8 actMessages[3];
    quint16 vAct = 0;
    quint16 vPerm = 0;
    quint16 vDest = 0;
};

#endif // LZB_H

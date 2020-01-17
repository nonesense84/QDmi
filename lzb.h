#ifndef LZB_H
#define LZB_H

#include <QObject>
#include <QColor>
#include <QDebug>
#include "db.h"
class lzb : public QObject
{
    Q_OBJECT
public:
    lzb();


public slots:
    void setStates(QVector<quint8> states);
    void setAnalogValues(QVector<quint8> values);
    void setTextUsing(bool useAutomText);
    void setVAct(qreal V);

private slots:
    void addIndicator(quint8 indId, quint8 blinking, bool invers);
    void removeIndicator(quint8 indId);

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
    void newIconBehav1(bool anabled, quint8 freq, bool inverse);
    void newIconBehav2(bool anabled, quint8 freq, bool inverse);
    void newIconBehav3(bool anabled, quint8 freq, bool inverse);
    void newIconBehav4(bool anabled, quint8 freq, bool inverse);
    void newIconBehav5(bool anabled, quint8 freq, bool inverse);
    void newIconBehav6(bool anabled, quint8 freq, bool inverse);
    void newIconBehav7(bool anabled, quint8 freq, bool inverse);
    void newIconBehavC9(bool anabled, quint8 freq, bool inverse);
    void newVTarget(quint16 vZiel, bool visible);
    void newVPermit(quint16 vSoll, bool visible);
    void newOverspeed(qreal vOverspeed, bool forcedBrake, bool warning);
    void newTarDist(quint16 zielEntf, bool visible);
    void gotLzbMessage();

private:
    #define Keine 0
    #define ZugartO 1
    #define ZugartM 2
    #define ZugartU 3
    #define ZugartOBlink 4
    #define ZugartMBlink 5
    #define ZugartUBlink 6
    #define Tausend 1
    #define Fuenfhundert 2
    #define Zwangsbremsung 3
    #define Stoerbetrieb 4
    bool useTxtMsgByLm = false;
    bool blauBlink = false;
    bool zwangsbremsung = false;
    bool tausendBeinfl = false;
    bool fuenfhuBeinfl = false;
    bool zweitauBeinfl = false;
    bool gue = false;
    bool restriktiv = false;
    //bool showLzb;
    quint8 stoerschalter;
    quint8 zugart = 2;
    quint8 zustLmBlau;
    quint8 magnLm;
    quint8 actState;
    quint8 lastLimitMessage;
    quint8 indicatorField[7] = {255,255,255,255,255,255,255};
    quint8 actMessages[3];
    QString messages[63];
    qreal vAct = 0;
    quint16 vPerm;
    quint16 vDest;



private slots:
    void sentSpetLimitMessage(quint8 limit);

};

#endif // LZB_H
#ifndef LZB_H
#define LZB_H
#include <QDebug>
#include <QObject>
#include <QColor>
#include <QTimer>
//#define vMAuffordZDE  50    // Maximum speed, till "Zugdaten eingeben" or 55/70/85 is blinking
#define levelPzbLzbNtc  6
#define levelUnbefinde  0
//# define indOff 0
//# define indOn 1
//# define indBlink 5
class lzb : public QObject
{
    Q_OBJECT
public:
    lzb();
    enum class deviceTypePlzb: quint8{
        I60                   = 1,
        I60_m_R2              = 2,
        I60_m_ER24            = 3,
        I60R                  = 4,
        I60R_m_PZB90          = 5,
        I80                   = 6,
        I80_m_PZB90           = 7,
        PZ80                  = 8,
        PZ80R                 = 9,
        PZ80R_m_PZB90         = 10
    };Q_ENUM(deviceTypePlzb)
    enum class ModusWirksameZugdaten: quint8{
        undefiniert           = 0,
        Grunddaten            = 4,
        Ersatzzugdaten        = 5,
        Normalbetrieb         = 6
    };Q_ENUM(ModusWirksameZugdaten)
    enum class ZustandZugbeeinflussung: quint16{
        Ausgeschaltet         = 1,
        abgeschaltet_gestoert = 2,
        HLL_kl_3bar           = 3,
        Aufforderung_ZDE      = 4,
        Normalbetrieb         = 5,
        Prueflauf             = 6,
        Prueflauf_Ack_erw     = 7
    };Q_ENUM(ZustandZugbeeinflussung)
    enum class  Systemstatus: quint8{
        Ausgeschaltet         = 0,
        Abgeschaltet          = 1,
        Unterdrueckt          = 2,
        Aktiv                 = 3
    };Q_ENUM(Systemstatus)
    enum class  Luftabsperrhahn: quint8{
        abgesperrt            = 1,
        offen                 = 2,
    };Q_ENUM(Luftabsperrhahn)
    enum class  Stoerschalter: quint8{
        abgeschaltet          = 1,
        eingeschaltet         = 2,
    };Q_ENUM(Stoerschalter)
    enum class Zwangsbremsungen: quint8{
        keine_Zwangsbremsung                                =  0,
        Wachsam                                             =  1,
        Geschwindigkeitsueberschreitung_1000Hz              =  2,
        Geschwindigkeitsueberschreitung_500Hz               =  3,
        Beinflussung_2000Hz                                 =  4,
        Kein_Halt_nach_Befreiung_aus_Zwangsbremsung         =  5,
        Fahrzeug_vMax_ueberschritten                        =  6,
        Funktionspruefung                                   =  7,
        Unberechtigtige_Befreiung_vor_500Hz                 =  8,
        LZB_Halt_ueberfahren                                =  9,
        LZB_Rechnerausfall                                  = 10,
        LZB_Nothalt_ueberfahren                             = 11,
        Uebertragungsausfall_in_verdeckter_Aufnahme         = 12,
        Geschwindigkeitsueberschreitung_nach_LZB_Ue_Ausfall = 13,
        Richtungsschalter_verlegt                           = 14,
        LZB_Rueckrollueberwachung                           = 25,
        LZB_Ueberschreitung_200m_nach_Befehl_40_blinkend    = 26,
        Allgemeine_Stoerung                                 = 27,
        Stromversorgung_fehlt                               = 28
    };Q_ENUM(Zwangsbremsungen)
    enum class LzbMelderbild: quint8{
        Normalzustand                           = 0,
        NichRestriktiv1000Hznach700m            = 1,
        Restriktiv                              = 2,
        RestriktivMit1000Hz                     = 3,
        RestriktivMit500Hz                      = 4,
        PruefablaufNachLZBUebertragungsausfall  = 5
    };Q_ENUM(LzbMelderbild)
    enum class LzbZustaende: quint8{
        Keine_LZB_Fuehrung   = 0,
        Normale_LZB_Fahrt    = 1,
        Nothalt              = 2,
        LZB_Halt_ueberfahren = 3,
        Rechnerausfall       = 4,
        Nachfahrauftrag      = 5,
        Funktionspruefung    = 6
    };Q_ENUM(LzbZustaende)
    enum class LzbUebertragungsausfaelle: quint8{
        Kein_Ausfall                                          = 0,
        Eingeleitet                                           = 1,
        Ue_blinkt                                             = 2,
        Erste_Quittierung_erfolgt                             = 3,
        Bedingung_fuer_zw_Quittierung_gegeben                 = 4,
        Zweite_Quittierung_erfolgt                            = 5,
        Ausfall_nach_verdeckter_LZB_Aufnahme_CE               = 6,
        Ausfall_nach_verdeckter_LZB_Aufnahme_CE_Befehl_blinkt = 7
    };Q_ENUM(LzbUebertragungsausfaelle)
    enum class LzbElAuftraege: quint8{
        Kein_Auftrag         = 0,   // Not part of Zusi spec
        Hauptschalter_aus    = 1,
        Stromabnehmer_senken = 2,
        Hauptschalter_Ein    = 3,   // Not part of Zusi spec
        Stromabnehmer_heben  = 4,   // Not part of Zusi spec
        El_erledigt          = 5    // Not part of Zusi spec
    };Q_ENUM(LzbElAuftraege)

    enum class indicator: quint8{
        Off           = 0,
        On            = 1,
        BlinkSlow     = 3,
        BlinkFast     = 5,
        BlinkSlowInv  = 11,
        BlinkFastInv  = 13,
    };Q_ENUM(indicator)
    enum class Zugarten: quint8{
        Keine   = 0,
        Obere = 3,
        Mittlere = 2,
        Untere = 1
    };Q_ENUM(Zugarten)
public slots:
    void setIndicators(QVector<quint8> states);
    void setAnalogValues(QVector<quint8> values);
    void setTextUsing(quint8 useAutomText);
    void setZusiTextUsing(quint8 klartextEnumZusi);
    void setZusiAsDataSource(bool value);
    void setVAct(quint16 V);
    void setSilent(bool silient);
    void setBrakeFromEtcs(bool brake);

private slots:
    void addIndicator(quint8 indId, quint8 blinking, bool invers);
    void interpretStatesByLm();
    void interpretSpeedlimitsByLm();
    void removeIndicator(quint8 indId);
    void checkPzb90();
  //bool isOn(indicator state);
  //bool isOff(indicator state);
    bool isBlinking(indicator state);
    void setGrundZwangsbremnsung(Zwangsbremsungen value);
    void setZustandZugbeeinflussung(ZustandZugbeeinflussung value);
    void setModusWirksameZugdaten(ModusWirksameZugdaten wirks, bool thr1, bool thr2, bool thr3);
    void setLzbZustand(LzbZustaende value);
    void setBremseinsatzpunkt();
    void setEndeverfahren(uint8_t value);
    void setErsatzauftrag(uint8_t value);
    void setFalschfahrauftrag(uint8_t value);
    void setVorsichtsauftrag(uint8_t value);
    void setLzbNothalt(uint8_t value);
    void setLzbElAuftrag(LzbElAuftraege value);
    void setIndusiStoerschalter(uint8_t value);
    void setLzbStoerschalter(uint8_t value);
    void setPlzbLuftabsperrhahn(Luftabsperrhahn value);
    void setSystemstatusLzb(Systemstatus value);
    void setSystemstatusPzb(Systemstatus value);
    void setFahrtUeberLlzbHaltPerBefehl(uint8_t value);
    void setUebertragungsausfall(LzbUebertragungsausfaelle value, uint8_t vZiel);
    void setDeviceTypePlzb(deviceTypePlzb type);

signals:
    void newLzbTextMessage(QString text, QColor forColor, QColor bgColor, quint8 id);
    void removeMessage(quint8 id);
    void newIcon1(QString lmOn, QString lmOff);
    void newIcon2(QString lmOn, QString lmOff);
    void newIcon3(QString lmOn, QString lmOff);
    void newIcon4(QString lmOn, QString lmOff);
    void newIcon5(QString lmOn, QString lmOff);
    void newIcon6(QString lmOn, QString lmOff);
    void newIcon7(QString lmOn, QString lmOff);
    void newIconC9(QString lmOn, QString lmOff);
    void newIconG1(QString lmOn, QString lmOff);
    void newIconBehav1(bool anabled, quint8 freq, bool inverse);
    void newIconBehav2(bool anabled, quint8 freq, bool inverse);
    void newIconBehav3(bool anabled, quint8 freq, bool inverse);
    void newIconBehav4(bool anabled, quint8 freq, bool inverse);
    void newIconBehav5(bool anabled, quint8 freq, bool inverse);
    void newIconBehav6(bool anabled, quint8 freq, bool inverse);
    void newIconBehav7(bool anabled, quint8 freq, bool inverse);
    void newIconBehavC9(bool anabled, quint8 freq, bool inverse);
    void newIconBehavG1(bool anabled, quint8 freq, bool inverse);
    void newVTarget(quint16 vZiel, bool visible, bool fromEtcs);
    void newVPermit(quint16 vSoll, bool visible, bool fromEtcs);
    void newIntervenation(bool intervenation);
    void newVMaxReducing(bool vMaxReducing);
    void newVOverspeed(quint16 V);
    void newOverspeed(bool warning);
    void newTarDist(quint16 zielEntf, bool visibleBar, bool visibleDigital, bool fromEtcs);
    void lzbAvailable(bool available);
    void speedGt0(bool);
    void speedGt40(bool);
    void newActiveLevel(quint16 level);

private:
    //#define ZugartOBlink 4
    //#define ZugartMBlink 5
    //#define ZugartUBlink 6
    #define Tausend 1
    #define Fuenfhundert 2
    #define Zwangsbremse 3
    #define Stoerbetrieb 4
    bool staySilent = false;
    bool useTxtMsgByLm = false;
    bool zusiIsDataSource = false;
    bool blauBlink = false;
    bool ZB = false;
    bool ZB_changed = false;
    bool overspeed = false;
    bool geschwWechsel = false;
    bool bremseinsatzpunkt = false;
    bool pzb90 = false;
    bool brakeFromEtcs = false;
    bool tausendBeinflRe=false;
    bool fuenfhuBeinflRe=false;
    bool fuenfhuBeinflNr=false;
    bool zweitauBeinfl=false;
    bool restriktiv=false;
    bool auffordZde = false; // FIME: Benötigt?
    bool vKl5 = true;
    bool vKl20 = true;
    bool vKl50 = true;

    quint8 indicatorField[7] = {255,255,255,255,255,255,255};
    quint8 indicatorFieldBehav[7] = {255,255,255,255,255,255,255};

    quint16 vAct = 0;
    quint16 vPerm = 0;
    quint16 vDest = 0;
    quint16 vOver = 0;
    Zugarten zugart = Zugarten::Keine;
    indicator lm1000Hz, lm85, lm70, lm55, lm95, lm75, lm60, lmIndusi, lmBefehl, lm500Hz, lmS, lmPzb,
        lmH, lmE40, lmB, lmUe, lmG,lmEl, lmEnde, lmEL, lmV40, lmGnt, lmGnt_Ue, lmGnt_G, lmGnt_S;
    quint8 endeverfahren = 0, lzbHaltueberf=0, falschfahrauftrag = 0, vorsichtauftrag = 0, lzbNothalt = 0;
    quint16 FahrtUeberLlzbHaltPerBefehl = 0;
    LzbMelderbild melderbild = melderbild;
    ModusWirksameZugdaten     modWirksZugd         = ModusWirksameZugdaten::Normalbetrieb;
    ZustandZugbeeinflussung   zustZugbeeinflussung = ZustandZugbeeinflussung::Ausgeschaltet;
    Systemstatus              systemstatusLzb      = Systemstatus::Ausgeschaltet;
    Systemstatus              systemstatusPzb      = Systemstatus::Ausgeschaltet;
    Zwangsbremsungen          grundZwangsbrmnsung  = Zwangsbremsungen::keine_Zwangsbremsung;
    LzbZustaende              lzbZustand           = LzbZustaende::Keine_LZB_Fuehrung;
    LzbUebertragungsausfaelle Uebertragungsausfall = LzbUebertragungsausfaelle::Kein_Ausfall;
    LzbElAuftraege            lzbElAuftrag         = LzbElAuftraege::Kein_Auftrag;
    deviceTypePlzb            plzbTyp;
    bool lzbVorhanden;
    bool ktp = false;
    bool vMaxReducing = false;
    quint8 zusiKtp=255; // Give a strange value to guarantee initialization when receiving the first Zusi data.
    quint8 userKtp=0;
    bool ersatzauftrag = false;

    enum class texte : int {
        Zwangsbremsung = 0,
        LZB_Halt_ueberfahren,
        LZB_Nothalt,
        LZB_Nothalt_ueberfahren,
        LZB_Nothaltschalter_betaetigt,
        Geschwindigkeitsueberschreitung,
        Vmax_100kmh,
        V_Ueberwachung_150kmh,
        V_Ueberwachung_140kmh,
        V_Ueberwachung_130kmh,
        V_Ueberwachung_120kmh,
        V_Ueberwachung_110kmh,
        V_Ueberwachung_100kmh,
        V_Ueberwachung_90kmh,
        V_Ueberwachung_85kmh,
        V_Ueberwachung_80kmh,
        V_Ueberwachung_70kmh,
        V_Ueberwachung_60kmh,
        V_Ueberwachung_55kmh,
        V_Ueberwachung_50kmh,
        V_Ueberwachung_45kmh,
        V_Ueberwachung_40kmh,
        V_Ueberwachung_35kmh,
        V_Ueberwachung_30kmh,
        V_Ueberwachung_25kmh,
        V_Ueberwachung_20kmh,
        V_Ueberwachung_10kmh,
        Ggf_weitere_Stromabn_beachten,
        LZB_Ende,
        LZB_Vorsichtauftrag,
        LZB_Ersatzauftrag,
        LZB_Gegengleisfahrauftrag,
        LZB_mit_Stoerschalter_abgeschaltet,
        PZB_LZB_Luftabsperrhahn_zu,
        PZB_LZB_ausgeschaltet,
        LZB_Stoerung,
        LZB_Uebertragungsausfall,
        Funktionspruefung_laeuft,
        Fdl_beteiligen,
        Mit_Betriebsbremsung_anhalten,
        Stromabnehmer_senken,
        Stromabnehmer_heben,
        Hauptschalter_ausschalten,
        Hauptschalter_einschalten,
        Wachsamkeitstaste_betaetigen,
        Freitaste_betaetigen,
        Zugdaten_eingeben,
        Abbremsen_unter_85kmh,
        Abbremsen_unter_80kmh,
        Abbremsen_unter_70kmh,
        Abbremsen_unter_60kmh,
        Abbremsen_unter_55kmh,
        Abbremsen_unter_50kmh,
        Abbremsen_unter_40kmh,
        Abbremsen_unter_30kmh,
        Abbremsen_unter_20kmh,
        Abbremsen_unter_10kmh,
        Signale_und_Fahrplan_beachten,
        Geschwindigkeitswechsel,
        Grunddaten_wirksam,
        Wirksame_Zugdaten,
        Bremseinsatzpunkt_erwarten,
        BRH_kl_60_eingegeben,
        Unberechtigtes_Befreien,
        WT_nicht_zeitgerecht_betaetigt,
        Hz2000_Beeinflussung,
        PZB_mit_Stoerschalter_abgeschaltet,
        PZB_Stoerung,
        Zwangsbremsausgabe_Stoerung,
        Ersatzzugdaten_wirksam,
        V_Ueberwachung_95kmh,
        V_Ueberwachung_75kmh,
        Count
    };
    const QColor textBgColors[74]{
        QColor(191,0,2), QColor(191,0,2), QColor(191,0,2), QColor(191,0,2), QColor(191,0,2), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0),
        QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0),
        QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0),
        QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), Qt::cyan, Qt::cyan, Qt::cyan, Qt::cyan, Qt::cyan,
        Qt::cyan, Qt::cyan, Qt::cyan, Qt::cyan, Qt::cyan, Qt::cyan, Qt::cyan, Qt::cyan, Qt::cyan, Qt::cyan,
        Qt::cyan, Qt::cyan, Qt::cyan, Qt::cyan, Qt::cyan, Qt::gray, Qt::gray, Qt::gray, Qt::gray,
        QColor(255,219,0),QColor(255,219,0),QColor(255,219,0),QColor(255,219,0),QColor(255,219,0),Qt::gray, QColor(191,0,2),Qt::gray, QColor(255,219,0), QColor(255,219,0), QColor(255,219,0)
    };
    const QColor textFontColors[74]{
        Qt::white, Qt::white, Qt::white, Qt::white, Qt::white, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black,
        Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black,Qt::black,
        Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black,Qt::black,
        Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black,
        Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black,
        Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black,
        Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black
    };
    const QString indicatorFiles[26][2] = {
        {   ":/icons/LmB_on.svg",       ":/icons/LmB_off.svg",},        // 0
        {   ":/icons/Lm85_on.svg",      ":/icons/Lm85_off.svg",},       // 1
        {   ":/icons/Lm70_on.svg",      ":/icons/Lm70_off.svg",},       // 2
        {   ":/icons/Lm55_on.svg",      ":/icons/Lm55_off.svg",},       // 3
        {   ":/icons/LmPzb_on.svg",     ":/icons/LmPzb_off.svg",},      // 4
        {   ":/icons/LmS_on.svg",       ":/icons/LmS_off.svg",},        // 5
        {   ":/icons/LmH_on.svg",       ":/icons/LmH_off.svg",},        // 6
        {   ":/icons/LmG_on.svg",       ":/icons/LmG_off.svg",},        // 7
        {   ":/icons/LmE40_on.svg",     ":/icons/LmE40_off.svg",},      // 8
        {   ":/icons/LmV40_on.svg",     ":/icons/LmV40_off.svg",},      // 9
        {   ":/icons/LmBef40_on.svg",   ":/icons/LmBef40_off.svg",},    // 10
        {   ":/icons/Lm1000Hz_on.svg",  ":/icons/Lm1000Hz_off.svg",},   // 11
        {   ":/icons/Lm500Hz_on.svg",   ":/icons/Lm500Hz_off.svg",},    // 12
        {   ":/icons/LmEL_on.svg",      ":/icons/LmEL_off.svg",},       // 13
        {   ":/icons/LmUe_on.svg",      ":/icons/LmUe_off.svg",},       // 14
        {   ":/icons/LmEnde_on.svg",     ":/icons/LmEnde_off.svg",},    // 15
        {   ":/icons/LmGnt_on.svg",     ":/icons/LmGnt_off.svg",},      // 16
        {   ":/icons/LmUe_Gnt_on.svg",     ":/icons/LmUe_Gnt_off.svg",},// 17
        {   ":/icons/LmG_Gnt_on.svg",     ":/icons/LmG_Gnt_off.svg",},  // 18
        {   ":/icons/LmS_Gnt_on.svg",     ":/icons/LmS_Gnt_off.svg",},  // 19
        {   ":/icons/Lm95_on.svg",      ":/icons/Lm95_off.svg",},       // 20
        {   ":/icons/Lm75_on.svg",      ":/icons/Lm75_off.svg",},       // 21
        {   ":/icons/Lm60_on.svg",      ":/icons/Lm60_off.svg",},       // 22
        {   ":/icons/LmIndusi_on.svg",  ":/icons/LmIndusi_on.svg",},    // 23
        {   ":/icons/ST_01.svg",        ":/icons/ST_01.svg",},          // 24 Forced brake
    };
    texte lastLimitMessage=texte::V_Ueberwachung_150kmh; // We need to take an unpropper message as initial
    //static const std::array<QString, static_cast<int>(texte::Count)> MeldungsTexte;
    //static const std::array<QColor, static_cast<int>(texte::Count)> MeldungsFarbenBg;
    //static const std::array<QColor, static_cast<int>(texte::Count)> MeldungsFarbenTxt;

    static const QVector<QString>  MeldungsTexte;
    static const QVector<QColor> MeldungsFarbenBg;
    static const QVector<QColor> MeldungsFarbenTxt;
    void emitTextmessage(texte text);
    void emitLimitTextmessage(texte text);
    void removeTextmessage(texte text);
    void removeTextmessageDelayed(texte text, quint16 delay);
};


#endif // LZB_H

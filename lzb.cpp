#include "lzb.h"

const QVector<QString>  lzb::MeldungsTexte = {
    "Zwangsbremsung",
    "LZB-Halt überfahren",
    "LZB-Nothalt",
    "LZB-Nothalt überfahren",
    "LZB-Nothaltschalter betätigt",
    "Geschwindigkeitsüberschreitung",
    "Vmax 100 km/h",
    "V-Überwachung 150 km/h",
    "V-Überwachung 140 km/h",
    "V-Überwachung 130 km/h",
    "V-Überwachung 120 km/h",
    "V-Überwachung 110 km/h",
    "V-Überwachung 100 km/h",
    "V-Überwachung 90 km/h",
    "V-Überwachung 85 km/h",
    "V-Überwachung 80 km/h",
    "V-Überwachung 70 km/h",
    "V-Überwachung 60 km/h",
    "V-Überwachung 55 km/h",
    "V-Überwachung 50 km/h",
    "V-Überwachung 45 km/h",
    "V-Überwachung 40 km/h",
    "V-Überwachung 35 km/h",
    "V-Überwachung 30 km/h",
    "V-Überwachung 25 km/h",
    "V-Überwachung 20 km/h",
    "V-Überwachung 10 km/h",
    "Ggf. weitere Stromabn. beachten",
    "LZB-Ende",
    "LZB-Vorsichtauftrag",
    "LZB-Ersatzauftrag",
    "LZB-Gegengleisfahrauftrag ",
    "LZB mit Störschalter abgeschaltet",
    "PZB-/LZB-Luftabsperrhahn zu",
    "PZB-/LZB ausgeschaltet",
    "LZB-Störung",
    "LZB-Übertragungsausfall",
    "Funktionsprüfung läuft",
    "Fdl beteiligen",
    "Mit Betriebsbremsung anhalten",
    "Stromabnehmer senken",
    "Stromabnehmer heben",
    "Hauptschalter ausschalten",
    "Hauptschalter einschalten",
    "Wachsamkeitstaste betätigen",
    "Freitaste betätigen",
    "Zugdaten eingeben",
    "Abbremsen unter 85 km/h",
    "Abbremsen unter 80 km/h",
    "Abbremsen unter 70 km/h",
    "Abbremsen unter 60 km/h",
    "Abbremsen unter 55 km/h",
    "Abbremsen unter 50 km/h",
    "Abbremsen unter 40 km/h",
    "Abbremsen unter 30 km/h",
    "Abbremsen unter 20 km/h",
    "Abbremsen unter 10 km/h",
    "Signale und Fahrplan beachten",
    "Geschwindigkeitswechsel",
    "Grunddaten wirksam",
    "Wirksame Zugdaten",
    "Bremseinsatzpunkt erwarten",
    "BRH < 60 eingegeben ",
    "Unberechtigtes Befreien",
    "WT nicht zeitgerecht betätigt",
    "2000 Hz Beeinflussung",
    "PZB mit Störschalter abgeschaltet ",
    "PZB Störung ",
    "Zwangsbremsausgabe Störung",
    "Ersatzzugdaten wirksam",
    "V-Überwachung 95 km/h",
    "V-Überwachung 75 km/h"
};
struct IndicatorIcon {
    QString onIcon;
    QString offIcon;
};

const QVector<QColor> lzb::MeldungsFarbenBg = {
    QColor(191,0,2), QColor(191,0,2), QColor(191,0,2), QColor(191,0,2), QColor(191,0,2), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0),
    QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0),
    QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0),
    QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), Qt::cyan, Qt::cyan, Qt::cyan, Qt::cyan, Qt::cyan,
    Qt::cyan, Qt::cyan, Qt::cyan, Qt::cyan, Qt::cyan, Qt::cyan, Qt::cyan, Qt::cyan, Qt::cyan, Qt::cyan,
    Qt::cyan, Qt::cyan, Qt::cyan, Qt::cyan, Qt::cyan, Qt::gray, Qt::gray, Qt::gray, Qt::gray,
    QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), Qt::gray, QColor(191,0,2), Qt::gray, QColor(255,219,0), QColor(255,219,0)
};

const QVector<QColor> lzb::MeldungsFarbenTxt = {
    Qt::white, Qt::white, Qt::white, Qt::white, Qt::white, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black,
    Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black,
    Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black,
    Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black,
    Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black,
    Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black,
    Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black
};
lzb::lzb(){
}
void lzb::setSilent(bool silient){
    staySilent = silient;
}
void lzb::setBrakeFromEtcs(bool brake){
    brakeFromEtcs = brake;
}
void lzb::setTextUsing(quint8 useAutomText){
    // 0: Allways
    // 1: Automatic
    // 2: Never
    userKtp = useAutomText;
    if(userKtp == 1){
        switch (zusiKtp) {
            case 0: ktp=false;return;
            case 1: ktp=false;return;
            case 2: ktp=false;return;
            case 3: ktp= true;return;
            case 4: ktp= true;return;
            default: ktp= true;return;
        }
    }
    else{
        ktp = userKtp==0;
    }
    if(!ktp){
        for(quint8 i = 0; i < MeldungsTexte.size(); i++){
            emit removeMessage(i);
        }
    }
    useTxtMsgByLm = ktp && !zusiIsDataSource;
}

void  lzb::setZusiTextUsing(quint8 klartextEnumZusi){
    // 0: Keine Klartextmeldungen möglich
    // 1: Klartextmeldungen möglich aber nicht aktiv
    // 2: Klartextmeldungen aktiv
    // 3: Nur Klartextmeldungen möglich
    if(zusiKtp == klartextEnumZusi)return;
    zusiKtp = klartextEnumZusi;
    if(userKtp == 1){
        switch (zusiKtp) {
            case 0: ktp=false;return;
            case 1: ktp=false;return;
            case 2: ktp=false;return;
            case 3: ktp= true;return;
            case 4: ktp= true;return;
            default: ktp= true;return;
        }
    }
    else{
        ktp = userKtp==0;
    }
    useTxtMsgByLm = false;
}

void lzb::setZusiAsDataSource(bool value){
    if(zusiIsDataSource != value){
        for(quint8 i = 0; i < MeldungsTexte.size(); i++){
            emit removeMessage(i);
        }
        removeTextmessage(lastLimitMessage);
    }
    zusiIsDataSource = value;
}

void lzb::setAnalogValues(QVector<quint8> values){
    if(staySilent)return;
    if(values.length()>=7){
        bool lzbShow = (values[0] & 0x0f) > 0;
        quint16 newVDest = static_cast<quint16>((values[2] << 8) + values[1]);
        quint16 newVPerm = static_cast<quint16>((values[4] << 8) + values[3]);
        quint16 distance = static_cast<quint16>((values[6] << 8) + values[5]);
        if(newVPerm <  vPerm   )vMaxReducing = true;
        if(newVPerm == newVDest)vMaxReducing = false;
        emit newVMaxReducing(vMaxReducing);
        vPerm = newVPerm;
        vOver = (static_cast<quint16>((values[4] << 8) + values[3])) + 9;
        if(newVDest < vDest && lzbZustand != LzbZustaende::Keine_LZB_Fuehrung){
            if(ktp)emitTextmessage(texte::Geschwindigkeitswechsel);
            geschwWechsel = true;
            QTimer::singleShot(5000, this, [this]() {
                removeTextmessage(texte::Geschwindigkeitswechsel);
            });
        }
        vDest = newVDest;
        if(vDest == vPerm) geschwWechsel = false;
        if(systemstatusLzb == Systemstatus::Aktiv){
            emit newVTarget(static_cast<quint16>((values[2] << 8) + values[1]), lzbShow, false);
            emit newVPermit(vPerm, lzbShow, false);
            emit newVOverspeed(vOver);
            emit newTarDist(distance, lzbShow, distance >= 4000, false);
        }
    }
}

void lzb::setVAct(quint16 V){
    if(staySilent)return;
    if(V  > 40 && vAct <= 40)emit speedGt40(true);
    if(V <= 40 && vAct  > 40)emit speedGt40(false);
    if(V  >  0 && vAct ==  0)emit speedGt0(true);
    if(V ==  0 && vAct  >  0)emit speedGt0(false);
    // Threshold for Grund- and Ersatzdaten. Certain text messages have to appear and disappear at soeme speed thresholds
    if(V  >  5 && vAct <=  5)setModusWirksameZugdaten(modWirksZugd, false,  true,  true);
    if(V  > 20 && vAct <= 20)setModusWirksameZugdaten(modWirksZugd, false, false,  true);
    if(V  > 50 && vAct <= 50)setModusWirksameZugdaten(modWirksZugd, false, false, false);
    if(V <=  5 && vAct  >  5)setModusWirksameZugdaten(modWirksZugd,  true,  true,  true);
    if(V <= 20 && vAct  > 20)setModusWirksameZugdaten(modWirksZugd, false,  true,  true);
    if(V <= 50 && vAct  > 50)setModusWirksameZugdaten(modWirksZugd, false, false,  true);
    vAct = V;
}

void lzb::setIndicators(QVector<quint8> states){
    lmB         = static_cast<indicator>(states[ 0]);  // IndLzbB
    lm85        = static_cast<indicator>(states[ 1]);  // IndLzbO
    lm70        = static_cast<indicator>(states[ 2]);  // IndLzbM
    lm55        = static_cast<indicator>(states[ 3]);  // IndLzbU
    lmPzb       = static_cast<indicator>(states[ 4]);  // IndLzbPzb', 'default': 1},
    lmS         = static_cast<indicator>(states[ 5]);  // IndLzbS
    lmH         = static_cast<indicator>(states[ 6] ); // IndLzbH
    lmG         = static_cast<indicator>(states[ 7] ); // IndLzbG
    lmE40       = static_cast<indicator>(states[ 8] ); // IndLzbE40
    lmV40       = static_cast<indicator>(states[ 9] ); // IndLzbV40
    lmBefehl    = static_cast<indicator>(states[10]);  // IndLzbBef40
    lm1000Hz    = static_cast<indicator>(states[11]);  // IndLzb1000
    lm500Hz     = static_cast<indicator>(states[12]);  // IndLzb500
    lmEl        = static_cast<indicator>(states[13]);  // IndLzbEl
    lmUe        = static_cast<indicator>(states[14]);  // IndLzbUe
    lmEnde      = static_cast<indicator>(states[15]);  // IndLzbEnd
    lmGnt       = static_cast<indicator>(states[16]);  // IndGnt
    lmGnt_Ue    = static_cast<indicator>(states[17]);  // IndGntUe
    lmGnt_G     = static_cast<indicator>(states[18]);  // IndGntG
    lmGnt_S     = static_cast<indicator>(states[19]);  // IndGntS
    lm95        = static_cast<indicator>(states[20]);  // IndLzbO
    lm75        = static_cast<indicator>(states[21]);  // IndLzbM
    lm60        = static_cast<indicator>(states[22]);  // IndLzbU
    lmIndusi    = static_cast<indicator>(states[23]);  // IndLzbPzb
    zugart      = static_cast<Zugarten>(states[25]);   // InfPzbZa
    setDeviceTypePlzb(static_cast<deviceTypePlzb>(states[24])); // InfLzbSystem
    checkPzb90();
    setZusiAsDataSource(states[26]);
    if(zusiIsDataSource){
        if(isBlinking(lm1000Hz) && states[41] == 0) states[41] = 255;
        setGrundZwangsbremnsung(        static_cast<Zwangsbremsungen>(states[41]));
        setModusWirksameZugdaten(       static_cast<ModusWirksameZugdaten>(states[27]), vKl5, vKl20, vKl50);
        setIndusiStoerschalter(         states[28]);
        setLzbStoerschalter(            states[39]);
        setPlzbLuftabsperrhahn(         static_cast<Luftabsperrhahn>(states[30]));
        setSystemstatusLzb(             static_cast<Systemstatus>(states[31]));
        setSystemstatusPzb(             static_cast<Systemstatus>(states[32]));
        setZustandZugbeeinflussung(     static_cast<ZustandZugbeeinflussung>(states[33]));
        setLzbZustand(                  static_cast<LzbZustaende>(states[34]));
        setLzbElAuftrag(                static_cast<LzbElAuftraege>(states[35]));
        setEndeverfahren(               states[36]);
        setErsatzauftrag(               states[37]);
        setFahrtUeberLlzbHaltPerBefehl( states[38]);
        setFalschfahrauftrag(           states[39]);
        setVorsichtsauftrag(            states[40]);
        setUebertragungsausfall(        static_cast<LzbUebertragungsausfaelle>(states[42]),
                                        states[43]);
        setZusiTextUsing(               states[44]);
        melderbild =                    static_cast<LzbMelderbild>(states[45]);
    }
    // EMB is shown as 500Hz with 1000Hz blinkng or as indicator S
    bool newZB = (isBlinking(lm1000Hz) && isBlinking(lm500Hz)) || lmS == indicator::On;
    ZB_changed = false;
    if (ZB != newZB) {
        ZB_changed = true;
        ZB = newZB;
        emit newIntervenation(ZB);
    }
    interpretSpeedlimitsByLm();
    setBremseinsatzpunkt();
    if (overspeed != (isBlinking(lmG))) {// Overspeed is shown as indicator G flashing
        overspeed = isBlinking(lmG);
        emit newOverspeed(overspeed);
    }
    if (!zusiIsDataSource)
        interpretStatesByLm();
    // Update some indicators in states, since they may...
    // ...have ben manipulated in interpretSpeedlimitsByLm()
    states[ 1] = static_cast<quint8>(lm85);
    states[ 2] = static_cast<quint8>(lm70);
    states[ 3] = static_cast<quint8>(lm55);
    states[ 4] = static_cast<quint8>(lmPzb);
    states[ 5] = static_cast<quint8>(lmS);
    states[11] = static_cast<quint8>(lm1000Hz);
    states[12] = static_cast<quint8>(lm500Hz);
    states[15] = static_cast<quint8>(lmEnde);
    states[20] = static_cast<quint8>(lm95);
    states[21] = static_cast<quint8>(lm75);
    states[22] = static_cast<quint8>(lm60);
    states[23] = static_cast<quint8>(lmIndusi);

    // Remove all not used indicators
    for (quint8 i = 0; i <= 23; ++i) if (states[i] == 0) removeIndicator(i);

    // Set new or changed indicators
    for (quint8 i = 0; i <= 23; ++i) {
        //if(useTxtMsgByLm && !zusiIsDataSource && i ==  1) i =  3;// Set all indicators but O/M/U (PZB90)
        //if(useTxtMsgByLm && !zusiIsDataSource && i == 22) i = 25;// Set all indicators but O/M/U (I60)
        if (states[i] > 0) addIndicator(i, (states[i] & 0x06) >> 1, (states[i] & 0x08) > 0);
    }
}
void lzb::interpretStatesByLm(){
    using I = indicator;
    // =========== Systemstatus PZB / LZB ========================================
    if(lm85 != I::Off || lm70 != I::Off || lm55 != I::Off || lm500Hz != I::Off || lm1000Hz != I::Off)
        setSystemstatusPzb(Systemstatus::Aktiv);
    else
        setSystemstatusPzb(Systemstatus::Unterdrueckt);
    if(lmUe != I::Off)
        setSystemstatusLzb(Systemstatus::Aktiv);
    else
        setSystemstatusLzb(Systemstatus::Unterdrueckt);
    if(!ktp)
        return;
    // =========== Zwangsbremsung (gehend) =======================================
    if (!ZB) setGrundZwangsbremnsung(Zwangsbremsungen::keine_Zwangsbremsung);
    // =========== Endeverfahren ==================================================
    switch(lmEnde){
    case I::Off:    setEndeverfahren(0); break;
    case I::On:     setEndeverfahren(2); break;
    default :       setEndeverfahren(1); break;
    }
    // =========== Erstatz- und Falschfahrauftrag  ================================
    switch(lmE40){
    case I::Off:    setFalschfahrauftrag(0);
                    setErsatzauftrag(0);
                    break;
    case I::On:     setErsatzauftrag(1);
                    break;
    default:        setFalschfahrauftrag(1);
                    removeTextmessageDelayed(texte::Wachsamkeitstaste_betaetigen, 5000);
    }
    // =========== Vorsichtauftrag ================================================
    switch(lmV40){
    case I::Off:    setVorsichtsauftrag(0);break;
    case I::On:     setVorsichtsauftrag(3);break;
    default:        setVorsichtsauftrag(1);break;
    }
    // =========== LZB Halt und Nothalt ===========================================
    switch(lmH){
    case I::Off:    setLzbNothalt(0);
                    break;
    case I::On:     setLzbNothalt(3);
                    break;
    default:        if( ZB)setLzbNothalt(2);
                    if(!ZB)setLzbNothalt(1);
                    break;
    }
    // =========== LZB EL Auftrag =================================================
    using E = LzbElAuftraege;
    switch (lmEl){
    case I::Off: setLzbElAuftrag(E::Kein_Auftrag);break;
    case I::On:  setLzbElAuftrag(E::Hauptschalter_aus);break;
    default:     setLzbElAuftrag(E::Stromabnehmer_senken);
    }
    // =========== Übertragungsausfall ============================================
    using A = LzbUebertragungsausfaelle;
    quint8 vZiel = 85;
    if(zugart == Zugarten::Mittlere) vZiel = 70;
    if(zugart == Zugarten::Untere) vZiel = 55;
    if (isBlinking(lmUe))
        setUebertragungsausfall(A::Ue_blinkt, vZiel);
    // ---------------------------------------------------------------------------
    else if((lmUe == I::Off) && (Uebertragungsausfall == A::Ue_blinkt))
        setUebertragungsausfall(A::Erste_Quittierung_erfolgt, vZiel);
    // ---------------------------------------------------------------------------
    else if((isBlinking(lm85) || isBlinking(lm75) || isBlinking(lm55)) && Uebertragungsausfall == A::Erste_Quittierung_erfolgt)
        setUebertragungsausfall(A::Bedingung_fuer_zw_Quittierung_gegeben, vZiel);
    // ---------------------------------------------------------------------------
    else if((lm85 == I::On || lm70 == I::On || lm55 == I::On) && Uebertragungsausfall == A::Bedingung_fuer_zw_Quittierung_gegeben)
        setUebertragungsausfall(A::Zweite_Quittierung_erfolgt, vZiel);
    else
        setUebertragungsausfall(Uebertragungsausfall, vZiel);
    // =========== Zwangsbremsung (kommend) =======================================
    if ( ZB) setGrundZwangsbremnsung(Zwangsbremsungen::Allgemeine_Stoerung);
}
void lzb::interpretSpeedlimitsByLm(){
    using I = indicator;
    using T = texte;
    using Z = Zugarten;
    using U = LzbUebertragungsausfaelle;
    if (!ktp) return;
    // =========== Geschwindigkeiten ==============================================
    restriktiv      = isBlinking(lm85) && isBlinking(lm70);
    blauBlink       = (isBlinking(lm85) + isBlinking(lm70) + isBlinking(lm55)) == 1;
    if(zusiIsDataSource && melderbild == LzbMelderbild::Normalzustand)  // Spezialfall AuffZDE behandeln
        blauBlink = false;
    tausendBeinflRe = lm500Hz  == I::Off && restriktiv;
    fuenfhuBeinflNr = lm1000Hz == I::Off && lm500Hz == I::On && !restriktiv;
    fuenfhuBeinflRe = lm1000Hz == I::Off && lm500Hz == I::On &&  restriktiv;
    zweitauBeinfl   = lmBefehl == I::On;
    bool zAO        = zugart   == Z::Obere;
    bool zAM        = zugart   == Z::Mittlere;
    bool zAU        = zugart   == Z::Untere;
    if(Uebertragungsausfall == U::Kein_Ausfall){
        if(zAO && blauBlink )       emitLimitTextmessage(T::V_Ueberwachung_85kmh);
        if(zAM && blauBlink )       emitLimitTextmessage(T::V_Ueberwachung_70kmh);
        if(zAU && blauBlink )       emitLimitTextmessage(T::V_Ueberwachung_55kmh);
        if(zAO && fuenfhuBeinflNr)  emitLimitTextmessage(T::V_Ueberwachung_45kmh);
        if(zAM && fuenfhuBeinflNr)  emitLimitTextmessage(T::V_Ueberwachung_35kmh);
        if(zAU && fuenfhuBeinflNr)  emitLimitTextmessage(T::V_Ueberwachung_25kmh);
        if(tausendBeinflRe)         emitLimitTextmessage(T::V_Ueberwachung_45kmh);
        if(fuenfhuBeinflRe)         emitLimitTextmessage(T::V_Ueberwachung_25kmh);
        if(zweitauBeinfl)           emitLimitTextmessage(T::V_Ueberwachung_40kmh);
    }
    // =========== Geschwindigkeitsueberschreitung ================================
    if(isBlinking(lmG))                emitTextmessage(  T::Geschwindigkeitsueberschreitung);
    if(lmG == I::Off && lmS == I::Off) removeTextmessage(T::Geschwindigkeitsueberschreitung);
    // =========== Leuchtnelder anpassen an Systeme mit Texten ====================
    if(!blauBlink && !fuenfhuBeinflNr && !fuenfhuBeinflRe && !zweitauBeinfl && !fuenfhuBeinflRe && !tausendBeinflRe)
        removeTextmessage(lastLimitMessage);
    if(restriktiv){
        lm85 = I::Off;
        lm70 = I::Off;
        switch(zugart){
        case Z::Obere:    lm85 = I::BlinkFast; break;
        case Z::Mittlere: lm70 = I::BlinkFast; break;
        case Z::Untere:   lm55 = I::BlinkFast; break;
        default: break;
        }
    }
    if(ZB){lm500Hz = I::Off;lm1000Hz = I::Off;lmS = I::On;}
}
// ================================================================================
void lzb::setGrundZwangsbremnsung(Zwangsbremsungen value) {
    //  Zwangsbremsung aktiv wegen:
    ///  0: keine Zwangsbremsung
    ///  1: Wachsam
    ///  2: 1000 Hz-Prüfung
    ///  3: 500 Hz-Prüfung
    ///  4: 2000 Hz
    ///  5: Kein Halt nach Befreiung aus Zwangsbremsung
    ///  6: Fahrzeug-v-Max überschritten
    ///  7: Funktionsprüfung
    ///  8: 500 Hz nach Befreiung
    ///  9: LZB-Halt überfahren
    /// 10: LZB-Rechnerausfall
    /// 11: LZB-Nothalt überfahren
    /// 12: Übertragungsausfall in verdeckter Aufnahme
    /// 13: Geschwindigkeitsüberschreitung nach LZB-Ü-Ausfall
    /// 14: Richtungsschalter verlegt
    /// 25: LZB-Rückrollüberwachung
    /// 26: LZB Überschreitung 200 m nach „Befehl 40 blinkt“
    /// 27: Allgemeine Störung
    /// 28: Stromversorgung fehlt
    using GR = Zwangsbremsungen;
    using LZ = LzbZustaende;
    using T = texte;
    using MWZ = ModusWirksameZugdaten;
    if (lzbZustand == LZ::LZB_Halt_ueberfahren)
        value = GR::LZB_Halt_ueberfahren;   // Workaround: Zusi does not set "Bremsgrund LZB-Halt überfahren (->9)". But luckily then "LzbZustand" is 3
    if (!ktp)
        value = GR::keine_Zwangsbremsung;   // Workaround: In case of EMB, we have to make space and remove any train data messages
    if (modWirksZugd != MWZ::Normalbetrieb)
        setModusWirksameZugdaten(MWZ::Normalbetrieb, vKl5, vKl20, vKl50);
    if (grundZwangsbrmnsung != value) {
        if (grundZwangsbrmnsung != GR::keine_Zwangsbremsung)
            removeTextmessage(lastLimitMessage);
        grundZwangsbrmnsung = value;
        lmS = indicator::On; // Workaround: Zusi does not set the indicator reliable
        switch (value) {
            case GR::keine_Zwangsbremsung:
                removeTextmessage(T::Zwangsbremsung);
                removeTextmessage(T::LZB_Halt_ueberfahren);
                removeTextmessage(T::Geschwindigkeitsueberschreitung);
                removeTextmessage(T::Fdl_beteiligen);
                removeTextmessage(T::Unberechtigtes_Befreien);
                removeTextmessage(T::WT_nicht_zeitgerecht_betaetigt);
                removeTextmessage(T::Hz2000_Beeinflussung);
                removeTextmessage(T::Hz2000_Beeinflussung);
                break;
            case GR::Wachsam:
                emitTextmessage(T::WT_nicht_zeitgerecht_betaetigt);
                emitTextmessage(T::Zwangsbremsung);
                break;
            case GR::Geschwindigkeitsueberschreitung_1000Hz:
            case GR::Geschwindigkeitsueberschreitung_500Hz:
                emitTextmessage(T::Geschwindigkeitsueberschreitung);
                emitTextmessage(T::Zwangsbremsung);
                break;
            case GR::Beinflussung_2000Hz:
                emitTextmessage(T::Hz2000_Beeinflussung);
                emitTextmessage(T::Zwangsbremsung);
                break;
            case GR::Unberechtigtige_Befreiung_vor_500Hz:
                emitTextmessage(T::Unberechtigtes_Befreien);
                emitTextmessage(T::Zwangsbremsung);
                break;
            case GR::LZB_Halt_ueberfahren:
                emitTextmessage(T::LZB_Halt_ueberfahren);
                emitTextmessage(T::Zwangsbremsung);
                if (vAct <= 1) emitTextmessage(T::Fdl_beteiligen);
                break;
            default:
                emitTextmessage(T::Zwangsbremsung);
                break;
        }
    }
    if(ktp && value != GR::keine_Zwangsbremsung){
        // During EMB 1000Hz and 500Hz must not flash if text messages are used
        lm1000Hz = indicator::Off;
        lm500Hz  = indicator::Off;
    }
}
void lzb::setZustandZugbeeinflussung(ZustandZugbeeinflussung value) {
    if(zustZugbeeinflussung == value) return;
    zustZugbeeinflussung = value;
}
void lzb::setModusWirksameZugdaten(ModusWirksameZugdaten wirks, bool thr1, bool thr2, bool thr3){
    using T = texte;
    using MWZ = ModusWirksameZugdaten;
    if (!ktp) return;
    if (grundZwangsbrmnsung != Zwangsbremsungen::keine_Zwangsbremsung)
        wirks = MWZ::Normalbetrieb; // Workaround: In case of EMB, we have to make space and remove any train data messages
    if(vKl5 == thr1 && vKl20 == thr2 && vKl50 == thr3 && modWirksZugd == wirks) return;
    vKl5  = thr1;
    vKl20 = thr2;
    vKl50 = thr3;
    modWirksZugd = wirks;
    if(modWirksZugd == MWZ::Normalbetrieb){
        removeTextmessage(T::Grunddaten_wirksam);
        removeTextmessage(T::Ersatzzugdaten_wirksam);
        removeTextmessage(T::Zugdaten_eingeben);
        return;
    }
    if(modWirksZugd == MWZ::Ersatzzugdaten){
        if(!lzbVorhanden && vKl5)
            emitTextmessage(T::Zugdaten_eingeben);
        if(!lzbVorhanden && !vKl5)
            removeTextmessage(T::Zugdaten_eingeben);
        if(vKl20)
            emitTextmessage(T::Ersatzzugdaten_wirksam);
    }
    if(modWirksZugd == MWZ::Grunddaten){
        if(vKl50){
            emitTextmessage(T::Zugdaten_eingeben);
            emitTextmessage(T::Grunddaten_wirksam);
        }
        else{
            removeTextmessage(T::Zugdaten_eingeben);
            removeTextmessage(T::Grunddaten_wirksam);
        }
    }
    // Without text messages, all indicators would light up simultaneously, so we cancel that.
    if(zugart != Zugarten::Obere) lm85 = indicator::Off;
    if(zugart != Zugarten::Mittlere) lm70 = indicator::Off;
    if(zugart != Zugarten::Untere) lm55 = indicator::Off;
}
void lzb::setLzbZustand(LzbZustaende value) {
    //  LZB-Zustand:
    /// 0: Keine LZB-Führung
    /// 1: Normale LZB-Fahrt
    /// 2: Nothalt
    /// 3: LZB-Halt überfahren
    /// 4: Rechnerausfall
    /// 5: Nachfahrauftrag
    /// 6: Funktionsprüfung
    if (lzbZustand == value) return;
    lzbZustand = value;
    if (!ktp) return;
    using T = texte;
    using Z = LzbZustaende;
    switch (value) {
    case Z::Keine_LZB_Fuehrung:
            removeTextmessage(T::LZB_Halt_ueberfahren);
            removeTextmessage(T::Fdl_beteiligen);
            removeTextmessage(T::Geschwindigkeitswechsel);
            break;
    case Z::LZB_Halt_ueberfahren:
            setGrundZwangsbremnsung(Zwangsbremsungen::LZB_Halt_ueberfahren);
            lmH = indicator::On;
            break;
    case Z::Funktionspruefung:
            emitTextmessage(T::LZB_Halt_ueberfahren);
            break;
        default: break;
    }
}
void lzb::setBremseinsatzpunkt(){
    if(lmG == indicator::On && geschwWechsel){
        removeTextmessage(texte::Geschwindigkeitswechsel);
        geschwWechsel = false;
    }
    if(lmG == indicator::On && ktp && !bremseinsatzpunkt){
        bremseinsatzpunkt = true;
        emitTextmessage(texte::Bremseinsatzpunkt_erwarten);
        removeTextmessageDelayed(texte::Bremseinsatzpunkt_erwarten, 20000);
    }
    if(lmG == indicator::Off && bremseinsatzpunkt){
        bremseinsatzpunkt = false;
        removeTextmessage(texte::Bremseinsatzpunkt_erwarten);
    }
}
void lzb::setEndeverfahren(quint8 value) {
    /// 1: eingeleitet
    /// 2: quittiert
    if (ktp) lmEnde = indicator::Off;
    if (endeverfahren == value) return;
    endeverfahren = value;
    if (!ktp) return;
    using T = texte;
    switch (endeverfahren) {
        case 0:
            removeTextmessage(T::LZB_Ende);
            removeTextmessage(T::Freitaste_betaetigen);
            removeTextmessage(T::Signale_und_Fahrplan_beachten);
            break;
        case 1:
            emitTextmessage(T::LZB_Ende);
            emitTextmessage(T::Freitaste_betaetigen);
            break;
        case 2:
            removeTextmessage(T::Freitaste_betaetigen);
            emitTextmessage(T::LZB_Ende);
            emitTextmessage(T::Signale_und_Fahrplan_beachten);
            break;
        default: break;
    }
}
void lzb::setErsatzauftrag(quint8 value) {
    if (value > 1) value = 1;
    if (ersatzauftrag == value) return;
    ersatzauftrag = value;
    if (!ktp) return;
    if (value == 0) removeTextmessage(texte::LZB_Ersatzauftrag);
    else emitTextmessage(texte::LZB_Ersatzauftrag);
}
void lzb::setFalschfahrauftrag(quint8 value) {
    if (falschfahrauftrag == value) return;
    falschfahrauftrag = value;
    if (!ktp) return;
    using T = texte;
    switch (value) {
        case 0:
            removeTextmessage(T::LZB_Gegengleisfahrauftrag);
            removeTextmessage(T::Wachsamkeitstaste_betaetigen);
            break;
        case 1:
            emitTextmessage(T::LZB_Gegengleisfahrauftrag);
            emitTextmessage(T::Wachsamkeitstaste_betaetigen);
            break;
        case 2:
            emitTextmessage(T::LZB_Gegengleisfahrauftrag);
            removeTextmessage(T::Wachsamkeitstaste_betaetigen);
            break;
        default: break;
    }
}
void lzb::setVorsichtsauftrag(quint8 value) {
    /// 1: eingeleitet
    /// 2: quittiert/Dunkelschaltung
    /// 3: Fahrt auf Sicht (V40-Melder Dau-erlicht)
    if (vorsichtauftrag == value) return;
    vorsichtauftrag = value;
    if (!ktp) return;
    using T = texte;
    switch (value) {
        case 0:
            removeTextmessage(T::LZB_Vorsichtauftrag);
            removeTextmessage(T::Freitaste_betaetigen);
            break;
        case 1:
            emitTextmessage(T::LZB_Vorsichtauftrag);
            emitTextmessage(T::Freitaste_betaetigen);
            break;
        case 3:
            removeTextmessage(T::Freitaste_betaetigen);
            emitTextmessage(T::LZB_Vorsichtauftrag);
            break;
        default: break;
    }
}
void lzb::setLzbNothalt(quint8 value) {
    /// 1: Nothalt empfangen
    /// 2: Nothalt überfahren
    /// 3: Nothalt aufgehoben
    if (lzbNothalt == value) return;
    lzbNothalt = value;
    if (!ktp) return;
    using T = texte;
    switch (value) {
        case 0:
            removeTextmessage(T::LZB_Nothalt);
            removeTextmessage(T::LZB_Halt_ueberfahren);
            removeTextmessage(T::Fdl_beteiligen);
            break;
        case 1://1: Nothalt empfangen
            emitTextmessage(T::LZB_Nothalt);
            break;
        case 2://2: Nothalt überfahren
            removeTextmessage(T::LZB_Nothalt);
            emitTextmessage(T::LZB_Nothalt_ueberfahren);
            emitTextmessage(T::Zwangsbremsung);
            if (vAct == 0) emitTextmessage(T::Fdl_beteiligen);
            break;
        case 3://3: Nothalt aufgehoben
            removeTextmessage(T::LZB_Nothalt);
            removeTextmessage(T::LZB_Nothalt_ueberfahren);
            emitTextmessage(T::LZB_Halt_ueberfahren);
            emitTextmessage(T::Zwangsbremsung);
            emitTextmessage(T::Fdl_beteiligen);
            break;
        default: break;
    }
}
void lzb::setLzbElAuftrag(LzbElAuftraege value) {
    // Workaround:
    // Zusi does only give us the state of the indicator.
    // We need some more than that to set and remove all text messages correctly
    if (!ktp) return;
    using L = LzbElAuftraege;
    using T = texte;
    if (lzbElAuftrag == L::Hauptschalter_aus && value == L::Kein_Auftrag){
        value = L::Hauptschalter_Ein;
        QTimer::singleShot(20000, this, [this]() {
            setLzbElAuftrag(L::El_erledigt);
        });
    }
    if (lzbElAuftrag == L::Stromabnehmer_senken && value == L::Kein_Auftrag){
        value = L::Stromabnehmer_heben;
        QTimer::singleShot(20000, this, [this]() {
            setLzbElAuftrag(L::El_erledigt);
        });
    }
    if (lzbElAuftrag == value) return;
    lzbElAuftrag = value;
    switch (value) {
        case L::Kein_Auftrag:break;
        case L::Hauptschalter_aus:
            removeTextmessage(T::Stromabnehmer_senken);
            emitTextmessage(T::Hauptschalter_ausschalten);
            break;
        case L::Stromabnehmer_senken:
            removeTextmessage(T::Hauptschalter_ausschalten);
            emitTextmessage(T::Stromabnehmer_senken);
            break;
        case L::Hauptschalter_Ein:
            removeTextmessage(T::Stromabnehmer_senken);
            removeTextmessage(T::Stromabnehmer_heben);
            removeTextmessage(T::Hauptschalter_ausschalten);
            emitTextmessage(T::Ggf_weitere_Stromabn_beachten);
            emitTextmessage(T::Hauptschalter_einschalten);
            break;
        case L::Stromabnehmer_heben:
            removeTextmessage(T::Stromabnehmer_senken);
            removeTextmessage(T::Hauptschalter_ausschalten);
            removeTextmessage(T::Hauptschalter_einschalten);
            emitTextmessage(T::Ggf_weitere_Stromabn_beachten);
            emitTextmessage(T::Stromabnehmer_heben);
            break;
        case L::El_erledigt:
            removeTextmessage(T::Ggf_weitere_Stromabn_beachten);
            removeTextmessage(T::Stromabnehmer_senken);
            removeTextmessage(T::Stromabnehmer_heben);
            removeTextmessage(T::Hauptschalter_ausschalten);
            removeTextmessage(T::Hauptschalter_einschalten);
            break;
    }
}
void lzb::setIndusiStoerschalter(quint8 value) {
    ///Störschalter
    // 1: Indusi abgeschaltet
    // 2: Indusi eingeschaltet
    using T = texte;
    Stoerschalter schalter = static_cast<Stoerschalter>(value);
    if (schalter == Stoerschalter::abgeschaltet && ktp && lmUe == indicator::Off) {
        emitTextmessage(T::PZB_mit_Stoerschalter_abgeschaltet);
        emitTextmessage(T::V_Ueberwachung_50kmh);
    }
    if(schalter == Stoerschalter::eingeschaltet) {
        removeTextmessage(T::PZB_mit_Stoerschalter_abgeschaltet);
        removeTextmessage(T::V_Ueberwachung_50kmh);
    }
}
void lzb::setLzbStoerschalter(quint8 value) {
    ///Störschalter
    // 1: LZB abgeschaltet
    // 2: LZB eingeschaltet
    using T = texte;
    Stoerschalter schalter = static_cast<Stoerschalter>(value);
    if (schalter == Stoerschalter::eingeschaltet)
        removeTextmessage(T::LZB_mit_Stoerschalter_abgeschaltet);
    if (schalter == Stoerschalter::abgeschaltet && ktp)
        emitTextmessage(T::LZB_mit_Stoerschalter_abgeschaltet);
}
void lzb::setPlzbLuftabsperrhahn(Luftabsperrhahn value) {
    ///Luftabsperrhahn
    // 1: abgesperrt
    // 2: offen
    using T = texte;
    if (value == Luftabsperrhahn::offen) removeTextmessage(T::PZB_LZB_Luftabsperrhahn_zu);
    if (value == Luftabsperrhahn::abgesperrt && ktp) emitTextmessage(T::PZB_LZB_Luftabsperrhahn_zu);
}
void lzb::setSystemstatusLzb(Systemstatus value){
    ///Systemstatus
    // 0: Ausgeschaltet
    // 1: Abgeschaltet
    // 2: Unterdrückt
    // 3: Aktiv
    if(systemstatusLzb != value){
        systemstatusLzb = value;
        if(systemstatusLzb == Systemstatus::Abgeschaltet || systemstatusLzb == Systemstatus::Aktiv)
            emit newActiveLevel(levelPzbLzbNtc);

    }
}
void lzb::setSystemstatusPzb(Systemstatus value){
    if(systemstatusPzb != value){
        systemstatusPzb = value;
        if(systemstatusPzb == Systemstatus::Abgeschaltet || systemstatusPzb == Systemstatus::Aktiv)
            emit newActiveLevel(levelPzbLzbNtc);
        if(systemstatusPzb == Systemstatus::Ausgeschaltet || systemstatusPzb == Systemstatus::Unterdrueckt)
            emit newActiveLevel(levelUnbefinde);
    }
    if(value == Systemstatus::Aktiv)
        lmPzb = indicator::On;
    else
        lmPzb = indicator::Off;
}
void lzb::setFahrtUeberLlzbHaltPerBefehl(quint8 value){FahrtUeberLlzbHaltPerBefehl = value;}
void lzb::setUebertragungsausfall(LzbUebertragungsausfaelle value, quint8 vZiel){
    ///0: Kein Ausfall
    ///1: eingeleitet
    ///2: Ü blinkt
    ///3: erste Quittierung erfolgt
    ///4: Bedingung für 2. Quittierung gegeben
    ///5: zweite Quittierung erfolgt
    ///6: Ausfall nach verdeckter LZB-Aufnahme (CE)
    ///7: dito, Befehl blinkt
    using A = LzbUebertragungsausfaelle;
    // Basiccly we can leave the function if the value hasnt changed.
    // But tehre are two cases in which other external conditions may differ
    if (Uebertragungsausfall == value &&
        Uebertragungsausfall != A::Bedingung_fuer_zw_Quittierung_gegeben &&
        Uebertragungsausfall != A::Erste_Quittierung_erfolgt &&
        ! ZB_changed) return;
    Uebertragungsausfall = value;
    using T = texte;
    vZiel = (vZiel / 5) * 5;    // Round vZiel to multiple of 5
    texte vText = T::Abbremsen_unter_40kmh;
    switch (vZiel){
    case 85: vText = T::Abbremsen_unter_85kmh; break;
    case 80: vText = T::Abbremsen_unter_80kmh; break;
    case 70: vText = T::Abbremsen_unter_70kmh; break;
    case 60: vText = T::Abbremsen_unter_60kmh; break;
    case 55: vText = T::Abbremsen_unter_55kmh; break;
    case 50: vText = T::Abbremsen_unter_50kmh; break;
    case 40: vText = T::Abbremsen_unter_40kmh; break;
    case 30: vText = T::Abbremsen_unter_30kmh; break;
    case 20: vText = T::Abbremsen_unter_20kmh; break;
    case 10: vText = T::Abbremsen_unter_10kmh; break;
    }
    switch (value) {
    case A::Kein_Ausfall:
        removeTextmessage(T::LZB_Stoerung);
        removeTextmessage(T::LZB_Uebertragungsausfall);
        removeTextmessage(T::Freitaste_betaetigen);
        removeTextmessage(T::Signale_und_Fahrplan_beachten);
        for(quint8 i = static_cast<quint8>(T::Abbremsen_unter_85kmh); i <= static_cast<quint8>(T::Abbremsen_unter_10kmh); i++)
            removeTextmessage(static_cast<T>(i));
        break;
    case A::Eingeleitet:
        emitTextmessage(T::LZB_Stoerung);
        break;
    case A::Ue_blinkt:
        emitTextmessage(T::LZB_Uebertragungsausfall);
        emitTextmessage(T::Freitaste_betaetigen);
        break;
    case A::Erste_Quittierung_erfolgt:
        removeTextmessage(T::Freitaste_betaetigen);
        emitTextmessage(vText);
        if(vAct <= 160)
            emitTextmessage(T::Signale_und_Fahrplan_beachten);
        break;
    case A::Bedingung_fuer_zw_Quittierung_gegeben:
        removeTextmessage(vText);
        if(ZB)
            removeTextmessage(T::Signale_und_Fahrplan_beachten);
        else
            emitTextmessage(T::Signale_und_Fahrplan_beachten);
        emitTextmessage(T::Freitaste_betaetigen);
        break;
    case A::Zweite_Quittierung_erfolgt:
        removeTextmessage(T::LZB_Uebertragungsausfall);
        removeTextmessage(T::Freitaste_betaetigen);
        removeTextmessage(T::Signale_und_Fahrplan_beachten);
        break;
    case A::Ausfall_nach_verdeckter_LZB_Aufnahme_CE:
    case A::Ausfall_nach_verdeckter_LZB_Aufnahme_CE_Befehl_blinkt:
        break;
    }
}
void lzb::setDeviceTypePlzb(deviceTypePlzb type){
    if(plzbTyp == type)return;
    plzbTyp = type;
    lzbVorhanden = (plzbTyp==deviceTypePlzb::I80_m_PZB90 || plzbTyp==deviceTypePlzb::I80);
    emit lzbAvailable(lzbVorhanden);
}
// ================================================================================
void lzb::emitTextmessage(texte text){
    emit newLzbTextMessage(
                MeldungsTexte.at    (static_cast<quint8>(text)),
                MeldungsFarbenTxt.at(static_cast<quint8>(text)),
                MeldungsFarbenBg.at (static_cast<quint8>(text)),
                static_cast<quint8>(text));
}
void lzb::emitLimitTextmessage(texte text){
    removeTextmessage(lastLimitMessage);
    emit newLzbTextMessage(
                MeldungsTexte.at    (static_cast<quint8>(text)),
                MeldungsFarbenTxt.at(static_cast<quint8>(text)),
                MeldungsFarbenBg.at (static_cast<quint8>(text)),
                static_cast<quint8>(text));
    lastLimitMessage = text;
}
void lzb::removeTextmessage(texte text){
    emit removeMessage(static_cast<quint8>(text));
}
void lzb::removeTextmessageDelayed(texte text, quint16 delay){
    QTimer::singleShot(delay, this, [this, text]() {
        removeTextmessage(text);
    });
}
void lzb::checkPzb90(){
    if( plzbTyp == deviceTypePlzb::I60  |
        plzbTyp == deviceTypePlzb::I60R |
        plzbTyp == deviceTypePlzb::I80  |
        plzbTyp == deviceTypePlzb::PZ80 |
        plzbTyp == deviceTypePlzb::PZ80R ){
        lm85  = indicator::Off;
        lm70  = indicator::Off;
        lm55  = indicator::Off;
        lmPzb = indicator::Off;
    }
    else{
        lm95     = indicator::Off;
        lm75     = indicator::Off;
        lm60     = indicator::Off;
        lmIndusi = indicator::Off;
    }
}
bool lzb::isBlinking(indicator state){
    bool A = state == indicator::Off;
    bool B = state == indicator::On;
    bool C = !(A || B);
    return (C);
}
void lzb::addIndicator(quint8 indId, quint8 blinking, bool invers){
    // indId represents an indicatorFiles in db.h
    if(indId == 16){                        // Indicator GNT
        emit newIconG1(indicatorFiles[16][0],indicatorFiles[16][1]);
        emit newIconBehavG1(true, blinking, invers);
        return;
    }
    if(indId == 5){                        // Indicator S as Era Brake
        emit newIconC9(indicatorFiles[24][0],indicatorFiles[24][1]);
        emit newIconBehavC9(true, blinking, invers);
    }
    quint8 i = 0;
    bool indAllrUsed = false;
    // Search for a position, this indicator is allready used. ...
    // If it is, check, if the behaviour is the same.
    // If it is, there is noting left to do.
    // If no, leave the loop and put indicator with new behaviour at same position (i).
    for(i=0; i<=6; i++){
        if(indicatorField[i] == indId){
            indAllrUsed = true;
            if(indicatorFieldBehav[i] == blinking){
                return;
            }
            else{
                break;
            }
        }
    }
    // If the inidcator is not used jet, search for the first free field.
    if(!indAllrUsed)for(i=1; i<=6; i++){
            if(indicatorField[i] == 255)break;
        }
    // Some indicators require special handling.
    switch(indId){
    case  0: i = 0; break;  // "B" must be at field 1
    case  4: i = 5; break;  // "PZB"  must be at field 5.
    case 23: i = 5; break;  // "Indusi" for I60 must be at field 5.
    case 14: i = 6; break;  // "Ue" must be at pos 6
    case 17: i = 6; break;  // "Ü GNT" must be at pos 6
    default: break;
    }

    indicatorField[i] = indId;
    indicatorFieldBehav[i] = blinking;
    switch (i){
    case 0:
        emit newIcon1(indicatorFiles[indId][0], indicatorFiles[indId][0]);
        emit newIconBehav1(true, blinking, invers);
        break;
    case 1:
        emit newIcon2(indicatorFiles[indId][0], indicatorFiles[indId][1]);
        emit newIconBehav2(true, blinking, invers);
        break;
    case 2:
        emit newIcon3(indicatorFiles[indId][0], indicatorFiles[indId][1]);
        emit newIconBehav3(true, blinking, invers);
        break;
    case 3:
        emit newIcon4(indicatorFiles[indId][0], indicatorFiles[indId][1]);
        emit newIconBehav4(true, blinking, invers);
        break;
    case 4:
        emit newIcon5(indicatorFiles[indId][0], indicatorFiles[indId][1]);
        emit newIconBehav5(true, blinking, invers);
        break;
    case 5:
        emit newIcon6(indicatorFiles[indId][0], indicatorFiles[indId][1]);
        emit newIconBehav6(true, blinking, invers);
        break;
    case 6:
        emit newIcon7(indicatorFiles[indId][0], indicatorFiles[indId][1]);
        emit newIconBehav7(true, blinking, invers);
        break;
    default: break;
    }
}
void lzb::removeIndicator(quint8 indId){
    quint8 i = 0;
    for(i=0; i<=6; i++){
        if(indicatorField[i] == indId){
            indicatorField[i] = 255;
            break;
        }
    }
    if(indId == 16 && !brakeFromEtcs){
        emit newIconC9("", "");
        //emit newIconBehavC9(false, 0, false);
    }
    switch (i){
    case 0:
        emit newIcon1("", "");
        emit newIconBehav1(false, 0, false);
        break;
    case 1:
        emit newIcon2("", "");
        emit newIconBehav2(false, 0, false);
        break;
    case 2:
        emit newIcon3("", "");
        emit newIconBehav3(false, 0, false);
        break;
    case 3:
        emit newIcon4("", "");
        emit newIconBehav4(false, 0, false);
        break;
    case 4:
        emit newIcon5("", "");
        emit newIconBehav5(false, 0, false);
        break;
    case 5:
        emit newIcon6("", "");
        emit newIconBehav6(false, 0, false);
        break;
    case 6:
        emit newIcon7("", "");
        emit newIconBehav7(false, 0, false);
        break;
    default: break;
    }
}

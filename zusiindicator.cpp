#include "zusiindicator.h"

zusiIndicator::zusiIndicator(QObject *parent) : QObject(parent){
    lzbValuesToDecoder.resize(7);
    lmElBlinkTestTimer = new QTimer(this);
    lmElBlinkTestTimer->setSingleShot(true);
    connect(lmElBlinkTestTimer,  SIGNAL(timeout()),this,SLOT(setLzbElAuftrag12()));
}

void zusiIndicator::setZugbeeinflussungssystem(QString value){
    lzbVorhanden = value.contains("LZB");
    if(!lzbVorhanden){  // Nach Wechsel von LZB- nach PZB Fahrzeug bleiben Fuhrungsgrößen sethen
        setZielweg(-1);
        lmB = 0; lmUe = 0;
        setLzbZustand(0);
    }
}
void zusiIndicator::setMelderbild(uint8_t value){
    //qDebug() << "MelderbildRoh:  " + QString::number(value);
    if(lm1000Hz > 0 && value == 0){
        melderbild = 6;
    }
    else{
        melderbild = value;  // Melderbild wird nach LZB-Ende fälschlich kurz auf 5 gesetzt.
    }
    //qDebug() << "MelderbildNeu:  " + QString::number(melderbild);
    restriktiv = (melderbild == 2) || (melderbild == 3) || (melderbild == 4);
}
void zusiIndicator::setZustandZugsicherung(uint16_t value){
    //qDebug() << "Zustand Zugb.:      " + QString::number(value);
    zustandZugsicherung = value;
}
void zusiIndicator::setGrundZwangsbrmnsung(uint16_t value){//qDebug() << "ZB wegen ID: " + QString::number(value);
    if(lzbZustand == 3)value = 9;   //Workarround: Zusi does not set "Bremsgrund LZB-Halt überfahren (->9)". But luckily then "LzbZustand" is 3
    //qDebug() << "GrundZwangsbrmnsung:      " + QString::number(value);
    ZwangsbremsungAktiv = value > 0;
    if(ktp == false) value = 0;
    if(grundZwangsbrmnsung != value){
        grundZwangsbrmnsung = value;
        switch (value){
        case 0:
                emit removeMessage(0);
                emit removeMessage(1);
                emit removeMessage(38);
                emit removeMessage(64);
                emit removeMessage(65);
                emit removeMessage(66);
                //emit removeMessage(3);
                emit removeTechnicalMessage(102);
                emit removeTechnicalMessage(103);
                emit removeTechnicalMessage(105);
                emit removeTechnicalMessage(106);
                emit removeTechnicalMessage(107);
                emit removeTechnicalMessage(110);
                emit removeTechnicalMessage(112);
                emit removeTechnicalMessage(113);
                emit removeTechnicalMessage(114);
                emit removeTechnicalMessage(125);
                emit removeTechnicalMessage(126);
                break;
        case 1: emit newTextMessage(db::messages[0 ], db::textFontColors[0 ], db::textBgColors[0 ], 0);
                emit newTextMessage(db::messages[65], db::textFontColors[65], db::textBgColors[65], 65);
                break;  //Wachsam nicht betätigt
        case 2: emit newTextMessage(db::messages[0], db::textFontColors[0], db::textBgColors[0], 0);
                emit newTechnicalMessage("1000 Hz-Prüfung", era::grey, era::darkBlue, 102);
                break;  //1000 Hz-Prüfung
        case 3: emit newTextMessage(db::messages[0], db::textFontColors[0], db::textBgColors[0], 0);
                emit newTechnicalMessage("500 Hz-Prüfung", era::grey, era::darkBlue, 103);
                break;  //500 Hz-Prüfung
        case 4: emit newTextMessage(db::messages[0 ], db::textFontColors[0 ], db::textBgColors[0 ], 0);
                emit newTextMessage(db::messages[65], db::textFontColors[65], db::textBgColors[65], 65);
                break;  //2000 Hz-Prüfung
        case 5: emit newTextMessage(db::messages[0], db::textFontColors[0], db::textBgColors[0], 0);
                emit newTechnicalMessage("Kein Halt nach Befr aus ZB", era::grey, era::darkBlue, 105);
                break;  //Kein Halt nach Befreiung aus Zwangsbremsung
        case 6: emit newTextMessage(db::messages[0], db::textFontColors[0], db::textBgColors[0], 0);
                emit newTechnicalMessage("Fahrzeug-v-Max überschritten", era::grey, era::darkBlue, 106);
                break; //Fahrzeug-v-Max überschritten
        case 7: emit newTextMessage(db::messages[0], db::textFontColors[0], db::textBgColors[0], 0);
                emit newTechnicalMessage("Funktionsprüfung", era::grey, era::darkBlue, 107);
                break;//Funktionsprüfung
        case 8: emit newTextMessage(db::messages[0 ], db::textFontColors[0 ], db::textBgColors[0 ], 0);
                emit newTextMessage(db::messages[64], db::textFontColors[64], db::textBgColors[64], 64);
                break;//500Hz nach Befreiung
        case 9: emit newTextMessage(db::messages[0], db::textFontColors[0], db::textBgColors[0], 0);
                emit newTextMessage(db::messages[1], db::textFontColors[1], db::textBgColors[1], 1);
                if(vIst <= 1)emit newTextMessage(db::messages[38], db::textFontColors[38], db::textBgColors[38], 38);
                break;//LZB-Halt überfahren
        case 10: emit newTextMessage(db::messages[0], db::textFontColors[0], db::textBgColors[0], 0);
                 emit newTechnicalMessage("LZB-Rechnerausfall", era::grey, era::darkBlue, 110);
                 break;//LZB-Rechnerausfall
        case 11: //emit newTextMessage(db::messages[0], db::textFontColors[0], db::textBgColors[0], 0);
                 //emit newTextMessage(db::messages[3], db::textFontColors[0], db::textBgColors[0], 0);
                 break;//LZB-Nothalt überfahren   ...Comented, because to be handled in nothalt routine
        case 12: emit newTextMessage(db::messages[0], db::textFontColors[0], db::textBgColors[0], 0);
                 emit newTechnicalMessage("Ü-Ausfall in verd. Aufn.", era::grey, era::darkBlue, 112);
                 break;//Übertragungsausfall in verdeckter Aufnahme
        case 13: emit newTextMessage(db::messages[0], db::textFontColors[0], db::textBgColors[0], 0);
                 emit newTechnicalMessage("LZB-Rechnerausfall", era::grey, era::darkBlue, 113);
                 break;//LZB-Rechnerausfall
        case 14: emit newTextMessage(db::messages[0], db::textFontColors[0], db::textBgColors[0], 0);
                 emit newTechnicalMessage("Richtungsschalter verlegt", era::grey, era::darkBlue, 114);
                 break;//Richtungsschalter verlegt
        case 25: emit newTextMessage(db::messages[0], db::textFontColors[0], db::textBgColors[0], 0);
                 emit newTechnicalMessage("LZB-Rückrollüberwachung", era::grey, era::darkBlue, 125);
                 break;//LZB-Rückrollüberwachung
        case 26: emit newTextMessage(db::messages[0], db::textFontColors[0], db::textBgColors[0], 0);
                 emit newTechnicalMessage("LZB Überschr. 200m n. Bef40", era::grey, era::darkBlue, 126);
                 break;//LZB Überschreitung 200 m nach Befehl 40
        }
    }

}
void zusiIndicator::setZugart(uint8_t value){//qDebug() << "Zugart:    " + QString::number(value);
    zugart = value; // U_M_O 2_3_4  1_Undef
    if(zugart == 1 && ktp) emit newTextMessage(db::messages[46], db::textFontColors[46], db::textBgColors[46], 46);
}
void zusiIndicator::setKlartextmeldungen(uint8_t value){//qDebug() << "Klartextmeldungen : " + QString::number(value);
    klartextmeldungen = value;
    ktp = value > 1;
}
void zusiIndicator::setLmHauptschalter(uint8_t value){
    //qDebug() << "HS   " + QString::number(value);
    if(value == 0 && lmHauptschalter == 1)setLzbElAuftrag(21);// Workarround: El-Auftrag does not get reset after end
    lmHauptschalter = value;
}
void zusiIndicator::setStatusStromabnehmer(uint8_t value){
    //qDebug() << "Stromabnehmer " + QString::number(value);
    stromabn1Oben =    value & 0x01;
    stromabn2Oben =   (value & 0x02) >> 1;
    stromabn3Oben =   (value & 0x04) >> 2;
    stromabn4Oben =   (value & 0x08) >> 3;
    stromabn1Hebend = (value & 0x10) >> 4;
    stromabn2Hebend = (value & 0x20) >> 5;
    stromabn3Hebend = (value & 0x40) >> 6;
    stromabn4Hebend = (value & 0x80) >> 7;
    if(value > 0)setLzbElAuftrag(22);
}
void zusiIndicator::setLmSchleudern(uint8_t value){
    //qDebug() << "Schl: " + QString::number(value);
    lmSchleudern = value;
}
void zusiIndicator::setLmGleiten(uint8_t value){
    //qDebug() << "Glei: " + QString::number(value);
    lmGleiten = value;
}
void zusiIndicator::setLmUhrzeitDigital(uint8_t value){}
void zusiIndicator::setLm1000Hz(uint8_t value){
    lm1000Hz = value;
    //if(ZwangsbremsungAktiv      && !ktp) lm1000Hz = 5;
    if(zustandZugsicherung == 2 && !ktp) lm1000Hz = 5;  //abgeschaltet/gestört (1000 Hzblinkt)
    if(zustandZugsicherung == 3 && !ktp) lm1000Hz = 5;  //Hauptluftleitung unter Grenzdruck (ca. 2,2 bar, 1000 Hz blinkt)
    if(zustandZugsicherung == 3 &&  ktp) lm1000Hz = 0;
    if(melderbild == 0 && lm1000Hz  > 0) melderbild = 6;
    if(melderbild == 6 && lm1000Hz == 0) melderbild = 0;
}
void zusiIndicator::setErsatzdatenWirksam(uint8_t value){
    if(value == 0)emit removeMessage(59);
    if(value >  0 && ktp)emit newTextMessage(db::messages[59], db::textFontColors[59], db::textBgColors[59], 59);
}
void zusiIndicator::setLm500Hz(uint8_t value){
    lm500Hz = value;
    //if(ZwangsbremsungAktiv) lm500Hz = 5;
}
void zusiIndicator::setLmBefehl(uint8_t value){
    lmBefehl = value;//qDebug() << "Befehl 40:   " + QString::number(value);
    if(Uebertragungsausfall == 2) lmBefehl = 5;
}
void zusiIndicator::setLzbStoerschalter(uint8_t value){
    if(value == 0)emit removeMessage(32);
    if(value >  0 && ktp)emit newTextMessage(db::messages[32], db::textFontColors[32], db::textBgColors[59], 32);
}
void zusiIndicator::setLzbZustand(uint8_t value){
    if(lzbZustand != value){
        if(value == 0){// Keine LZB-Führung
            emit removeMessage(1);
            emit removeMessage(37);
            emit removeMessage(58);
            setLzbNothalt(0); // Workarround: Lzb-Nothalt does not get reset after end
            setEndeverfahren(0); // Workarround: Endeverfahren does not get reset after fall to PZB
        }
        if(value == 3){// LZB-Halt überfahren
            setGrundZwangsbrmnsung(9);   //Workarround: Zusi does not set "Bremsgrund LZB-Halt überfahren (->9)". But luckily then "LzbZustand" is 3
            lmH = 1;
        }
        if(value == 6 && ktp)emit newTextMessage(db::messages[37], db::textFontColors[37], db::textBgColors[37], 37);
    }
    lzbZustand = value;//qDebug() << "LZB-Zustand  " + QString::number(value);
}
void zusiIndicator::setEndeverfahren(uint8_t value){
    if(endeverfahren != value){
        //qDebug() << "LZB Ende:    " + QString::number(value);
        if(value == 0 || lmUe == 0){
            emit removeMessage(28);
            emit removeMessage(45);
            emit removeMessage(57);
            lmEnde = 0;
        }
        if(value == 1 && lmUe >  0){
            emit removeMessage(61); //Workarround: "Bremseinsatzpunkt erwarten" is often displayed at the same time and is unimportand
            if(ktp)emit newTextMessage(db::messages[28], db::textFontColors[28], db::textBgColors[28], 28);
            if(ktp)emit newTextMessage(db::messages[45], db::textFontColors[45], db::textBgColors[45], 45);
            if(!ktp)lmEnde = 5;
        }
        if(value == 2 && lmUe >  0){
            emit removeMessage(45);
            if(ktp)emit newTextMessage(db::messages[28], db::textFontColors[28], db::textBgColors[28], 28);
            if(ktp)emit newTextMessage(db::messages[57], db::textFontColors[57], db::textBgColors[57], 57);
            if(!ktp)lmEnde = 1;
        }
    }
    endeverfahren = value;
}
void zusiIndicator::setErsatzauftrag(uint8_t value){
    //qDebug() << "LZB Ersatzauftrag:    " + QString::number(value);
    if(value > 1) value = 1;
    if(ersatzauftrag != value){
        if(value == 0){
            emit removeMessage(30);
            ersatzauftrag = 0;
            lmE40 = 0;
        }
        if(value >= 1){
            if(ktp)emit newTextMessage(db::messages[30], db::textFontColors[30], db::textBgColors[28], 30);
            ersatzauftrag = 1;
            lmE40 = 1;
        }
    }
}
void zusiIndicator::setFalschfahrauftrag(uint8_t value){
    //qDebug() << "LZB Falschfahrauftrag:    " + QString::number(value);
    if(falschfahrauftrag != value){
        if(value == 0){
            emit removeMessage(31);
            emit removeMessage(44);
            lmE40 = 0;
        }
        if(value == 1){
            if(ktp)emit newTextMessage(db::messages[31], db::textFontColors[31], db::textBgColors[28], 31);
            if(ktp)emit newTextMessage(db::messages[44], db::textFontColors[44], db::textBgColors[45], 44);
            lmE40 = 5;
        }
        if(value == 2){
            if(ktp)emit newTextMessage(db::messages[31], db::textFontColors[31], db::textBgColors[31], 31);
            emit removeMessage(44);
            lmE40 = 5;
        }
        falschfahrauftrag = value;
    }
}
void zusiIndicator::setVorsichtsauftrag(uint8_t value){
    if(vorsichtauftrag != value){
        if(value == 3 && lmV40Roh == 0)value = 0;   //Workarround: Even V40 is switched off in real and by setLmV40, Zusi still is sending 3 to this function
        //qDebug() << "LZB Vorsichtauftrag:    " + QString::number(value);
        if(value == 0){
            emit removeMessage(29);
            emit removeMessage(45);
            lmV40 = 0;
        }
        if(value == 1){//Vorsichtauftrag eingeleitet
            if(ktp)emit newTextMessage(db::messages[29], db::textFontColors[29], db::textBgColors[29], 29);
            if(ktp)emit newTextMessage(db::messages[45], db::textFontColors[45], db::textBgColors[45], 45);
            lmV40 = 5;
        }
        if(value >= 2){//Vorsichtauftrag quittiert/Dunkelschaltung oder Fahrt auf Sicht (V40-Melder Dau-erlicht)
            emit removeMessage(45);
            if(ktp)emit newTextMessage(db::messages[29], db::textFontColors[29], db::textBgColors[29], 29);
            lmV40 = 1;
        }
        vorsichtauftrag = value;
    }
}
void zusiIndicator::setLzbNothalt(uint8_t value){
    if(lzbNothalt != value){
        if(value >  0 && ktp)emit newTextMessage(db::messages[2], db::textFontColors[2], db::textBgColors[59], 2);
        if(value == 0){
            emit removeMessage(2);
            emit removeMessage(1);
            emit removeMessage(38);
            lmH = 0;
        }
        if(value == 1){//Nothalt empfangen
            if(ktp)emit newTextMessage(db::messages[2], db::textFontColors[2], db::textBgColors[2], 2);
            lmH = 5;
        }
        if(value == 2){//Nothalt überfahren
            emit removeMessage(2);
            if(ktp)emit newTextMessage(db::messages[3], db::textFontColors[3], db::textBgColors[3], 3);
            if(ktp)emit newTextMessage(db::messages[0], db::textFontColors[0], db::textBgColors[0], 0);
            if(vIst == 0 && ktp)emit newTextMessage(db::messages[38], db::textFontColors[38], db::textBgColors[38], 38);
            lmH = 5;
        }
        if(value == 3){//Nothalt aufgehoben
            emit removeMessage(2);
            emit removeMessage(3);
            if(ktp)emit newTextMessage(db::messages[1], db::textFontColors[1], db::textBgColors[1], 1);
            if(ktp)emit newTextMessage(db::messages[0], db::textFontColors[0], db::textBgColors[0], 0);
            if(ktp)emit newTextMessage(db::messages[38], db::textFontColors[38], db::textBgColors[38], 38);
            lmH = 1;
        }    
        lzbNothalt = value;
    }
}
void zusiIndicator::setLzbElAuftrag(uint8_t value){
    if(lzbElAuftrag != value){
        if(value == 0){
            emit removeMessage(27);//weitere beachten
            emit removeMessage(40);//senken
            emit removeMessage(41);//heben
            emit removeMessage(42);//Hauptschalter ausschalten
            emit removeMessage(43);//Hauptschalter einschalten
            lmEl = 0;
        }
        if(value == 1){//Hauptschalter aus (EL zeigt Dauerlicht)
            emit removeMessage(40);
            if(ktp)emit newTextMessage(db::messages[42], db::textFontColors[42], db::textBgColors[42], 42);
            lmEl = 1;
        }
        if(value == 2){//Stromabnehmer senken (EL blinkt)
            emit removeMessage(42);
            if(ktp)emit newTextMessage(db::messages[40], db::textFontColors[40], db::textBgColors[40], 40);
            lmEl = 5;
        }
        if(value == 11){//Hauptschalter Ein
            emit removeMessage(40);
            emit removeMessage(41);
            emit removeMessage(42);
            if(ktp)emit newTextMessage(db::messages[27], db::textFontColors[27], db::textBgColors[27], 27);
            if(ktp)emit newTextMessage(db::messages[43], db::textFontColors[43], db::textBgColors[43], 43);
            lmEl = 0;
        }
        if(value == 21){//Hauptschalter Ein erledigt
            emit removeMessage(27);
            emit removeMessage(43);
            lmEl = 0;
        }
        if(value == 12){//Stromabnehmer heben
            emit removeMessage(40);
            emit removeMessage(42);
            emit removeMessage(43);
            if(ktp)emit newTextMessage(db::messages[27], db::textFontColors[27], db::textBgColors[27], 27);
            if(ktp)emit newTextMessage(db::messages[41], db::textFontColors[41], db::textBgColors[41], 41);
            lmEl = 0;
        }
        if(value == 22){//Stromabnehmer heben erledigt
            emit removeMessage(27);
            emit removeMessage(41);
            lmEl = 0;
        }
        lzbElAuftrag = value;
    }
}
void zusiIndicator::setLmE40(uint8_t value){
    if(ersatzauftrag && value == 0){// Workarround: Ersatzauftrag does not get reset after end
        setErsatzauftrag(0);
    }
    if((falschfahrauftrag > 0) && value == 0){// Workarround: Falschfahrauftrag does not get reset after end
       // May be, I need a timer, that gets puuled up, every time, value=0. Then its slot needs to do setFalschfahrauftrag(0);
        setFalschfahrauftrag(0);
    }
}
void zusiIndicator::setLmB(uint8_t value){
    lmB = value;//qDebug() << "LZB B        " + QString::number(value);
}
void zusiIndicator::setLmUe(uint8_t value){
    lmUe = value;//qDebug() << "LZB Ü        " + QString::number(value);
    if(Uebertragungsausfall == 2 || FahrtUeberLlzbHaltPerBefehl == 2) lmUe = 5;
}
void zusiIndicator::setLmG(uint8_t value){
    if(value == 0){
        lmG = 0;
        emit removeMessage(5);
        emit removeMessage(61);
    }
    if(value == 1){
        lmG = 1;
        emit removeMessage(5);
        if(ktp)emit newTextMessage(db::messages[61], db::textFontColors[61], db::textBgColors[61], 61);
    }
    if(value == 2){
        lmG = 5;
        emit removeMessage(61);
        if(ktp)emit newTextMessage(db::messages[5], db::textFontColors[5], db::textBgColors[5], 5);
    }
}
void zusiIndicator::setLmEl(uint8_t value){
    //lmEl = value;//qDebug() << "LZB EL       " + QString::number(value);
    if(value == 0 && lmEl == 1)setLzbElAuftrag(11);// Workarround: El-Auftrag does not get reset after end
    if(value == 0 && lmEl == 5)lmElBlinkTestTimer->start(2000);
}
void zusiIndicator::setLmV40(uint8_t value){
    lmV40Roh = value;   //For workarround in setVorsichtsauftrag
    if(lmV40 == 1 && value == 0)setVorsichtsauftrag(0); // Workarround: Vorsichtauftrag does not get reset after end
}
void zusiIndicator::setLmS(uint8_t value){
    // Workarround: If a blue indicator is till activ, wait am moment with indicator for forced brake, to placed on the position of the blue.
    if((lm85 > 0  || lm70 > 0 || lm55 > 0) && value > 0){
        lmSDelayed = value;
        QTimer::singleShot(10,this,SLOT(setLmSDeleyed()));
    }
    else{
        lmS = value;
    }
}
void zusiIndicator::setLmSDeleyed(){
    setLmS(lmSDelayed);
}
void  zusiIndicator::setLmGnt(uint8_t value){
    lmGnt = value;
}
void  zusiIndicator::setLmGntUe(uint8_t value){
    if(value == 2){lmGnt_Ue = 5;}
    else          {lmGnt_Ue = value;}
}
void  zusiIndicator::setLmGntG(uint8_t value){
    if(value == 2){lmGnt_G = 5;}
    else          {lmGnt_G = value;}
}
void  zusiIndicator::setLmGntS(uint8_t value){
    if(value == 2){lmGnt_S = 5;}
    else          {lmGnt_S = value;}
}

void zusiIndicator::setLmPruefStoer(uint8_t value){
    lmPruefStoer = value;//qDebug() << "Prüf/Stör    " + QString::number(value);
}
void zusiIndicator::setFahrtUeberLlzbHaltPerBefehl(uint16_t value){
    FahrtUeberLlzbHaltPerBefehl = value;
}
void zusiIndicator::setUebertragungsausfall(uint16_t value){
    Uebertragungsausfall = value;
}
void zusiIndicator::setVSoll(int16_t value){
    setLzbValue(value, 3);
}
void zusiIndicator::setVIst(uint16_t value){
    vIst = value;
}
void zusiIndicator::setVZiel(int16_t value){
    if (value < 0) value = 0;
    if(vZiel > value && lzbZustand > 0){
        if(ktp)emit newTextMessage(db::messages[58], db::textFontColors[58], db::textBgColors[58], 58);
        QTimer::singleShot(2000,this,SLOT(remooveMessage58()));// Todo!: 20 Sekunden, nicht 2!
    }
    vZiel = value;//qDebug() << "V-Ziel       " + QString::number(value);
    setLzbValue(value, 1);
}
void zusiIndicator::setZielweg(int16_t value){
    setLzbValue(value, 5);
    emit newLzbValues(lzbValuesToDecoder);
    //qDebug() << "Zielweg        " + QString::number(zielweg);
}
void zusiIndicator::setAfbSoll(uint16_t value){
    afbSoll = value;
    emit newAfbSoll(afbSoll, afbAn);
}
void zusiIndicator::setAfbAn(bool value){
    afbAn = value;
    emit newAfbSoll(afbSoll, afbAn);
}

void zusiIndicator::remooveMessage58(){
    emit removeMessage(58);
}
void zusiIndicator::setLzbElAuftrag12(){
    setLzbElAuftrag(12);
}
void zusiIndicator::setFzgVMax(uint16_t value){
    emit newFzgVmaxTacho(value + 20);
}

void zusiIndicator::calcPzbTextmessages(){
    #define ZugartUndef 1
    #define ZugartU 2
    #define ZugartM 3
    #define ZugartO 4

    #define meBiNormal 0
    #define meBi1000HzN700m 1
    #define meBiRest 2
    #define meBiRest1000Hz 3
    #define meBiRest500Hz 4
    #define meBiPrüfablaufNachLzbUebertragungsausfallGNull 5
    #define meBi1000HzV700m 6
    if( melderbild == meBiNormal && !lm500Hz && !lm1000Hz){//   /85
        emit removeMessage(lastLimitMessage);
        lastLimitMessage = 0;
    }
    if( melderbild == meBiRest500Hz){
        sentSpetLimitMessage(24);//25
    }
    switch (zugart){
        case ZugartO:
            if( melderbild == meBiRest || melderbild == meBiRest1000Hz){//45km/h / 1000 / 85'
                sentSpetLimitMessage(20);//45
            }

            if( melderbild == meBi1000HzV700m || melderbild == meBi1000HzN700m){//85km/h /      / 85'
                sentSpetLimitMessage(14);//85
            }
            if(!restriktiv &&  lm500Hz){//45km/h / 500  /25
                sentSpetLimitMessage(20);//45
            }
            break;
        case ZugartM:
            if( melderbild == meBiRest || melderbild == meBiRest1000Hz){//45km/h / 1000 / 70'
                sentSpetLimitMessage(20);//45
            }
            if(melderbild == meBi1000HzV700m || melderbild == meBi1000HzN700m){//70km/h /      / 70'
                sentSpetLimitMessage(16);//70
            }
            if(!restriktiv &&  lm500Hz){//35km/h / 500  /70
                sentSpetLimitMessage(22);//35
            }
            break;
        case ZugartU:
            if( melderbild == meBiRest){//45km/h / 1000 / 55'
                sentSpetLimitMessage(20);//45
            }
            if( melderbild == meBiRest1000Hz){//45km/h / 1000 / 55'
                sentSpetLimitMessage(20);//45
            }
            if(melderbild == meBi1000HzN700m || melderbild == meBi1000HzV700m){//55km/h /      / 55'
                sentSpetLimitMessage(18);//55
            }
            if(!restriktiv &&  lm500Hz){//25km/h / 500  /55
                sentSpetLimitMessage(24);//25
            }

           break;
        case ZugartUndef:
            emit removeMessage(lastLimitMessage);
            lastLimitMessage = 0;
            break;
     }

    if(lmBefehl){
        if(lastLimitMessage <= 21) sentSpetLimitMessage(21);
    }
    else{
        if(lastLimitMessage == 21){
            emit removeMessage(lastLimitMessage);
            lastLimitMessage = 0;
        }
    }
}
void zusiIndicator::sentSpetLimitMessage(quint8 limit){
    if(limit != lastLimitMessage){
        emit removeMessage(lastLimitMessage);
        emit newTextMessage(db::messages[limit], db::textFontColors[limit], db::textBgColors[limit], limit);
        lastLimitMessage = limit;
    }
}

void zusiIndicator::makeLzbLmDatagram(){
    // Do not display blue indicators after a forced brake, as long as the "S" or blinking 1000Hz are displayed
    if(lmS > 0 || lm1000Hz > 1 || lmEnde > 0 || lmG > 0 || ZwangsbremsungAktiv){
        lm85 = lm70 = lm55 = 0;
        if(!ktp && ZwangsbremsungAktiv && !lmS && !lzbVorhanden) lm500Hz = lm1000Hz = 5;
    }
    else{
        lm85 = lm70 = lm55 = 0;
        if(zustandZugsicherung == 6){//Funktionsprüfung
            lm85 = 5;
            lm70 = 5;
            lm55 = 5;}
        if(lzbZustand == 0) {//Keine LZB-Führung
            if(zugart == 2 && zustandZugsicherung == 5 && melderbild > 1 && melderbild < 6 && !ktp)lm85 = 5;
            if(zugart == 2 && zustandZugsicherung == 5 && melderbild > 1 && melderbild < 6 && ktp)lm55 = 5;
            if(zugart == 3 && zustandZugsicherung == 5 && melderbild > 1 && melderbild < 5 && !ktp)lm85 = 13;
            if(zugart == 3 && zustandZugsicherung == 5 && melderbild == 5)lm85 = 5;
            if(zugart == 4 && zustandZugsicherung >= 5)lm85 = 5;
            if(zugart == 4 && zustandZugsicherung == 5 && melderbild == 0)lm85 = 1;
            if(zugart == 2 && zustandZugsicherung == 5 && melderbild > 1 && melderbild < 5 && !ktp)lm70 = 13;
            if(zugart == 2 && zustandZugsicherung == 5 && melderbild == 5)lm70 = 5;
            if(zugart == 3 && zustandZugsicherung >= 5)lm70 = 5;
            if(zugart == 3 && zustandZugsicherung == 5 && melderbild == 0)lm70 = 1;
            if(zugart == 4 && zustandZugsicherung == 5 && melderbild > 1 && melderbild < 5 && !ktp)lm70 = 13;
            if(zugart == 4 && zustandZugsicherung == 5 && melderbild == 5)lm70 = 5;
            if(zugart == 2 && zustandZugsicherung == 5 && (melderbild == 1 || melderbild == 6))lm55 = 5;
            if(zugart == 2 && zustandZugsicherung == 5 && melderbild == 0)lm55 = 1;
            if(zustandZugsicherung == 4 && melderbild == 0)lm55 = 5;
        }
        if(ktp)calcPzbTextmessages();
    }
    QVector<quint8> lmsToDecoder(22,0);
    lmsToDecoder[0] = lmB;
    lmsToDecoder[1] = lm85;
    lmsToDecoder[2] = lm70;
    lmsToDecoder[3] = lm55;
    lmsToDecoder[4] = 0;
    lmsToDecoder[5] = 0;
    lmsToDecoder[6] = lmS;
    lmsToDecoder[7] = lmH;
    lmsToDecoder[8] = lmG;
    lmsToDecoder[9] = lmE40;
    lmsToDecoder[10] = lmV40;
    lmsToDecoder[11] = lmBefehl;
    lmsToDecoder[12] = lm1000Hz;
    lmsToDecoder[13] = lm500Hz;
    lmsToDecoder[14] = lmEl;
    lmsToDecoder[15] = lmUe;
    lmsToDecoder[16] = lmS;
    lmsToDecoder[17] = lmEnde;
    lmsToDecoder[18] = lmGnt;
    lmsToDecoder[19] = lmGnt_Ue;
    lmsToDecoder[20] = lmGnt_G;
    lmsToDecoder[21] = lmGnt_S;
    if(lmsToDecoder != lmsToDecoderOld){
        lmsToDecoderOld = lmsToDecoder;
        emit newLzbIndicators(lmsToDecoder);
    }
}

void zusiIndicator::clearData(){
    melderbild = 0; melderbildOld = 0; lm1000Hz = 0; lm85 = 0; lm70 = 0; lm55 = 0;
    zugart = 0; klartextmeldungen = 0; ktp = 0; lmBefehl = 0; lm500Hz = 0; lmS = 0;
    lmSDelayed = 0; lmH = 0; lmE40 = 0; lmB = 0; lmUe = 0; lmG = 0; lmEl = 0; lmEnde = 0;
    lmHauptschalter = 0; lmGetriebe = 0; lmSchleudern = 0; lmGleiten = 0;
    lmUhrzeitDigital = 0; StwgHauptschalter = 0; lmSifa = 0; SifaHupe = 0;
    SifaStoerschalter = 0; SifaLuftabsperrhahn = 0; ErsatzdatenWirksam = 0; lzbZustand = 0;
    falschfahrauftrag = 0; vorsichtauftrag = 0; lzbNothalt = 0; lzbRechnerausfall = 0;
    lzbElAuftrag = 0; lmEL = 0; lmV40 = 0; lmPruefStoer = 0; stromabn1Oben = 0;
    stromabn2Oben = 0; stromabn3Oben = 0; stromabn4Oben = 0; stromabn1Hebend = 0;
    stromabn2Hebend = 0;stromabn3Hebend = 0;stromabn4Hebend = 0; ZwangsbremsungAktiv = 0;
    lmV40Roh = 0; lmGnt = 0; lmGnt_Ue = 0; lmGnt_G = 0; lmGnt_S = 0; endeverfahren = 0;
    zustandZugsicherung = 0;  Uebertragungsausfall = 0; FahrtUeberLlzbHaltPerBefehl = 0;
    afbSoll = 0; grundZwangsbrmnsung = 0; vZiel = 0; vIst = 0;
    makeLzbLmDatagram();
    setVSoll(0);
    setVZiel(0);
    setZielweg(0);
    setAfbSoll(0);
    setAfbAn(0);
    emit removeMessage(lastLimitMessage);
}

void zusiIndicator::setLzbValue(int16_t input, uint8_t pos){
    union { //Pos1/3 VZiel/Soll , //Pos5Entfernung
      uint8_t byte[2];
      uint16_t word;
    } tmp;
    if(pos == 5)lzbValuesToDecoder[0] = input > 0;  // Flag setzten, ob Führungsgrößen angezeigt werden
    tmp.word = qFabs(input);
    //qDebug() << "setLzbValue(input " + QString::number(tmp.word) + ", pos " + QString::number(pos) + ")";
    if(tmp.byte[0] != lzbValuesToDecoder[pos] || tmp.byte[1] != lzbValuesToDecoder[pos + 1]){
        lzbValuesToDecoder[pos    ] = tmp.byte[0];
        lzbValuesToDecoder[pos + 1] = tmp.byte[1];
    }
}
/*void zusiIndicator::makeLzbAnalogDatagram(){
    QVector<quint8> lzbValuesToDecoder(7,0);
//#                    S V       V       E
//#                    h Z       P       n
//telegramLzbLm  = "\x01\xA0\x00\x6A\x00\xFF\x01"
    lzbValuesToDecoder[6] = LzbZustand > 0;
}*/

#include "zusiindicator.h"

zusiIndicator::zusiIndicator(QObject *parent) : QObject(parent){
    lmElBlinkTestTimer = new QTimer(this);
    lmElBlinkTestTimer->setSingleShot(true);
    connect(lmElBlinkTestTimer,  SIGNAL(timeout()),this,SLOT(setLzbElAuftrag12()));
}
void zusiIndicator::setZugbeeinflussungssystem(QString value){
    lzbVorhanden = value.contains("LZB");
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
    calcLmBlau();
}
void zusiIndicator::setZustandZugsicherung(uint16_t value){
    //qDebug() << "Zustand Zugb.:      " + QString::number(value);
    zustandZugsicherung = value;
    calcLmBlau();
}
void zusiIndicator::setGrundZwangsbrmnsung(uint16_t value){//qDebug() << "ZB wegen ID: " + QString::number(value);
    if(LzbZustand == 3)value = 9;   //Workarround: Zusi does not set "Bremsgrund LZB-Halt überfahren (->9)". But luckily then "LzbZustand" is 3
    //qDebug() << "GrundZwangsbrmnsung:      " + QString::number(value);
    ZwangsbremsungAktiv = value > 0;
    if(ZwangsbremsungAktiv > 0){
        calcLmBlau();
    }
    if(ktp == false) value = 0;
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
void zusiIndicator::setZugart(uint8_t value){//qDebug() << "Zugart:    " + QString::number(value);
    zugart = value; // U_M_O 2_3_4  1_Undef
    if(zugart == 1 && ktp) emit newTextMessage(db::messages[46], db::textFontColors[46], db::textBgColors[46], 0);
    calcLmBlau();
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
void zusiIndicator::setLmGetriebe(uint8_t value){}
void zusiIndicator::setLmSchleudern(uint8_t value){
    //qDebug() << "Schl: " + QString::number(value);
    lmSchleudern = value;
}
void zusiIndicator::setLmGleiten(uint8_t value){
    //qDebug() << "Glei: " + QString::number(value);
    lmGleiten = value;
}
void zusiIndicator::setLmMgBremse(uint8_t value){}
void zusiIndicator::setLmHBremse(uint8_t value){}
void zusiIndicator::setLmRBremse(uint8_t value){}
void zusiIndicator::setLmHochabbremsung(uint8_t value){}
void zusiIndicator::setLmSchnellbremsung(uint8_t value){}
void zusiIndicator::setLmUhrzeitDigital(uint8_t value){}
void zusiIndicator::setLmDrehzahlverstellung(uint8_t value){}
void zusiIndicator::setLmFahrtrichtungVor(uint8_t value){}
void zusiIndicator::setLmFahrtrichtungZurueck(uint8_t value){}
void zusiIndicator::setLmFahrtrichtungM(uint8_t value){}
void zusiIndicator::setEntgleisung(uint8_t value){}
void zusiIndicator::setStwgHauptschalter (uint8_t value){}
void zusiIndicator::setLmSifa(uint8_t value){
    //qDebug() << "Sifa-LM:    " + QString::number(value);
    lmSifa = value;
}
void zusiIndicator::setSifaHupe(uint8_t value){
    //qDebug() << "Sifa-Hupe:  " + QString::number(value);
    SifaHupe = value;
}
void zusiIndicator::setSifaHauptschalter(uint8_t value){
    //qDebug() << "Sifa-HS:    " + QString::number(value);
    SifaHauptschalter = value;
}
void zusiIndicator::setSifaStoerschalter(uint8_t value){}
void zusiIndicator::setSifaLuftabsperrhahn(uint8_t value){}
void zusiIndicator::setPzbHauptschalter(uint8_t value){}
void zusiIndicator::setPzbStoerschalter(uint8_t value){}
void zusiIndicator::setPzbLuftabsperrhahn(uint8_t value){}
void zusiIndicator::setLm1000Hz(uint8_t value){
    lm1000Hz = value;
    //if(ZwangsbremsungAktiv      && !ktp) lm1000Hz = 5;
    if(zustandZugsicherung == 2 && !ktp) lm1000Hz = 5;  //abgeschaltet/gestört (1000 Hzblinkt)
    if(zustandZugsicherung == 3 && !ktp) lm1000Hz = 5;  //Hauptluftleitung unter Grenzdruck (ca. 2,2 bar, 1000 Hz blinkt)
    if(zustandZugsicherung == 3 &&  ktp) lm1000Hz = 0;
    if(melderbild == 0 && lm1000Hz  > 0) melderbild = 6;
    if(melderbild == 6 && lm1000Hz == 0) melderbild = 0;
    calcLmBlau();
}
void zusiIndicator::setLmZugartU(uint8_t value){}    // Nicht benutzen!
void zusiIndicator::setLmZugartM(uint8_t value){}    // Nicht benutzen!
void zusiIndicator::setLmZugartO(uint8_t value){}    // Nicht benutzen!
void zusiIndicator::setErsatzdatenWirksam(uint8_t value){
    if(value == 0)emit removeMessage(59);
    if(value >  0 && ktp)emit newTextMessage(db::messages[59], db::textFontColors[59], db::textBgColors[59], 59);
}
void zusiIndicator::setPzbHupe(uint8_t value){}
void zusiIndicator::setLm500Hz(uint8_t value){
    lm500Hz = value;
    //if(ZwangsbremsungAktiv) lm500Hz = 5;
    calcLmBlau();
    //qDebug() << "PZB 500Hz    " + QString::number(lm500Hz);
}
void zusiIndicator::setLmBefehl(uint8_t value){
    lmBefehl = value;//qDebug() << "Befehl 40:   " + QString::number(value);
    if(Uebertragungsausfall == 2) lmBefehl = 5;
}
void zusiIndicator::setLmZugartLinksSBahn(uint8_t value){}
void zusiIndicator::setLmZugartSBahn(uint8_t value){}
void zusiIndicator::setLmZugartRechtsSBahn(uint8_t value){}
void zusiIndicator::setLzbStoerschalter(uint8_t value){
    if(value == 0)emit removeMessage(32);
    if(value >  0 && ktp)emit newTextMessage(db::messages[32], db::textFontColors[32], db::textBgColors[59], 32);
}
void zusiIndicator::setLzbZustand(uint8_t value){
    LzbZustand = value;//qDebug() << "LZB-Zustand  " + QString::number(value);
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
void zusiIndicator::setEndeverfahren(uint8_t value){
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
    if(!ktp)calcLmBlau();
}
void zusiIndicator::setErsatzauftrag(uint8_t value){
    //qDebug() << "LZB Ersatzauftrag:    " + QString::number(value);
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
    calcLmBlau();
}
void zusiIndicator::setFalschfahrauftrag(uint8_t value){
    //qDebug() << "LZB Falschfahrauftrag:    " + QString::number(value);
    if(value == 0){
        emit removeMessage(31);
        emit removeMessage(44);
        falschfahrauftrag = 0;
        lmE40 = 0;
    }
    if(value == 1){
        if(ktp)emit newTextMessage(db::messages[31], db::textFontColors[31], db::textBgColors[28], 31);
        if(ktp)emit newTextMessage(db::messages[44], db::textFontColors[44], db::textBgColors[45], 44);
        falschfahrauftrag = 1;
        lmE40 = 5;
    }
    if(value == 2){
        if(ktp)emit newTextMessage(db::messages[31], db::textFontColors[31], db::textBgColors[31], 31);
        emit removeMessage(44);
        falschfahrauftrag = 1;
        lmE40 = 5;
    }
    calcLmBlau();
}
void zusiIndicator::setVorsichtsauftrag(uint8_t value){
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
    calcLmBlau();
}
void zusiIndicator::setLzbNothalt(uint8_t value){
    if(value >  0 && ktp)emit newTextMessage(db::messages[2], db::textFontColors[2], db::textBgColors[59], 2);
    if(value == 0){
        lzbNothalt = false;
        emit removeMessage(2);
        emit removeMessage(1);
        emit removeMessage(38);
        lmH = 0;
    }
    if(value == 1){//Nothalt empfangen
        lzbNothalt = true;
        if(ktp)emit newTextMessage(db::messages[2], db::textFontColors[2], db::textBgColors[2], 2);
        lmH = 5;
    }
    if(value == 2){//Nothalt überfahren
        lzbNothalt = true;
        emit removeMessage(2);
        if(ktp)emit newTextMessage(db::messages[3], db::textFontColors[3], db::textBgColors[3], 3);
        if(ktp)emit newTextMessage(db::messages[0], db::textFontColors[0], db::textBgColors[0], 0);
        if(vIst == 0 && ktp)emit newTextMessage(db::messages[38], db::textFontColors[38], db::textBgColors[38], 38);
        lmH = 5;
    }
    if(value == 3){//Nothalt aufgehoben
        lzbNothalt = true;
        emit removeMessage(2);
        emit removeMessage(3);
        if(ktp)emit newTextMessage(db::messages[1], db::textFontColors[1], db::textBgColors[1], 1);
        if(ktp)emit newTextMessage(db::messages[0], db::textFontColors[0], db::textBgColors[0], 0);
        if(ktp)emit newTextMessage(db::messages[38], db::textFontColors[38], db::textBgColors[38], 38);
        lmH = 1;
    }
    calcLmBlau();
}
void zusiIndicator::setLzbRechnerausfall(uint8_t value){}
void zusiIndicator::setLzbElAuftrag(uint8_t value){
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
    calcLmBlau();
}
void zusiIndicator::setLmH(uint8_t value){}    // Nicht benutzen!
void zusiIndicator::setLmE40(uint8_t value){
    if(ersatzauftrag && value == 0){// Workarround: Ersatzauftrag does not get reset after end
        setErsatzauftrag(0);
    }
    if(falschfahrauftrag && value == 0){// Workarround: Falschfahrauftrag does not get reset after end
       // May be, I need a timer, that gets puuled up, every time, value=0. Then its slot needs to do setFalschfahrauftrag(0);
        setFalschfahrauftrag(0);
    }
}
void zusiIndicator::setLmEnde(uint8_t value){}// Nicht benutzen!
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
    makeLzbLmDatagram();
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
        makeLzbLmDatagram();
    }
}
void zusiIndicator::setLmSDeleyed(){
    setLmS(lmSDelayed);
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
void zusiIndicator::setVSoll(float value){
    vSoll = value;//qDebug() << "V-Soll       " + QString::number(value);
}
void zusiIndicator::setVIst(float value){
    vIst = value;
}
void zusiIndicator::setVZiel(float value){
    if (value < 0) value = 0;
    if(vZiel > value && LzbZustand > 0){
        if(ktp)emit newTextMessage(db::messages[58], db::textFontColors[58], db::textBgColors[58], 58);
        QTimer::singleShot(2000,this,SLOT(remooveMessage58()));// Todo!: 20 Sekunden, nicht 2!
        makeLzbLmDatagram();
    }
    vZiel = value;//qDebug() << "V-Ziel       " + QString::number(value);
}
void zusiIndicator::setZielweg(float value){
    zielweg = value;
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
void zusiIndicator::setCirEelkeModus(uint8_t value){
    CirEelkeModus = value;//qDebug() << "CIR-ELKE     " + QString::number(value);
}
void zusiIndicator::setMfaAnzeigemodusZugdaten(uint8_t value){
    MfaAnzeigemodusZugdaten = value;//qDebug() << "ZDE Anzeigen " + QString::number(value);
}
void zusiIndicator::setFunktionspruefungLaeuft(uint8_t value){}
void zusiIndicator::setAlleMelderBlinken(uint8_t value){}
void zusiIndicator::setAnzeigeDerFuehrungsgroessen(uint8_t value){}
void zusiIndicator::setLmBAnUndUeAus(uint8_t value){}
void zusiIndicator::setZwangsbremsungAktiv(uint8_t value){}
void zusiIndicator::remooveMessage58(){
    emit removeMessage(58);
}
void zusiIndicator::setLzbElAuftrag12(){
    setLzbElAuftrag(12);
}
void zusiIndicator::setFzgVMax(uint16_t value){//qDebug() << "setFzgVMax    " + QString::number(value);
    emit newFzgVmaxTacho(value + 20);
}
void zusiIndicator::calcLmBlau(){
    // Do not display blue indicators after a forced brake, as long as the "S" or blinking 1000Hz are displayed
    if(lmS > 0 || lm1000Hz > 1 || lmEnde > 0 || lmG > 0 || ZwangsbremsungAktiv){
        //QTimer::singleShot(10,this,SLOT(calcLmBlau()));
        lm85 = lm70 = lm55 = 0;
        if(!ktp && ZwangsbremsungAktiv && !lmS && !lzbVorhanden) lm500Hz = lm1000Hz = 5;
    }
    else{
        lm85 = lm70 = lm55 = 0;
        if(zustandZugsicherung == 6){//Funktionsprüfung
            lm85 = 5;
            lm70 = 5;
            lm55 = 5;}
        if(LzbZustand == 0) {//Keine LZB-Führung
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
    makeLzbLmDatagram();
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
            if(!restriktiv &&  lm500Hz){//45km/h / 500  /70
                sentSpetLimitMessage(20);//45
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
    QVector<quint8> lmsToDecoder(18,0);
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
    emit newLzbIndicators(lmsToDecoder);
}
/*void zusiIndicator::makeLzbAnalogDatagram(){
    QVector<quint8> lzbValuesToDecoder(7,0);
//#                    S V       V       E
//#                    h Z       P       n
//telegramLzbLm  = "\x01\xA0\x00\x6A\x00\xFF\x01"
    lzbValuesToDecoder[6] = LzbZustand > 0;
}*/

# include <QDebug>
#include "lzb.h"

lzb::lzb(){
}

void lzb::setTextUsing(quint8 useAutomText){
    if(useTxtMsgByLm != (useAutomText == 0)){
        //qDebug()  << db::numMessages;
        for(quint8 i = 0; i < db::numMessages; i++){
            removeMessage(i);
        }
        removeMessage(lastLimitMessage);
        lastLimitMessage = 255;
    }
    useTxtMsgByLm = useAutomText == 0; // 0: Allways, 1: Automatic, 2: Never
}

void lzb::setZusiAsDataSource(bool value){
    if(zusiIsDataSource != value){
        //qDebug()  << db::numMessages;
        for(quint8 i = 0; i < db::numMessages; i++){
            removeMessage(i);
        }
        removeMessage(lastLimitMessage);
        lastLimitMessage = 255;
    }
    zusiIsDataSource = value;
}

void lzb::setAnalogValues(QVector<quint8> values){
    if(values.length()>=7){
        vPerm = static_cast<quint16>((values[4] << 8) + values[3]);
        quint16 newVDest = static_cast<quint16>((values[2] << 8) + values[1]);
        if(newVDest < vDest) geschwWechsel = true;
        if(newVDest == vPerm) geschwWechsel = false;
        vDest = newVDest;
        emit newVTarget(static_cast<quint16>((values[2] << 8) + values[1]), (values[0] & 0x0f) > 0);
        emit newVPermit(static_cast<quint16>((values[4] << 8) + values[3]), (values[0] & 0x0f) > 0);
        emit newTarDist(static_cast<quint16>((values[6] << 8) + values[5]), (values[0] & 0x0f) > 0);
    }
}

void lzb::setVAct(quint16 V){
    vAct = V;
    emit newOverspeed(overspeed || ((vAct > vPerm) &&  vPerm > 0));
    emit newIntervenation(intervenation);
}

void lzb::setStates(QVector<quint8> states){
    quint8 lmO = states[1];
    quint8 lmM = states[2];
    quint8 lmU = states[3];
    pzb90 = (states[26] == 3 || states[26] == 5 || states[26] == 7 || states[26] == 10);
    states[ 1] = states[ 1] *  pzb90;  // 85
    states[ 2] = states[ 2] *  pzb90;  // 70
    states[ 3] = states[ 3] *  pzb90;  // 55
    states[ 4] = states[ 4] *  pzb90;  // PZB
    states[22] = states[22] * !pzb90;  // 95
    states[23] = states[23] * !pzb90;  // 75
    states[24] = states[24] * !pzb90;  // 65
    states[25] = states[25] * !pzb90;  // indusi
    intervenation = ((states[12] > 1 && states[13] > 1) || states[ 6] >  0);
    overspeed = states[ 8] >  1;
    emit newOverspeed(overspeed || ((vAct > vPerm) &&  vPerm > 0));
    emit newIntervenation(intervenation);
    emit newVMaxReducing(states[ 8] >  0);
    for(quint8 i = 0; i <= 25; i++){    // Remove all indicators, that have to be rmoved, to make space for new ones.
        if(states[i]  == 0){
            removeIndicator(i);
        }
    }
    if(useTxtMsgByLm && !zusiIsDataSource){
        quint8 blinkFrequency = 2;
        zugart = states[27];
        if(lmO == 0 && lmM == 0 && lmU == 0) zugart = Keine; // When direction switch is not set to forward. -> No blue indicator.
        bool restriktiv = lmO > 1 && lmM > 1;
        blauBlink = lmO > 1 || lmM > 1 || lmU >  1;
        bool tausendBeinfl =  (states[12] == 1 && states[13] == 0);
        bool fuenfhuBeinfl =  (states[12] == 0 && states[13] == 1);
        bool zweitauBeinfl =   states[11] == 1;
      //bool stoerschalter =  (states[12] >  1 && states[13] == 0);
        bool bremsEinsPunkt =  states[8 ] == 1;
        if(bremsEinsPunkt) geschwWechsel = 0;                   // The message Geschwindigkeitswechsel has to disapear, when G appears.
        bool vorsAuftrag    =  states[10] >  0;
        bool ersAuftrag     =  states[ 9] == 1;
        if(states[ 9] >  1 && !gegenGlAuftr){                   // Workaround: messages WT-Erwartet disapers after some time (Chaper 10.3) and it is not identifiable by indicators. So just use a timer.
            wachTastErw =  true;
            QTimer::singleShot(5000, this, SLOT(resetExpectWt()));
        }
        if(states[ 14] == 0 && pantoSenken){                    // Workaround: messages Panto-heben disapers after 20s (Chaper 11.2) and it is not identifiable by indicators. So just use a timer.
            pantoHeben =  true;
            QTimer::singleShot(20000, this, SLOT(resetPanto()));
        }
        pantoSenken = states[14] >  1;

        if(states[ 14] == 0 && hsAus){                          // Workaround: messages HS-an disapers after 20s (Chaper 12.2) and it is not identifiable by indicators. So just use a timer.
            hsAn =  true;
            QTimer::singleShot(20000, this, SLOT(resetHs()));
        }
        hsAus = states[14] == 1;
        if(states[15] > 1)ueAusfall = true;
        if(states[15] == 1 ||                                   // Uebertragung zurueck
           states[0] == 0  ||                                   // LZB turned off (B = 0)
           states[4] >  0)ueAusfall = false;                    // PZB-Melder an

        gegenGlAuftr   =  states[ 9] >  1;
        bool freiTastErw    = (states[10] >  1 ||               // Vorsichtsauftrag
                               states[17] >  1 ||               // LZB Ende
                               states[15] >  1);                // Uebertragungsausfall
        bool lzbEnde        = (states[17] >  0);
        bool sigUFplBeacht  = (states[17] == 1);
        states[17] = 0;                                         // The Ende-Indicator has to be hidden, since we are using text messages.
        if(states[12] > 1)states[12] = 0;                       // The 1000Hz-Indicator has to be hidden if blinking, since we are using text messages. (Chapter 3.2)
        bool lzbHaltueberf = states[7] == 1;
        bool vMax40 = states[11] == 1;
        bool fdlBeteil = (lzbHaltueberf && (vAct == 0)) ||      // LZB-Halt ueberfahren
                         (ueAusfall && states[11] > 1)  ;       // Zweite Quitierung nach Uebertragungsausfall
        bool lzbNotHalt = false;
        if(states[7] > 1) lzbNotHalt = true;
        if(states[7] ==0) lzbNotHalt = false;    
        if(intervenation){                                      // In case of intervenation 500Hz and 1000Hz must be off
            states[12] = 0;
            states[13] = 0;
            removeIndicator(12);
            removeIndicator(13);
        }
        addOrRemoveMessage(2,  lzbNotHalt);
        addOrRemoveMessage(38, fdlBeteil);
        addOrRemoveMessage(58, geschwWechsel);
        addOrRemoveMessage(1,  lzbHaltueberf);
        addOrRemoveMessage(0,  intervenation);
        addOrRemoveMessage(21, vMax40);
        addOrRemoveMessage(5,  overspeed);
        addOrRemoveMessage(61, bremsEinsPunkt);
      //addOrRemoveMessage(66, stoerschalter);
        addOrRemoveMessage(29, vorsAuftrag);
        addOrRemoveMessage(30, ersAuftrag);
        addOrRemoveMessage(31, gegenGlAuftr);
        addOrRemoveMessage(44, wachTastErw);
        addOrRemoveMessage(28, lzbEnde);
        addOrRemoveMessage(36, ueAusfall);
        addOrRemoveMessage(45, freiTastErw);
        addOrRemoveMessage(57, sigUFplBeacht);
        addOrRemoveMessage(40, pantoSenken);
        addOrRemoveMessage(41, pantoHeben);
        addOrRemoveMessage(27, pantoHeben);
        addOrRemoveMessage(42, hsAus);
        addOrRemoveMessage(43, hsAn);
      /*qDebug() << "=============================================";
        qDebug() << "zugart:        " + QString::number(zugart);
        qDebug() << "restriktiv:    " + QString::number(restriktiv);
        qDebug() << "fuenfhuBeinfl: " + QString::number(fuenfhuBeinfl);
        qDebug() << "1000Hz: " + QString::number(states[12]);
        qDebug() << "500Hz:  " + QString::number(states[13]);*/
        quint8 indBlau = 0;
        switch (zugart){
            case ZugartO:
                indBlau = 1;
                if(!pzb90)indBlau =22;
                blinkFrequency = (lmO & 0x06) >> 1;
                removeIndicator(2);
                removeIndicator(3);
                if( restriktiv &&  !fuenfhuBeinfl){//45km/h / 1000 / 85'
                    addIndicator(indBlau, blinkFrequency, false);
                    sentSpetLimitMessage(20);//45
                }
                if( restriktiv &&  fuenfhuBeinfl){//25km/h / 500  / 85'
                    addIndicator(indBlau, blinkFrequency, false);
                    sentSpetLimitMessage(24);//25
                }
                if(!restriktiv && blauBlink && !(fuenfhuBeinfl || tausendBeinfl)){//85km/h /      / 85'
                    addIndicator(indBlau, blinkFrequency, false);
                    if( pzb90)sentSpetLimitMessage(14);//85
                    if(!pzb90)sentSpetLimitMessage(70);//95
                }
                if(!restriktiv &&  tausendBeinfl){//85km/h / 1000 / 85'
                    addIndicator(indBlau, blinkFrequency, false);
                    if( pzb90)sentSpetLimitMessage(14);//85
                    if(!pzb90)sentSpetLimitMessage(70);//95
                }
                if(!restriktiv &&  fuenfhuBeinfl){//45km/h / 500  /25
                    addIndicator(indBlau, 0, false);
                    sentSpetLimitMessage(20);//45
                }
                if(!blauBlink &&  !restriktiv && !fuenfhuBeinfl && !tausendBeinfl){//   /85
                    addIndicator(indBlau, 0, false);
                    emit removeMessage(lastLimitMessage);
                    lastLimitMessage = 255;
                }
            break;
            case ZugartM:
                indBlau = 2;
                if(!pzb90)indBlau = 23;
                blinkFrequency = (lmM & 0x06) >> 1;
                removeIndicator(1);
                removeIndicator(3);
                if( restriktiv &&  !tausendBeinfl && !fuenfhuBeinfl){//45km/h / 1000 / 70'
                    addIndicator(indBlau, blinkFrequency, false);
                    sentSpetLimitMessage(20);//45
                }
                if( restriktiv &&  tausendBeinfl){//45km/h / 1000 / 70'
                    addIndicator(indBlau, blinkFrequency, false);
                    sentSpetLimitMessage(20);//45
                }
                if( restriktiv &&  fuenfhuBeinfl){//25km/h / 500  / 70'
                    addIndicator(indBlau, blinkFrequency, false);
                    sentSpetLimitMessage(24);//25
                }
                if(!restriktiv && blauBlink && !(fuenfhuBeinfl || tausendBeinfl)){//70km/h /      / 70'
                    addIndicator(indBlau, blinkFrequency, false);
                    if( pzb90)sentSpetLimitMessage(16);//70
                    if(!pzb90)sentSpetLimitMessage(71);//75
                }
                if(!restriktiv &&  tausendBeinfl){//70km/h / 1000 / 70'
                    addIndicator(indBlau, blinkFrequency, false);
                    if( pzb90)sentSpetLimitMessage(16);//70
                    if(!pzb90)sentSpetLimitMessage(71);//75
                }
                if(!restriktiv &&  fuenfhuBeinfl){//45km/h / 500  /70
                    addIndicator(indBlau, 0, false);
                    if( pzb90)sentSpetLimitMessage(23);//30
                    if(!pzb90)sentSpetLimitMessage(20);//45
                }
                if(!blauBlink &&  !restriktiv && !fuenfhuBeinfl && !tausendBeinfl){//   /70
                    addIndicator(indBlau, 0, false);
                    emit removeMessage(lastLimitMessage);
                    lastLimitMessage = 0;
                }
            break;
            case ZugartU:
                indBlau = 3;
                if(!pzb90)indBlau = 24;
                if(!restriktiv)blinkFrequency = (lmU & 0x06) >> 1;
                if( restriktiv)blinkFrequency = (lmM & 0x06) >> 1;
                removeIndicator(1);
                removeIndicator(2);
                if( restriktiv &&  !tausendBeinfl && !fuenfhuBeinfl){//45km/h / 1000 / 55'
                    addIndicator(indBlau, blinkFrequency, false);
                    sentSpetLimitMessage(20);//45
                }
                if( restriktiv &&  tausendBeinfl){//45km/h / 1000 / 55'
                    addIndicator(indBlau, blinkFrequency, false);
                    sentSpetLimitMessage(20);//45
                }
                if( restriktiv &&  fuenfhuBeinfl){//25km/h / 500  / 55'
                    addIndicator(indBlau, blinkFrequency, false);
                    sentSpetLimitMessage(24);//25
                }
                if(!restriktiv && blauBlink && !(fuenfhuBeinfl || tausendBeinfl)){//55km/h /      / 55'
                    addIndicator(indBlau, blinkFrequency, false);
                    if( pzb90)sentSpetLimitMessage(18);//55
                    if(!pzb90)sentSpetLimitMessage(17);//60
                }
                if(!restriktiv &&  tausendBeinfl){//55km/h / 1000 / 55'
                    addIndicator(indBlau, blinkFrequency, false);
                    if( pzb90)sentSpetLimitMessage(18);//55
                    if(!pzb90)sentSpetLimitMessage(17);//60
                }
                if(!restriktiv &&  fuenfhuBeinfl){//25km/h / 500  /55
                    addIndicator(indBlau, 0, false);
                    if(!pzb90)sentSpetLimitMessage(24);//25
                    if(!pzb90)sentSpetLimitMessage(21);//40
                }
                if(!blauBlink &&  !restriktiv && !fuenfhuBeinfl && !tausendBeinfl){//   /55
                addIndicator(indBlau, 0, false);
                emit removeMessage(lastLimitMessage);
                lastLimitMessage = 255;
            }
            break;
            case Keine:
                removeIndicator(1);
                removeIndicator(2);
                removeIndicator(3);
                removeIndicator(22);
                removeIndicator(23);
                removeIndicator(24);
                emit removeMessage(lastLimitMessage);
                lastLimitMessage = 255;
            break;
        }
        if(zweitauBeinfl){
            if(lastLimitMessage <= 21) sentSpetLimitMessage(21);
        }
        else{
            if(lastLimitMessage == 21){
                emit removeMessage(lastLimitMessage);
                lastLimitMessage = 255;
            }
        }
    }
    for(quint8 i = 0; i <= 25; i++){
        if(useTxtMsgByLm && !zusiIsDataSource && i ==  1) i =  3;// Set all indicators but O/M/U (PZB90)
        if(useTxtMsgByLm && !zusiIsDataSource && i == 22) i = 25;// Set all indicators but O/M/U (I60)
        if(states[i]  > 0){
            addIndicator(i, ((states[i] & 0x06 ) >> 1),(states[i] & 0x08) > 0);
        }
    }
}
void lzb::addOrRemoveMessage(quint8 message, bool add){
    if(add && emittedMessages[message] != add){
        emit newTextMessage(db::messages[message], db::textFontColors[message], db::textBgColors[message], message);
        emittedMessages[message] = add;
    }
    if(add != emittedMessages[message]){
        emit removeMessage(message);
        emittedMessages[message] = add;
    }
}
void lzb::sentSpetLimitMessage(quint8 limit){
    if(limit != lastLimitMessage){
        emit removeMessage(lastLimitMessage);
        emit newTextMessage(db::messages[limit], db::textFontColors[limit], db::textBgColors[limit], limit);
        lastLimitMessage = limit;
    }
}
void lzb::addIndicator(quint8 indId, quint8 blinking, bool invers){
    //qDebug() << "lzb::addIndicator";
    // indId represents an indicatorFiles in db.h
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
    if((indId > 0) && (indId != 4) && (indId != 15) && (indId != 16) && (indId != 19) && !indAllrUsed){
        for(i=1; i<=6; i++){
            if(indicatorField[i] == 255)break;
        }
    }
    if(indId == 15 || indId == 19)i = 6;    // "Ue" or "Ue GNT" must be at pos 6
    if(indId == 0                )i = 0;    // "B" must be at field 1
    if(indId == 4  || indId == 25)i = 5;    // "PZB"  must be at field 5. Also "Indusi" for I60.
    if(indId == 16){                        // Era Brake
        emit newIconC9(db::indicatorFiles[16][0],db::indicatorFiles[16][1]);
        emit newIconBehavC9(true, blinking, invers);
        return;
    }
    if(indId == 18){                        // Indicator GNT
        emit newIconG10(db::indicatorFiles[18][0],db::indicatorFiles[18][1]);
        emit newIconBehavG10(true, blinking, invers);
        return;
    }
    indicatorField[i] = indId;
    indicatorFieldBehav[i] = blinking;
    switch (i){
    case 0:
        emit newIcon1(db::indicatorFiles[indId][0], db::indicatorFiles[indId][0]);
        emit newIconBehav1(true, blinking, invers);
    break;
    case 1:
        emit newIcon2(db::indicatorFiles[indId][0], db::indicatorFiles[indId][1]);
        emit newIconBehav2(true, blinking, invers);
    break;
    case 2:
        emit newIcon3(db::indicatorFiles[indId][0], db::indicatorFiles[indId][1]);
        emit newIconBehav3(true, blinking, invers);
    break;
    case 3:
        emit newIcon4(db::indicatorFiles[indId][0], db::indicatorFiles[indId][1]);
        emit newIconBehav4(true, blinking, invers);
    break;
    case 4:
        emit newIcon5(db::indicatorFiles[indId][0], db::indicatorFiles[indId][1]);
        emit newIconBehav5(true, blinking, invers);
    break;
    case 5:
        emit newIcon6(db::indicatorFiles[indId][0], db::indicatorFiles[indId][1]);
        emit newIconBehav6(true, blinking, invers);
    break;
    case 6:
        emit newIcon7(db::indicatorFiles[indId][0], db::indicatorFiles[indId][1]);
        emit newIconBehav7(true, blinking, invers);
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
    if(indId == 16){
        emit newIconC9("", "");
        emit newIconBehavC9(false, 0, false);
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
    }
}
void lzb::resetExpectWt(){
    wachTastErw = false;
    addOrRemoveMessage(44, wachTastErw);
}
void lzb::resetPanto(){
    pantoHeben = false;
    addOrRemoveMessage(41, pantoHeben);
    addOrRemoveMessage(27, pantoHeben);
}
void lzb::resetHs(){
    hsAn = false;
    addOrRemoveMessage(43, hsAn);
}

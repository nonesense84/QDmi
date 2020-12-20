# include <QDebug>
#include "lzb.h"

lzb::lzb(){
    //messages = db::messages;
}

void lzb::setTextUsing(bool useAutomText){
    useTxtMsgByLm = useAutomText;
}

void lzb::setAnalogValues(QVector<quint8> values){
    emit gotLzbMessage();
    if(values.length()>=7){
        vPerm = static_cast<quint16>((values[4] << 8) + values[3]);
        //vDest = static_cast<quint16>((values[4] << 8) + values[3]);
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
    emit gotLzbMessage();
    intervenation = ((states[12] > 1 && states[13] > 1) || states[ 6] >  0);
    overspeed = states[ 8] >  1;
    emit newOverspeed(overspeed || ((vAct > vPerm) &&  vPerm > 0));
    emit newIntervenation(intervenation);
    emit newVMaxReducing(states[ 8] >  0);
    if(useTxtMsgByLm){
        if(states[ 1] > 0  && states[ 2] == 0 && states[03] == 0) zustLmBlau = ZugartO;
        if(states[ 1] == 0 && states[ 2] >  0 && states[03] == 0) zustLmBlau = ZugartM;
        if(states[ 1] == 0 && states[ 2] == 0 && states[03] >  0) zustLmBlau = ZugartU;
        if(states[ 1] == 0 && states[ 2] == 0 && states[03] == 0) zustLmBlau = Keine;
        if(states[ 1] > 1  && states[ 2] == 0 && states[03] == 0) zustLmBlau = ZugartOBlink;
        if(states[ 1] == 0 && states[ 2] >  1 && states[03] == 0) zustLmBlau = ZugartMBlink;
        if(states[ 1] == 0 && states[ 2] == 0 && states[03] >  1) zustLmBlau = ZugartUBlink;
        restriktiv = states[ 1] > 1 && states[ 2] > 1;
        if((zustLmBlau == ZugartO) || ((zustLmBlau == ZugartOBlink) && (zustLmBlau != ZugartMBlink))) zugart = ZugartO;
        if((zustLmBlau == ZugartM) || ((zustLmBlau == ZugartMBlink) && (zustLmBlau != ZugartOBlink))) zugart = ZugartM;
        if((zustLmBlau == ZugartU) || zustLmBlau == ZugartUBlink)                                     zugart = ZugartU;
        if (zustLmBlau == Keine)                                                                      zugart = Keine;
        blauBlink = ((zustLmBlau == ZugartOBlink) || (zustLmBlau == ZugartMBlink) || (zustLmBlau == ZugartUBlink));
        tausendBeinfl = (states[12] == 1 && states[13] == 0);
        fuenfhuBeinfl = (states[12] == 0 && states[13] == 1);
        zweitauBeinfl = states[11] == 1;
        stoerschalter = (states[12] >  1 && states[13] == 0);

        //showLzb = states[18] >  0;

        switch (zugart){
            case ZugartO:
                removeIndicator(2);
                removeIndicator(3);
                if( restriktiv &&  !fuenfhuBeinfl){//45km/h / 1000 / 85'
                    addIndicator(1, 2, false);
                    sentSpetLimitMessage(20);//45
                }
                if( restriktiv &&  fuenfhuBeinfl){//25km/h / 500  / 85'
                    addIndicator(1, 2, false);
                    sentSpetLimitMessage(24);//25
                }
                if(!restriktiv && blauBlink && !(fuenfhuBeinfl || tausendBeinfl)){//85km/h /      / 85'
                    addIndicator(1, 2, false);
                    sentSpetLimitMessage(14);//85
                }
                if(!restriktiv &&  tausendBeinfl){//85km/h / 1000 / 85'
                    addIndicator(1, 2, false);
                    sentSpetLimitMessage(14);//85
                }
                if(!restriktiv &&  fuenfhuBeinfl){//45km/h / 500  /25
                    addIndicator(1, 0, false);
                    sentSpetLimitMessage(20);//45
                }
                if(!blauBlink &&  !restriktiv && !fuenfhuBeinfl && !tausendBeinfl){//   /85
                    addIndicator(1, 0, false);
                    emit removeMessage(lastLimitMessage);
                    lastLimitMessage = 0;
                }
            break;
            case ZugartM:
                removeIndicator(1);
                removeIndicator(3);
                if( restriktiv &&  !tausendBeinfl && !fuenfhuBeinfl){//45km/h / 1000 / 70'
                    addIndicator(2, 2, false);
                    sentSpetLimitMessage(20);//45
                }
                if( restriktiv &&  tausendBeinfl){//45km/h / 1000 / 70'
                    addIndicator(2, 2, false);
                    sentSpetLimitMessage(20);//45
                }
                if( restriktiv &&  fuenfhuBeinfl){//25km/h / 500  / 70'
                    addIndicator(2, 2, false);
                    sentSpetLimitMessage(24);//25
                }
                if(!restriktiv && blauBlink && !(fuenfhuBeinfl || tausendBeinfl)){//70km/h /      / 70'
                    addIndicator(2, 2, false);
                    sentSpetLimitMessage(16);//70
                }
                if(!restriktiv &&  tausendBeinfl){//70km/h / 1000 / 70'
                    addIndicator(2, 2, false);
                    sentSpetLimitMessage(16);//70
                }
                if(!restriktiv &&  fuenfhuBeinfl){//45km/h / 500  /70
                    addIndicator(2, 0, false);
                    sentSpetLimitMessage(20);//45
                }
                if(!blauBlink &&  !restriktiv && !fuenfhuBeinfl && !tausendBeinfl){//   /70
                    addIndicator(2, 0, false);
                    emit removeMessage(lastLimitMessage);
                    lastLimitMessage = 0;
                }
            break;
            case ZugartU:
                removeIndicator(1);
                removeIndicator(2);
                if( restriktiv &&  !tausendBeinfl && !fuenfhuBeinfl){//45km/h / 1000 / 55'
                    addIndicator(3, 2, false);
                    sentSpetLimitMessage(20);//45
                }
                if( restriktiv &&  tausendBeinfl){//45km/h / 1000 / 55'
                    addIndicator(3, 2, false);
                    sentSpetLimitMessage(20);//45
                }
                if( restriktiv &&  fuenfhuBeinfl){//25km/h / 500  / 55'
                    addIndicator(3, 2, false);
                    sentSpetLimitMessage(24);//25
                }
                if(!restriktiv && blauBlink && !(fuenfhuBeinfl || tausendBeinfl)){//55km/h /      / 55'
                    addIndicator(3, 2, false);
                    sentSpetLimitMessage(18);//55
                }
                if(!restriktiv &&  tausendBeinfl){//55km/h / 1000 / 55'
                    addIndicator(3, 2, false);
                    sentSpetLimitMessage(18);//55
                }
                if(!restriktiv &&  fuenfhuBeinfl){//25km/h / 500  /55
                    addIndicator(3, 0, false);
                    sentSpetLimitMessage(24);//25
                }
                if(!blauBlink &&  !restriktiv && !fuenfhuBeinfl && !tausendBeinfl){//   /55
                addIndicator(3, 0, false);
                emit removeMessage(lastLimitMessage);
                lastLimitMessage = 0;
            }
            break;
            case Keine:
                removeIndicator(1);
                removeIndicator(2);
                removeIndicator(3);
                emit removeMessage(lastLimitMessage);
                lastLimitMessage = 0;
            break;
        }
        if(zweitauBeinfl){
            //addIndicator(11, 0);
            if(lastLimitMessage <= 21) sentSpetLimitMessage(21);
        }
        else{
            //removeIndicator(11);
            if(lastLimitMessage == 21){
                emit removeMessage(lastLimitMessage);
                lastLimitMessage = 0;
            }
        }
        if(intervenation && !intervenationEmitted){
            //emit newIconC9(db::indicatorFiles[16][0],db::indicatorFiles[16][1]);
            //emit newIconBehavC9(true, 0, false);
            emit newTextMessage(db::messages[0], db::textFontColors[0], db::textBgColors[0], 0);
            intervenationEmitted = true;
        }
        else{
            if(intervenationEmitted){
                removeIndicator(6);
                //emit newIconC9("","");
                //emit newIconBehavC9(false, 0, false);
                emit removeMessage(0);
                intervenationEmitted = false;
            }
        }
    }
    for(quint8 i = 0; i <= 21; i++){
        if(useTxtMsgByLm && i == 1) i = 5;// Set all indicators but O/M/U/PZB/LZB
        if(states[i]  > 0){
            addIndicator(i, ((states[i] & 0x06 ) >> 1),(states[i] & 0x08) > 0);
        }
        else{
            removeIndicator(i);
        }
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
    // If it is, there is noting left to do.
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
    if((indId > 0) && (indId != 15) && (indId != 16) && (indId != 19) && !indAllrUsed){
        for(i=1; i<=6; i++){
            if(indicatorField[i] == 255)break;
        }
    }
    if(indId == 15 || indId == 19)i = 6;    // "Ue" or "Ue GNT" must be at pos 6
    if(indId == 0                )i = 0;    // "B" must be at field 1
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

#include "zusi3etcs.h"
#include <QDebug>
zusi3etcs::zusi3etcs(){
    emit newReleaseSpeed(0);
    emit newActiveLevelIcon(era::levelIcons[0]);
    emit newEvcPresent(true);
}
void zusi3etcs::setSpeed(quint16 speed){
    if(vAct == speed)return;
    vAct = speed;
    if(activeLevel == level_1 || activeLevel == level_2 || activeLevel == level_3){
        emit newOverspeed((vAct > permittedSpeed) && (vAct > releaseSpeed));// Req 7.4.3.1 & 7.4.3.2 (OvS)
    }
  //if((vAct > permittedSpeed) && (vAct > releaseSpeed))playEraSound("qrc:/sounds/S1_toofast.wav");// Req 7.4.3.3
}
void zusi3etcs::setTdeState(quint8 state){
    tdestate = static_cast<etcsSettingsState>(state);
    switch (tdestate) {
    case trainDataEntryUndefined:
        emit newBehavBtnStartOfMission(false, "Start");
        break;
    case trainDataEntryNecessary:
        emit newBehavBtnStartOfMission(false, "Start");
        break;
    case trainDataEntryEntered:
        emit newBehavBtnStartOfMission(true, "Start");
        break;
    case startOfMissionFinished:
        emit newBehavBtnStartOfMission(false, "Start");
        break;
    }
}
void zusi3etcs::addNtcToListIndex(quint16 index){
    nextNtcIindex = index;
}
void zusi3etcs::addNtcToListName(QString name){
    if(nextNtcIindex == 1){
        listNtc = new QStringList;
        listNtc->append("No NTC");
    }
    listNtc->append(name);
}
void zusi3etcs::setBrakingPercentage(quint16 percentage){
    brakingPercentage = percentage;
}
void zusi3etcs::setTrainCategory(quint16 category){
    if(trainCategory != category){
        trainCategory = category;
        switch (trainCategory) {
            case  1: emit newActiveTct("PASS1");break;  // FIXME: I'ts unknown, which categorys are supported and how the are associated
            case  2: emit newActiveTct("PASS2");break;
            case  3: emit newActiveTct("PASS3");break;
            case  4: emit newActiveTct("FP1");  break;
            case  5: emit newActiveTct("FP2");  break;
            case  6: emit newActiveTct("FP3");  break;
            case  7: emit newActiveTct("FP4");  break;
            case  8: emit newActiveTct("FG1");  break;
            case  9: emit newActiveTct("FG2");  break;
            case 10: emit newActiveTct("FG3");  break;
            case 11: emit newActiveTct("FG4");  break;
            case 12: emit newActiveTct("TILT1");break;
        }
    }
}
void zusi3etcs::setAirTight(QString airTight){ // Not yet part of Zusi TCP.
    emit newActiveAit(airTight);
}

void zusi3etcs::setLoadingGauge(QString loadingGauge){ // Not yet part of Zusi TCP.
    emit newActiveLdg(loadingGauge);
}

void zusi3etcs::setTrainLength(quint16 length){
    trainLength = length;
}
void zusi3etcs::setMaxSpeed(quint16 speed){
    maxSpeed = speed;
}
void zusi3etcs::setAxleLoad(quint16 load){
    if(axleLoad != load){
        axleLoad = load;
        switch (axleLoad) {
            case  1: emit newActiveAxl("A");   break;  // FIXME: I'ts unknown, which loads are supported and how the are associated
            case  2: emit newActiveAxl("B1");  break;
            case  3: emit newActiveAxl("B2");  break;
            case  4: emit newActiveAxl("C2");  break;
            case  5: emit newActiveAxl("C3");  break;
            case  6: emit newActiveAxl("C4");  break;
            case  7: emit newActiveAxl("D2");  break;
            case  8: emit newActiveAxl("D3");  break;
            case  9: emit newActiveAxl("D4");  break;
            case 10: emit newActiveAxl("D4XL");break;
            case 11: emit newActiveAxl("E4");  break;
            case 12: emit newActiveAxl("E5");  break;
        }
    }
}
void zusi3etcs::setTrainNumber(QString number){
    trainNumber = number;
}
void zusi3etcs::setDriverId(QString id){
    driverId = id;
}
void zusi3etcs::setRbcNumber(quint32 number){
    rbcNumber = number;
}
void zusi3etcs::setRbcTelNumber(QString number){
    telNumber = number;
}
void zusi3etcs::setRbcId(quint32 id){
    rbcId = id;
}
void zusi3etcs::setRbcCountry(quint32 country){
    rbcCountry = country;
}
void zusi3etcs::setRadioNetworkId(quint32 id){
    radioNetworkId = id;
}
void zusi3etcs::setRadioNetworkIdIsFix(quint8 isFix){
    radioNetworkIdIsFix = static_cast<fixedradioNetwork>(isFix);
  // qDebug() <<  QtEnumToString(radioNetworkIdIsFix);
}
void zusi3etcs::setAdhesionValue(quint8 value){
    adhesionValue = static_cast<adhesionValueState>(value);
    emit newAdhesionIcon(era::adhesionReduced[adhesionValue]);
  // qDebug() << "adhesionValue: " <<  QtEnumToString(adhesionValue);
}
void zusi3etcs::setEtcsPassiveSwitch(quint8 passiveSwitch){
    etcsPassiveSwitch = static_cast<switchState>(passiveSwitch);
  // qDebug() <<  QtEnumToString(etcsPassiveSwitch);
}
void zusi3etcs::setEtcsFaultySwitch(quint8 faultySwitch){
    etcsFaultySwitch = static_cast<switchState>(faultySwitch);
    emit newEvcPresent(etcsFaultySwitch == normal_position);
  // qDebug() <<  QtEnumToString(etcsFaultySwitch);
}
void zusi3etcs::setAirShutOff(quint8 shutOff){
    airShutOff = static_cast<valveState>(shutOff);
  // qDebug() <<  QtEnumToString(airShutOff);
}
void zusi3etcs::setEtcsAcknowledger(quint8 acknowledger){
    etcsAcknowledger = static_cast<requestSwitchState>(acknowledger);
  // qDebug() <<  QtEnumToString(etcsAcknowledger);
}
void zusi3etcs::setOverrideRequested(quint8 requested){
    overrideRequested = static_cast<requestSwitchState>(requested);
    qDebug() <<  QtEnumToString(overrideRequested);
}
/*
void zusi3etcs::setRequestedLevel(quint16 level){
    requestedLevel = level;
}
void zusi3etcs::setIndexOfSelectedStm(quint16 index){
    indexOfSelectedStm = index;
}
void zusi3etcs::setRequestedModus(quint16 modus){
    requestedModus = modus;
}
void zusi3etcs::setTafModus(quint8 modus){
    tafModus = modus;
}*/
void zusi3etcs::setTrainRestart(quint8 restart){
    trainRestart = restart;
}
void zusi3etcs::setMaxBaseline(quint8 baseline){
    mxBaseline = static_cast<baselineType>(baseline);
  // qDebug() <<  QtEnumToString(mxBaseline);
}
void zusi3etcs::setVehicleHasEtcsCB(quint8 hastCB){
    vehicleHasEtcsCB = hastCB;
}
void zusi3etcs::setVehicleHasEtcsPS(quint8 hastCB){
    vehicleHasEtcsPS = hastCB;
}
void zusi3etcs::setVehicleHasEtcsRSW(quint8 hastRSW){
    vehicleHasEtcsRSW = hastRSW;
}
void zusi3etcs::setVehicleHasEtcsRSK(quint8 hastRSK){
    vehicleHasEtcsRSK = hastRSK;
}
void zusi3etcs::setEtcsCBState(quint8 cBState){
    etcsCBState = static_cast<circuitBreakerState>(cBState);
  // qDebug() <<  QtEnumToString(etcsCBState);
}
void zusi3etcs::setEvcTyte(QString type){
    evcTyte = type;
}
void zusi3etcs::setActiveLevel(quint16 level){
    if(level == 1)level = 6; // Workaround: 6 as level PZB/LZB, because we dont get info which NTC will be the next
    if(activeLevel != level){
        activeLevel = static_cast<etcsLevel>(level);
        emit newActiveLevel(activeLevel);
        emit newActiveLevelIcon(era::levelIcons[activeLevel]);
    }
    if(activeLevel == nextLevel){
        setNextLevel(0xffff, acknowledge_effective);
    }
    if(D)qDebug() << "========================================";
    if(D)qDebug() << "01    level: " << QtEnumToString(activeLevel);
}
void zusi3etcs::setActiveMode(quint16 mode){
    if(activeMode != mode){
        activeMode = static_cast<etcsMode>(mode);
        emit newActiveMode(activeMode);
        emit newActiveModeIcon(era::modeIcons[mode]);
    }
    if(activeMode == nextMode){
        setNextMode(0xffff, acknowledge_effective);
    }
    if(D)qDebug() << "02    ActiveMode " << QtEnumToString(activeMode);
}
void zusi3etcs::setReasonOfEmrBreakEnum(quint16 reason){
    reasonEmergencyBrackeWasTransmitted = true;
    if(reasonOfEmrBreakEnumeratiom != reason){
        reasonOfEmrBreakEnumeratiom = static_cast<reasonOfBreakEnum>(reason);
      /*if(reasonOfEmrBreakEnumeratiom == reason_overSpeed){
            emit newIntervenation(true);
            emit newVOverspeed(emergencyBreakSpeed);
        }
        else{
            emit newIntervenation(false);
            emit newVOverspeed(alertSpeed);
        }*/
    }
    if(D)qDebug() << "03    ReasonOfEmrBreak " << QtEnumToString(reasonOfEmrBreakEnumeratiom);
}
void zusi3etcs::setReasonOfEmrBreakText(QString reason){
    reasonOfEmrBreakText = reason;
    if(D)qDebug() << "04    reasonOfEmrBreakText " << reasonOfEmrBreakText;
}
void zusi3etcs::setIndexOfActiveStm(quint16 index){
    if(indexOfActiveNtc != index){
        indexOfActiveNtc = index;
        if(index > listNtc->length() - 1) return;
    }
    if(D)qDebug() << "05 01 IndexOfActiveStm " << indexOfActiveNtc << " " << listNtc->at(indexOfActiveNtc);
}
void zusi3etcs::setNextLevel(quint16 level, quint8 acknowledge){
    bool sync = false;
    if(level == 1)level = 6; // Workaround: 6 as level PZB/LZB, because we dont get info which NTC will be the next
    if(nextLevel != level && level != 0xffff){
        nextLevel = static_cast<etcsLevel>(level);
        if(D)qDebug() << "06 01 NextLevel            " << QtEnumToString(nextLevel);
        sync = true;
    }
    if(nextLevelAcknowledge != acknowledge && acknowledge != 0xff){
        nextLevelAcknowledge = static_cast<acknowledgeState>(acknowledge);
        emit newNextLevelAckFrame(nextLevelAcknowledge == acknowledge_necessary);
        if(D)qDebug() << "06 02 nextLevelAcknowledge " << QtEnumToString(nextLevelAcknowledge);
        ackTextMessagePersistend = (nextLevelAcknowledge == acknowledge_necessary);
        sync = true;
    }
    if(sync){
        if(!textMessages[1].contains(era::nextLevelTextmessage[nextLevel])){
            textMessages[0].insert(0, simTime);
            textMessages[1].insert(0, era::nextLevelTextmessage[nextLevel]);
        }
        switch (nextLevelAcknowledge) {
        case acknowledge_notNecessary:
        case acknowledge_notYetNecessary:
        case acknowledge_necessaryButYetNotAllConditionsGiven:
            emit newTextmessages(textMessages[0], textMessages[1]);
            emit newNextLevelIcon(era::nextLevelIcons[nextLevel]);
            break;
        case acknowledge_necessary:
            emit newNextLevelIcon(era::nextLevelIconsAck[nextLevel]);
            emit newTextmessages({simTime},{era::nextLevelTextmessageAck[nextLevel]});
            break;
        case acknowledge_done:
            emit newNextLevelIcon(era::nextLevelIcons[nextLevel]);
            emit newTextmessages(textMessages[0], textMessages[1]);
            break;
        case acknowledge_effective:
            emit newNextLevelIcon(era::nextLevelIcons[level_undefined]);
            int i = textMessages[1].indexOf(era::nextLevelTextmessage[nextLevel]);
            if(i>-1)textMessages[0].removeAt(i);
            if(i>-1)textMessages[1].removeAt(i);
            emit newTextmessages(textMessages[0], textMessages[1]);
            break;
        }
    }
}
void zusi3etcs::setNextMode(quint16 mode, quint8 acknowledge){
    bool sync = false;
    if(nextMode != mode && mode != 0xffff){
        nextMode = static_cast<etcsMode>(mode);
        if(D)qDebug() << "07 01 nextMode: " << QtEnumToString(nextMode);
        sync = true;
    }
    if(nextModeAcknowledge != acknowledge && acknowledge != 0xff){
        nextModeAcknowledge = static_cast<acknowledgeState>(acknowledge);
        if(D)qDebug() << "07 02 nextModeAcknowledge: " << QtEnumToString(nextModeAcknowledge);
        emit newNextLevelAckFrame(nextModeAcknowledge == acknowledge_necessary);
        ackTextMessagePersistend = nextModeAcknowledge == acknowledge_necessary;
        sync = true;
    }
    if(sync){
        if(!textMessages[1].contains(era::nextModeTextmessage[nextMode])){
            textMessages[0].insert(0, simTime);
            textMessages[1].insert(0, era::nextModeTextmessage[nextMode]);
        }
        switch (nextModeAcknowledge) {
        case acknowledge_notNecessary:
        case acknowledge_notYetNecessary:
        case acknowledge_necessaryButYetNotAllConditionsGiven:
            emit newTextmessages(textMessages[0], textMessages[1]);
            break;
        case acknowledge_necessary:
            emit newNextLevelIcon(era::nextModeIcons[nextMode]);
            emit newTextmessages({simTime},{era::nextModeTextmessageAck[nextMode]});
            break;
        case acknowledge_done:
            emit newNextLevelIcon(era::nextModeIcons[mode_undefined]);
            emit newTextmessages(textMessages[0], textMessages[1]);
            break;
        case acknowledge_effective:
            emit newNextLevelIcon(era::nextModeIcons[mode_undefined]);
            int i = textMessages[1].indexOf(era::nextModeTextmessage[nextMode]);
            if(i>-1)textMessages[0].removeAt(i);
            if(i>-1)textMessages[1].removeAt(i);
            emit newTextmessages(textMessages[0], textMessages[1]);
            break;
        }
    }

}
void zusi3etcs::setRadioState(quint8 state){
    if(radioConnectionState != state){
        radioConnectionState = static_cast<radioState>(state);
        switch(radioConnectionState){
        case zusi3etcs::radio_noConnection:
            emit newRadioStateIcon(":/icons/blanc.svg");
            return;
        case zusi3etcs::radio_connectionEstablishment:
        case zusi3etcs::radio_connectionEstablised:
            emit newRadioStateIcon(":/icons/ST_03.svg");
            return;
        default:
            emit newRadioStateIcon(":/icons/ST_04.svg");
        }
    }
    if(D)qDebug() << "08 01 radio: " << QtEnumToString(radioConnectionState);
}
void zusi3etcs::setAnnouncementOfNextStm(quint16 index){        // Not implemented in Zusi
    nextStm = index;
    if(index > listNtc->length() - 1) return;
    qDebug() << "next   STM: " + QString::number(indexOfActiveNtc) + " " + listNtc->at(0);
}
void zusi3etcs::setTargetSpeed(float speed){
    targetInfoPresent = speed >= 0;
    if(speed < 0)return;
    targetSpeed = static_cast<quint16>(qRound(speed * static_cast<float>(3.6)));
    emit newVTarget(targetSpeed,  activeLevel > level_Stm, true);
    if(D)qDebug() << "09    TargetSpeed: " << targetSpeed;
}
void zusi3etcs::setTargetDistance(float distance){
    if(targetDistance != distance){
        targetDistance = static_cast<quint16>(distance);
        if(distance < 0){                               // Zielweg in m (Wert<0 → dunkel)
            emit newTarDist(0, false, false, true);
            return;
        }
        bool showBarGraph = false;
        bool showDigital = false;
        showBarGraph = permittedSpeedReducing && ((activeMode == mode_FS)&
                                                  !targetInfoPresent     ||
                                                   activeMode == mode_RV);   // Req. 8.2.2.1.8
        showDigital = permittedSpeedReducing  &&  (activeMode == mode_FS ||
                                                   activeMode == mode_OS ||
                                                   activeMode == mode_SR ||
                                                   activeMode == mode_RV);   // Req. 8.2.2.2.7
        emit newTarDist(targetDistance, showBarGraph, showDigital, true);
    }
    if(D)qDebug() << "0A    targetDistance: " << targetDistance;
}
void zusi3etcs::setBrakeApplicationPointDistance(float distance){
    brakeApplicationPointDistance = distance;
    if(D)qDebug() << "0B    brakeApplicationPointDistance: " << brakeApplicationPointDistance;
}
void zusi3etcs::setReleaseSpeed(float speed){
    if(activeLevel < etcsLevel::level_1)return;    // Workaround: Zusi also send a release speed if there is no ETCS
    if(speed < 0) speed = 0;
    if(releaseSpeed != static_cast<quint16>(qRound(speed * static_cast<float>(3.6)))){
        releaseSpeed = static_cast<quint16>(qRound(speed * static_cast<float>(3.6)));
        emit newReleaseSpeed(releaseSpeed);
    }
    if(D)qDebug() << "0C    releaseSpeed: " << releaseSpeed;
}
void zusi3etcs::setPermittedSpeed(float speed){
    if(speed < 0)return;
    permittedSpeed = static_cast<quint16>(qRound(speed * static_cast<float>(3.6)));
    if(activeLevel < level_1)return; // Workaround: Zusi also send a permitted speed if there is no ETCS
    emit newVPermit(permittedSpeed, true, true);
    emit newOverspeed((vAct > permittedSpeed) && (vAct > releaseSpeed));// Req 7.4.3.1 & 7.4.3.2 (OvS)
    if((vAct > permittedSpeed) && (vAct > releaseSpeed))playEraSound("qrc:/sounds/S1_toofast.wav");// Req 7.4.3.3
    if(D)qDebug() << "0D    permittedSpeed: " << permittedSpeed;
}
void zusi3etcs::setAlertSpeed(float speed){
    if(speed < 0)return;
    alertSpeed = static_cast<quint16>(qRound(speed * static_cast<float>(3.6)));
    if(reasonOfEmrBreakEnumeratiom     != reason_overSpeed &&
       reasonOfSrvBrakeEnumeration != reason_overSpeed){
        emit newVOverspeed(alertSpeed);
    }
    if(D)qDebug() << "0E    AlertSpeed: " << alertSpeed;
}
void zusi3etcs::setServBreakSpeed(float speed){
    if(speed < 0)return;
    serviceBreakSpeed = static_cast<quint16>(qRound(speed * static_cast<float>(3.6)));
    if(reasonOfEmrBreakEnumeratiom     == reason_overSpeed &&
       reasonOfSrvBrakeEnumeration != reason_overSpeed){
        emit newVOverspeed(serviceBreakSpeed);
    }
    if(D)qDebug() << "0F    serviceBreakSpeed: " << serviceBreakSpeed;
}
void zusi3etcs::setEmergencyBreakSpeed(float speed){
    if(speed < 0)return;
    emergencyBreakSpeed = static_cast<quint16>(qRound(speed * static_cast<float>(3.6)));
    if(reasonOfSrvBrakeEnumeration == reason_overSpeed){
        emit newVOverspeed(emergencyBreakSpeed);
    }
    if(D)qDebug() << "10    EmergencyBreakSpeed: " << emergencyBreakSpeed;
}
void zusi3etcs::setPermittedSpeedReducing(bool reducing){    
    if(permittedSpeedReducing != reducing){
        permittedSpeedReducing = reducing;
        emit newVMaxReducing(permittedSpeedReducing);
        if(!permittedSpeedReducing)emit newTarDist(0, false, false, true);
    }
    if(D)qDebug() << "11    permittedSpeedReducing: " << permittedSpeedReducing;
}
void zusi3etcs::addPlanningInfoOrigin(quint16 origin, quint16 index){
    if(planningInfos.length() == index)planningInfos.append(new planningInfo);
    planningInfos[index]->origin = static_cast<enum planningInfoOrigin>(origin);
}
void zusi3etcs::addPlanningInfoSpeed(float speed, quint16 index){
    if(planningInfos.length() == index)planningInfos.append(new planningInfo);
    planningInfos[index]->speed = static_cast<quint16>(qRound(speed * static_cast<float>(3.6)));
}
void zusi3etcs::addPlanningInfoDistance(float distance, quint16 index){
  /*if(index == 0){
      //qDebug() << "List info points:";
        for(int i = 0; i < planningInfos.length(); i++){
          //qDebug() << QString::number(planningInfos[i]->distance) +  ";"+
                        QString::number(planningInfos[i]->speed) + ";"+
                        QString::number(planningInfos[i]->altitude) + ";"+
                        QtEnumToString(planningInfos[i]->parameter) + ";"+
                        QtEnumToString(planningInfos[i]->origin);
        }
    }*/
    if(planningInfos.length() == index)planningInfos.append(new planningInfo);
    planningInfos[index]->distance = qRound(distance);
}
void zusi3etcs::addPlanningGradient(float gradient, quint16 index){
    if(planningInfos.length() == index)planningInfos.append(new planningInfo);
    planningInfos[index]->altitude = qRound(gradient);
}
void zusi3etcs::addPlanningParameter(quint16 parameter, quint16 index){
    if(planningInfos.length() == index)planningInfos.append(new planningInfo);
    planningInfos[index]->parameter = static_cast<planningInfoParameter>(parameter);
}
void zusi3etcs::setTrackAheadFreeRequestState(quint8 RequestState){
    trackAheadFreeRequestState = static_cast<tafState>(RequestState);
    if(D)qDebug() << "13    trackAheadFreeRequestState: " <<  QtEnumToString(trackAheadFreeRequestState);
}
void zusi3etcs::setOverrideActive(quint8 active){
    overrideActiveWasTransmitted = true;
    if(overrideActive != active){
        overrideActive = static_cast<switchState>(active);
        emit newOverrideIcon(era::modeOverrideIcon[overrideActive == applied_position]);
        emit newOverrideActive(overrideActive == applied_position);
    }
    if(D)qDebug() << "14    overrideActive: " <<  QtEnumToString(overrideActive);
}
void zusi3etcs::setEmergencyStop(quint8 stop){
    emergencyStop = static_cast<emergencyStopState>(stop);
    if(D)qDebug() << "15    emergencyStop: " <<  QtEnumToString(emergencyStop);
}
void zusi3etcs::setReasonServBrake(quint16 reason){
    reasonServiceBrackeWasTransmitted = true;
    if(reasonOfSrvBrakeEnumeration != reason){
        reasonOfSrvBrakeEnumeration = static_cast<reasonOfBreakEnum>(reason);
      /*if(reasonOfSrvBrakeEnumeration == reason_overSpeed){
            emit newIntervenation(true);
            emit newVOverspeed(serviceBreakSpeed);
        }
        else{
            emit newIntervenation(false);
            emit newVOverspeed(alertSpeed);
        }*/
    }
    if(D)qDebug() << "16    reasonOfSrvBrakeEnumeration: " << QtEnumToString(reasonOfSrvBrakeEnumeration);
}
void zusi3etcs::setElOrder(quint8 order){
    elOrder = static_cast<elOrderState>(order);
    if(elOrder != el_noOrder)elOrderWasTransmitted = true;
    if(D) qDebug() << "17    elOrder:" <<  QtEnumToString(elOrder);
    switch (elOrder) {
    case el_noOrder:
        emit newIconB5(era::ordersAndAnnouncementofTrackConditionIcons[0]);break;
    case el_prennouncementMainSwitchOff:
        emit newIconB5(era::ordersAndAnnouncementofTrackConditionIcons[6]);break;   // TC06
    case el_orderMainSwitchOff:
        emit newIconB5(era::ordersAndAnnouncementofTrackConditionIcons[7]);break;   // TC07
    case el_orderMainSwitchOn:
        emit newIconB5(era::ordersAndAnnouncementofTrackConditionIcons[8]);break;   // TC08
    case el_prennouncementLowerPantograph:
        emit newIconB5(era::ordersAndAnnouncementofTrackConditionIcons[2]);break;   // TC02
    case el_orderLowerPantograph:
        emit newIconB5(era::ordersAndAnnouncementofTrackConditionIcons[3]);break;   // TC03
    case el_orderRaisPantograph:
        emit newIconB5(era::ordersAndAnnouncementofTrackConditionIcons[4]);break;   // TC04
    }
}
void zusi3etcs::setEtcsTestRunnState(quint8 state){
    etcsTestRunn = static_cast<etcsTestRunnState>(state);
    if(D)qDebug() << "18    etcsTestRunn: " <<  QtEnumToString(etcsTestRunn);
}
void zusi3etcs::setEvcBootupState(quint8 state){
    evcBootup = static_cast<evcBootUpState>(state);
    if(D)qDebug() << "19    evcBootup: " <<  QtEnumToString(evcBootup);
}
void zusi3etcs::setTextMessageByEnum(quint16 message){
    if(message != 0)textMessageByEnumWasTransmitted = true;
    if(textMessages[1].contains(era::reasonEmgrBreakTextmessage[message]) && message != 0)return;
    //textMessage = QtEnumToString(static_cast<reasonOfBreakEnum>(message));
    if(message > 0 && message < era::reasonEmgrBreakTextmessage.length()){
        textMessages[0].insert(0, simTime);
        textMessages[1].insert(0, era::reasonEmgrBreakTextmessage[message]);
    }
    else{
        for(int n = 0; n < era::reasonEmgrBreakTextmessage.length(); n++){
            int i = textMessages[1].indexOf(era::reasonEmgrBreakTextmessage[n]);
            if(i>-1){
                textMessages[0].removeAt(i);
                textMessages[1].removeAt(i);
                n=0;
            }
        }
    }
    if(!ackTextMessagePersistend)emit newTextmessages(textMessages[0], textMessages[1]);
    if(D)qDebug() << "1A 01 TextMessageByEnum:    " << textMessage;
}
void zusi3etcs::setTextMessageByString(QString message){
    if(D)qDebug() << "1A  02 TextMessageByString: " << message;
    if(textMessages[1].contains(message) && message != "")return;
    if(message != ""){
        textMessages[0].insert(0, simTime);
        textMessages[1].insert(0, message);
    }
    else{
        int i = textMessages[1].indexOf(textMessage);
        if(i>-1){
            textMessages[0].removeAt(i);
            textMessages[1].removeAt(i);
        }
    }
    if(!ackTextMessagePersistend)emit newTextmessages(textMessages[0], textMessages[1]);
    textMessage = message;
}
void zusi3etcs::doStuffAfterFinishOfZusiTelegram(){
    emit newEmrOrSrvBrakeState(                   reasonOfEmrBreakEnumeratiom != reason_noEmergencyBrake ||
                                                  reasonOfSrvBrakeEnumeration != reason_noEmergencyBrake);
    emit newEmrOrSrvBrakeIcon(era::emergencyBrake[reasonOfEmrBreakEnumeratiom != reason_noEmergencyBrake ||
                                                  reasonOfSrvBrakeEnumeration != reason_noEmergencyBrake]);
    emit newIntervenation(                        reasonOfEmrBreakEnumeratiom == reason_overSpeed ||
                                                  reasonOfSrvBrakeEnumeration == reason_overSpeed );
    if(!reasonEmergencyBrackeWasTransmitted)      reasonOfEmrBreakEnumeratiom =  reason_noEmergencyBrake;
    if(!reasonServiceBrackeWasTransmitted)        reasonOfSrvBrakeEnumeration =  reason_noEmergencyBrake;
    if(!textMessageByEnumWasTransmitted)          setTextMessageByEnum(reason_noEmergencyBrake);
    if(!elOrderWasTransmitted)                    setElOrder(el_noOrder);
    if(!overrideActiveWasTransmitted){            emit newOverrideIcon(era::modeOverrideIcon[overrideActive == normal_position]);
                                                  emit newOverrideActive(overrideActive == normal_position);
                                     }

    overrideActiveWasTransmitted = false;
    reasonEmergencyBrackeWasTransmitted = false;
    reasonServiceBrackeWasTransmitted = false;
    textMessageByEnumWasTransmitted = false;
    elOrderWasTransmitted = false;
    if(D)qDebug() << "reasonOfEmrBreakEnumeratiom: " <<  QtEnumToString(reasonOfEmrBreakEnumeratiom);
    if(D)qDebug() << "reasonOfSrvBrakeEnumeration: " <<  QtEnumToString(reasonOfSrvBrakeEnumeration);
}
void zusi3etcs::setSimTime(QString time){
    simTime = time;
}
void zusi3etcs::playEraSound(QString filename){
/*    QMediaPlayer *sound =new QMediaPlayer;          // workaround: QSound::play crash on some android devices
    sound->setMedia(QUrl(filename));
    sound->setVolume(50);
    sound->play();*/
}

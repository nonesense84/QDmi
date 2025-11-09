#include "zusi3etcs.h"
#include <QDebug>

zusi3etcs::zusi3etcs(QObject *parent) : QObject(parent){
    qRegisterMetaType<QList<EtcsTextMessage>>("QList<EtcsTextMessage>");
}

void zusi3etcs::reset(){
    emit newReleaseSpeed(0);
    emit newActiveLevelIcon(era::levelIcons[0]);
    emit newEvcPresent(false);
    setActiveLevel                 (etcsLevel::level_Pzb);
    setActiveMode                  (etcsMode::mode_SN);
    setReasonOfEmrBreakEnum        (reasonOfBreakEnum::reason_noEmergencyBrake);
    setReasonOfEmrBreakText        ("");
    setNextLevel                   (etcsLevel::level_undefined, false);
    setNextMode                    (etcsMode::mode_undefined, false);
    setRadioState                  (radioState::radio_noConnection);
    setTargetSpeed                 (0);
    setTargetDistance              (0);
    setBrakeApplicationPointDistance(0);
    setReleaseSpeed                (0);
    setPermittedSpeed              (165);
    setAlertSpeed                  (0);
    setEmergencyBreakSpeed         (0);
    setServBreakSpeed              (0);
    setTargetSpeedMonitoring       (false);
    AccelerationSpeedTimer.start();
}
void zusi3etcs::calcTti() {
    const float& bad = brakeApplicationPointDistance;   // Distance until permitted speed begins to decrease.
    const float& V = speedF;                            // Current train speed [m/s].
    const float& a = accelerationF;                     // Current train acceleration [m/s²].
    if (bad > 1000) return;                             // We dont need to go on, if the distance is that high.
    float tti = std::numeric_limits<float>::infinity(); // Our TTI, with its initial value infinity.
    if (!targetInfoPresent || bad <= 0.0f) {            // If no target info is present or the brake curve start point is already passed, ...
        emit newTti(tti, false);                        // ...TTI is infinite and may not be be displayed
        return;
    }
    if (std::abs(a) < 0.01f) {                          // If the speed is nearly constant (acceleration ~0), ...
        tti = bad / V;                                  // ...we can use that simple formula to calculate TTI.
        emit newTti(tti, true);
    } else {                                            // But if the speed is changing by a constant acceleration, ...
                                                        // Its getting complicated:
        float discr = V * V + 2 * a * bad;              // Discriminant of quadratic equation.
        if (discr < 0.0f) {                             // No real solution (would never reach brake curve point), ...
            emit newTti(tti, false);                    // ...TTI is infinite and may not be be displayed.
            return;
        }
        float t1 = (-V + std::sqrt(discr)) / a;         // Calculate both possible solutions.
        float t2 = (-V - std::sqrt(discr)) / a;
        if (t1 < 0 && t2 < 0) {                         // Both candidates would not be physically meaningful and ...
            emit newTti(tti, false);                    // ...TTI is infinite and may not be be displayed.
        }
        else{
            tti = (t1 > 0) ? t1 : ((t2 > 0) ? t2 : 0);  // Choose positive time solution (physically meaningful).
            emit newTti(tti, true);
        }
    }
}

void zusi3etcs::setSpeed(float speed){
    if(speed < 0) speed = 0.0f;
    quint16 vActNew = static_cast<quint16>(speed * 3.6f);
    if (vAct != vActNew) {
        if((vAct ==  0 && vActNew >  0) || (vAct >  0 && vActNew ==  0)) updateButtons();
        if((vAct <= 40 && vActNew > 40) || (vAct > 40 && vActNew <= 40)) updateButtons();
        vAct = vActNew;
        if (activeLevel == level_1 || activeLevel == level_2 || activeLevel == level_3) {
            emit newOverspeed((vAct > permittedSpeed) && (vAct > releaseSpeed));
        }
    }
    speedF = speed;
    static bool firstCall = true;
    if (firstCall) {
        recentSpeeds.clear();
        recentSpeeds.append(speedF);
        AccelerationSpeedTimer.restart();
        firstCall = false;
        return;
    }
    float deltaTime = static_cast<float>(AccelerationSpeedTimer.elapsed()) / 1000.0f;
    if (deltaTime >= 0.1f) { // Nur berechnen, wenn ausreichend Zeit vergangen ist
        recentSpeeds.append(speedF);
        if (recentSpeeds.size() > maxAccelSamples)
            recentSpeeds.removeFirst();

        // Glätten über Mittelwert
        float sum = std::accumulate(recentSpeeds.begin(), recentSpeeds.end(), 0.0f);
        float smoothedSpeed = sum / static_cast<float>(recentSpeeds.size());

        float deltaSpeed = smoothedSpeed - speedOldF;
        if (std::abs(deltaSpeed) > 0.001f && std::abs(deltaSpeed) < 10.0f) {
            accelerationF = static_cast<float>(qRound (deltaSpeed * 10 / deltaTime)) / 10;
            speedOldF = smoothedSpeed;
        } else if (deltaTime > 0.2f) {
            accelerationF = 0.0f;
            speedOldF = smoothedSpeed;
        }
        AccelerationSpeedTimer.restart();
    }
    calcTti();
}
//if((vAct > permittedSpeed) && (vAct > releaseSpeed))playEraSound("qrc:/sounds/S1_toofast.wav");// Req 7.4.3.3
void zusi3etcs::setTdeState(quint8 state){
    if(D) qDebug() << "TdeState " << static_cast<etcsSettingsState>(state);
    if(tdestate == static_cast<etcsSettingsState>(state))return;
    tdestate = static_cast<etcsSettingsState>(state);
    updateButtons();

}
void zusi3etcs::addNtcToList(quint16 index, QString name){
    if(index == 1){
        listNtc.clear();
        listNtc.append("No NTC");
    }
    if(listNtc.length() < index){
     listNtc[index] = name;
    }
    else{
        listNtc.append(name);
    }
}
void zusi3etcs::setBrakingPercentage(quint16 percentage){brakingPercentage = percentage;}
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
            default: break;
        }
    }
}
void zusi3etcs::setAirTight(QString airTight){emit newActiveAit(airTight);} // Not yet part of Zusi TCP.
void zusi3etcs::setLoadingGauge(QString loadingGauge){emit newActiveLdg(loadingGauge);} // Not yet part of Zusi TCP.
void zusi3etcs::setTrainLength(quint16 length){trainLength = length;}
void zusi3etcs::setMaxSpeed(quint16 speed){maxSpeed = speed;}
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
            default: break;
        }
    }
}
void zusi3etcs::setTrainNumber(QString number){     trainNumber     = number;}
void zusi3etcs::setDriverId(QString id){            driverId        = id;}
void zusi3etcs::setRbcNumber(quint32 number){       rbcNumber       = number;}
void zusi3etcs::setRbcTelNumber(QString number){    telNumber       = number;}
void zusi3etcs::setRbcId(quint32 id){               rbcId           = id;}
void zusi3etcs::setRbcCountry(quint32 country){     rbcCountry      = country;}
void zusi3etcs::setRadioNetworkId(quint32 id){      radioNetworkId  = id;}
void zusi3etcs::setRadioNetworkIdIsFix(quint8 isFix){radioNetworkIdIsFix = static_cast<fixedradioNetwork>(isFix);}
void zusi3etcs::setAdhesionValue(quint8 value){
    adhesionValue = static_cast<adhesionValueState>(value);
    emit newAdhesionIcon(era::adhesionReduced[adhesionValue]);}
void zusi3etcs::setEtcsPassiveSwitch(quint8 passiveSwitch){etcsPassiveSwitch = static_cast<switchState>(passiveSwitch);}
void zusi3etcs::setEtcsFaultySwitch(quint8 faultySwitch){
    etcsFaultySwitch = static_cast<switchState>(faultySwitch);
}
void zusi3etcs::setAirShutOff(quint8 shutOff){          airShutOff          = static_cast<valveState>(shutOff);}
void zusi3etcs::setEtcsAcknowledger(quint8 acknowledger){etcsAcknowledger   = static_cast<requestSwitchState>(acknowledger);}
void zusi3etcs::setOverrideRequested(quint8 requested){ overrideRequested   = static_cast<requestSwitchState>(requested);}
/*
void zusi3etcs::setRequestedLevel(quint16 level){       requestedLevel      = level;}
void zusi3etcs::setIndexOfSelectedStm(quint16 index){   indexOfSelectedStm  = index;}
void zusi3etcs::setRequestedModus(quint16 modus){       requestedModus      = modus;}
void zusi3etcs::setTafModus(quint8 modus){              tafModus            = modus;}*/
void zusi3etcs::setTrainRestart(quint8 restart){        trainRestart        = restart;}
void zusi3etcs::setMaxBaseline(quint8 baseline){
    mxBaseline          = static_cast<baselineType>(baseline);
    if(D)qDebug() << "Max baseline" << mxBaseline;
}
void zusi3etcs::setVehicleHasEtcsCB(quint8 hastCB){     vehicleHasEtcsCB    = hastCB;}
void zusi3etcs::setVehicleHasEtcsPS(quint8 hastCB){     vehicleHasEtcsPS    = hastCB;}
void zusi3etcs::setVehicleHasEtcsRSW(quint8 hastRSW){   vehicleHasEtcsRSW   = hastRSW;}
void zusi3etcs::setVehicleHasEtcsRSK(quint8 hastRSK){   vehicleHasEtcsRSK   = hastRSK;}
void zusi3etcs::setEtcsCBState(quint8 cBState){         etcsCBState         = static_cast<circuitBreakerState>(cBState);}
void zusi3etcs::setEvcTyte(QString type){               evcTyte             = type;}
void zusi3etcs::setActiveLevel(quint16 level){
    if(level == 1)level = 6; // Workaround: 6 as level PZB/LZB, because we dont get info which NTC will be the next
    if(activeLevel != level){
        activeLevel = static_cast<etcsLevel>(level);
        emit newActiveLevel(activeLevel);
        emit newActiveLevelIcon(era::levelIcons[activeLevel]);
        setNextLevel(level_undefined, acknowledge_effective);   // Workaround: Zusi does not relaiably reset next acknowledger after level change
        updateButtons();
        if(D)qDebug() << "========================================";
        if(D)qDebug() << "01    level: " << QtEnumToString(activeLevel);
    }
}
void zusi3etcs::setActiveMode(quint16 mode){
    if(activeMode != mode){
        emit newEvcPresent(true);
        activeMode = static_cast<etcsMode>(mode);
        emit newActiveMode(activeMode);
        emit newActiveModeIcon(era::modeIcons[mode]);
        setNextLevel(mode_undefined, acknowledge_effective);   // Workaround: Zusi does not relaiably reset next acknowledger after mode change
        updateButtons();
        if(D)qDebug() << "02    ActiveMode " << QtEnumToString(activeMode);
    }
    if(activeMode == nextMode)setNextMode(mode, acknowledge_effective);
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
        updateButtons();
        if(D)qDebug() << "03    ReasonOfEmrBreak " << QtEnumToString(reasonOfEmrBreakEnumeratiom);
    }
}
void zusi3etcs::setReasonOfEmrBreakText(QString reason){
    reasonOfEmrBreakText = reason;
    if(D)qDebug() << "04    reasonOfEmrBreakText " << reasonOfEmrBreakText;
}
void zusi3etcs::setIndexOfActiveStm(quint16 index){
    if(indexOfActiveNtc != index){
        indexOfActiveNtc = index;
        if(index > listNtc.length() - 1) return;
        if(D)qDebug() << "05 01 IndexOfActiveStm " << indexOfActiveNtc << " " << listNtc.at(indexOfActiveNtc);
    }
}
void zusi3etcs::setNextLevel(quint16 level, quint8 acknowledge){
    bool sync = false;
    if(level == 1)level = 6; // Workaround: 6 as level PZB/LZB, because we dont get info which NTC will be the next
    if(nextLevel != level){
        nextLevel = static_cast<etcsLevel>(level);
        if(D)qDebug() << "06 01 NextLevel            " << QtEnumToString(nextLevel);
        sync = true;
    }
    if(nextLevelAcknowledge != acknowledge){
        nextLevelAcknowledge = static_cast<acknowledgeState>(acknowledge);
        emit newNextLevelAckFrame(nextLevelAcknowledge == acknowledge_necessary);
        if(D)qDebug() << "06 02 nextLevelAcknowledge " << QtEnumToString(nextLevelAcknowledge);
        sync = true;
    }
    if(sync){
        switch (nextLevelAcknowledge) {
        case acknowledge_notNecessary:
        case acknowledge_notYetNecessary:
        case acknowledge_necessaryButYetNotAllConditionsGiven: emit newNextLevelIcon(era::nextLevelIcons[nextLevel]); break;
        case acknowledge_necessary: emit newNextLevelIcon(era::nextLevelIconsAck[nextLevel]); break;
        case acknowledge_done:      emit newNextLevelIcon(era::nextLevelIcons[nextLevel]); break;
        case acknowledge_effective: emit newNextLevelIcon(era::nextLevelIcons[level_undefined]); break;
        }
    }
}
void zusi3etcs::setNextMode(quint16 mode, quint8 acknowledge){
    bool sync = false;
    if(nextMode != mode){
        nextMode = static_cast<etcsMode>(mode);
        if(D)qDebug() << "07 01 nextMode: " << QtEnumToString(nextMode);
        sync = true;
    }
    if(nextModeAcknowledge != acknowledge){
        nextModeAcknowledge = static_cast<acknowledgeState>(acknowledge);
        if(D)qDebug() << "07 02 nextModeAcknowledge: " << QtEnumToString(nextModeAcknowledge);
        emit newNextLevelAckFrame(nextModeAcknowledge == acknowledge_necessary);
        sync = true;
    }
    if(sync){
        switch (nextModeAcknowledge) {
        case acknowledge_notNecessary:
        case acknowledge_notYetNecessary:
        case acknowledge_necessaryButYetNotAllConditionsGiven:
            break;
        case acknowledge_necessary:
            emit newNextLevelIcon(era::nextModeIcons[nextMode]);
            break;
        case acknowledge_done:
            emit newNextLevelIcon(era::nextModeIcons[mode_undefined]);
            break;
        case acknowledge_effective:
            emit newNextLevelIcon(era::nextModeIcons[mode_undefined]);
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
        updateButtons();
    }
    if(D)qDebug() << "08 01 radio: " << QtEnumToString(radioConnectionState);
}
void zusi3etcs::setAnnouncementOfNextStm(quint16 index){        // Not implemented in Zusi
    nextStm = index;
    if(index > listNtc.length() - 1) return;
    if(D)qDebug() << "next   STM: " + QString::number(indexOfActiveNtc) + " " + listNtc.at(0);
}
void zusi3etcs::setTargetSpeed(float speed){
    if(speed < 0)return;
    targetSpeedF = speed;
    targetSpeed = static_cast<quint16>(qRound(speed * 3.6f));
    targetInfoPresent = targetSpeed < permittedSpeed;
    emit newVTarget(targetSpeed,  activeLevel > level_Stm, true);
    emit newTarDist(targetDistance, showTargetDistanceGraph, showTargetDistanceDigital, true);
    //if(D)qDebug() << "09    TargetSpeed: " << targetSpeed;
}
void zusi3etcs::setTargetDistance(float distance){
    if(targetDistance != distance){
        targetDistance = static_cast<quint16>(distance);
        if(distance < 0){                               // Zielweg in m (Wert<0 → dunkel)
            emit newTarDist(0, false, false, true);
            return;
        }
        showTargetDistanceGraph =    (activeMode == mode_FS && targetInfoPresent)||
                                      activeMode == mode_RV;   // Req. 8.2.2.1.8 Assuming national value uses it at CSM
        showTargetDistanceDigital =  (activeMode == mode_FS || // Req. 8.2.2.2.7 Assuming national value uses it at CSM
                                      activeMode == mode_OS ||
                                      activeMode == mode_SR)&&
                                     (supState   == state_TSM ||
                                      supState   == state_RSM ||
                                     (supState   == state_CSM && targetInfoPresent));
        emit newTarDist(targetDistance, showTargetDistanceGraph, showTargetDistanceDigital, true);
    if(D)qDebug() << "0A    targetDistance: " << targetDistance;
    }
    calcTti();
}
void zusi3etcs::setBrakeApplicationPointDistance(float distance){
    brakeApplicationPointDistance = distance;
    //calcTti();
    if(D)qDebug() << "0B    brakeApplicationPointDistance: " << brakeApplicationPointDistance;
}
void zusi3etcs::setReleaseSpeed(float speed){
    if(activeLevel < etcsLevel::level_1)return;    // Workaround: Zusi also send a release speed if there is no ETCS
    if(speed < 0) speed = 0;
    if(speed > 0) supState = supervisionStatus::state_RSM;
    if(releaseSpeed != static_cast<quint16>(qRound(speed * 3.6f))){
        releaseSpeed = static_cast<quint16>(qRound(speed * 3.6f));
        emit newReleaseSpeed(releaseSpeed);
        if(D)qDebug() << "0C    releaseSpeed: " << releaseSpeed;
    }
}
void zusi3etcs::setPermittedSpeed(float speed){
    if(speed < 0)return;
    permittedSpeedF = speed;
    permittedSpeed = static_cast<quint16>(qRound(speed * 3.6f));
    targetInfoPresent = targetSpeed < permittedSpeed;
    if(supState == supervisionStatus::state_CSM)
        permittedSpeedF0 = permittedSpeedF;
  //calcTti();
    if(activeLevel < level_1)return; // Workaround: Zusi also send a permitted speed if there is no ETCS
    emit newVPermit(permittedSpeed, true, true);
    emit newOverspeed((vAct > permittedSpeed) && (vAct > releaseSpeed));// Req 7.4.3.1 & 7.4.3.2 (OvS)
    emit newTarDist(targetDistance, showTargetDistanceGraph, showTargetDistanceDigital, true);
    if((vAct > permittedSpeed) && (vAct > releaseSpeed))playEraSound("qrc:/sounds/S1_toofast.wav");// Req 7.4.3.3
    if(D)qDebug() << "0D    permittedSpeed: " << permittedSpeed;
}
void zusi3etcs::setAlertSpeed(float speed){
    if(speed < 0)return;
    alertSpeed = static_cast<quint16>(qRound(speed * 3.6f));
    if(reasonOfEmrBreakEnumeratiom     != reason_overSpeed &&
       reasonOfSrvBrakeEnumeration != reason_overSpeed){
        emit newVOverspeed(alertSpeed);
        if(D)qDebug() << "0E    AlertSpeed: " << alertSpeed;
    }
}
void zusi3etcs::setServBreakSpeed(float speed){
    if(speed < 0)return;
    serviceBreakSpeed = static_cast<quint16>(qRound(speed * 3.6f));
    if(reasonOfEmrBreakEnumeratiom     == reason_overSpeed &&
       reasonOfSrvBrakeEnumeration != reason_overSpeed){
        emit newVOverspeed(serviceBreakSpeed);
    }
    if(D)qDebug() << "0F    serviceBreakSpeed: " << serviceBreakSpeed;
}
void zusi3etcs::setEmergencyBreakSpeed(float speed){
    if(speed < 0)return;
    emergencyBreakSpeed = static_cast<quint16>(qRound(speed * 3.6f));
    if(reasonOfSrvBrakeEnumeration == reason_overSpeed){
        emit newVOverspeed(emergencyBreakSpeed);
    }
    if(D)qDebug() << "10    EmergencyBreakSpeed: " << emergencyBreakSpeed;
}
void zusi3etcs::setTargetSpeedMonitoring(bool tsm){
    if(permittedSpeed == targetSpeed) tsm = false; // Workaround: Sometimes Zusi sends TSM = true, even though it makes no sense.
    if((activeLevel != level_Pzb) && (activeLevel != level_Stm))
        emit newVMaxReducing(tsm);
    if(tsm)
        supState = supervisionStatus::state_TSM;
    else
        if(releaseSpeed == 0)
            supState = supervisionStatus::state_CSM;
    if(D)qDebug() << "11    TargetSpeedMonitoring: " << tsm;
}
void zusi3etcs::addPlanningInfo(quint16 origin, float speed, float distance, float gradient, quint16 parameter){
    previewPoint newPreviewPoint = { origin, speed, distance, gradient, parameter};
    planningPreviewPoints.append(newPreviewPoint);
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
    if(D)qDebug() << "17    elOrder:" <<  QtEnumToString(elOrder);
    switch (elOrder) {
    case el_noOrder:                        emit newIconB5(era::ordersAndAnnouncementofTrackConditionIcons[0]);break;
    case el_prennouncementMainSwitchOff:    emit newIconB5(era::ordersAndAnnouncementofTrackConditionIcons[7]);break;   // TC07
    case el_orderMainSwitchOff:             emit newIconB5(era::ordersAndAnnouncementofTrackConditionIcons[6]);break;   // TC06
    case el_orderMainSwitchOn:              emit newIconB5(era::ordersAndAnnouncementofTrackConditionIcons[9]);break;   // TC09
    case el_prennouncementLowerPantograph:  emit newIconB5(era::ordersAndAnnouncementofTrackConditionIcons[3]);break;   // TC03
    case el_orderLowerPantograph:           emit newIconB5(era::ordersAndAnnouncementofTrackConditionIcons[2]);break;   // TC02
    case el_orderRaisPantograph:            emit newIconB5(era::ordersAndAnnouncementofTrackConditionIcons[5]);break;   // TC05
    }
}
void zusi3etcs::setEtcsTestRunnState(quint8 state){etcsTestRunn = static_cast<etcsTestRunnState>(state);}
void zusi3etcs::setEvcBootupState(quint8 state){evcBootup = static_cast<evcBootUpState>(state);}
void zusi3etcs::setImmediatTextMessage(QString msgText){
    setTextMessage(0, QDateTime::currentDateTime(), 0, "", msgText, 0);
    doStuffAfterFinishOfZusiTelegram();
}
void zusi3etcs::setTextMessage(uint32_t msgID, QDateTime timeStamp, uint16_t zusiKennung, QString msgTypeEtcsSpec,QString msgText, uint8_t ackNeeded){
    EtcsTextMessage newMessage = { msgID, timeStamp, zusiKennung, msgTypeEtcsSpec, msgText, "", ackNeeded };
    messageList.append(newMessage);
}
void zusi3etcs::doStuffAfterFinishOfZusiTelegram(){
    emit newTextMessages(messageList);
    messageList.clear();
    planningPreviewPoints.clear();
    emit newEmrOrSrvBrakeState(                   reasonOfEmrBreakEnumeratiom != reason_noEmergencyBrake ||
                                                  reasonOfSrvBrakeEnumeration != reason_noEmergencyBrake);
    emit newEmrOrSrvBrakeIcon(era::emergencyBrake[reasonOfEmrBreakEnumeratiom != reason_noEmergencyBrake ||
                                                  reasonOfSrvBrakeEnumeration != reason_noEmergencyBrake]);
    emit newIntervenation(                        reasonOfEmrBreakEnumeratiom == reason_overSpeed ||
                                                  reasonOfSrvBrakeEnumeration == reason_overSpeed );
    if(!reasonEmergencyBrackeWasTransmitted)      reasonOfEmrBreakEnumeratiom =  reason_noEmergencyBrake;
    if(!reasonServiceBrackeWasTransmitted)        reasonOfSrvBrakeEnumeration =  reason_noEmergencyBrake;
    if(!elOrderWasTransmitted)                    setElOrder(el_noOrder);
    overrideActiveWasTransmitted = false;
    reasonEmergencyBrackeWasTransmitted = false;
    reasonServiceBrackeWasTransmitted = false;
    elOrderWasTransmitted = false;
}
void zusi3etcs::setSimTime(QString time){simTime = time;}
void zusi3etcs::playEraSound(QString filename){
    (void)filename;
    /*
    QMediaPlayer *sound =new QMediaPlayer;          // workaround: QSound::play crash on some android devices
    sound->setMedia(QUrl(filename));
    sound->setVolume(50);
    sound->play();*/
}
void zusi3etcs::updateButtons(){
    // Enable or disable buttons as specified in SRS 3.6
    // Chapter 11.2: Table 33, 34
    bool tdeOk = tdestate == trainDataEntryEntered;
    bool v0 = vAct == 0;
    bool vLt40 = vAct <= 40;
    bool lOk = activeLevel != level_undefined;
    bool l0 = activeLevel == level_0;
    bool l1 = activeLevel == level_1;
    bool l2 = activeLevel == level_2;
    bool ln = activeLevel == level_Pzb || activeLevel == level_Stm;
    bool rOk = radioConnectionState == radio_connectionEstablised;
    bool nb = reasonOfEmrBreakEnumeratiom != reason_noEmergencyBrake;
    bool SB = activeMode == mode_SB;
    bool FS = activeMode == mode_FS;
    bool OS = activeMode == mode_OS;
    bool SR = activeMode == mode_SR;
    bool SH = activeMode == mode_SH;
    bool UN = activeMode == mode_UN;
    bool NL = activeMode == mode_NL;
    bool SN = activeMode == mode_SN;
    bool LS = activeMode == mode_LS;
    bool PT = activeMode == mode_PT;
    bool nLs = false; // non leading signal
    bool pSs = false; // passive shunting singnal
    bool dIdOk = true; // Driver ID Valid - not delivered by Zusi
    bool trnOk = true; // Train running number valid - not delivered by Zusi
    bool nvChDidMoov = false; // National value, that allows to change driver ID while running
    quint8 limEoa = 40; // limit for triggering the override function
    bool startBtnActive =
        (v0 && SB && dIdOk && tdeOk && lOk && trnOk) ||
        (v0 && PT && tdeOk && (l1 || (l2 && rOk && nb))) ||
        (SR && l2 && rOk);
    bool driverIdBtnActive =
        (v0 && SB && dIdOk && lOk) ||
        ((nvChDidMoov || (!nvChDidMoov && v0)) && (SH||FS||LS||SR||OS||NL||UN||SN));
    bool TdeBtnActive =
        v0 && dIdOk && lOk && (SB||FS||LS||SR||OS||UN||SN);
    bool levelBtnActive =
        v0 && dIdOk && (SB||FS||LS||SR||OS||NL||UN||SN);
    bool trnBtnActive =
        (v0 && SB && dIdOk && lOk)|| (FS||LS||SR||OS||NL||UN||SN);
    bool shuntingBtnActive =
        (v0 && dIdOk && (SB||FS||LS||SR||OS||UN||SN) && lOk && ((l0||l1||ln) || (l2 && rOk))) ||
        (v0 && PT && (l1 || (l2 && rOk && nb)));
    bool exitShuntingBtnActive =
        v0 && (SH) ;
    bool nonLeadingBtnActive = v0 && dIdOk && lOk && (SB||SH||FS||LS||SR||OS) && nLs ;
    bool maintainShuntingBtnActive = SH && pSs;
    bool radioDataBtnActive =
        v0 && dIdOk && lOk && (SB||FS||LS||SR||OS||NL||PT||UN||SN);
    bool eoaBetnActive =
        (vLt40 && (FS||LS||SR||OS||UN||SN||SH))||
        (vLt40 && SB && dIdOk && tdeOk && trnOk && lOk && l2)||
        (vLt40 && PT && tdeOk && trnOk);
    emit newBehavBtnStart(startBtnActive);
    emit newBehavBtnLevel(levelBtnActive);
    emit newBehavBtnShunting(shuntingBtnActive);
    emit newBehavBtnNonLeading(nonLeadingBtnActive);
    emit newBehavBtndriverId(driverIdBtnActive);
    emit newBehavBtnTde(TdeBtnActive);
    emit newBehavBtnTrn(trnBtnActive);
    emit newBehavBtnExitShunting(exitShuntingBtnActive);
    emit newBehavBtnMaintainShunting(maintainShuntingBtnActive);
    emit newBehavBtnRadio(radioDataBtnActive);
    emit newBehavBtnEoa(eoaBetnActive);
}





#ifndef ZUSI3ETCS_H
#define ZUSI3ETCS_H
#include <QObject>
#include <QMetaEnum>
#include "era.h"
class zusi3etcs : public QObject
{
    Q_OBJECT
public:
    zusi3etcs();
    enum etcsLevel: quint16
    {
        level_undefined  = 0,
        level_Stm        =  1,
        level_0          =  2,
        level_1          =  3,
        level_2          =  4,
        level_3          =  5,
        level_Pzb        =  6,
    };
    Q_ENUM(etcsLevel)
    enum etcsMode: quint16
    {
        mode_undefined  = 0,
        mode_FS         =  1,
        mode_OS         =  2,
        mode_SR         =  3,
        mode_SH         =  4,
        mode_UN         =  5,
        mode_SL         =  6,
        mode_SB         =  7,
        mode_TR         =  8,
        mode_PT         =  9,
        mode_SF         = 10,
        mode_IS         = 11,
        mode_NP         = 12,
        mode_NL         = 13,
        mode_SE         = 14,
        mode_SN         = 15,
        mode_RV         = 16,
        mode_LS         = 17,
        mode_PS         = 18
    };
    Q_ENUM(etcsMode)
    enum etcsSettingsState: quint8
    {
        trainDataEntryUndefined = 0,
        trainDataEntryNecessary = 1,
        trainDataEntryEntered = 2,
        startOfMissionFinished = 3
    };
    Q_ENUM(etcsSettingsState)
    enum fixedradioNetwork: quint16
    {
        fixedOnVehicle  = 0,
        toBeEnteredByDriver  =  1
    };
    Q_ENUM(fixedradioNetwork)
    enum adhesionValueState: quint8
    {
        reduced  = 1,
        notReduced  =  2
    };
    Q_ENUM(adhesionValueState)
    enum switchState: quint8
    {
        applied_position  = 1,
        normal_position  =  2
    };
    Q_ENUM(switchState)
    enum valveState: quint8
    {
        closed  = 1,
        open  =  2
    };
    Q_ENUM(valveState)
    enum requestSwitchState: quint8
    {
        applied  = 1,
        basicPosition  =  2
    };
    Q_ENUM(requestSwitchState)
    enum baselineType: quint8
    {
        baseline_2_2_2 = 0,
        baseline_2_3_0_d = 1,
        baseline_3_6_0 = 2
    };
    Q_ENUM(baselineType)
    enum circuitBreakerState: quint8
    {
        circuit_undefined   = 0,
        circuit_off         = 1,
        circuit_on          = 2
    };
    Q_ENUM(circuitBreakerState)
    enum reasonOfBreakEnum: quint16
    {
        reason_noEmergencyBrake        =  0,
        reason_1                       =  1,
        reason_2                       =  2,
        reason_3                       =  3,
        reason_4                       =  4,
        reason_5                       =  5,
        reason_overSpeed               =  6,
        reason_TestRun                 =  7,
        reason_EvcBreakdown            = 10,
        reason_EtcsEmergencyStopPassed = 11,
        reason_EtcsStopPassed          = 15,
        reason_EtcsRollProtection      = 16,
        reason_etcsNotAcknowledged     = 17,
        reason_EtcsRaidoBreakdown      = 18,
        reason_EtcsBalisMalfunction    = 19,
        reason_EtcsManualChangeOfLevel = 20,
        reason_EtcsGeneralMalfunction  = 27,
        reason_EtcsPowerSupplyFaulty   = 28
    };
    Q_ENUM(reasonOfBreakEnum)
    enum acknowledgeState: quint16
    {
        acknowledge_notNecessary                          = 0,
        acknowledge_notYetNecessary                       = 1,
        acknowledge_necessaryButYetNotAllConditionsGiven  = 2,
        acknowledge_necessary                             = 3,
        acknowledge_done                                  = 4,
        acknowledge_effective                             = 5
    };
    Q_ENUM(acknowledgeState)
    enum radioState: quint8
    {
        radio_noConnection                        = 0,
        radio_connectionEstablishment             = 1,
        radio_connectionEstablised                = 2,
        radio_connectionReestablishment           = 3,
        radio_connectionDialUpError               = 4,
        radio_connectionMalfunction               = 5,
        radio_NoNetwork                           = 6,
        radio_ConnentionCanotBeEstablisht_Failure = 7
    };
    Q_ENUM(radioState)
    enum tafState: quint8
    {
        taf_notActive       = 0,
        taf_willBeNecessary = 1,
        taf_requested       = 2,
        taf_acknowledged    = 3,
        taf_denied          = 4
    };
    Q_ENUM(tafState)
    enum emergencyStopState: quint8
    {
        emergencyStop_none              = 0,
        emergencyStop_conditionally     = 1,
        emergencyStop_notConditionally  = 2
    };
    Q_ENUM(emergencyStopState)
    enum elOrderState: quint8
    {
        el_noOrder                       = 0, // Not part of spec
        el_prennouncementMainSwitchOff   = 1,
        el_orderMainSwitchOff          = 2,
        el_orderMainSwitchOn           = 3,
        el_prennouncementLowerPantograph = 8,
        el_orderLowerPantograph        = 9,
        el_orderRaisPantograph         = 10
    };
    Q_ENUM(elOrderState)
    enum etcsTestRunnState: quint8
    {
        testRun_NoVotableState         = 0,
        testRun_EtcsbrakeIntervention  = 1,
        testRun_OtherbrakeIntervention = 2,
        testRun_acknowledgeExpected    = 3,
        testRun_successful             = 4,
        testRun_notSuccessful          = 5
    };
    Q_ENUM(etcsTestRunnState)
    enum evcBootUpState: quint8
    {
        bootUp_undefined                              = 0, // Not part of spec
        bootUp_Start_ModeSf_brakePipeVoided           = 1,
        bootUp_DmiAlmostDark                          = 2,
        bootUp_brakePipeFilling_ModeSb_selfTestStarts = 3,
        bootUp_selfTestOk                             = 4,
        bootUp_StmWillBeFound                         = 5
    };
    Q_ENUM(evcBootUpState)
    enum planningInfoOrigin: quint8
    {
        origin_track  = 1,
        origin_mainSignal = 3,
        origin_shuntingSignal = 9,
        origin_etcs = 14,
        origin_infrastructureInfo_furtherInfoInParamerter = 17
    };Q_ENUM(planningInfoOrigin)
    enum planningInfoParameter: quint8
    {
        parameter_commandLowerPantograph = 2,
        parameter_commandRaisPantograph  = 4,
        parameter_commandMainSwitchOff   = 6,
        parameter_commandMainSwitchOn    = 8,
    };Q_ENUM(planningInfoParameter)
private:
    bool D = false;
    bool ackTextMessagePersistend = false;
    QString simTime = "--:--";
    template<typename QEnum>
    QString QtEnumToString (const QEnum value)
    {
      return QString(QMetaEnum::fromType<QEnum>().valueToKey(value));
    }
    QStringList         textMessages[2] = {{""},{""}};//{{"00:00"},{"Beispiel zum testen"}};
    quint16             vAct = 0;
    etcsSettingsState   tdestate = etcsSettingsState::trainDataEntryUndefined;
    quint16             nextNtcIindex = 0;
    QStringList        *listNtc;
    quint16             brakingPercentage = 0;
    quint16             trainCategory = 0;
    quint16             trainLength = 0;
    quint16             maxSpeed = 0;
    quint16             axleLoad = 0;
    QString             trainNumber = "";
    QString             driverId = "";
    quint32             rbcNumber = 0;
    QString             telNumber = "";
    quint32             rbcId = 0;
    quint32             rbcCountry = 0;
    quint32             radioNetworkId = 0;
    fixedradioNetwork   radioNetworkIdIsFix = fixedradioNetwork::fixedOnVehicle;
    adhesionValueState  adhesionValue = adhesionValueState::notReduced;
    switchState         etcsPassiveSwitch = switchState::normal_position;
    switchState         etcsFaultySwitch = switchState::normal_position;
    valveState          airShutOff = valveState::open;
    requestSwitchState  etcsAcknowledger = requestSwitchState::basicPosition;
    requestSwitchState  overrideRequested = requestSwitchState::basicPosition;
  //etcsLevel           requestedLevel;
  //etcsMode            requestedModus;
  //quint8              tafModus;
    quint8              trainRestart = 0;
    baselineType        mxBaseline = baselineType::baseline_3_6_0;
    quint8              vehicleHasEtcsCB = 0;   // CB: circuit breaker
    quint8              vehicleHasEtcsPS = 0;   // PS: passive switch
    quint8              vehicleHasEtcsRSW = 0;  // RSW: reset switch
    quint8              vehicleHasEtcsRSK = 0;  // RSK: reset softkey
    circuitBreakerState etcsCBState = circuitBreakerState::circuit_undefined; // CB: circuit breaker
    QString             evcTyte = "";

    etcsLevel           activeLevel = etcsLevel::level_undefined;
    etcsMode            activeMode = etcsMode::mode_undefined;
    reasonOfBreakEnum   reasonOfEmrBreakEnumeratiom = reasonOfBreakEnum::reason_noEmergencyBrake;
    QString             reasonOfEmrBreakText = "";
    quint16             indexOfActiveNtc = 0;
    quint16             nextStm = 0;
    etcsLevel           nextLevel = etcsLevel::level_undefined;
    acknowledgeState    nextLevelAcknowledge = acknowledgeState::acknowledge_notNecessary;
    etcsMode            nextMode = etcsMode::mode_undefined;
    acknowledgeState    nextModeAcknowledge = acknowledgeState::acknowledge_notNecessary;
    radioState          radioConnectionState = radioState::radio_noConnection;
    quint16             targetSpeed = 0;
    quint16             targetDistance = 0;
    float               brakeApplicationPointDistance = 0;
    quint16             releaseSpeed = 0;
    quint16             permittedSpeed = 0;
    quint16             alertSpeed = 0;
    quint16             serviceBreakSpeed = 0;
    quint16             emergencyBreakSpeed = 0;
    bool                permittedSpeedReducing = false;
    tafState            trackAheadFreeRequestState = tafState::taf_notActive;
    switchState         overrideActive = switchState::normal_position;
    emergencyStopState  emergencyStop = emergencyStopState::emergencyStop_none;
    reasonOfBreakEnum   reasonOfSrvBrakeEnumeration = reasonOfBreakEnum::reason_noEmergencyBrake;
    elOrderState        elOrder = elOrderState::el_noOrder;
    etcsTestRunnState   etcsTestRunn = etcsTestRunnState::testRun_NoVotableState;
    evcBootUpState      evcBootup = evcBootUpState::bootUp_undefined;
    QString             textMessage = "";
    bool                OvS = false;
    bool                WaS = false;
    bool                IntS = false;
    bool                reasonServiceBrackeWasTransmitted = false;
    bool                reasonEmergencyBrackeWasTransmitted = false;
    bool                overrideActiveWasTransmitted = false;
    bool                textMessageByEnumWasTransmitted = false;
    bool                elOrderWasTransmitted = false;
    bool                targetInfoPresent = false;
    class planningInfo : public QObject
    {
        public:
        enum planningInfoOrigin origin;
        quint16 speed = 0;
        qint32 distance = 0;
        qint32 altitude = 0;
        enum planningInfoParameter parameter ;
    };
    QList<planningInfo *> planningInfos;

public slots:
    void setSimTime(            QString time);
    void setSpeed(              quint16 speed);
    void setTdeState(           quint8  state);
    void addNtcToListIndex(     quint16 index);
    void addNtcToListName(      QString name);
    void setBrakingPercentage(  quint16 percentage);
    void setTrainCategory(      quint16 category);
    void setAirTight(           QString airTight); // Not yet part of Zusi TCP.
    void setLoadingGauge (QString loadingGauge); // Not yet part of Zusi TCP.
    void setTrainLength(        quint16 length);
    void setMaxSpeed(           quint16 speed);
    void setAxleLoad(           quint16 load);
    void setTrainNumber(        QString number);
    void setDriverId(           QString id);
    void setRbcNumber(          quint32 number);
    void setRbcTelNumber(       QString number);
    void setRbcId(              quint32 id);
    void setRbcCountry(         quint32 country);
    void setRadioNetworkId(     quint32 id);
    void setRadioNetworkIdIsFix(quint8  isFix);
    void setAdhesionValue(      quint8  value);
    void setEtcsPassiveSwitch(  quint8  passiveSwitch);
    void setEtcsFaultySwitch(   quint8  faultySwitch);
    void setAirShutOff(         quint8  shutOff);
    void setEtcsAcknowledger(   quint8  acknowledger);
    void setOverrideRequested(  quint8  requested);
  //void setRequestedLevel(     quint16 level);
  //void setIndexOfSelectedStm( quint16 index);
  //void setRequestedModus(     quint16 modus);
  //void setTafModus(           quint8  modus);
    void setTrainRestart(       quint8  restart);
    void setMaxBaseline(        quint8  baseline);
    void setVehicleHasEtcsCB(   quint8  hastCB);    // CB: circuit breaker
    void setVehicleHasEtcsPS(   quint8  hastCB);    // PS: passive switch
    void setVehicleHasEtcsRSW(  quint8  hastRSS);   // RSW: reset switch
    void setVehicleHasEtcsRSK(  quint8  hastRSK);   // RSK: reset softkey
    void setEtcsCBState(        quint8  cBState);   // CB: circuit breaker
    void setEvcTyte(            QString type);

    void setActiveLevel(quint16 level);
    void setActiveMode(quint16 mode);
    void setReasonOfEmrBreakEnum(quint16 reason);
    void setReasonOfEmrBreakText(QString reason);
    void setIndexOfActiveStm(quint16 index);
    void setAnnouncementOfNextStm(quint16 index);
    void setNextLevel(quint16 level, quint8 acknowledge);
    //void setNextLevelAcknowledge(quint16 level, quint8 acknowledge);
    void setNextMode(quint16 index, quint8 acknowledge);
    //void setNextModeAcknowledge(quint8 acknowledge);
    void setRadioState(quint8 state);
    void setTargetSpeed(float speed);
    void setTargetDistance(float distance);
    void setBrakeApplicationPointDistance(float distance);
    void setReleaseSpeed(float speed);
    void setPermittedSpeed(float speed);
    void setAlertSpeed(float speed);
    void setEmergencyBreakSpeed(float speed);
    void setServBreakSpeed(float speed);
    void setPermittedSpeedReducing(bool reducing);

    void addPlanningInfoOrigin(quint16 origin, quint16 index);
    void addPlanningInfoSpeed(float speed, quint16 index);
    void addPlanningInfoDistance(float distance, quint16 index);
    void addPlanningGradient(float gradient, quint16 index);
    void addPlanningParameter(quint16 parameter, quint16 index);

    void setTrackAheadFreeRequestState(quint8 RequestState);
    void setOverrideActive(quint8 active);
    void setEmergencyStop(quint8 stop);
    void setReasonServBrake(quint16 reason);
    void setElOrder(quint8 order);
    void setEtcsTestRunnState(quint8 state);
    void setEvcBootupState(quint8 state);
    void setTextMessageByEnum(quint16 message);
    void setTextMessageByString(QString message);
    void doStuffAfterFinishOfZusiTelegram();

private slots:
    void playEraSound(QString filename);

signals:
    void newVPermit(quint16 V, bool visible, bool fromEtcs);
    void newVTarget(quint16 V, bool visible, bool fromEtcs);
    void newIntervenation(bool intervenation);
    void newOverspeed(bool overspeed);
    void newVOverspeed(quint16 V);
    void newVMaxReducing(bool reducing);
    void newTarDist(quint16 distance, bool visibleBar, bool visibleDigital, bool fromEtcs);
    void newActiveLevel(quint16 level);
    void newActiveLevelIcon(QString levelIcon);
  //void newActiveLevel(QString level);
    void newNextLevelIcon(QString levelIcon);
    void newNextLevelAckFrame(bool acknowledge);
    void newActiveMode(quint16 mode);
    void newActiveModeIcon(QString modeIcon);
    void newRadioStateIcon(QString icon);
    void newEmrOrSrvBrakeIcon(QString icon);
    void newEmrOrSrvBrakeState(bool active);
    void newReleaseSpeed(quint16 speed);
    void newAdhesionIcon(QString icon);
    void newOverrideIcon(QString icon);
    void newOverrideActive(bool active);
    void newIconB5(QString icon);
    void newTextmessages(QStringList times, QStringList messages);
    void newEvcPresent(bool installed);
    void newActiveTct(QString TCT);
    void newActiveAxl(QString AXL);
    void newActiveAit(QString AIT);
    void newActiveLdg(QString LDG);
    void newBehavBtnStartOfMission(bool enabled, QString btnText);
    void newBehavBtnLevel(bool enabled, QString btnText);
    void newBehavBtnShounting(bool enabled, QString btnText);
    void newBehavBtnNonLeading(bool enabled, QString btnText);
  //void newStateLzb(bool silent);
};


#endif // ZUSI3ETCS_H

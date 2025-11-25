#include "zusipower.h"

zusiPower::zusiPower(QObject *parent) : QObject(parent){
    powerValuesToDecoder.resize(6);
    settings = new QSettings("QDmi", "QDmi-ZusiPower");
}
void zusiPower::setFahrstufe(float stufe){
    if(forwardDriveModeDisplay){
        float roundToNearest = 0;
        if(driveModeDivisor != 0)roundToNearest = 0.5;
        qint8 driveMode = static_cast<qint8>(stufe / driveModeDivisor + roundToNearest + driveModeOffset);
        if(driveMode < 0) driveMode = 0;
        emit newDriveMode(static_cast<quint8>(driveMode));
    }
}
void zusiPower::setVIst(quint16 V){
    VIst = V;
    syncPowerIndicator();
}
void zusiPower::setBaureihe(QString fahrzeug){
    bool isLoko = false;
    haveLokoInList = false;
    fahrzeug.replace('E', '1');
    if(fahrzeug.indexOf(" ") > -1)fahrzeug.truncate(fahrzeug.indexOf(" "));
    if(fahrzeug.toInt(&isLoko) == vehicleType) return;
    if(isLoko){
        vehicleType = fahrzeug.toInt();
        vehicleName = fahrzeug;
        restoreExtrems();
        zGes=0;           zGesOld=0;           // 0x0009 (9)                Zugkraft gesamt
        zPAchs=0;         zPAchsOld=0;         // 0x000A (10)               Zugkraft pro Achse
        zSollGes=0;       zSollGesOld=0;       // 0x000B (11)               Zugkraft-Soll gesamt
        zSollPAchs=0;     zSollPAchsOld=0;     // 0x000C (12)               Zugkraft-Soll pro Achse
        zGesStwg=0;       zGesStwgOld=0;       // 0x007C (124) Steuerwagen: Zugkraft gesamt
        zPAchsStwg=0;     zPAchsStwgOld=0;     // 0x007D (125) Steuerwagen: Zugkraft pro Achse
        zSollGesStwg=0;   zSollGesStwgOld=0;   // 0x007E (126) Steuerwagen: Zugkraft-Soll gesamt
        zSollPAchsStwg=0; zSollPAchsStwgOld=0; // 0x007F (127) Steuerwagen: Zugkraft-Soll pro Achse
        zSollNorm=0;      zSollNormOld=0;      // 0x0090 (144)              Zug- und Brems-Gesamtkraftsoll normiert
        zSollNormStwg=0;  zSollNormStwgOld=0;  // 0x0091 (145) Steuerwagen: Zug- und Brems-Gesamtkraftsoll normiert
        zNorm=0;          zNormOld=0;          // 0x0093 (147)              Zug- und Brems-Gesamtkraftsoll absolut normiert
        zNormStwg=0;      zNormStwgOld=0;      // 0x0094 (148) Steuerwagen: Zug- und Brems-Gesamtkraftsoll absolut normiert
        for(int i = 0; i < 69; i++){
            if(skalen[i][0] == vehicleType){
                haveLokoInList = true;
                driveModeDivisor = skalen[i][2];
                driveModeOffset = skalen[i][3];
                forwardDriveModeDisplay = skalen[i][1] > 0;
                emit hasDriveModeDisplay(forwardDriveModeDisplay);
            }
        }
    }
}

void zusiPower::relativatePower(float power, qint32 &min, qint32 &max, qint32 &old, qint32 &dest, QSettings *settings, const QString &keyPrefix){
    bool nextExtremFound = false;
    qint32 P = static_cast<qint32>(power);
    if(P > max){
        nextExtremFound = true;
        max = P;
    }
    if(P < min){
        nextExtremFound = true;
        min = P;
    }
    if (nextExtremFound && settings && !keyPrefix.isEmpty()) {
        settings->setValue(keyPrefix + "Min", min);
        settings->setValue(keyPrefix + "Max", max);
    }
    if(old != P){
        old = P;
        if(P>=0){
            if(max == 0) max = 1;
            dest = (P * 100) / max;
        }
        else{
            if(min == 0) min = 1;
            dest = (P * 100) / qAbs(min);
        }
        syncPowerIndicator();
    }
}
void zusiPower::setZugkraftProAchse(float power){
    relativatePower(power, zPAchsMin, zPAchsMax, zPAchsOld, zPAchs, settings, vehicleName + "zPAchs");
}
void zusiPower::setZugkraftSollGesammt(float power){               // 0x000B
    relativatePower(power, zSollGesMin, zSollGesMax, zSollGesOld, zSollGes, settings, vehicleName + "zSollGes");
}
void zusiPower::setZugkraftSollProAchse(float power){
    relativatePower(power,zSollPAchsMin, zSollPAchsMax,zSollPAchsOld, zSollPAchs,settings,vehicleName + "zSollPAchs");
}
void zusiPower::setZugkraft(float power){
    relativatePower(power, zGesMin, zGesMax, zGesOld, zGes, settings, vehicleName + "zGes");
}
void zusiPower::setZugkraftGesammtSteuerwagen(float power){        // 0x007C
    relativatePower(power, zGesStwgMin, zGesStwgMax, zGesStwgOld, zGesStwg, settings, vehicleName + "zGesStwg");
}
void zusiPower::setZugkraftProAchseSteuerwagen(float power){       // 0x007D
    relativatePower(power, zPAchsStwgMin, zPAchsStwgMax, zPAchsStwgOld, zPAchsStwg, settings, vehicleName + "zPAchsStwg");
}
void zusiPower::setZugkraftSollGesammtSteuerwagen(float power){    // 0x007E
    relativatePower(power, zSollGesStwgMin, zSollGesStwgMax, zSollGesStwgOld, zSollGesStwg, settings, vehicleName + "zSollGesStwg");
}
void zusiPower::setZugkraftSollProAchseSteuerwagen(float power){   // 0x007F
    relativatePower(power, zSollPAchsStwgMin, zSollPAchsStwgMax, zSollPAchsStwgOld, zSollPAchsStwg, settings, vehicleName + "zSollPAchsStwg");
}
void zusiPower::setZugkraftSollNormiert(float power){              // 0x0090
    relativatePower(power, zSollNormMin, zSollNormMax, zSollNormOld, zSollNorm, settings, vehicleName + "zSollNorm");
}
void zusiPower::setZugkraftSollNormiertSteuerwagen(float power){   // 0x0091
    relativatePower(power, zSollNormStwgMin, zSollNormStwgMax, zSollNormStwgOld, zSollNormStwg, settings, vehicleName + "zSollNormStwg");
}
void zusiPower::setZugkraftNormiert(float power){                  // 0x0093
    relativatePower(power, zNormMin, zNormMax, zNormOld, zNorm, settings, vehicleName + "zNorm");
}
void zusiPower::setZugkraftNormiertSteuerwagen(float power){       // 0x0094
    relativatePower(power, zNormStwgMin, zNormStwgMax, zNormStwgOld, zNormStwg, settings, vehicleName + "zNormStwg");
}
void zusiPower::syncPowerIndicator(){
    if(VIst == 0){ // Workarround: Zusi does not set braking force to zero, aftrer stop
        if(zGes < 0)             zGes = 0;
        if(zPAchs < 0)           zPAchs = 0;
        if(zNorm < 0)            zNorm = 0;
        if(zNormStwg < 0)        zNormStwg = 0;
        if(zGesStwg < 0)         zGesStwg = 0;
        if(zPAchsStwg < 0)       zPAchsStwg = 0;
        if(zSollGesStwg < 0)     zSollGesStwg = 0;
        if(zSollPAchsStwg < 0)   zSollPAchsStwg = 0;
        if(zSollNorm < 0)        zSollNorm = 0;
        if(zSollNormStwg < 0)    zSollNormStwg = 0;
        if(zSollGes < 0)         zSollGes = 0;
        if(zSollPAchs < 0)       zSollPAchs = 0;
    }
    std::array<qint32, 6> valsIst ={
        zGes,
        zPAchs,
        zGesStwg,
        zPAchsStwg,
        zNorm,
        zNormStwg
    };
    std::array<qint32, 6> valsSoll ={
        zSollGes,
        zSollPAchs,
        zSollGesStwg,
        zSollPAchsStwg,
        zSollNorm,
        zSollNormStwg,
    };
    qint32 ZIst  = *std::max_element(valsIst.begin() , valsIst.end() ,[](int x, int y) {return std::abs(x) < std::abs(y);});
    qint32 ZSoll = *std::max_element(valsSoll.begin(), valsSoll.end(),[](int x, int y) {return std::abs(x) < std::abs(y);});
    powerValuesToDecoder[0] = ZIst;
    powerValuesToDecoder[2] = ZSoll;
    powerValuesToDecoder[1] = ZIst;
    powerValuesToDecoder[3] = ZSoll;
    emit newPowerValues(powerValuesToDecoder);
}
void zusiPower::restoreExtrems(){
    if (!settings)return;
    auto loadExtrem = [this](const QString &prefix, qint32 &min, qint32 &max) {
        min = settings->value(prefix + "Min", 0).toInt();
        max = settings->value(prefix + "Max", 0).toInt();
    };
    loadExtrem(vehicleName + "zPAchs",        zPAchsMin,        zPAchsMax);
    loadExtrem(vehicleName + "zSollGes",      zSollGesMin,      zSollGesMax);
    loadExtrem(vehicleName + "zSollPAchs",    zSollPAchsMin,    zSollPAchsMax);
    loadExtrem(vehicleName + "zGes",          zGesMin,          zGesMax);
    loadExtrem(vehicleName + "zSollNorm",     zSollNormMin,     zSollNormMax);
    loadExtrem(vehicleName + "zNorm",         zNormMin,         zNormMax);
    loadExtrem(vehicleName + "zGesStwg",      zGesStwgMin,      zGesStwgMax);
    loadExtrem(vehicleName + "zPAchsStwg",    zPAchsStwgMin,    zPAchsStwgMax);
    loadExtrem(vehicleName + "zSollGesStwg",  zSollGesStwgMin,  zSollGesStwgMax);
    loadExtrem(vehicleName + "zSollPAchsStwg",zSollPAchsStwgMin,zSollPAchsStwgMax);
    loadExtrem(vehicleName + "zSollNormStwg", zSollNormStwgMin, zSollNormStwgMax);
    loadExtrem(vehicleName + "zNormStwg",     zNormStwgMin,     zNormStwgMax);
}

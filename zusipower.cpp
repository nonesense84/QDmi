#include "zusipower.h"

zusiPower::zusiPower(QObject *parent) : QObject(parent){
    powerValuesToDecoder.resize(6);
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
    fahrzeug.replace('E', '1');
    if(fahrzeug.indexOf(" ") > -1)fahrzeug.truncate(fahrzeug.indexOf(" "));
    int br = fahrzeug.toInt(&isLoko);
    if(isLoko){
        haveLokoInList = false;
        for(int i = 0; i < 68; i++){
            if(skalen[i][0] == br){ //{424,150,150,1,25,25,2,},
                haveLokoInList = true;
                // Typ1 kN, Ty2 kN/FM, typ3 %, Typ4 Stufen {111,140,145,1,70,75,2,},
                zMaxNumber = skalen[i][5];
                zMaxLine = skalen[i][6];
                zMinNumber = skalen[i][1];
                zMinLine = skalen[i][2];
                zMinCorr = skalen[i][4];
                zMaxCorr = skalen[i][8];
                unitBraking = unitType[skalen[i][3]];
                unitAccelerating = unitType[skalen[i][7]];
                driveModeDivisor = skalen[i][10];
                driveModeOffset = skalen[i][11];
                forwardDriveModeDisplay = skalen[i][9] > 0;
                emit unitBrakingText(unitBraking);
                emit unitAcceleratingText(unitAccelerating);
                emit maxPowerPositiveNumber(zMaxNumber);
                emit maxPowerPositiveLine(zMaxLine);
                emit maxPowerNegativeNumber(zMinNumber);
                emit maxPowerNegativeLine(zMinLine);
                emit hasDriveModeDisplay(forwardDriveModeDisplay);
            }
        }
    }
}

qint16 zusiPower::calcPower(float power){
    if(power > 0)
        return static_cast<qint16>(ceil(static_cast<double>(power) / 100000 * zMaxCorr));
    return static_cast<qint16>(ceil(static_cast<double>(power) / 100000 * zMinCorr));
}

void zusiPower::setZugkraftProAchse(float power){
    zPAchsNew = calcPower(power);
    if(zPAchs != zPAchsNew) zPAchs = zPAchsNew; syncPowerIndicator();
}
void zusiPower::setZugkraftSollGesammt(float power){               // 0x000B
     zSollGesNew = calcPower(power);
     if(zSollGes != zSollGesNew) zSollGes = zSollGesNew; syncPowerIndicator();
}
void zusiPower::setZugkraftSollProAchse(float power){
    zSollPAchsNew = calcPower(power);
    if(zSollPAchs != zSollPAchsNew) zSollPAchs = zSollPAchsNew; syncPowerIndicator();
}
void zusiPower::setZugkraft(float power){
    zGesNew = calcPower(power);
    if(zGes != zGesNew) zGes = zGesNew; syncPowerIndicator();
}
void zusiPower::setZugkraftGesammtSteuerwagen(float power){        // 0x007C
    zGesStwgNew = calcPower(power);
    if(zGesStwg != zGesStwgNew) zGesStwg = zGesStwgNew; syncPowerIndicator();
}
void zusiPower::setZugkraftProAchseSteuerwagen(float power){       // 0x007D
    zPAchsStwgNew = calcPower(power);
    if(zPAchsStwg != zPAchsStwgNew) zPAchsStwg = zPAchsStwgNew; syncPowerIndicator();
}
void zusiPower::setZugkraftSollGesammtSteuerwagen(float power){    // 0x007E
    zSollGesStwgNew = calcPower(power);
    if(zSollGesStwg !=zSollGesStwgNew) zSollGesStwg = zSollGesStwgNew; syncPowerIndicator();
}
void zusiPower::setZugkraftSollProAchseSteuerwagen(float power){   // 0x007F
    zSollPAchsStwgNew = calcPower(power);
    if(zSollPAchsStwg != zSollPAchsStwgNew) zSollPAchsStwg = zSollPAchsStwgNew; syncPowerIndicator();
}
void zusiPower::setZugkraftSollNormiert(float power){              // 0x0090
    zSollNormNew = static_cast<qint16>(ceil(static_cast<double>(power * 100)));
    if(zSollNorm != zSollNormNew) zSollNorm = zSollNormNew; syncPowerIndicator();
}
void zusiPower::setZugkraftSollNormiertSteuerwagen(float power){   // 0x0091
    zSollNormStwgNew = static_cast<qint16>(ceil(static_cast<double>(power * 100)));
    if(zSollNormStwg != zSollNormStwgNew) zSollNormStwg = zSollNormStwgNew; syncPowerIndicator();
}
void zusiPower::setZugkraftNormiert(float power){                  // 0x0093
    zNormNew = static_cast<qint16>(ceil(static_cast<double>(power * 100)));
    if(zNorm != zNormNew) zNorm = zNormNew; syncPowerIndicator();
}
void zusiPower::setZugkraftNormiertSteuerwagen(float power){       // 0x0094
    zNormStwgNew = static_cast<qint16>(ceil(static_cast<double>(power * 100)));
    if(zNormStwg != zNormStwgNew) zNormStwg = zNormStwgNew; syncPowerIndicator();
}

void zusiPower::syncPowerIndicator(){
    if(zNorm > 0){
        qint16 zMaxPAchsTest = static_cast<qint16>(ceil(100.0 * (zPAchs / zNorm)));
        if(zMaxPAchsTest != zMaxPAchsTestOld){
            zMaxPAchsTestOld = zMaxPAchsTest;
            zMaxPAchsOkCount = 0;
        }
        else{
            zMaxPAchsOkCount ++;
            if(zMaxPAchsOkCount > 3){
                zMaxPAchs = zMaxPAchsTest;
                zMaxPAchsOkCount = 4;
            }
        }
    }
    if(VIst == 0 && (zPAchs < 0 || zPAchs < 0 ||zNorm < 0 ||zNormStwg < 0)){ // Workarround: Zusi does not set braking force to zero, aftrer stop
        zPAchs = 0;
        zPAchsStwg = 0;
        zNorm = 0;
        zNormStwg = 0;
    }
  //qint16 zGesMax = maxAbs(zGes, zGesStwg);
    qint16 zPAchsMax = maxAbs(zPAchs, zPAchsStwg);
  //qint16 zSollMax = maxAbs(zSollGes, zSollGesStwg);
    qint16 zSollPAchsMax = maxAbs(zSollPAchs, zSollPAchsStwg);
    powerValuesToDecoder[0] = zPAchsMax;
    powerValuesToDecoder[2] = zSollPAchsMax;
    powerValuesToDecoder[1] = zPAchsMax;//makeNormPlausible(zNorm, zNormStwg);
    powerValuesToDecoder[3] = zSollPAchsMax;//makeNormPlausible(zSollNorm, zSollNormStwg);
    /*
    if((unitBraking == "kN" && zGesMax <= 0))
        powerValuesToDecoder[0] = zGesMax;
    if((unitAccelerating == "kN" && zGesMax >= 0))
        powerValuesToDecoder[0] = zGesMax;
    if((unitBraking == "kN/FM" && zPAchsMax <= 0))
        powerValuesToDecoder[0] = zPAchsMax;
    if((unitAccelerating == "kN/FM" && zPAchsMax >= 0))
        powerValuesToDecoder[0] = zPAchsMax;
    */
    //powerValuesToDecoder[1] = makeNormPlausible(zNorm, zNormStwg);
    /*
    if((unitBraking == "kN" && zSollMax <= 0))
        powerValuesToDecoder[2] = zSollMax;
    if((unitAccelerating == "kN" && zSollMax >= 0))
        powerValuesToDecoder[2] = zSollMax;
    if((unitBraking == "kN/FM" && zSollPAchsMax<= 0))
        powerValuesToDecoder[2] = zSollPAchsMax;
    if((unitAccelerating == "kN/FM" && zSollPAchsMax>= 0))
        powerValuesToDecoder[2] = zSollPAchsMax;
    */
    //powerValuesToDecoder[3] = makeNormPlausible(zSollNorm, zSollNormStwg);
    emit newPowerValues(powerValuesToDecoder);

    //qDebug() <<  "x0009;x000A;x000B;x000C;x007C;x007D;x007E;x007F;x0090;x0091;x0093;x0094;V";
    /*qDebug() <<         QString::number(zGes)
                + ";" + QString::number(zPAchs)
                + ";" + QString::number(zSollGes)
                + ";" + QString::number(zPAchsStwg)
                //+ ";" + QString::number(zSollGesStwg)
                //+ ";" + QString::number(zSollPAchsStwg)
                + ";" + QString::number(zGesSollNorm)
                //+ ";" + QString::number(zGesSollNormStwg)
                //+ ";" + QString::number(zSollNormiertStwg)
                //+ ";" + QString::number(zGesStwg)
                + ";" + QString::number(zSollPAchs)
                + ";" + QString::number(zNorm)
                + ";" + QString::number(zMaxPAchs);*/
}
qint16 zusiPower::maxAbs(qint16 A, qint16 B){
    if(A > 0 || B > 0){
        return qMax(A, B);
    }
    else{
        return qMin(A, B);
    }
}

qint16 zusiPower::makeNormPlausible(qint16 A, qint16 B){
    bool aOk = ((-100 <= A) && (A <= 100));
    bool bOk = ((-100 <= B) && (B <= 100));
    if(aOk && bOk) return maxAbs(A, B);
    if(aOk) return A;
    if(bOk) return B;
    if((A < -100) && (B < -100)) return -100;
    if((A >  100) && (B >  100)) return  100;
    return 0;
}

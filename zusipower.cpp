#include "zusipower.h"

zusiPower::zusiPower(QObject *parent) : QObject(parent){
    powerValuesToDecoder.resize(6);
}

void zusiPower::setVIst(qint16 V){
    VIst = V;
    syncPowerIndicator();
}
void zusiPower::setBaureihe(QString fahrzeug){
    if(bdb) qDebug() << "Baureihe: " + fahrzeug;
    bool isLoko = false;
    int br = fahrzeug.toInt(&isLoko);
    if(isLoko){
        haveLokoInList = false;
        for(int i = 0; i < 37; i++){
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
                unitAccelerating= unitType[skalen[i][7]];
                emit unitBrakingText(unitBraking);
                emit unitAcceleratingText(unitAccelerating);
                emit maxPowerPositiveNumber(zMaxNumber);
                emit maxPowerPositiveLine(zMaxLine);
                emit maxPowerNegativeNumber(zMinNumber);
                emit maxPowerNegativeLine(zMinLine);
            }
        }
    }
}

qint16 zusiPower::calcPower(float power){
    if(power > 0)
        return static_cast<qint16>(ceil(static_cast<double>(power) / 100000 * zMaxCorr));
    return static_cast<qint16>(ceil(static_cast<double>(power) / 100000 * zMinCorr));
}

void zusiPower::setZugkraftProAchse(float power){                  // 0x000A
    if(bdb) qDebug() << "zPAchs roh: " + QString::number(power);
    zPAchsNew = calcPower(power);
    if(zPAchs != zPAchsNew) zPAchs = zPAchsNew; syncPowerIndicator();
    if(bdb) qDebug() << "zPAchs: " + QString::number(zPAchs);
}
void zusiPower::setZugkraftSollGesammt(float power){               // 0x000B
     zSollGesNew = calcPower(power);
     if(zSollGes != zSollGesNew) zSollGes = zSollGesNew; syncPowerIndicator();
     if(bdb) qDebug() << "zSollGes: " + QString::number(zSollGes);
}
void zusiPower::setZugkraftSollProAchse(float power){              // 0x000C
   zSollPAchsNew = calcPower(power);
    if(zSollPAchs != zSollPAchsNew) zSollPAchs = zSollPAchsNew; syncPowerIndicator();
    if(bdb) qDebug() << "zSollPAchs: " + QString::number(zSollPAchs);
}
void zusiPower::setZugkraft(float power){
    zGesNew = calcPower(power);
    if(zGes != zGesNew) zGes = zGesNew; syncPowerIndicator();
    if(bdb) qDebug() << "zGes: " + QString::number(zGes);
}
void zusiPower::setZugkraftGesammtSteuerwagen(float power){        // 0x007C
    zGesStwgNew = calcPower(power);
    if(zGesStwg != zGesStwgNew) zGesStwg = zGesStwgNew; syncPowerIndicator();
    if(bdb) qDebug() << "zGesStwg: " + QString::number(zGesStwg);
}
void zusiPower::setZugkraftProAchseSteuerwagen(float power){       // 0x007D
    zPAchsStwgNew = calcPower(power);
    if(zPAchsStwg != zPAchsStwgNew) zPAchsStwg = zPAchsStwgNew; syncPowerIndicator();
    if(bdb) qDebug() << "zPAchsStwg: " + QString::number(zPAchsStwg);
}
void zusiPower::setZugkraftSollGesammtSteuerwagen(float power){    // 0x007E
    zSollGesStwgNew = calcPower(power);
    if(zSollGesStwg !=zSollGesStwgNew) zSollGesStwg = zSollGesStwgNew; syncPowerIndicator();
    if(bdb) qDebug() << "zSollGesStwg: " + QString::number(zSollGesStwg);
}
void zusiPower::setZugkraftSollProAchseSteuerwagen(float power){   // 0x007F
    zSollPAchsStwgNew = calcPower(power);
    if(zSollPAchsStwg != zSollPAchsStwgNew) zSollPAchsStwg = zSollPAchsStwgNew; syncPowerIndicator();
    if(bdb) qDebug() << "zSollPAchsStwg: " + QString::number(zSollPAchsStwg);
}
void zusiPower::setZugkraftSollNormiert(float power){              // 0x0090
    zSollNormNew = static_cast<qint16>(ceil(static_cast<double>(power * 100)));
    if(zSollNorm != zSollNormNew) zSollNorm = zSollNormNew; syncPowerIndicator();
    if(bdb) qDebug() << "zSollNorm: " + QString::number(zSollNorm);
}
void zusiPower::setZugkraftSollNormiertSteuerwagen(float power){   // 0x0091
    zSollNormStwgNew = static_cast<qint16>(ceil(static_cast<double>(power * 100)));
    if(zSollNormStwg != zSollNormStwgNew) zSollNormStwg = zSollNormStwgNew; syncPowerIndicator();
    if(bdb) qDebug() << "zSollNormStwg: " + QString::number(zSollNormStwg);
}
void zusiPower::setZugkraftNormiert(float power){                  // 0x0093
    zNormNew = static_cast<qint16>(ceil(static_cast<double>(power * 100)));
    if(zNorm != zNormNew) zNorm = zNormNew; syncPowerIndicator();
    if(bdb) qDebug() << "zNorm: " + QString::number(zNorm);
}
void zusiPower::setZugkraftNormiertSteuerwagen(float power){       // 0x0094
    zNormStwgNew = static_cast<qint16>(ceil(static_cast<double>(power * 100)));
    if(zNormStwg != zNormStwgNew) zNormStwg = zNormStwgNew; syncPowerIndicator();
    if(bdb) qDebug() << "zNormStwg: " + QString::number(zNormStwg);
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
    qint16 zGesMax = maxAbs(zGes, zGesStwg);
    qint16 zPAchsMax = maxAbs(zPAchs, zPAchsStwg);
    qint16 zSollMax = maxAbs(zSollGes, zSollGesStwg);
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

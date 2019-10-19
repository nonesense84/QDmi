#include "mtd.h"

mtd::mtd(){}

void mtd::initialize(){
    emit newIconG1(indicatorFiles[0][0],indicatorFiles[0][1]);  //indSifaOff
    //emit newIconG2(indicatorFiles[1][0],indicatorFiles[1][1]);  // indDors indDorsBlink
    //emit newIconG3(indicatorFiles[2][0],indicatorFiles[2][1]);  // indPantoUp indPantoDown
    emit newIconG4(indicatorFiles[3][0],indicatorFiles[3][1]);  // indMainSwitch
    emit newIconG5(indicatorFiles[4][0],indicatorFiles[4][1]);  // indTrainLine
    initialized = true;
}

void mtd::setAnalogValues(QVector<qint16> values){
    if(values.length()>=4){
        emit newPowerRelative(values[1]);
        emit newPowerAbsolute(values[0]);
        emit newPowerRelativeSet(values[3]);
        emit newPowerAbsoluteSet(values[2]);
    }
}

void mtd::setStates(QVector<quint8> states){
    indSifaOff    = states[ 0] == 0;
    indSifaOpto   = states[ 1] >  0;
    indSifaAudio  = states[ 2] == 1;
    indSifaBrake  = states[ 2] == 2;
    //indDors       = states[ 3] >  0;
    //indDorsBlink  = states[ 4] >  1;
    //indPantoUp    = states[ 5] >  0;
    InfPanto      = states[ 6];
    indMainSwitch = states[ 7] >  0;
    indTrainLine  = states[ 8];
    InfDoorSystem = states[ 9];
    InfDoorStatL  = states[10];
    InfDoorStatR  = states[11];
    if(!initialized)initialize();

    if(InfDoorSystem > 1){
        if((InfDoorStatL < 4 && InfDoorStatL > 0) || (InfDoorStatR < 4 && InfDoorStatR > 0)){
            emit newIconG2(indicatorFiles[1][0],indicatorFiles[1][1]);  // doors are open
            emit newIconBehavG2(true, 0, false);
        }
        if(InfDoorStatL == 0 && InfDoorStatR == 0){
            emit newIconG2(indicatorFiles[8][0],indicatorFiles[8][1]);  // doors are closed
            emit newIconBehavG2(true, 0, false);
        }
        if(InfDoorStatL > 3 || InfDoorStatR > 3){
            emit newIconG2(indicatorFiles[8][0],indicatorFiles[8][1]);  // doors are closing
            emit newIconBehavG2(true, 2, false);
        }
    }
    else{
        emit newIconG2(indicatorFiles[1][1],indicatorFiles[1][1]);  // no doors
        emit newIconBehavG2(false, 2, false);
    }
    if(indSifaOpto){      // akt    fr  invers
        emit newIconBehavE3(true, 0, false);
        emit newIconE3(indicatorFiles[5][0],indicatorFiles[5][1]);
    }
    if(indSifaAudio){
        emit newIconBehavE3(true, 0, false);
        emit newIconE3(indicatorFiles[6][0],indicatorFiles[6][1]);
    }
    if(indSifaBrake){
        emit newIconBehavE3(true, 0, false);
        emit newIconE3(indicatorFiles[7][0],indicatorFiles[7][1]);
    }
    if(!indSifaOpto && !indSifaAudio && !indSifaBrake){
        emit newIconBehavE3(false, 0, false);
    }
    emit newIconBehavG1(indSifaOff, 0, false);
    if((InfPanto & 0x0f) > 0){  // One or more pantographs are up or rising
        emit newIconG3(indicatorFiles[9][0],indicatorFiles[9][1]);  // indPantoUp indPantoUpArrow
        emit newIconBehavG3(true, 2, (InfPanto & 0xf0) > 0);        // Behavior is blinking, if one panto is rising
    }
    if((InfPanto & 0x0f) == 0){  // No pantographs is up
        emit newIconG3(indicatorFiles[2][0],indicatorFiles[2][1]);  // indPantoUp indBlanc
        emit newIconBehavG3(true, 2, false);
    }
    emit newIconBehavG4(indMainSwitch, 0, false);
    emit newIconBehavG5(indTrainLine, 0, false);

}

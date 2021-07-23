#include "zusitraindata.h"
#include <QDebug>
//#include <QIntValidator>
zusiTraindata::zusiTraindata(){}

void zusiTraindata::TdeCompeteClicked(){
    disableAllButtons();
    setAllDataInvalid();
    emit newBehavCmpltBtn("Ja", true, false, false, true, true);
}

void zusiTraindata::setLzbAvailable(bool available){
    lzbApplicable = available;
    emit newBehavVmzBtn(QString::number(enteredVmz), true, enabledVmz, validVmz, lzbApplicable);
    emit newBehavZlBtn (QString::number (enteredZl), true, enabledZl,  validZl,  lzbApplicable);
    emit newBehavZlLbl("Zuglänge ", false, false, false, lzbApplicable);
    emit newBehavVmzLbl("Höchstgeschwindigkeit ", false, false, false, lzbApplicable);
}

void zusiTraindata::setKeycommand(quint16 command){                         // To use in case of using Wachsamtaste
    if(validBra && validBrh && validZl && validVmz && command == 0x33){
        validated = true;
        emit newTraindata(enteredBra, enteredBrh, enteredZl, enteredVmz, validated);
    }
}

void zusiTraindata::dataValidButtonPressed(QString buttonText){
    if(buttonText == "Ja" || buttonText == "Yes"){
        validated = true;
        emit newTraindata(enteredBra, enteredBrh, enteredZl, enteredVmz, validated);
        finalizeDataEntry();
        emit closeDataEntryWindow();
    }
    else{
        finalizeDataEntry();            // FIXME: Maybe the "No" procedure need to be changed in a way, that the window switch back to entry page
        emit repeateDataEntry();
    }
}

void zusiTraindata::finalizeDataEntry(){
    disableAllButtons();
    setAllDataInvalid();
    transmitAllButtons();
    enteredDataString = "";
    emit requestKeyboardLayout(0);
    emit newBehavCmpltBtn("Ja", true, false, false, true, true);
}
// ======================= Section data comming from keyboard =============================
void zusiTraindata::setTextFromKeyboard(QString text){
    if(!(enabledBra || enabledBrh || enabledZl || enabledVmz)) return;
    enteredDataString = text;
    if(enabledBra){
        emit newBehavBraBtn(enteredDataString, true, enabledBra, validBra, true);
    }
    if(enabledBrh){
        emit newBehavBrhBtn(enteredDataString, true, enabledBrh, validBrh, true);
    }
    if(enabledZl){
        emit newBehavZlBtn (enteredDataString, true, enabledZl,  validZl,  lzbApplicable);
    }
    if(enabledVmz){
        emit newBehavVmzBtn(enteredDataString, true, enabledVmz, validVmz, lzbApplicable);
    }
}
// ======================= Section data comming from input fields =============================
void zusiTraindata::setBra(QString BRA, bool btnEnabled){
    disableAllButtons();
    if(lzbApplicable)
        emit requestKeyboardLayout(4);
    else
        emit requestKeyboardLayout(3);
    if(btnEnabled){
        enteredBra = static_cast<quint16>(BRA.remove("_").toInt());
        if(enteredBra == 0)BRA = BRA.remove("0");
        validBra = true;
        validated = false;
        emit newTraindata(enteredBra, enteredBrh, enteredZl, enteredVmz, validated);
        enabledBrh = true;
        emit newBehavCmpltBtn("Ja", true, validBra && validBrh && ((validZl && validVmz) | !lzbApplicable), false, true, true);
        emit requestDataEntrStrInitials(4, QString::number(enteredBrh));
        emit requestKeyboardLayout(0);
    }
    else{
        emit requestDataEntrStrInitials(1, QString::number(enteredBra));
        enabledBra = true;
    }
    enteredDataString = "";
    transmitAllButtons();
}
void zusiTraindata::setBrh(QString BRH, bool btnEnabled){
    disableAllButtons();
    emit requestKeyboardLayout(0);
    if(btnEnabled){
        enteredBrh = static_cast<quint16>(BRH.remove("_").toInt());
        if(enteredBrh == 0)BRH = BRH.remove("0");
        validBrh = true;
        validated = false;
        newTraindata(enteredBra, enteredBrh, enteredZl, enteredVmz, validated);
        if(lzbApplicable)enabledZl = true;
        emit newBehavCmpltBtn("Ja", true, validBra && validBrh && ((validZl && validVmz) | !lzbApplicable), false, true, true);
        emit requestDataEntrStrInitials(3, QString::number(enteredZl));
    }
    else{
        emit requestDataEntrStrInitials(4, QString::number(enteredBrh));
        enabledBrh = true;
    }
    enteredDataString = "";
    transmitAllButtons();
}
void zusiTraindata::setZl (QString ZL,  bool btnEnabled){
    disableAllButtons();
    emit requestKeyboardLayout(0);
    if(btnEnabled){
        enteredZl =  static_cast<quint16>(ZL.remove("_").toInt());
        validZl = true;
        validated = false;
        newTraindata(enteredBra, enteredBrh, enteredZl, enteredVmz, validated);
        enabledVmz = true;
        emit newBehavCmpltBtn("Ja", true, validBra && validBrh && validZl && validVmz, false, true, true);
        emit requestDataEntrStrInitials(3, QString::number((enteredVmz)));
    }
    else{
        emit requestDataEntrStrInitials(3, QString::number(enteredZl));
        enabledZl =  true;
    }
    enteredDataString = "";
    transmitAllButtons();
}
void zusiTraindata::setVmz(QString VMZ, bool btnEnabled){
    emit requestKeyboardLayout(0);
    disableAllButtons();
    if(btnEnabled){
        enteredVmz = static_cast<quint16>(VMZ.remove("_").toInt());
        validVmz = true;
        validated = false;
        newTraindata(enteredBra, enteredBrh, enteredZl, enteredVmz, validated);
        emit newBehavCmpltBtn("Ja", true, validBra && validBrh && validZl && validVmz, false, true, true);
    }
    else{
        emit requestDataEntrStrInitials(3, QString::number(enteredVmz));
        enabledVmz = true;
    }
    enteredDataString = "";
    transmitAllButtons();
}
// ======================= Section "Ersatzdaten" comming from Zusi =============================
void zusiTraindata::setActiveBra(quint16 BRA){
    // As long as we are performing the data entry, the mirrored data should not overwright our data:
   if(validBra || validBrh || validZl || validVmz) return;
    if(BRA != enteredBra){
        setAllDataInvalid();
        disableAllButtons();
        enteredBra = BRA;
        transmitAllButtons();
    }
}
void zusiTraindata::setActiveBrh(quint16 BRH){
     // As long as we are performing the data entry, the mirrored data should not overwright our data:
    if(validBra || validBrh || validZl || validVmz) return;
    if(BRH != enteredBrh){
        setAllDataInvalid();
        disableAllButtons();
        enteredBrh = BRH;
        transmitAllButtons();
    }
}
void zusiTraindata::setActiveZl(quint16 ZL){
    // As long as we are performing the data entry, the mirrored data should not overwright our data:
   if(validBra || validBrh || validZl || validVmz) return;
    if(ZL != enteredZl){
        setAllDataInvalid();
        disableAllButtons();
        enteredZl = ZL;
        transmitAllButtons();
    }
}
void zusiTraindata::setActiveVmz(quint16 VMZ){
    // As long as we are performing the data entry, the mirrored data should not overwright our data:
   if(validBra || validBrh || validZl || validVmz) return;
    if(VMZ != enteredVmz){
        setAllDataInvalid();
        disableAllButtons();
        enteredVmz = VMZ;
        transmitAllButtons();
    }
}
// ======================= private functions =============================
void zusiTraindata::setAllDataInvalid(){
    validBra = false;
    validBrh = false;
    validZl  = false;
    validVmz = false;
    validated = false;
}

void zusiTraindata::disableAllButtons(){
    enabledBra = false;
    enabledBrh = false;
    enabledZl  = false;
    enabledVmz = false;
}

void zusiTraindata::transmitAllButtons(){
    emit newBehavBraBtn(QString::number(enteredBra), true, enabledBra, validBra, true);
    emit newBehavBrhBtn(QString::number(enteredBrh), true, enabledBrh, validBrh, true);
    emit newBehavZlBtn (QString::number (enteredZl), true, enabledZl,  validZl,  lzbApplicable);
    emit newBehavVmzBtn(QString::number(enteredVmz), true, enabledVmz, validVmz, lzbApplicable);
}

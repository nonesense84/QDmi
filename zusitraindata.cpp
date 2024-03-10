#include "zusitraindata.h"
#include <QDebug>
//#include <QIntValidator>
zusiTraindata::zusiTraindata(){
    setEtcsAvailable(true);
}

void zusiTraindata::TdeCompeteClicked(){
    disableAllButtons();
    setAllDataInvalid();
    emit newBehavCmpltBtn("Ja", true, false, false, true, true);
}

void zusiTraindata::setLzbAvailable(bool available){
    lzbApplicable = available;
    emit newBehavVmzBtn(enteredVmz, true, enabledVmz, validVmz, lzbApplicable);
    emit newBehavZlBtn (enteredZl, true, enabledZl,  validZl,  lzbApplicable);
    emit newBehavZlLbl("Zuglänge ", false, false, false, lzbApplicable);
    emit newBehavVmzLbl("Höchstgeschwindigkeit ", false, false, false, lzbApplicable);
}

void zusiTraindata::setEtcsAvailable(bool available){
    etcsApplicable = available;
}

void zusiTraindata::setKeycommand(quint16 command){                         // To use in case of using Wachsamtaste
    if(validBra && validBrh && validZl && validVmz && command == 0x33){
        validated = true;
        emit newTraindataLzb(enteredBra.toInt(), enteredBrh.toInt(), enteredZl.toInt(), enteredVmz.toInt(), validated);
    }
}

void zusiTraindata::dataValidButtonPressed(QString buttonText){
    if(buttonText == "Ja" || buttonText == "Yes"){
        validated = true;
        emit newTraindataLzb(enteredBra.toInt(), enteredBrh.toInt(), enteredZl.toInt(), enteredVmz.toInt(), validated);
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
    emit requestKeyboardLayout(P_Keyboard_Numeric);
    emit newBehavCmpltBtn("Ja", true, false, false, true, true);
}
// ======================= Section data comming from keyboard =============================
void zusiTraindata::setTextFromKeyboard(QString text){
    if(!(enabledBra || enabledBrh || enabledZl || enabledVmz || enabledTct || enabledAxl || enabledAit || enabledLdg)) return;
    enteredDataString.remove("_");
    if(text == "Del"){
        enteredDataString.chop(1);
        text = "";
    }
  /*if(enabledBrh || enabledZl || enabledVmz){
      //if( hasUnderline)enteredDataString = enteredDataString + "_";
      //if(!hasUnderline)enteredDataString.remove("_");
        enteredDataString = enteredDataString + text;
    }*/
  //if((enabledTct || enabledAxl || enabledAit || enabledLdg) && text != "")enteredDataString = text;
    if(enabledBra){
        enteredDataString = text;
        emit newBehavBraBtn(enteredDataString, true, enabledBra, validBra, true);
    }
    if(enabledBrh && enteredDataString.length() <=2){
        enteredDataString = enteredDataString + text;
        emit newBehavBrhBtn(enteredDataString, true, enabledBrh, validBrh, true);
    }
    if(enabledZl && enteredDataString.length() <=2){
        enteredDataString = enteredDataString + text;
        emit newBehavZlBtn (enteredDataString, true, enabledZl,  validZl,  lzbApplicable);
    }
    if(enabledVmz && enteredDataString.length() <=2){
        enteredDataString = enteredDataString + text;
        emit newBehavVmzBtn(enteredDataString, true, enabledVmz, validVmz, lzbApplicable);
    }
    if(enabledTct){
        enteredDataString = text;
        emit newBehavTctBtn(enteredDataString, true, enabledTct, validTct, etcsApplicable);
    }
    if(enabledAxl){
        enteredDataString = text;
        emit newBehavAxlBtn(enteredDataString, true, enabledAxl, validAxl, etcsApplicable);
    }
    if(enabledAit){
        enteredDataString = text;
        emit newBehavAitBtn(enteredDataString, true, enabledAit, validAit, etcsApplicable);
    }
    if(enabledLdg){
        enteredDataString = text;
        emit newBehavLdgBtn(enteredDataString, true, enabledLdg, validLdg, etcsApplicable);
    }
}
void zusiTraindata::setCursorBlink(){
    if(enteredDataString.contains("_")){
        enteredDataString.remove("_");
    }
    else{
        enteredDataString = enteredDataString + "_";
    }
    if(enabledBra) emit newBehavBraBtn(enteredDataString, true, enabledBra, validBra, true);
    if(enabledBrh) emit newBehavBrhBtn(enteredDataString, true, enabledBrh, validBrh, true);
    if(enabledZl ) emit newBehavZlBtn (enteredDataString, true, enabledZl,  validZl,  lzbApplicable);
    if(enabledVmz) emit newBehavVmzBtn(enteredDataString, true, enabledVmz, validVmz, lzbApplicable);
    if(enabledTct) emit newBehavTctBtn(enteredDataString, true, enabledTct, validTct, etcsApplicable);
    if(enabledAxl) emit newBehavAxlBtn(enteredDataString, true, enabledAxl, validAxl, etcsApplicable);
    if(enabledAit) emit newBehavAitBtn(enteredDataString, true, enabledAit, validAit, etcsApplicable);
    if(enabledLdg) emit newBehavLdgBtn(enteredDataString, true, enabledLdg, validLdg, etcsApplicable);
    if(!etcsApplicable){
        enteredTct = "";
        enteredAxl = "";
        enteredAit = "";
        enteredLdg = "";
    }
}
// ======================= Section data comming from input fields =============================
void zusiTraindata::setBra(QString BRA, bool btnEnabled){
    BRA.remove("_");
    if(BRA == "") BRA = "0";
    disableAllButtons();
    if(lzbApplicable)
        emit requestKeyboardLayout(P_Keyboard_brakingRegimeLzb);
    else
        emit requestKeyboardLayout(P_Keyboard_brakingRegimePzb);
    if(btnEnabled){
        enteredBra = BRA;
        validBra = true;
        validated = false;
        emit newTraindataLzb(enteredBra.toInt(), enteredBrh.toInt(), enteredZl.toInt(), enteredVmz.toInt(), validated);
        emit newTraindataEtcs(enteredBrh.toInt(), enteredTct, enteredZl.toInt(), enteredVmz.toInt(), enteredAxl, enteredAit, enteredLdg, validated);
        enabledBrh = true;
        emit requestDataEntrStrInitials(4, enteredBrh);
        emit requestKeyboardLayout(P_Keyboard_Numeric);
        if(enteredBrh == "0")
            enteredDataString = "";
        else
            enteredDataString = enteredBrh;
    }
    else{
        emit requestDataEntrStrInitials(1, enteredBra);
        enabledBra = true;
        if(enteredBra == "0")
            enteredDataString = "";
        else
            enteredDataString = enteredBra;
    }
    transmitAllButtons();
}
void zusiTraindata::setBrh(QString BRH, bool btnEnabled){
    BRH.remove("_");
    if(BRH == "") BRH = "0";
    disableAllButtons();
    emit requestKeyboardLayout(P_Keyboard_Numeric);
    if(btnEnabled){
        enteredBrh = BRH;
        if(enteredBrh == "0")BRH = BRH.remove("0");
        validBrh = true;
        validated = false;
        emit newTraindataLzb(enteredBra.toInt(), enteredBrh.toInt(), enteredZl.toInt(), enteredVmz.toInt(), validated);
        emit newTraindataEtcs(enteredBrh.toInt(), enteredTct, enteredZl.toInt(), enteredVmz.toInt(), enteredAxl, enteredAit, enteredLdg, validated);
        if(lzbApplicable)enabledZl = true;
        emit requestDataEntrStrInitials(3, enteredZl);
        if(enteredZl == "0")
            enteredDataString = "";
        else
            enteredDataString = enteredZl;
    }
    else{
        emit requestDataEntrStrInitials(4,enteredBrh);
        enabledBrh = true;
        if(enteredBrh == "0")
            enteredDataString = "";
        else
            enteredDataString = enteredBrh;
    }
    transmitAllButtons();
}
void zusiTraindata::setZl (QString ZL,  bool btnEnabled){
    ZL.remove("_");
    if(ZL == "") ZL = "0";
    disableAllButtons();
    emit requestKeyboardLayout(P_Keyboard_Numeric);
    if(btnEnabled){
        enteredZl =  ZL;
        validZl = true;
        validated = false;
        emit newTraindataLzb(enteredBra.toInt(), enteredBrh.toInt(), enteredZl.toInt(), enteredVmz.toInt(), validated);
        emit newTraindataEtcs(enteredBrh.toInt(), enteredTct, enteredZl.toInt(), enteredVmz.toInt(), enteredAxl, enteredAit, enteredLdg, validated);
        enabledVmz = true;
        emit requestDataEntrStrInitials(3, enteredVmz);
        if(enteredVmz == "0")
            enteredDataString = "";
        else
            enteredDataString = enteredVmz;
    }
    else{
        emit requestDataEntrStrInitials(3, enteredZl);
        enabledZl =  true;
        if(enteredZl == "0")
            enteredDataString = "";
        else
            enteredDataString = enteredZl;
    }
    transmitAllButtons();
}
void zusiTraindata::setVmz(QString VMZ, bool btnEnabled){
    VMZ.remove("_");
    if(VMZ == "") VMZ = "0";
    emit requestKeyboardLayout(P_Keyboard_Numeric);
    disableAllButtons();
    if(btnEnabled){
        enteredVmz = VMZ;
        validVmz = true;
        validated = false;
        emit newTraindataLzb(enteredBra.toInt(), enteredBrh.toInt(), enteredZl.toInt(), enteredVmz.toInt(), validated);
        emit newTraindataEtcs(enteredBrh.toInt(), enteredTct, enteredZl.toInt(), enteredVmz.toInt(), enteredAxl, enteredAit, enteredLdg, validated);
        if(etcsApplicable){
            enabledTct = true;
            enteredDataString = enteredTct;
            emit requestKeyboardLayout(P_Keyboard_TrainCategory);
            emit requestSecondTdePage(true);
        }
    }
    else{
        emit requestDataEntrStrInitials(3, enteredVmz);
        enabledVmz = true;
        if(enteredVmz == "0")
            enteredDataString = "";
        else
            enteredDataString = enteredVmz;
    }
    transmitAllButtons();
}
void zusiTraindata::setTct(QString TCT, bool btnEnabled){
    TCT.remove("_");
    emit requestKeyboardLayout(P_Keyboard_TrainCategory);
    disableAllButtons();
    if(btnEnabled){
        enteredTct = TCT;
        validTct = true;
        validated = false;
        emit newTraindataEtcs(enteredBrh.toInt(), enteredTct, enteredZl.toInt(), enteredVmz.toInt(), enteredAxl, enteredAit, enteredLdg, validated);
        enabledAxl = true;
        emit requestKeyboardLayout(P_Keyboard_axleLoad);
        emit requestDataEntrStrInitials(3, enteredAxl);
        enteredDataString = enteredAxl;
    }
    else{
        emit requestDataEntrStrInitials(6,enteredTct);
        enabledTct = true;
        enteredDataString = enteredTct;
    }
    transmitAllButtons();
}
void zusiTraindata::setAxl(QString AXL, bool btnEnabled){
    AXL.remove("_");
    emit requestKeyboardLayout(P_Keyboard_axleLoad);
    disableAllButtons();
    if(btnEnabled){
        enteredAxl = AXL;
        validAxl = true;
        validated = false;
        emit newTraindataEtcs(enteredBrh.toInt(), enteredTct, enteredZl.toInt(), enteredVmz.toInt(), enteredAxl, enteredAit, enteredLdg, validated);
        enabledAit = true;
        emit requestKeyboardLayout(P_Keyboard_yesNo);
        emit requestDataEntrStrInitials(3, enteredAit);
        enteredDataString = enteredAit;
    }
    else{
        emit requestDataEntrStrInitials(6, enteredAxl);
        enabledAxl = true;
        enteredDataString = enteredAxl;
    }
    transmitAllButtons();
}
void zusiTraindata::setAit(QString AIT, bool btnEnabled){
    AIT.remove("_");
    emit requestKeyboardLayout(P_Keyboard_yesNo);
    disableAllButtons();
    if(btnEnabled){
        enteredAit = AIT;
        validAit = true;
        validated = false;
        emit newTraindataEtcs(enteredBrh.toInt(), enteredTct, enteredZl.toInt(), enteredVmz.toInt(), enteredAxl, enteredAit, enteredLdg, validated);
        enabledLdg = true;
        emit requestKeyboardLayout(P_Keyboard_loadingGauge);
        emit requestDataEntrStrInitials(3, enteredLdg);
        enteredDataString = enteredLdg;
    }
    else{
        emit requestDataEntrStrInitials(3, enteredAit);
        enabledAit = true;
        enteredDataString = enteredAit;
    }
    transmitAllButtons();
}
void zusiTraindata::setLdg(QString LDG, bool btnEnabled){
    LDG.remove("_");
    emit requestKeyboardLayout(P_Keyboard_loadingGauge);
    disableAllButtons();
    if(btnEnabled){
        enteredLdg = LDG;
        validLdg = true;
        validated = false;
        emit newTraindataEtcs(enteredBrh.toInt(), enteredTct, enteredZl.toInt(), enteredVmz.toInt(), enteredAxl, enteredAit, enteredLdg, validated);
    }
    else{
        emit requestDataEntrStrInitials(3, enteredLdg);
        enabledLdg = true;
        enteredDataString = enteredLdg;
    }
    transmitAllButtons();
}

// ======================= Section "Aktive Zugdaten" comming from Zusi =============================
void zusiTraindata::setActiveBra(quint16 BRA){
    // As long as we are performing the data entry, the mirrored data should not overwright our data:
   if(validBra || validBrh || validZl || validVmz || validTct || validAxl || validAit || validLdg) return;
    if(QString::number(BRA) != enteredBra){
        setAllDataInvalid();
        disableAllButtons();
        enteredBra = QString::number(BRA);
        transmitAllButtons();
    }
}
void zusiTraindata::setActiveBrh(quint16 BRH){
     // As long as we are performing the data entry, the mirrored data should not overwright our data:
    if(validBra || validBrh || validZl || validVmz || validTct || validAxl || validAit || validLdg) return;
    if(QString::number(BRH) != enteredBrh){
        setAllDataInvalid();
        disableAllButtons();
        enteredBrh = QString::number(BRH);
        transmitAllButtons();
    }
}
void zusiTraindata::setActiveZl(quint16 ZL){
    // As long as we are performing the data entry, the mirrored data should not overwright our data:
   if(validBra || validBrh || validZl || validVmz || validTct || validAxl || validAit || validLdg) return;
    if(QString::number(ZL) != enteredZl){
        setAllDataInvalid();
        disableAllButtons();
        enteredZl = QString::number(ZL);
        transmitAllButtons();
    }
}
void zusiTraindata::setActiveVmz(quint16 VMZ){
    // As long as we are performing the data entry, the mirrored data should not overwright our data:
   if(validBra || validBrh || validZl || validVmz || validTct || validAxl || validAit || validLdg) return;
    if(QString::number(VMZ) != enteredVmz){
        setAllDataInvalid();
        disableAllButtons();
        enteredVmz = QString::number(VMZ);
        transmitAllButtons();
    }
}

void zusiTraindata::setActiveTct(QString TCT){
    // As long as we are performing the data entry, the mirrored data should not overwright our data:
   if(validBra || validBrh || validZl || validVmz || validTct || validAxl || validAit || validLdg) return;
    if(TCT != enteredTct){
        setAllDataInvalid();
        disableAllButtons();
        enteredTct = TCT;
        transmitAllButtons();
    }
}

void zusiTraindata::setActiveAxl(QString AXL){
    // As long as we are performing the data entry, the mirrored data should not overwright our data:
   if(validBra || validBrh || validZl || validVmz || validTct || validAxl || validAit || validLdg) return;
    if(AXL != enteredAxl){
        setAllDataInvalid();
        disableAllButtons();
        enteredAxl = AXL;
        transmitAllButtons();
    }
}

void zusiTraindata::setActiveAit(QString AIT){
    // As long as we are performing the data entry, the mirrored data should not overwright our data:
   if(validBra || validBrh || validZl || validVmz || validTct || validAxl || validAit || validLdg) return;
    if(AIT != enteredAit){
        setAllDataInvalid();
        disableAllButtons();
        enteredAit = AIT;
        transmitAllButtons();
    }
}

void zusiTraindata::setActiveLdg(QString LDG){
    // As long as we are performing the data entry, the mirrored data should not overwright our data:
   if(validBra || validBrh || validZl || validVmz || validTct || validAxl || validAit || validLdg) return;
    if(LDG != enteredLdg){
        setAllDataInvalid();
        disableAllButtons();
        enteredLdg = LDG;
        transmitAllButtons();
    }
}

// ======================= private functions =============================
void zusiTraindata::setAllDataInvalid(){
    validBra = false;
    validBrh = false;
    validZl  = false;
    validVmz = false;
    validTct = false;
    validAxl = false;
    validAit = false;
    validLdg = false;
    validated = false;
}

void zusiTraindata::disableAllButtons(){
    enabledBra = false;
    enabledBrh = false;
    enabledZl  = false;
    enabledVmz = false;
    enabledTct = false;
    enabledAxl = false;
    enabledAit = false;
    enabledLdg = false;
}

void zusiTraindata::transmitAllButtons(){
    emit newBehavCmpltBtn("Ja",
                          true,
                          validBra && validBrh && ((validZl && validVmz) || !lzbApplicable) && ((validTct && validAxl && validAit && validLdg) || !etcsApplicable),
                          false,
                          true,
                          true);
    emit newBehavBraBtn(enteredBra, true, enabledBra, validBra, true);
    emit newBehavBrhBtn(enteredBrh, true, enabledBrh, validBrh, true);
    emit newBehavZlBtn (enteredZl,  true, enabledZl,  validZl,  lzbApplicable);
    emit newBehavVmzBtn(enteredVmz, true, enabledVmz, validVmz, lzbApplicable);
    emit newBehavTctBtn(enteredTct, true, enabledTct, validTct, true);
    emit newBehavAxlBtn(enteredAxl, true, enabledAxl, validAxl, true);
    emit newBehavAitBtn(enteredAit, true, enabledAit, validAit, true);
    emit newBehavLdgBtn(enteredLdg, true, enabledLdg, validLdg, true);
}

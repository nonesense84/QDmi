#ifndef ZUSITRAINDATA_H
#define ZUSITRAINDATA_H

#include <QObject>

#define P_Keyboard_Numeric          0
#define P_Keyboard_Alphanumeric     1
#define P_Keyboard_ValidYesNo       2
#define P_Keyboard_brakingRegimePzb 3
#define P_Keyboard_brakingRegimeLzb 4
#define P_Keyboard_TrainCategory    5
#define P_Keyboard_yesNo            6
#define P_Keyboard_axleLoad         7
#define P_Keyboard_loadingGauge     8

class zusiTraindata : public QObject
{
    Q_OBJECT
public:
    zusiTraindata();
private:
   // quint16 ersatzBra;
   // quint16 ersatzBrh;
   // quint16 ersatzZl;
   // quint16 ersatzVmz;
    QString enteredDataString;
    QString enteredBra = "0";
    QString enteredBrh = "0";
    QString enteredZl  = "0";
    QString enteredVmz = "0";
    QString enteredTct = "";
    QString enteredAxl = "";
    QString enteredAit = "";
    QString enteredLdg = "";
    QString driverId;
    QString trainRunningNumber;
    quint16 TCT_out; // For transmitting to Zusi
    quint16 AXL_out; // For transmitting to Zusi
    bool validBra = false;  // Bremsart valid
    bool validBrh = false;  // Bremshunderstel valid
    bool validZl  = false;  // Train length valid
    bool validVmz = false;  // Maximum train speed valid
    bool validTct = false;  // Train category valid
    bool validAxl = false;  // Axle load category valid
    bool validAit = false;  // Airtight valid
    bool validLdg = false;  // Loading gauge valid
    bool enabledBra = false;// Editing of bremsart activ
    bool enabledBrh = false;// Editing of bremshunderstel activ
    bool enabledZl  = false;// Editing of train lngth activ
    bool enabledVmz = false;// Editing of train speed activ
    bool enabledTct = false;// Editing of train category activ
    bool enabledAxl = false;// Editing of axle load category activ
    bool enabledAit = false;// Editing of airtight activ
    bool enabledLdg = false;// Editing of loading gauge activ

    bool validated = false;
    bool lzbApplicable = true;
    bool etcsApplicable = true;

    void setAllDataInvalid();
    void disableAllButtons();
    void transmitAllButtons();

public slots:
    void setBra(QString BRA, bool btnEnabled);
    void setBrh(QString BRH, bool btnEnabled);
    void setZl(QString ZL, bool btnEnabled);
    void setVmz(QString VMZ, bool btnEnabled);
    void setTct(QString, bool);
    void setAxl(QString, bool);
    void setAit(QString, bool);
    void setLdg(QString, bool);
    void setActiveBra(quint16 BRA);
    void setActiveBrh(quint16 BRH);
    void setActiveZl (quint16 ZL);
    void setActiveVmz(quint16 VMZ);
    void setActiveTct(QString TCT);
    void setActiveAxl(QString AXL);
    void setActiveAit(QString AIT);
    void setActiveLdg(QString LDG);
    void setTrainRunningNumber(QString trn);
    void setDriverId(QString ID);
    void setTextFromKeyboard(QString text);
    void setCursorBlink();
    void finalizeDataEntry();
    void setKeycommand(quint16 command);
    void dataValidButtonPressed(QString buttonText);
    void setLzbAvailable(bool available);
    void setEtcsAvailable(bool available);
    void TdeCompeteClicked();

public: signals:
    void closeDataEntryWindow();
    void repeateDataEntry();
    void newTraindataLzb(quint16 BRH, quint16 BRA, quint16 ZL, quint16 VMZ, bool validated);
    void newTraindataEtcs(quint16 BRH, quint16 TCT, quint16 ZL, quint16 VMZ, quint16 AXL, QString AIT, QString LDG);
    void newBehavBrhBtn(QString text, bool editable, bool enabled, bool dataValid, bool applicable);
    void newBehavBraBtn(QString text, bool editable, bool enabled, bool dataValid, bool applicable);
    void newBehavZlBtn (QString text, bool editable, bool enabled, bool dataValid, bool applicable);
    void newBehavVmzBtn(QString text, bool editable, bool enabled, bool dataValid, bool applicable);
    void newBehavTctBtn(QString text, bool editable, bool enabled, bool dataValid, bool applicable);
    void newBehavAxlBtn(QString text, bool editable, bool enabled, bool dataValid, bool applicable);
    void newBehavAitBtn(QString text, bool editable, bool enabled, bool dataValid, bool applicable);
    void newBehavLdgBtn(QString text, bool editable, bool enabled, bool dataValid, bool applicable);
    void newBehavZlLbl (QString text, bool editable, bool enabled, bool dataValid, bool applicable);
    void newBehavVmzLbl(QString text, bool editable, bool enabled, bool dataValid, bool applicable);
    void newBehavTctLbl(QString text, bool editable, bool enabled, bool dataValid, bool applicable); // FIMXE: Not used yet
    void newBehavAxlLbl(QString text, bool editable, bool enabled, bool dataValid, bool applicable); // FIMXE: Not used yet
    void newBehavAitLbl(QString text, bool editable, bool enabled, bool dataValid, bool applicable); // FIMXE: Not used yet
    void newBehavLdgLbl(QString text, bool editable, bool enabled, bool dataValid, bool applicable); // FIMXE: Not used yet
    void requestKeyboardLayout(quint8 keyboardtype);
    void requestSecondTdePage(bool secondPage);
    void requestDataEntrStrInitials(quint8 maxlength, QString startvalue);

    void newBehavCmpltBtn(QString text, bool editable, bool enabled, bool dataValid, bool applicable, bool isFullGrid);
};

#endif // ZUSITRAINDATA_H

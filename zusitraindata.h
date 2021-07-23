#ifndef ZUSITRAINDATA_H
#define ZUSITRAINDATA_H

#include <QObject>

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
    quint16 enteredBra = 0;
    quint16 enteredBrh = 0;
    quint16 enteredZl = 0;
    quint16 enteredVmz = 0;
    bool validBra = false;
    bool validBrh = false;
    bool validZl = false;
    bool validVmz = false;
    bool enabledBra = false;
    bool enabledBrh = false;
    bool enabledZl = false;
    bool enabledVmz = false;
    bool validated = false;
    bool lzbApplicable = true;

    void setAllDataInvalid();
    void disableAllButtons();
    void transmitAllButtons();

public slots:
    void setBra(QString BRA, bool btnEnabled);
    void setBrh(QString BRH, bool btnEnabled);
    void setZl(QString ZL, bool btnEnabled);
    void setVmz(QString VMZ, bool btnEnabled);
    void setActiveBra(quint16 BRA);
    void setActiveBrh(quint16 BRH);
    void setActiveZl (quint16 ZL);
    void setActiveVmz(quint16 VMZ);
    void setTextFromKeyboard(QString text);
    void finalizeDataEntry();
    void setKeycommand(quint16 command);
    void dataValidButtonPressed(QString buttonText);
    void setLzbAvailable(bool available);
    void TdeCompeteClicked();

public: signals:
    void closeDataEntryWindow();
    void repeateDataEntry();
    void newTraindata(quint16 BRH, quint16 BRA, quint16 ZL, quint16 VMZ, bool validated);
    void newBehavBrhBtn(QString text, bool editable, bool enabled, bool dataValid, bool applicable);
    void newBehavBraBtn(QString text, bool editable, bool enabled, bool dataValid, bool applicable);
    void newBehavZlBtn (QString text, bool editable, bool enabled, bool dataValid, bool applicable);
    void newBehavVmzBtn(QString text, bool editable, bool enabled, bool dataValid, bool applicable);
    void newBehavZlLbl (QString text, bool editable, bool enabled, bool dataValid, bool applicable);
    void newBehavVmzLbl(QString text, bool editable, bool enabled, bool dataValid, bool applicable);
    void requestKeyboardLayout(quint8 keyboardtype);
    void requestDataEntrStrInitials(quint8 maxlength, QString startvalue);
    //void newTextForTrainLengthValidLabel(QString);
    //void newTextForVMaxValidLabel(QString);
    //void newTextForTrainLengthValidValue(QString);
    //void newTextForMaxSpeedValidValue(QString);

    void newBehavCmpltBtn(QString text, bool editable, bool enabled, bool dataValid, bool applicable, bool isFullGrid);
};

#endif // ZUSITRAINDATA_H

#ifndef MTD_H
#define MTD_H

#include <QObject>
#include <QDebug>

class mtd : public QObject
{
    Q_OBJECT
public:
    mtd();

public slots:
    void setStates(QVector<quint8> states);
    void initialize();
    void setAnalogValues(QVector<qint16> values);

private:
    bool initialized = false;
    bool indSifaOpto;
    bool indSifaAudio;
    bool indSifabrake;
    bool indSifaOff;
    bool indDors;
    bool indDorsBlink;
    uint8_t indTrainLine;
    bool indSifaBrake;
    //bool indPantoUp;
    uint8_t InfPanto;
    bool indMainSwitch;
    uint8_t InfDoorSystem;
    uint8_t InfDoorStatL;
    uint8_t InfDoorStatR;
    QString indicatorFiles[10][2] = {
        {   ":/icons/sifa_off.svg",         ":/icons/blanc.svg",},
        {   ":/icons/doors_open.svg",       ":/icons/blanc.svg",},
        {   ":/icons/panto_down.svg",       ":/icons/blanc.svg",},
        {   ":/icons/ms_on.svg",            ":/icons/ms_off.svg",},
        {   ":/icons/zs_off.svg",           ":/icons/zs_on.svg",},
        {   ":/icons/sifaWarn1.svg",        ":/icons/blanc.svg",},
        {   ":/icons/sifaWarn2.svg",        ":/icons/blanc.svg",},
        {   ":/icons/sifaBrake.svg",        ":/icons/blanc.svg",},
        {   ":/icons/doors_closed.svg",     ":/icons/doors_closed_inv.svg",},
        {   ":/icons/panto_up.svg",         ":/icons/panto_up_arr.svg",}
    };         
private slots:

public slots:

signals:
    void newIconE3(QString lmOn, QString lmOff);
    void newIconG1(QString lmOn, QString lmOff);
    void newIconG2(QString lmOn, QString lmOff);
    void newIconG3(QString lmOn, QString lmOff);
    void newIconG4(QString lmOn, QString lmOff);
    void newIconG5(QString lmOn, QString lmOff);
    void newIconG6(QString lmOn, QString lmOff);
    void newIconG7(QString lmOn, QString lmOff);
    void newIconG8(QString lmOn, QString lmOff);
    void newIconG9(QString lmOn, QString lmOff);
    void newIconG10(QString lmOn, QString lmOff);
    void newIconBehavE3(bool anabled, quint8 freq, bool inverse);
    void newIconBehavG1(bool anabled, quint8 freq, bool inverse);
    void newIconBehavG2(bool anabled, quint8 freq, bool inverse);
    void newIconBehavG3(bool anabled, quint8 freq, bool inverse);
    void newIconBehavG4(bool anabled, quint8 freq, bool inverse);
    void newIconBehavG5(bool anabled, quint8 freq, bool inverse);
    void newIconBehavG6(bool anabled, quint8 freq, bool inverse);
    void newIconBehavG7(bool anabled, quint8 freq, bool inverse);
    void newIconBehavG8(bool anabled, quint8 freq, bool inverse);
    void newIconBehavG9(bool anabled, quint8 freq, bool inverse);
    void newIconBehavG10(bool anabled, quint8 freq, bool inverse);
    void newPowerRelative(qint16 P);
    void newPowerAbsolute(qint16 P);
    void newPowerRelativeSet(qint16 P);
    void newPowerAbsoluteSet(qint16 P);

};

#endif // MTD_H

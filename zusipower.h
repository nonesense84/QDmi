#ifndef ZUSIPOWER_H
#define ZUSIPOWER_H

#include <QObject>
#include <QtMath>
#include <QVector>
#include <QDebug>
#include <QSettings>

class zusiPower : public QObject
{
    Q_OBJECT
public:
    explicit zusiPower(QObject *parent = nullptr);

signals:
    void newPowerValues(QVector<qint16> powerValuesToDecoder);
    void maxPowerPositiveNumber(qint16 zMaxNumber);
    void maxPowerPositiveLine(qint16 zMaxLine);
    void maxPowerNegativeNumber(qint16 zMinNumber);
    void maxPowerNegativeLine(qint16 zMinLine);
    void unitBrakingText(QString unitBraking);
    void unitAcceleratingText(QString uinitAccelerating);
    void hasDriveModeDisplay(bool driveModeDisplay);
    void newDriveMode(quint8 driveMode);

private:
    QSettings *settings;
    QString unitBraking = "";
    QString unitAccelerating= "";
    QString unitType[5] = {"","kN", "kN/FM", "%", "Stufen"};
    QVector<qint16> powerValuesToDecoder;
    qint32 vehicleType = 0;
    QString vehicleName = "";
    qint32 driveModeDivisor = 1;                   // For workarroud: Zusi does not send propper drive mode for all classes
    qint32 driveModeOffset = 0;                    // For workarroud: Zusi does not send propper drive mode for all classes
    qint32 zGes=0,           zGesOld=0,           zGesMin=0,           zGesMax = 0;           // 0x0009 (9)                Zugkraft gesamt
    qint32 zPAchs=0,         zPAchsOld=0,         zPAchsMin=0,         zPAchsMax = 0;         // 0x000A (10)               Zugkraft pro Achse
    qint32 zSollGes=0,       zSollGesOld=0,       zSollGesMin=0,       zSollGesMax = 0;       // 0x000B (11)               Zugkraft-Soll gesamt
    qint32 zSollPAchs=0,     zSollPAchsOld=0,     zSollPAchsMin=0,     zSollPAchsMax = 0;     // 0x000C (12)               Zugkraft-Soll pro Achse
    qint32 zGesStwg=0,       zGesStwgOld=0,       zGesStwgMin=0,       zGesStwgMax = 0;       // 0x007C (124) Steuerwagen: Zugkraft gesamt
    qint32 zPAchsStwg=0,     zPAchsStwgOld=0,     zPAchsStwgMin=0,     zPAchsStwgMax = 0;     // 0x007D (125) Steuerwagen: Zugkraft pro Achse
    qint32 zSollGesStwg=0,   zSollGesStwgOld=0,   zSollGesStwgMin=0,   zSollGesStwgMax = 0;   // 0x007E (126) Steuerwagen: Zugkraft-Soll gesamt
    qint32 zSollPAchsStwg=0, zSollPAchsStwgOld=0, zSollPAchsStwgMin=0, zSollPAchsStwgMax = 0; // 0x007F (127) Steuerwagen: Zugkraft-Soll pro Achse
    qint32 zSollNorm=0,      zSollNormOld=0,      zSollNormMin=0,      zSollNormMax = 0;      // 0x0090 (144)              Zug- und Brems-Gesamtkraftsoll normiert
    qint32 zSollNormStwg=0,  zSollNormStwgOld=0,  zSollNormStwgMin=0,  zSollNormStwgMax = 0;  // 0x0091 (145) Steuerwagen: Zug- und Brems-Gesamtkraftsoll normiert
    qint32 zNorm=0,          zNormOld=0,          zNormMin=0,          zNormMax = 0;          // 0x0093 (147)              Zug- und Brems-Gesamtkraftsoll absolut normiert
    qint32 zNormStwg=0,      zNormStwgOld=0,      zNormStwgMin=0,      zNormStwgMax = 0;      // 0x0094 (148) Steuerwagen: Zug- und Brems-Gesamtkraftsoll absolut normiert
    quint16 VIst = 0;
    qint16 zMaxPAchs = 1;
    qint16 zMaxPAchsTestOld = 0;
    qint16 zMaxNumber = 0;
    qint16 zMaxLine = 0;
    qint16 zMinNumber = 0;
    qint16 zMinLine = 0;
    qint16 zMaxCorr = 100;
    qint16 zMinCorr = 100;
    quint8 zMaxPAchsOkCount = 0;
    bool useNormStwg = false, useNormSollStwg = false;
    bool haveLokoInList;
    bool forwardDriveModeDisplay = false;        // Only when true, drive mode needs to be forwarded.
    qint16 skalen[69][12] = {
      /* A: Class
       * I: Pre factor for accelerating values as workaround for unclean values from  Zusi
       * J: Drive mode display (Fahrstufenanzeige) (y/n),
       * K: Pre factor for drive mode values as workaround for unclean values from  Zusi
       * L: Offset for drive mode values as workaround for unclean values from  Zusi
                                       A  J  K   J*/
                                   { 101, 0, 1,  0 },//
                                   { 103, 1, 1,  0 },//
                                   { 100, 1, 1,  0 },// ; Kopie von 151, werte richtig?
                                   { 110, 1, 1,  0 },//
                                   { 111, 1, 1,  0 },//
                                   { 112, 1, 1,  0 },//
                                   { 114, 1, 1,  0 },// // FIXME: DR BR118 allready available
                                   { 120, 0, 1,  0 },//
                                   { 128, 0, 1,  0 },//
                                   { 139, 1, 1,  0 },// ; Kopie von 151, werte richtig?
                                   { 140, 1, 1,  0 },// ; Kopie von 151, werte richtig?
                                   { 141, 1, 1,  0 },// ; Kopie von 151, werte richtig?
                                   { 143, 1, 1,  0 },//
                                   { 145, 0, 1,  0 },//
                                   { 146, 0, 1,  0 },//
                                   { 147, 0, 1,  0 },// Decoration vehicle
                                   { 150, 1, 1,  0 },// ; Kopie von 151, werte richtig?
                                   { 151, 1, 1,  0 },//
                                   { 152, 0, 1,  0 },//
                                   { 155, 1, 1,  0 },//
                                   { 182, 0, 1,  0 },//
                                   { 183, 0, 1,  0 },//
                                   { 185, 0, 1,  0 },//
                                   { 186, 0, 1,  0 },//
                                   { 187, 0, 1,  0 },//
                                   { 189, 0, 1,  0 },//
                                   { 193, 0, 1,  0 },//
                                   { 210, 1, 9, -2 },// V160
                                   { 211, 1, 9, -2 },// V100
                                   { 212, 1, 9, -2 },// V100
                                   { 213, 1, 9, -2 },// V100
                                   { 214, 1, 9, -2 },// V100
                                   { 215, 1, 9, -2 },// V160
                                   { 216, 1, 9, -2 },// V160
                                   { 217, 1, 9, -2 },// V160
                                   { 218, 1, 9, -2 },// V160 To do: Implement force step
                                   { 220, 1, 9, -2 },// V200 To do: Implement force step
                                   { 221, 1, 9, -2 },// V200 To do: Implement force step
                                   { 246, 0, 1,  0 },//
                                   { 245, 0, 1,  0 },//
                                   { 285, 0, 1,  0 },//
                                   { 401, 0, 1,  0 },//
                                   { 402, 0, 1,  0 },//
                                   { 403, 0, 1,  0 },//
                                   { 406, 0, 1,  0 },//
                                   { 407, 0, 1,  0 },//
                                   { 411, 0, 1,  0 },//
                                   { 812, 0, 1,  0 },//
                                   { 415, 0, 1,  0 },//
                                   { 422, 0, 1,  0 },//
                                   { 423, 0, 1,  0 },//
                                   { 424, 0, 1,  0 },//
                                   { 425, 0, 1,  0 },//
                                   { 426, 0, 1,  0 },//
                                   { 430, 0, 1,  0 },//
                                   { 442, 0, 1,  0 },// Decoration vehicle
                                   { 445, 0, 1,  0 },// Decoration vehicle (KISS)
                                   { 470, 0, 1,  0 },//
                                   { 475, 0, 1,  0 },//
                                   { 482, 0, 1,  0 },//
                                   { 610, 1, 1,  0 },//
                                   { 611, 1, 1,  0 },//
                                   { 612, 0, 1,  0 },//
                                   { 628, 1, 10, 0 },//
                                   {1016, 0, 1,  0 },// Taurus, not implemented
                                   {1044, 0, 1,  0 },//
                                   {1116, 0, 1,  0 },//
                                   {1144, 0, 1,  0 },//
                                   {1216, 0, 1,  0 } //
    };
private slots:
    void syncPowerIndicator();    
    void relativatePower(float power, qint32 &min, qint32 &max, qint32 &old, qint32 &dest, QSettings *settings, const QString &keyPrefix);
    void restoreExtrems();

public slots:
    void setFahrstufe(float stufe);
    void setZugkraftProAchse(float power);                  // 0x000A
    void setZugkraftSollGesammt(float power);               // 0x000B
    void setZugkraftSollProAchse(float power);              // 0x000C
    void setZugkraft(float power);                          // 0x0009
    void setZugkraftGesammtSteuerwagen(float power);        // 0x007C
    void setZugkraftProAchseSteuerwagen(float power);       // 0x007D
    void setZugkraftSollGesammtSteuerwagen(float power);    // 0x007E
    void setZugkraftSollProAchseSteuerwagen(float power);   // 0x007F
    void setZugkraftSollNormiert(float power);              // 0x0090
    void setZugkraftSollNormiertSteuerwagen(float power);   // 0x0091
    void setZugkraftNormiert(float power);                  // 0x0093
    void setZugkraftNormiertSteuerwagen(float power);       // 0x0094
    void setVIst(quint16 V);
    void setBaureihe(QString fahrzeug);
};

#endif // ZUSIPOWER_H

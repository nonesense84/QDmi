#ifndef ZUSIPOWER_H
#define ZUSIPOWER_H

#include <QObject>
#include <QDebug>
#include <QtMath>

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
private:
    qint16 zGes, zGesNew = 0;                      // 0x0009 (9)                Zugkraft gesamt
    qint16 zPAchs, zPAchsNew = 0;                  // 0x000A (10)               Zugkraft pro Achse
    qint16 zSollGes, zSollGesNew = 0;              // 0x000B (11)               Zugkraft-Soll gesamt
    qint16 zSollPAchs, zSollPAchsNew = 0;          // 0x000C (12)               Zugkraft-Soll pro Achse
    qint16 zGesStwg, zGesStwgNew = 0;              // 0x007C (124) Steuerwagen: Zugkraft gesamt
    qint16 zPAchsStwg, zPAchsStwgNew = 0;          // 0x007D (125) Steuerwagen: Zugkraft pro Achse
    qint16 zSollGesStwg, zSollGesStwgNew = 0;      // 0x007E (126) Steuerwagen: Zugkraft-Soll gesamt
    qint16 zSollPAchsStwg, zSollPAchsStwgNew = 0;  // 0x007F (127) Steuerwagen: Zugkraft-Soll pro Achse
    qint16 zSollNorm, zSollNormNew = 0;            // 0x0090 (144)              Zug- und Brems-Gesamtkraftsoll normiert
    qint16 zSollNormStwg, zSollNormStwgNew = 0;    // 0x0091 (145) Steuerwagen: Zug- und Brems-Gesamtkraftsoll normiert
    qint16 zNorm, zNormNew = 0;                    // 0x0093 (147)              Zug- und Brems-Gesamtkraftsoll absolut normiert
    qint16 zNormStwg, zNormStwgNew = 0;            // 0x0094 (148) Steuerwagen: Zug- und Brems-Gesamtkraftsoll absolut normiert
    qint16 VIst = 0;
    QVector<qint16> powerValuesToDecoder;

    qint16 zMaxPAchs = 1;
    qint16 zMaxPAchsTestOld = 0;
    uint8_t zMaxPAchsOkCount = 0;

    bool useNormStwg = false, useNormSollStwg = false;
    bool haveLokoInList;
    qint16 zMaxNumber = 0;
    qint16 zMaxLine = 0;
    qint16 zMinNumber = 0;
    qint16 zMinLine = 0;
    QString unitBraking = "";
    QString unitAccelerating= "";
    QString unitType[5] = {"","kN", "kN/FM", "%", "Stufen"};
    bool bdb = false; // Basic debug
    qint16 skalen[38][7] = {       // Typ1 kN, Ty2 kN/FM, typ3 %, Typ4 Stufen
        {101,140,145,1,70,75,2,},
        {103,90,90,1,50,50,2,},
        {111,140,145,1,70,75,2,},
        {112,140,145,1,70,75,2,},
        {143,140,145,1,70,75,2,},
        {145,150,150,1,80,85,2,},
        {146,140,145,1,70,75,2,},
        {147,320,320,1,320,320,1,},
        {152,150,150,1,80,80,2,},
        {155,200,212,1,60,86,2,},
        {182,150,150,1,300,300,1,},
        {185,140,145,1,70,75,2,},
        {186,140,145,1,300,300,1,},
        {187,320,320,1,320,320,1,},
        {189,140,145,1,70,75,2,},
        {182,240,250,1,300,300,1,},
        {1016,240,250,1,300,300,1,},
        {1116,240,240,1,300,300,1,},
        {183,240,240,1,300,300,1,},
        {1216,240,240,1,300,300,1,},
        {193,140,150,1,350,350,1,},
        {218,5,7,4,15,15,4,},
        {246,140,145,1,300,300,1,},
        {285,140,145,1,300,300,1,},
        {245,140,145,1,300,300,1,},
        {401,150,150,1,50,50,2,},
        {403,100,100,3,100,100,3,},
        {406,100,100,3,100,100,3,},
        {411,100,100,3,100,100,3,},
        {415,100,100,3,100,100,3,},
        {422,140,150,1,25,25,2,},
        {423,150,150,1,25,25,2,},
        {424,150,150,1,25,25,2,},
        {425,150,150,1,25,25,2,},
        {426,150,150,1,25,25,2,},
        {430,140,150,1,25,25,2,},
        {442,100,100,3,100,100,3,},
        {445,150,150,1,300,300,1,}
    };
private slots:
    void syncPowerIndicator();
    qint16 makeNormPlausible(qint16 A, qint16 B);
    qint16 maxAbs(qint16 A, qint16 B);

public slots:
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
    void setVIst(qint16 V);
    void setBaureihe(QString fahrzeug);
};

#endif // ZUSIPOWER_H

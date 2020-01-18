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
    qint16 zMaxCorr = 100;
    qint16 zMinCorr = 100;
    QString unitBraking = "";
    QString unitAccelerating= "";
    QString unitType[5] = {"","kN", "kN/FM", "%", "Stufen"};
    bool bdb = false; // Basic debug
    qint16 skalen[49][9] = {       // Typ1 kN, Ty2 kN/FM, typ3 %, Typ4 Stufen
      //{BR, Number-Min, Line-Min, Typ, Pre-Factor-Br, Number-Max, Line-Max, Typ, Pre-Factor-Ac}
                                   { 101,140,145,1,400, 70, 75,2, 100},// Braking power adapted
                                   { 103, 90, 90,1,300, 50, 50,2, 100},// Braking power adapted
                                   { 111,140,145,1,400, 70, 75,2, 100},// Braking power adapted
                                   { 112,140,145,1,400, 70, 75,2, 100},// Braking power adapted
                                   { 114,140,145,1,400, 70, 75,2, 100},// Braking power adapted
                                   { 120,140,145,1,400, 70, 75,2, 100},// Braking power adapted
                                   { 128,140,145,1,400, 70, 75,2, 100},// Braking power adapted
                                   { 143,140,145,1,400, 70, 75,2, 100},// Braking power adapted
                                   { 145,150,150,1,400, 80, 85,2, 100},// Braking power adapted
                                   { 146,140,145,1,400, 70, 75,2, 100},// Braking power adapted
                                   { 147,320,320,1,400,320,320,1, 400},// Decoration vehicle
                                   { 151,140,145,1,400, 70, 75,2, 100},// Braking power adapted
                                   { 152,150,150,1,400, 80, 80,2, 100},// Braking power adapted
                                   { 155,200,212,1,400, 60, 86,2, 100},// Braking power adapted
                                   { 182,150,150,1,400,300,300,1, 400},// Braking power adapted
                                   { 183,240,240,1,400,300,300,1, 400},// Braking power adapted
                                   { 185,140,145,1,400, 70, 75,2, 100},// Braking power adapted
                                   { 186,140,145,1,400,300,300,1, 400},// Braking power adapted
                                   { 187,320,320,1,400,320,320,1, 400},// Braking power adapted
                                   { 189,140,145,1,400, 70, 75,2, 100},// Braking power adapted
                                   { 193,140,150,1,400,350,350,1, 400},// Braking power adapted
                                   { 218,  5,  7,4,100, 15, 15,4, 100},// To do: Implement force step
                                   { 246,140,145,1,400,300,300,1, 400},// Braking power adapted
                                   { 245,140,145,1,400,300,300,1, 400},// Braking power adapted
                                   { 285,140,145,1,400,300,300,1, 400},// Braking power adapted
                                   { 401,150,150,1,400, 50, 50,2, 100},// Braking power adapted
                                   { 402,150,150,1,400, 50, 50,2, 100},// Braking power adapted
                                   { 403,100,100,3,533,100,100,3, 533},// Braking power adapted
                                   { 406,100,100,3,533,100,100,3, 533},// Braking power adapted
                                   { 411,100,100,3,800,100,100,3, 800},// Braking power adapted
                                   { 412,100,100,3,800,100,100,3, 800},// Braking power adapted
                                   { 415,100,100,3,800,100,100,3, 800},// Braking power adapted
                                   { 422,140,150,1,789, 25, 25,2, 100},// Braking power adapted
                                   { 423,150,150,1,789, 25, 25,2, 100},// Braking power adapted
                                   { 424,150,150,1,800, 25, 25,2, 100},// Braking power adapted
                                   { 425,150,150,1,800, 25, 25,2, 100},// Braking power adapted
                                   { 426,150,150,1,800, 25, 25,2, 100},// Braking power adapted
                                   { 430,140,150,1,789, 25, 25,2, 100},// Braking power adapted
                                   { 442,100,100,3,100,100,100,3, 100},// Decoration vehicle
                                   { 445,150,150,1,100,300,300,1, 100},// Decoration vehicle (KISS)
                                   { 470,150,150,1,400,300,300,1, 400},// Braking power adapted
                                   { 475,140,150,1,400,350,350,1, 400},// Braking power adapted
                                   { 482,140,145,1,400, 70, 75,2, 100},// Braking power adapted
                                   {1016,240,250,1,100,300,300,1, 100},// Taurus, not implemented
                                   {1044,150,150,1,100,300,300,1, 100}, // Braking power adapted
                                   {1116,150,150,1,400,300,300,1, 400},// Braking power adapted
                                   {1144,150,150,1,100,300,300,1, 100},// Braking power adapted
                                   {1216,150,150,1,400,300,300,1, 400} // Braking power adapted
    };
private slots:
    void syncPowerIndicator();
    qint16 makeNormPlausible(qint16 A, qint16 B);
    qint16 maxAbs(qint16 A, qint16 B);
    qint16 calcPower(float power);

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

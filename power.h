#ifndef POWER_H
#define POWER_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include "era.h"
#include <QDebug>

class power : public QWidget
{
    Q_OBJECT
public:
    power(QWidget *parent = nullptr);

public slots:
    void setPowerRelative(qint16 P);
    void setPowerAbsolute(qint16 P);
    void setPowerRelativeSet(qint16 P);
    void setPowerAbsoluteSet(qint16 P);
    void setRelativeBrakingScale();
    void setAbsoluteBrakingScale();
    void setRelativeAccelerateScale();
    void setAbsoluteAccelerateScale();
    void setAbsoluteAccelerateMaximum(qint16 P);
    void setAbsoluteBrakingMaximum(qint16 P);
    void setDpi(qreal dpi);
    void setUnitAcceleratingText(QString unit);
    void setUnitBrakingText(QString unit);
private:
    QString unitUniversal = "%";
    QString unitBraking = "kN";
    QString uniteAccelerating = "kN/FM";

    QString uniteAcceleratingAbsDef = "kN/FM";
    QString uniteAcceleratingRelDef = "%";
    QString uniteBrakingAbsDef = "kN";
    QString uniteBrakingRelDef = "%";

    bool useBrakingRelative;
    bool useAcceleratingRelative;

    int side;
    qreal AbsoluteAccelerateMaximum = 100;
    qreal AbsoluteBrakingMaximum = 100;
    qreal arcAccel = 0;
    qreal arcAccelSet = 0;
    qreal dimensionMatrix = 480; // Hinterfragen, wie der Wert zusatnde kommt!
    quint16 widthPowerRing = 50;
    quint8 widthSetTriangle = 50;
    quint8 widthPowerBrakeseperator = 10;
    quint8 beginPowerBrakeseperator = 150;
    quint8 EndPowerBrakeseperator = 230;

    quint16 beginTriangle = 217;
    quint16 deepSetTriangle = 17;
    quint16 widthSetTriangleHalf = 13;


    qreal arcOpenTotal = 288;   // 288° -100% to +100%
    qreal arcOpenRight = 144;   //
    qreal arcOpenLeft = -144;   //
    qreal startPosM100 = -144; //-(144 + 90)
    qreal startPosZero = -90; //
    quint8 numAccLines = 10;
    quint8 numBraLines = 10;

    QString numbers100erBraking[10] = {"20","40","60","80","100","","","","",""};
    QString numbers150erBraking[10] = {"30","60","90","120","150","","","","",""};
    //QString numbers150erBraking[7] = {"20","40","60","80","100","120","140"};    // Variante mit vielen kleinen Nummern
    QString numbers250erBraking[10] = {"50","100","150","200","250","","","","",""};
    QString numbers320erBraking[10] = {"30","130","220","320","","","","","",""};


    QString numbers25erAccelerating[10] = {"0","5","10","15","20","25","","","",""};
    QString numbers50erAccelerating[10] = {"0","10","20","30","40","50","","","",""};
    QString numbers70erAccelerating[10] = {"0","15","30","45","60","75","","","",""};
    //QString numbers70erAccelerating[8] = {"0","10","20","30","40","50","60","70"};    // Variante mit vielen kleinen Nummern
    QString numbers100erAccelerating[10] = {"0","20","40","60","80","100","","","",""};
    QString numbers80erAccelerating[10] =  {"0","25","50","75","","","","","",""};
    QString numbers300erAccelerating[10] = {"0","60","120","180","240","300","","","",""};
    QString numbers320erAccelerating[10] = {"30","130","220","320","","","","",""};
    QString numbers350erAccelerating[10] = {"0","70","140","210","280","350","","","",""};

    int positionsBraking[8][2];
    int positionsAccelerating[8][2];
    //int positionsAccelerating75[8][2] = {{-5 , -140},{43 , -122},{ 87, -98},{ 115, -58},{132, -14},{134,  33},{119, 78},{90, 117}};
    int positionsAccelerating25[8][2] =  {{ -45, -167},{ 20, -148},{ 79, -102},{ 104, -33},{ 90, 35},{ 40, 95}};// 0,  5, 10, 15, 20,  25
    int positionsAccelerating50[8][2] =  {{ -45, -167},{ 27, -148},{ 79, -102},{ 104, -33},{ 90, 35},{ 40, 95}};// 0, 10, 20, 30, 40,  50
    int positionsAccelerating70[8][2] =  {{ -45, -167},{ 27, -148},{ 79, -102},{ 104, -33},{ 90, 35},{ 40, 95}};// 0, 15, 30, 45, 60,  75
    int positionsAccelerating100[8][2] = {{ -45, -167},{ 27, -148},{ 79, -102},{ 104, -33},{ 90, 35},{ 40, 95}};// 0, 20, 40, 60, 80, 100
    int positionsAccelerating300[8][2] = {{ -45, -167},{ 27, -148},{ 79, -102},{ 104 ,-33},{ 90, 35},{ 40, 95}};// 0, 60,120,180,240, 300
    int positionsAccelerating350[8][2] = {{ -45, -167},{ 27, -148},{ 77, -102},{ 102, -33},{ 88, 35},{ 40, 95}};// 0, 70,140,210,280, 350
    int positionsAccelerating80[8][2] =  {{ -45, -167},{ 52, -128},{ 104, -33},{ 67,   70}};                    // 0, 25, 50, 75
    int positionsAccelerating320[8][2] = {{ -2, -160},{ 75,  -102},{ 100, 2},{ 44, 92}};                        //30,130,220,320

    int positionsBraking100[8][2] =      {{ -80, -113},{ -135, -67},{ -155, 2},{ -140, 70},{ -100, 130}};//20,  40, 60,   80, 100
    int positionsBraking150[8][2] =      {{ -80, -113},{ -135, -67},{ -155, 2},{ -140, 70},{ -100, 130}};//30,  60, 90,  120, 150
    int positionsBraking250[8][2] =      {{ -80, -113},{ -135, -67},{ -155, 2},{ -140, 70},{ -100, 130}};//50, 100, 150, 200, 250
    int positionsBraking320[8][2] =      {{ -56, -125},{ -135, -67},{ -153, 37},{ -97, 127}};            //30, 130, 220, 320

    QString numbersAccelerating[10];
    QString numbersBraking[10];

    int numNumbersBraking = 5;
    int numNumbersAccelerating = 6;
    int brakingDial = 100;
    //int fontSiceDial = 17;        // Für 150er Skala
    QFont fontDial;
    QFont fontNose;
    QFont fontDescription;
    int fontSiceDial = 37;        // Für 100er Skala
    int fontSiceNose = 50;
    int fontSiceDescription = 23;

    void updateUnits();
protected:
    void paintEvent(QPaintEvent *event) override;

};

#endif // POWER_H

#ifndef GAUGE_H
#define GAUGE_H
#include <QWidget>
#include <QPainter>
#include "era.h"
#include <QFontDatabase>
#include <QDebug>
class gauge : public QWidget
{
    Q_OBJECT
public:
    explicit gauge(QWidget *parent = nullptr);

signals:

public slots:
    void setVMaxDial(quint16 V);
    void setVPerm(quint16 V, bool visible);
    void setVSet(quint16 V, bool visible);
    void setVTarget(quint16 V, bool visible);
    void setOverspeed(qreal V, bool intervenation, bool overspeedWarning);
    qreal calcPosition(quint16 V);
    void setVAct(qreal V);
    void setDpi(qreal dpi);
    void setEraUse(bool useEra);

private:
    int VMaxDial;
    quint16 vAct, vPerm, vSet, vTarget, vOverspeed;
    qreal scaleDegStep;
    qreal scaleDegStepSmall;
    quint8 numShortLines;
    quint8 numLongLines;
    quint8 numNumbers;
    bool showNeedle = true;
    bool useHook = true;
    bool pointerRed = false;
    bool pointerOrange = false;
    qreal arcOpenTotal = 288;   // 288° 0 to Vmax
    qreal startPosZero = -234; //-(144 + 90)
    qreal dimensionMatrix = 548;
    quint16 outEndLines = 250;
    quint16 lenShortLines = 30;
    quint16 lenLongLines = 50;
    quint16 beginTriangle = 253;
    quint16 deepSollTriangle = 17;
    quint16 widthSollTriangleHalf = 13;
    quint16 scale400Numbers[7] = {0,50,100,150,200,300,400};

    quint16 beginVSetArc = 222;
    quint16 diamVSetArc = 26;

    quint16 widthCsgRing = 18;
    quint16 outBorderCsg = 274;
    quint16 diamNose = 50;
    quint16 widhtNeedleThick = 18;
    quint16 widhtNeedleThin = 6;
    quint16 lenNeedleThick = 139;
    quint16 lenNeedleRamp = 155;
    quint16 lenNeedleThin = 185;
    quint16 lenHook = 40;
    qreal posCsg = 0;
    qreal posTarget = 0;
    qreal posVSet = false;
    qreal posNeedle = 0;
    qreal posOverspeed = 0;
    qreal fontSiceDial = 26;
    qreal fontSiceNose = 37;
    bool csgVisible = false;
    bool vSetVisible =false;

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // GAUGE_H

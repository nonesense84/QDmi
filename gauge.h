#ifndef GAUGE_H
#define GAUGE_H
#include <QWidget>
#include <QPainter>
#include "era.h"
#include <QFontDatabase>
#include <QTimer>
#include <QDebug>
#include "zusi3etcs.h"
class gauge : public QWidget
{
    Q_OBJECT
public:
    explicit gauge(QWidget *parent = nullptr);

signals:
    void activatedSpeedHooks(bool active);

public slots:
    void reset();
    void setVMaxDial(quint16 V);
    void setVPerm(quint16 V, bool visibl, bool fromEtcs);
    void setVSet(quint16 V, bool visible);
    void setVTarget(quint16 V, bool visible, bool fromEtcs);
    void setVRelease(quint16 V);
    void setIntervenation(bool intervenation);
    void setOverspeed(bool intervenation);
    void setVOverspeed(quint16 V);
    void setMode(quint16 newMode);
    void setBasicSpeedHooks(bool active);

    qreal calcPosition(quint16 V);
    void setVAct(quint16 V);
    void setDpi(qreal dpi);
    void setEraUse(bool useEra);
    void setCsmReducing(bool reducing);
private:
    #define lenHook 40
    #define widthCsgRing 18
    #define widthReleaseRingOuter 12
    #define widthReleaseRingInner 2
    #define zeroOffset  64 // 4 * 16. This is the position, where CSG starts. It is 4deg below zero
    QPen hookRed;
    QPen hookOrange;
    QPen hookDarkGrey;
    QPen hookMediumGrey;
    QPen hookWhite;
    QPen hookYellow;
    QPen ringRed;
    QPen ringOrange;
    QPen ringDarkGrey;
    QPen ringWhite;
    QPen ringWhiteRelWhite;
    QPen ringWhiteRelBlue;
    QPen ringYellow;
    QTimer *attenuationTimer = new QTimer();
    QVector<QPoint> numberPositions;
    qreal scaleDegStep;
    qreal scaleDegStepSmall;
    qreal arcOpenTotal = 288;   // 288° 0 to Vmax
    qreal startPosZero = -234; //-(144 + 90)
    qreal dimensionMatrix = 548;
    qreal posCsg = 0;
    qreal posCsgDest = 0;
    qreal posTarget = 0;
    qreal posRelease = 0;
    qreal posVSet = 0;
    qreal posVSetDest = 0;
    qreal posVOverspeed = 0;
    qreal posNeedle = 0;
    qreal posNeedleDest = 0;
    qreal fontSiceDial = 26;
    qreal fontSiceNose = 37;
    quint16 VMaxDial;
    quint16 vAct, vPerm, vSet, vTarget, vOverspeed = 0, vRelease = 0;
    quint16 outEndLines = 250;
    quint16 lenShortLines = 30;
    quint16 lenLongLines = 50;
    quint16 beginTriangle = 253;
    quint16 deepSollTriangle = 17;
    quint16 widthSollTriangleHalf = 13;
    quint16 scale400Numbers[7] = {0,50,100,150,200,300,400};
    quint16 mode = 0;
    quint16 beginVSetArc = 212;
    quint16 diamVSetArc = 19;
    quint16 outBorderCsg = 274;
    quint16 diamNose = 50;
    quint16 widhtNeedleThick = 18;
    quint16 widhtNeedleThin = 6;
    quint16 lenNeedleThick = 164;
    quint16 lenNeedleRamp = 180;
    quint16 lenNeedleThin = 210;
    quint8 numShortLines;
    quint8 numLongLines;
    bool showNeedle = true;
    bool useEraStyle = true;
    bool dataFromEtcs = false;
    bool useCsg = true;
    bool useBasicHooks = false;
    bool displayBasicHooks = false;
    bool IntS = false;          // Intervention Status information
    bool OvS = false;           // Over-speed Status information
    bool TSM = false;           // Target Speed Monitoring
    bool csgVisible = false;
    bool vSetVisible =false;
    QColor colorListCsg[14][5] ={   // 8.2.1.4.9 The CSG shall follow the DMI colour as defined in Table 9.
        {era::darkGrey,era::darkBlue,  era::darkBlue,era::darkBlue,era::darkBlue},  // CSM NoS
        {era::darkGrey,era::darkBlue,  era::darkBlue,era::darkBlue,era::orange  },  // CSM OvS
        {era::darkGrey,era::darkBlue,  era::darkBlue,era::darkBlue,era::orange  },  // CSM WaS
        {era::darkGrey,era::darkBlue,  era::darkBlue,era::darkBlue,era::red     },  // CSM IntS
        {era::darkBlue,era::darkBlue,  era::darkGrey,era::white,   era::darkBlue},  // CSM NoS
        {era::darkBlue,era::darkBlue,  era::darkGrey,era::white,   era::orange  },  // CSM OvS
        {era::darkBlue,era::darkBlue,  era::darkGrey,era::darkGrey,era::orange  },  // CSM WaS
        {era::darkBlue,era::darkBlue,  era::darkGrey,era::darkGrey,era::red     },  // CSM IntS (With target information)
        {era::darkBlue,era::mediumGrey,era::darkGrey,era::yellow,  era::darkBlue},  // TSM IndS
        {era::darkBlue,era::mediumGrey,era::darkGrey,era::yellow,  era::orange  },  // TSM OvS
        {era::darkBlue,era::mediumGrey,era::darkGrey,era::yellow,  era::orange  },  // TSM WaS
        {era::darkBlue,era::mediumGrey,era::darkGrey,era::yellow,  era::red     },  // TSM IntS
        {era::darkBlue,era::mediumGrey,era::darkBlue,era::yellow,  era::darkBlue},  // RSM IndS
        {era::darkBlue,era::mediumGrey,era::darkBlue,era::yellow,  era::darkBlue},  // RSM IntS
    };

    QColor colorListPointerFsOs[14][5] ={   // 8.2.1.2.5  Depending  on  the  mode  and  the  supervision  status,  the  pointer  shall  follow  the  DMI colour as specified in Table 8.
        {era::grey,    era::darkBlue,era::darkBlue,era::darkBlue,era::darkBlue},  // CSM NoS
        {era::darkBlue,era::darkBlue,era::darkBlue,era::darkBlue,era::orange  },  // CSM OvS
        {era::darkBlue,era::darkBlue,era::darkBlue,era::darkBlue,era::orange  },  // CSM WaS
        {era::grey,    era::darkBlue,era::darkBlue,era::darkBlue,era::red     },  // CSM IntS
        {era::darkBlue,era::darkBlue,era::grey,    era::white,   era::darkBlue},  // CSM NoS (With target information)
        {era::darkBlue,era::darkBlue,era::darkBlue,era::darkBlue,era::darkBlue},  // CSM OvS
        {era::darkBlue,era::darkBlue,era::darkBlue,era::darkBlue,era::darkBlue},  // CSM WaS
        {era::darkBlue,era::darkBlue,era::grey,    era::white,   era::darkBlue},  // CSM IntS
        {era::darkBlue,era::darkBlue,era::grey,    era::yellow,  era::darkBlue},  // TSM IndS
        {era::darkBlue,era::darkBlue,era::darkBlue,era::darkBlue,era::orange  },  // TSM OvS
        {era::darkBlue,era::darkBlue,era::darkGrey,era::darkBlue,era::orange  },  // TSM WaS
        {era::darkBlue,era::darkBlue,era::grey,    era::yellow,  era::red     },  // TSM IntS
        {era::darkBlue,era::yellow,  era::darkBlue,era::darkBlue,era::darkBlue},  // RSM IndS
        {era::darkBlue,era::yellow,  era::darkBlue,era::darkBlue,era::red     },  // RSM IntS
    };
    QBrush selectNoseBrush() const;
private slots:
    void attenuationRoutine();
    void paintVSet(QPainter *painter);
    void paintLzbTriangular(QPainter *painter);
    void paintHookCsg(QPainter *painter);
    void paintReleaseSpeed(QPainter *painter, quint8 part);
    void paintBasicHookPerm(QPainter *painter);
    void paintBasicHookTarget(QPainter *painter);
    void paintCircle(QPainter *painter);
    void paintOverspeed(QPainter *painter);
    void drawLines(QPainter* painter, int count, qreal step, int length, int halfStepStartIndex, qreal initialRotate);
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // GAUGE_H

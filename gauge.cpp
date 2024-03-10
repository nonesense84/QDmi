#include "gauge.h"

gauge::gauge(QWidget *parent) : QWidget(parent)
{
    QFontDatabase::addApplicationFont(":/fonts/FreeSans.ttf");
    setVMaxDial(400);
    setVPerm(0, false, false);
    setVAct(0.0);
    connect(attenuationTimer, SIGNAL(timeout()),this,SLOT(attenuationRoutine()));
    attenuationTimer->setInterval(40);
    attenuationTimer->start();
    hookRed           = QPen(era::red,        lenHook, Qt::SolidLine, Qt::FlatCap,Qt::BevelJoin);
    hookOrange        = QPen(era::orange,     lenHook, Qt::SolidLine, Qt::FlatCap,Qt::BevelJoin);
    hookDarkGrey      = QPen(era::darkGrey,   lenHook, Qt::SolidLine, Qt::FlatCap,Qt::BevelJoin);
    hookMediumGrey    = QPen(era::mediumGrey, lenHook, Qt::SolidLine, Qt::FlatCap,Qt::BevelJoin);
    hookWhite         = QPen(era::white,      lenHook, Qt::SolidLine, Qt::FlatCap,Qt::BevelJoin);
    hookYellow        = QPen(era::yellow,     lenHook, Qt::SolidLine, Qt::FlatCap,Qt::BevelJoin);
    ringRed           = QPen(era::red,        widthCsgRing, Qt::SolidLine, Qt::FlatCap,Qt::BevelJoin);
    ringOrange        = QPen(era::orange,     widthCsgRing, Qt::SolidLine, Qt::FlatCap,Qt::BevelJoin);
    ringDarkGrey      = QPen(era::darkGrey,   widthCsgRing, Qt::SolidLine, Qt::FlatCap,Qt::BevelJoin);
    ringWhite         = QPen(era::white,      widthCsgRing, Qt::SolidLine, Qt::FlatCap,Qt::BevelJoin);
    ringYellow        = QPen(era::yellow,     widthCsgRing, Qt::SolidLine, Qt::FlatCap,Qt::BevelJoin);
    ringWhiteRelWhite = QPen(era::white,      widthReleaseRingOuter, Qt::SolidLine, Qt::FlatCap,Qt::BevelJoin);
    ringWhiteRelBlue  = QPen(era::darkBlue,   widthReleaseRingInner, Qt::SolidLine, Qt::FlatCap,Qt::BevelJoin);
}
void gauge::setDpi(qreal dpi){
    fontSiceDial = 26.0 * 96 / dpi;
    fontSiceNose = 37.0 * 96 / dpi;
}

void gauge::attenuationRoutine(){
    bool updateNedded = false;
    qreal attenuation = 0.1;
    if(posNeedle < posNeedleDest){
        posNeedle = posNeedle + (posNeedleDest - posNeedle) * attenuation;
        if((posNeedleDest - posNeedle) < 0.25)posNeedle = posNeedleDest;
        updateNedded = true;
    }
    if(posNeedle > posNeedleDest){
        posNeedle = posNeedle - (posNeedle - posNeedleDest) * attenuation;
        if((posNeedle - posNeedleDest) < 0.25)posNeedle = posNeedleDest;
        updateNedded = true;
    }
    if(posCsg < posCsgDest){
        posCsg = posCsgDest;
        updateNedded = true;
    }
    if(posCsg > posCsgDest){
        posCsg = posCsg - (posCsg - posCsgDest) * attenuation;
        if((posCsg - posCsgDest) < 0.25)posCsg = posCsgDest;
        updateNedded = true;
    }
    if(vSetVisible){
        if(posVSet < posVSetDest){
            posVSet = posVSet + (posVSetDest - posVSet) * attenuation;
            if((posVSetDest - posVSet) < 0.25)posVSet = posVSetDest;
            updateNedded = true;
        }
        if(posVSet > posVSetDest){
            posVSet = posVSet - (posVSet - posVSetDest) * attenuation;
            if((posVSet - posVSetDest) < 0.25)posVSet = posVSetDest;
            updateNedded = true;
        }
    }
    if(updateNedded)update();
}

void gauge::setVAct(quint16 V){
    if(V != vAct){
        vAct = V;
        if(V > VMaxDial){
            showNeedle = false;
            update();
        }
        else{
            posNeedleDest = calcPosition(V);
            showNeedle = true;
            update();
        }
    }
}

void gauge::setVPerm(quint16 V, bool visible, bool fromEtcs){
    if((V != vPerm) || (visible != csgVisible)){
        vPerm = V;
        csgVisible = visible;
        posCsgDest = calcPosition(V);
        dataFromEtcs = fromEtcs;
        update();
    }
}

void gauge::setVSet(quint16 V, bool visible){
    if((V != vSet) || (visible != vSetVisible)){
        vSet = V;
        vSetVisible = visible;
        posVSetDest = calcPosition(V);
        update();
    }
}

void gauge::setVOverspeed(quint16 V){
    if(V != vOverspeed){
        vOverspeed = V;
        posVOverspeed = calcPosition(V);
        update();
    }
}
void gauge::setMode(quint16 newMode){
    if(mode != newMode){
        mode = newMode;
        useBasicHooks = mode == zusi3etcs::mode_OS || mode == zusi3etcs::mode_SR ||
                       mode == zusi3etcs::mode_SH || mode == zusi3etcs::mode_RV; // Req 8.2.1.5.7
        useCsg = mode == zusi3etcs::mode_FS;                                    // Req 8.2.1.4.9
        update();
    }
}

/*
void gauge::setVBreak(quint16 V){
    if(V != vAlert){
        vAlert = V;
        update();
    }
}
void gauge::setVEmerg(quint16 V){

}
*/
void gauge::setVTarget(quint16 V, bool visible, bool fromEtcs){
    if((V != vTarget) || (visible != csgVisible)){
        vTarget = V;
      //qDebug() << "vTarget: " << vTarget;
        csgVisible = visible;
        posTarget = calcPosition(V);
        dataFromEtcs = fromEtcs;
        update();
    }
}

void gauge::setVRelease(quint16 V){
    if(V!= vRelease){
        vRelease = V;
        posRelease = calcPosition(V);
        update();
    }
}

void gauge::setIntervenation(bool intervenation){
    if(IntS != intervenation){
        IntS = intervenation;
        update();
    }
}

void gauge::setOverspeed(bool overspeedWarning){
    if(overspeedWarning != OvS){
        OvS = overspeedWarning;
        update();
    }
}

void gauge::setCsmReducing(bool reducing){
    if(reducing != TSM){
        TSM = reducing;
      //qDebug() << "TSM: "  << TSM;
        update();
    }
}

qreal gauge::calcPosition(quint16 V){
    if(VMaxDial == 400){
        if(V < 200){
            return V / 200.0 * 192.0;
        }
        else{
            return 192.0 + ((V - 200.0) / 200.0 * (arcOpenTotal - 192.0));
        }
    }
    else{
        return V / static_cast<qreal> (VMaxDial) * static_cast<qreal> (arcOpenTotal);
    }
}

void gauge::setVMaxDial(quint16 V){
    quint16 newVMaxDial = 0;
    if (V < 251){
        newVMaxDial = 250;
        scaleDegStep = 23.04;
        scaleDegStepSmall = 23.04;
        numShortLines = 13;
        numLongLines = 13;
        numNumbers = 13;
    }
    if (V < 181){
        newVMaxDial = 180;
        scaleDegStep = 32.0;
        scaleDegStepSmall = 32.0;
        numShortLines = 9;
        numLongLines = 10;
        numNumbers = 10;
    }
    if (V < 141){
        newVMaxDial = 140;
        scaleDegStep = 41.14;
        scaleDegStepSmall = 41.14;
        numShortLines = 7;
        numLongLines = 8;
        numNumbers = 8;
    }
    if (V > 250){
        newVMaxDial = 400;
        scaleDegStep = 48;
        scaleDegStepSmall = 9.6;
        numShortLines = 40;
        numLongLines = 9;
        numNumbers = 7;
    }
    if(VMaxDial != newVMaxDial){
        VMaxDial = newVMaxDial;
        setVAct(vAct+1);
        setVAct(vAct-1);
        setVPerm(vPerm+1, csgVisible, dataFromEtcs);
        setVPerm(vPerm-1, csgVisible, dataFromEtcs);
        setVSet(vSet+1, vSetVisible);
        setVSet(vSet-1, vSetVisible);
        setVTarget(vTarget+1, csgVisible, dataFromEtcs);
        setVTarget(vTarget-1, csgVisible, dataFromEtcs);
        setOverspeed(!OvS);
        setOverspeed(!OvS);
        posVOverspeed = calcPosition(vOverspeed);
        posVSetDest = calcPosition(vSetVisible);
        posCsgDest = calcPosition(vPerm);
        posNeedleDest = calcPosition(vAct);
    }
}

void gauge::setEraUse(bool useEra){
    useEraStyle = useEra;
    update();
}

void gauge::paintEvent(QPaintEvent *)
{
    static const QPoint speedPointer[8] = {
        QPoint( diamNose -3,  widhtNeedleThick/2),
        QPoint( lenNeedleThick,  widhtNeedleThick/2),
        QPoint( lenNeedleRamp,   widhtNeedleThin/2),
        QPoint( lenNeedleThin,   widhtNeedleThin/2),
        QPoint( lenNeedleThin,  -widhtNeedleThin/2),
        QPoint( lenNeedleRamp,  -widhtNeedleThin/2),
        QPoint( lenNeedleThick, -widhtNeedleThick/2),
        QPoint( diamNose -3, -widhtNeedleThick/2)
    };
    static const QPoint numberPositions140[8] = {
        QPoint( -116, 157),
        QPoint( -186, 59),
        QPoint( -168, -68),
        QPoint( -76, -149),
        QPoint( 35, -149),
        QPoint( 109, -69),
        QPoint( 124, 59),
        QPoint( 60, 157)
    };
    static const QPoint numberPositions180[10] = {
        QPoint( -116, 158),     //0
        QPoint( -182, 78),      //20
        QPoint( -194, -16),      //40
        QPoint( -148, -104),      //60
        QPoint( -69, -162),      //80
        QPoint( 19, -162),      //100
        QPoint( 94, -104),      //120
        QPoint( 136, -16),      //140
        QPoint( 128, 78),      //160
        QPoint( 62, 158)      //180
    };
    static const QPoint numberPositions250[13] = {
        QPoint( -116, 158),     //0
        QPoint( -165, 99),      //20
        QPoint( -192, 33),      //40
        QPoint( -187, -36),      //60
        QPoint( -153, -99),      //80
        QPoint( -118, -144),      //100
        QPoint( -48, -166),      //120
        QPoint( 30, -158),      //140
        QPoint( 92, -122),      //160
        QPoint( 118, -66),      //180
        QPoint( 137, 2),      //200
        QPoint( 126, 71),      //220
        QPoint( 87, 136)      //240
    };
    static const QPoint numberPositions400[7] = {
        QPoint( -116, 158),     //0
        QPoint( -193,  27),     //50
        QPoint( -153, -103),     //100
        QPoint(  -30,-168),     //150
        QPoint(   99, -102),     //200
        QPoint(  138,  27),     //300
        QPoint(   65, 157),     //400
    };
    static QRectF digitalSpeedPos(-41,-30,100,50);
    int side = qMin(width(), height());
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / dimensionMatrix, side / dimensionMatrix);
    // Draw Numbers ===============================================
    painter.setPen(era::white);
    painter.setFont(QFont("FreeSans",fontSiceDial,QFont::Bold,false));
    for (int i = 0; i < numNumbers; ++i) {
        if(VMaxDial == 180)
            painter.drawText(numberPositions180[i],QString::number(i*20));
        if(VMaxDial == 140)
            painter.drawText(numberPositions140[i],QString::number(i*20));
        if(VMaxDial == 250)
            painter.drawText(numberPositions250[i],QString::number(i*20));
        if(VMaxDial == 400)
            painter.drawText(numberPositions400[i],QString::number(scale400Numbers[i]));
    }
    // Draw nose =============================================== Req 8.2.1.2.5
    painter.setPen(Qt::NoPen);
    painter.setBrush(era::grey);                                                     // Ceiling Speed Monitoring
    if(vAct > vTarget)                               painter.setBrush(era::white);   // Ceiling Speed Monitoring with target Information
    if((TSM && vAct > vTarget) || vAct < vRelease)   painter.setBrush(era::yellow);  // Target Speed Monitoring
    if(OvS)                                          painter.setBrush(era::orange);  // Over-speed Status information
    if(IntS && ((vAct > vPerm) || (vAct > vRelease)))painter.setBrush(era::red);     // Intervention Status information
    if(mode == zusi3etcs::mode_TR)                   painter.setBrush(era::red);     // Trip
    painter.drawEllipse(-diamNose,-diamNose,diamNose*2,diamNose*2);
    // Draw nose text ==========================================
    painter.setPen(era::black);
    if(IntS && ((vAct > vPerm) || (vAct > vRelease)))painter.setPen(era::white);
    if(mode == zusi3etcs::mode_TR)painter.setPen(era::white);
    painter.setFont(QFont("FreeSans",fontSiceNose,QFont::Bold,false));
    QRectF textRect = painter.boundingRect(digitalSpeedPos,Qt::AlignLeft,QString::number(vAct));
    qreal modPos = textRect.width() - 81;
    digitalSpeedPos.setLeft(-41 - modPos);
    painter.drawText(digitalSpeedPos,QString::number(vAct));
    // Draw long lines ===============================================
    qreal rotatetSteps = 0;
    painter.setPen(QPen(era::white, 3.5));
    painter.rotate(startPosZero);
    for (int i = 0; i < numLongLines; ++i) {
        painter.drawLine(outEndLines - lenLongLines, 0, outEndLines, 0);
        if((VMaxDial == 400)&& (i > 3)){
            painter.rotate(scaleDegStep / 2);
            rotatetSteps += scaleDegStep / 2;
        }
        else{
            painter.rotate(scaleDegStep);
            rotatetSteps += scaleDegStep;
        }
    }
    painter.rotate(-rotatetSteps);
    // Draw short lines ===============================================
    rotatetSteps = 0;
    painter.setPen(QPen(era::white, 3));
    if(VMaxDial == 400){
        painter.rotate(scaleDegStepSmall);
        rotatetSteps += scaleDegStepSmall;
    }
    else{
        painter.rotate(scaleDegStepSmall / 2);
        rotatetSteps += scaleDegStepSmall / 2;
    }
    for (int j = 0; j < numShortLines; ++j) {
        painter.drawLine(outEndLines - lenShortLines, 0, outEndLines, 0);
        if((VMaxDial == 400)&& (j > 18)){
            painter.rotate(scaleDegStepSmall / 2);
            rotatetSteps += scaleDegStepSmall / 2;
        }
        else{
            painter.rotate(scaleDegStepSmall);
            rotatetSteps += scaleDegStepSmall;
        }
    }
    painter.rotate(-rotatetSteps);
    // Draw needle ===============================================
    if (showNeedle){
        painter.setPen(Qt::NoPen);
        painter.rotate(posNeedle);
        painter.drawConvexPolygon(speedPointer, 8);
        painter.rotate(-posNeedle);
    }
    // Draw CSG and hooks ======================================
    if(csgVisible){
        if(!useEraStyle && !dataFromEtcs){
            paintLzbTriangular(&painter); // Old LZB Stlye
        }
        else{
            if(useCsg){
                paintOverspeed(&painter);   // Draw over speed, if needed
                paintReleaseSpeed(&painter, 0);
                paintCircle(&painter);      // Then draw upper and lower part of the CSG (ring)
                paintHookCsg(&painter);     // As last the hook, overlapping the upper part
                paintReleaseSpeed(&painter, 1);

            }
            if(useBasicHooks && displayBasicHooks){ // Draw only the hooks, wihout CSG (ring)
                paintBasicHookTarget(&painter);     // In case of just using basic hooks, vTarget mast be drawn as a hook. Req 8.2.1.5.7
                paintBasicHookPerm(&painter);
            }
        }
    }
    // Draw VSet ==============================================
    if(vSetVisible)paintVSet(&painter);
}
void gauge::paintVSet(QPainter *painter){
    painter->rotate(posVSet);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(era::white, Qt::SolidPattern));
    painter->drawEllipse(beginVSetArc, -diamVSetArc/2, diamVSetArc, diamVSetArc);
}
void gauge::paintLzbTriangular(QPainter *painter){
    static const QPoint vSollTriangle[3] = {
        QPoint( beginTriangle,  0),
        QPoint( beginTriangle + deepSollTriangle,  -widthSollTriangleHalf),
        QPoint( beginTriangle + deepSollTriangle,   widthSollTriangleHalf)
    };
    painter->rotate(posCsg);
    painter->setBrush(era::red);
    painter->setPen(Qt::NoPen);
    painter->drawPolygon(vSollTriangle, 3);
}
void gauge::paintHookCsg(QPainter *painter){
    int t = int(dimensionMatrix) - widthCsgRing;
    painter->rotate(posCsg);
    painter->setPen(hookDarkGrey);
    if((posCsg > posTarget)){
        painter->setPen(hookWhite);
        if(TSM && !useBasicHooks)painter->setPen(hookYellow);
    }
    painter->drawArc(-(t-22)/2,-(t-22)/2,(t-22),(t-22), -22, 44);
    painter->rotate(-posCsg);
}
void gauge::paintReleaseSpeed(QPainter *painter, quint8 part){
    if(part == 0){
        int t = int(dimensionMatrix) - widthCsgRing;
        painter->setPen(ringWhite);
        painter->drawArc(-t/2,-t/2,t,t, 0, static_cast<int>(-posRelease*16));
    }
    if(part == 1){
        int angle;
        if(posCsg < (posRelease - 1.375)){ // 22 / 16 = 1.375 (Is the thickness of the hook)
            angle = -static_cast<int>((posCsg - posTarget) * 16 + 22);
        }
        else{
            angle = -static_cast<int>(posRelease * 16);
        }
        int t = int(dimensionMatrix - widthReleaseRingOuter + 1);
        painter->setPen(ringWhiteRelWhite);
        painter->drawArc(-t/2,-t/2,t-1,t-1, 0, angle);
        t = int(dimensionMatrix - widthReleaseRingOuter - widthReleaseRingInner - 8);
        painter->setPen(ringWhiteRelBlue);
        painter->drawArc(-t/2,-t/2,t,t, 0, angle);
    }
}
void gauge::paintBasicHookPerm(QPainter *painter){
    int t = int(dimensionMatrix) - widthCsgRing;
    painter->rotate(posCsg);
    painter->setPen(hookWhite);        // Req 8.2.1.5.7
    painter->drawArc(-(t-22)/2,-(t-22)/2,(t-22),(t-22), 0, 66); // Hook has 10x20 cells. Req 8.2.1.5.4
    painter->rotate(-posTarget);
}
void gauge::paintBasicHookTarget(QPainter *painter){
    if(mode == zusi3etcs::mode_SH)return; // Dont draw basic hook of vTarget in case of shunting. Req 8.2.1.5.7
    int t = int(dimensionMatrix) - widthCsgRing;
    painter->rotate(posTarget);
    painter->setPen(hookMediumGrey);        // Req 8.2.1.5.7
    painter->drawArc(-(t-22)/2,-(t-22)/2,(t-22),(t-22), 0, 66); // Hook has 10x20 cells. Req 8.2.1.5.5
    painter->rotate(-posTarget);
}
void gauge::paintCircle(QPainter *painter){
    int t = int(dimensionMatrix) - widthCsgRing;
    // Draw the upper part from permitted to target speed:
    painter->rotate(posCsg);
    // Find out, whats the color of the upper part of the ring. Req 8.2.1.4.9:
    if(posCsg > posTarget){
        painter->setPen(ringWhite);
        if(TSM)painter->setPen(ringYellow);
    }
    painter->drawArc(-t/2,-t/2,t,t, 0, static_cast<int>((posCsg*16 - posTarget*16)));
    painter->rotate(-posCsg);
    // Then the lower part from zero to target speed (can be same as hook). Go 2/16deg more, to make shure, that it overlaps the upper part.
    painter->setPen(ringDarkGrey);
    painter->drawArc(-t/2,-t/2,t,t, zeroOffset, static_cast<int>(-posTarget*16) - zeroOffset - 2);
}
void gauge::paintOverspeed(QPainter *painter){
    if(vPerm > vOverspeed) return;
    int t = int(dimensionMatrix) - widthCsgRing;
    if((IntS || OvS)){//(posNeedle > posCsg) &&
        if(IntS){
            painter->setPen(hookRed);
        }
        else{
            painter->setPen(hookOrange);
        }
        painter->rotate(posCsg);
        painter->drawArc(-(t-22)/2,-(t-22)/2,(t-22),(t-22), 0,  static_cast<int>(posCsg*16-posVOverspeed*16));
        painter->rotate(-posCsg);
    }
}
void gauge::mousePressEvent(QMouseEvent *event){
    displayBasicHooks = !displayBasicHooks;
}

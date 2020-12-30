#include "gauge.h"

gauge::gauge(QWidget *parent) : QWidget(parent)
{
    QFontDatabase::addApplicationFont(":/fonts/FreeSans.ttf");
    setVMaxDial(400);
    setVPerm(0, false);
    setVAct(0.0);
}
void gauge::setDpi(qreal dpi){
    fontSiceDial = 26.0 * 96 / dpi;
    fontSiceNose = 37.0 * 96 / dpi;
}

void gauge::setVAct(quint16 V){
    if(V != vAct){
        vAct = V;
        if(V > VMaxDial){
            showNeedle = false;
            update();
        }
        else{
            posNeedle = calcPosition(V);
            showNeedle = true;
            update();
        }
        if(pointerOrange||pointerRed)posOverspeed = calcPosition(V);
    }
}

void gauge::setVPerm(quint16 V, bool visible){
    if((V != vPerm) || (visible != csgVisible)){
        vPerm = V;
        csgVisible = visible;
        posCsg = calcPosition(V);
        update();
    }
}

void gauge::setVSet(quint16 V, bool visible){
    if((V != vSet) || (visible != vSetVisible)){
        vSet = V;
        vSetVisible = visible;
        posVSet = calcPosition(V);
        update();
    }
}

void gauge::setVTarget(quint16 V, bool visible){
    if((V != vTarget) || (visible != csgVisible)){
        vTarget = V;
        csgVisible = visible;
        posTarget = calcPosition(V);
        update();
    }
}

void gauge::setIntervenation(bool intervenation){
    if(pointerRed != intervenation){
        posOverspeed = calcPosition(vAct);
        pointerRed = intervenation;
        update();
    }
}

void gauge::setOverspeed(bool overspeedWarning){
    if(overspeedWarning != pointerOrange){
        posOverspeed = calcPosition(vAct);
        pointerOrange = overspeedWarning;
        update();
    }
}

void gauge::setCsmReducing(bool reducing){
    if(reducing != csmYellow){
        csmYellow = reducing;
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
        setVPerm(vPerm+1, csgVisible);
        setVPerm(vPerm-1, csgVisible);
        setVSet(vSet+1, vSetVisible);
        setVSet(vSet-1, vSetVisible);
        setVTarget(vTarget+1, csgVisible);
        setVTarget(vTarget-1, csgVisible);
        setOverspeed(!pointerOrange);
        setOverspeed(!pointerOrange);
    }
}

void gauge::setEraUse(bool useEra){
    useHook = useEra;
    update();
}

void gauge::paintEvent(QPaintEvent *)
{
    static const QPoint speedPointer[8] = {
        QPoint( 0,  widhtNeedleThick/2),
        QPoint( lenNeedleThick,  widhtNeedleThick/2),
        QPoint( lenNeedleRamp,   widhtNeedleThin/2),
        QPoint( lenNeedleThin,   widhtNeedleThin/2),
        QPoint( lenNeedleThin,  -widhtNeedleThin/2),
        QPoint( lenNeedleRamp,  -widhtNeedleThin/2),
        QPoint( lenNeedleThick, -widhtNeedleThick/2),
        QPoint( 0,              -widhtNeedleThick/2)
    };
    static const QPoint vSollTriangle[3] = {
        QPoint( beginTriangle,  0),
        QPoint( beginTriangle + deepSollTriangle,  -widthSollTriangleHalf),
        QPoint( beginTriangle + deepSollTriangle,   widthSollTriangleHalf)
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

    // Draw long lines ===============================================
    painter.save();
    painter.setPen(QPen(era::white, 3.5));
    painter.rotate(startPosZero);
    for (int i = 0; i < numLongLines; ++i) {
        painter.drawLine(outEndLines - lenLongLines, 0, outEndLines, 0);
        if((VMaxDial == 400)&& (i > 3)){
            painter.rotate(scaleDegStep / 2);
        }
        else{
            painter.rotate(scaleDegStep);
        }
    }
    painter.restore();
    // Draw short lines ===============================================
    painter.save();
    painter.setPen(QPen(era::white, 3));
    if(VMaxDial == 400){
        painter.rotate(startPosZero + scaleDegStepSmall);
    }
    else{
        painter.rotate(startPosZero + scaleDegStepSmall / 2);
    }
    for (int j = 0; j < numShortLines; ++j) {
        painter.drawLine(outEndLines - lenShortLines, 0, outEndLines, 0);
        if((VMaxDial == 400)&& (j > 18)){
            painter.rotate(scaleDegStepSmall / 2);
        }
        else{
            painter.rotate(scaleDegStepSmall);
        }
    }
    painter.restore();
    // Draw Numbers ===============================================
    painter.save();
    painter.setPen(Qt::white);
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
    painter.restore();
    // Draw nose ===============================================
    painter.save();
    painter.setPen(Qt::NoPen);
    if(pointerRed){
        painter.setBrush(era::red);
    }
    else{
        if(pointerOrange){
            painter.setBrush(era::orange);
        }
        else{
            painter.setBrush(era::grey);
        }
    }
    painter.drawEllipse(-diamNose,-diamNose,diamNose*2,diamNose*2);
    //painter.restore();
    // Draw needle ===============================================
    //painter.setPen(Qt::NoPen);
    //painter.setBrush(era::grey);
    //if(pointerOrange)painter.setBrush(era::orange);
    //if(pointerRed)painter.setBrush(era::red);
    //painter.save();
    if (showNeedle){
        painter.rotate(startPosZero);
        painter.rotate(posNeedle);
        painter.drawConvexPolygon(speedPointer, 8);
    }
    painter.restore();
    // Draw nose text===========================================
    painter.save();
    painter.setPen(era::black);
    if(pointerRed)painter.setPen(era::white);
    painter.setFont(QFont("FreeSans",fontSiceNose,QFont::Bold,false));
    QRectF textRect = painter.boundingRect(digitalSpeedPos,Qt::AlignLeft,QString::number(vAct));
    qreal modPos = textRect.width() - 81;
    digitalSpeedPos.setLeft(-41 - modPos);
    painter.drawText(digitalSpeedPos,QString::number(vAct));

    // Draw CSG ===============================================
    if(csgVisible){
        painter.save();
        painter.rotate(startPosZero);
        painter.rotate(posCsg);
        if(useHook){
            int t = int(dimensionMatrix) - widthCsgRing;
            if((posOverspeed > posCsg) && (pointerRed || pointerOrange)){
                if(pointerRed){
                    painter.setPen(QPen(era::red, lenHook, Qt::SolidLine, Qt::FlatCap,Qt::BevelJoin));
                }
                else{
                    painter.setPen(QPen(era::orange, lenHook, Qt::SolidLine, Qt::FlatCap,Qt::BevelJoin));
                }
                painter.drawArc(-(t-22)/2,-(t-22)/2,(t-22),(t-22), 0,  static_cast<int>(posCsg-posOverspeed)*16);
            }
            if(posCsg > posTarget){ // If permitted speed is bigger than target speed, we have to draw
                                    // the upper part bright grey or yellow, when CSM is reducing
                // First the hook
                QColor hookColor = era::grey;
                if(csmYellow) hookColor = era::yellow;
                painter.setPen(QPen(hookColor, lenHook, Qt::SolidLine, Qt::FlatCap,Qt::BevelJoin));
                painter.drawArc(-(t-22)/2,-(t-22)/2,(t-22),(t-22), -22, 44);
                painter.restore();
                // Then lower part from zero to target speed:
                painter.rotate(startPosZero);
                painter.setPen(QPen(era::darkGrey, widthCsgRing));
                painter.drawArc(-t/2,-t/2,t,t,4*16, static_cast<int>(-(posTarget+3.0)*16));
                // Finaly the upper part from permitted to target speed:
                painter.setPen(QPen(hookColor, widthCsgRing));
                painter.rotate((posTarget+6.0));
                painter.drawArc(-t/2,-t/2,t,t,4*16, static_cast<int>(-(posCsg-posTarget-4)*16));
            }
            else{// If permitted speed and target speed are the same, draw everything dark grey.
                // First the hook
                painter.setPen(QPen(era::darkGrey, lenHook, Qt::SolidLine, Qt::FlatCap,Qt::BevelJoin));
                painter.drawArc(-(t-22)/2,-(t-22)/2,(t-22),(t-22), -22, 44);
                // Then lower part from zero to the hook:
                painter.restore();
                painter.setPen(QPen(era::darkGrey, widthCsgRing));
                painter.rotate(startPosZero);
                painter.drawArc(-t/2,-t/2,t,t,4*16, static_cast<int>(-(posCsg+2.0)*16));
            }
        }
        else{
            painter.setBrush(era::red);
            painter.setPen(Qt::NoPen);
            painter.drawPolygon(vSollTriangle, 3);
        }
        painter.restore();
    }
    // Draw VSet ==============================================
    if(vSetVisible){
        painter.save();
        painter.rotate(startPosZero);
        painter.rotate(posVSet);
        painter.setPen(QPen(era::grey,  2, Qt::SolidLine));
        painter.setBrush(QBrush(QColor(174, 50, 160), Qt::SolidPattern));
        painter.drawEllipse(beginVSetArc, -diamVSetArc/2, diamVSetArc, diamVSetArc);
        painter.restore();
    }
}

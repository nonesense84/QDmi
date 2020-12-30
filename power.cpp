#include "power.h"

power::power(QWidget *parent) : QWidget(parent){
    setRelativeBrakingScale();
    setRelativeAccelerateScale();
    setAbsoluteAccelerateMaximum(100);
    setAbsoluteBrakingMaximum(100);
    fontDescription = QFont("FreeSans",fontSiceDescription,QFont::Bold,false);
    fontDial = QFont("FreeSans",fontSiceDial,QFont::Bold,false);
    fontNose = QFont("FreeSans",fontSiceNose,QFont::Bold,false);
    connect(attenuationTimer, SIGNAL(timeout()),this,SLOT(attenuationRoutine()));
    attenuationTimer->setInterval(40);
    attenuationTimer->start();
}
void power::setDpi(qreal dpi){
    fontDial.setPointSizeF(26.0 * 96.0 / dpi);
    fontNose.setPointSizeF(37.0 * 96.0 / dpi);
    fontDescription.setPointSizeF(17.0 * 96.0 / dpi);
}

void power::attenuationRoutine(){
    //if(arcOpenRight == arcAccel) arcAccel = 0;
    //arcAccel++;
    if(arcAccel < arcAccelDest){
        arcAccel = arcAccel + (arcAccelDest - arcAccel) * 0.05;
        if((arcAccelDest - arcAccel) < 1)arcAccel = arcAccelDest;
        update();
    }
    if(arcAccel > arcAccelDest){
        arcAccel = arcAccel - (arcAccel - arcAccelDest) * 0.05;
        if((arcAccel - arcAccelDest) < 1)arcAccel = arcAccelDest;
        update();
    }
}

void power::setPowerRelative(qint16 P){
    if(P < -100) P = -100;
    if(P >  100) P = 100;
    if(useAcceleratingRelative)arcAccelDest = static_cast<double>(arcOpenRight / 100.0 * P);
}
void power::setPowerAbsolute(qint16 P){
    if(P > 0){
        if(!useAcceleratingRelative)arcAccelDest = static_cast<double>(arcOpenRight / AbsoluteAccelerateMaximum * P);
        if(arcAccel > arcOpenRight) arcAccelDest = arcOpenRight;
    }
    else{
        if(!useBrakingRelative)arcAccelDest = static_cast<double>(arcOpenRight / AbsoluteBrakingMaximum * P);
        if(arcAccel < arcOpenLeft) arcAccelDest = arcOpenLeft;
    }
}
void power::setPowerRelativeSet(qint16 P){
    if(P < -100) P = -100;
    if(P >  100) P = 100;
    if(useAcceleratingRelative)arcAccelSet = arcOpenRight / 100.0 * P;
    if(powerRelativeSet != P){
        update();
        powerRelativeSet = P;
    }
}
void power::setPowerAbsoluteSet(qint16 P){
   if(P > 0){
       if(!useAcceleratingRelative)arcAccelSet = arcOpenRight / AbsoluteAccelerateMaximum * P;
       if(arcAccelSet > arcOpenRight) arcAccelSet = arcOpenRight;
   }
   else{
       if(!useBrakingRelative)arcAccelSet = static_cast<double>(arcOpenRight / AbsoluteBrakingMaximum * P);
       if(arcAccelSet < arcOpenLeft) arcAccelSet = arcOpenLeft;
   }
}
void power::setRelativeBrakingScale(){
    useBrakingRelative = true;
    setAbsoluteBrakingMaximum(100);
    updateUnits();
}
void power::setAbsoluteBrakingScale(){
    useBrakingRelative = false;
    updateUnits();
}
void power::setUnitAcceleratingText(QString unit){
    if(unit == "%"){
        setRelativeAccelerateScale();
    }
    else{
        setAbsoluteAccelerateScale();
        uniteAcceleratingAbsDef = unit;
    }
}
void power::setUnitBrakingText(QString unit){
    if(unit == "%"){
        setRelativeBrakingScale();
    }
    else{
        setAbsoluteBrakingScale();
        uniteBrakingAbsDef = unit;
    }
}
void power::setDriveMode(quint8 mode){
    if(driveMode != mode){
        driveMode = QString::number(mode);
        update();
    }
}
void power::setModeDisplay(bool show){
    showDriveMode = show;
    updateUnits();
}
void power::setRelativeAccelerateScale(){
    useAcceleratingRelative = true;
    setAbsoluteAccelerateMaximum(100);
    updateUnits();
}
void power::setAbsoluteAccelerateScale(){
    useAcceleratingRelative = false;
    updateUnits();
}
void power::setAbsoluteAccelerateMaximum(qint16 P){
    if(useAcceleratingRelative)P = 100;
    for(int i = 0; i < 8; i++){
      if(  0 < P && P <  26){
          numbersAccelerating[i] = numbers25erAccelerating[i];
          positionsAccelerating[i][0] = positionsAccelerating25[i][0];
          positionsAccelerating[i][1] = positionsAccelerating25[i][1];
          AbsoluteAccelerateMaximum = 25;
      }
      if( 25 < P && P <  51){
          numbersAccelerating[i] = numbers50erAccelerating[i];
          positionsAccelerating[i][0] = positionsAccelerating50[i][0];
          positionsAccelerating[i][1] = positionsAccelerating50[i][1];
          AbsoluteAccelerateMaximum = 50;
      }
      if( 50 < P && P <  71){
          numbersAccelerating[i] = numbers70erAccelerating[i];
          positionsAccelerating[i][0] = positionsAccelerating70[i][0];
          positionsAccelerating[i][1] = positionsAccelerating70[i][1];
          AbsoluteAccelerateMaximum = 75;
       }
      if( 70 < P && P <  84){
          numbersAccelerating[i] = numbers80erAccelerating[i];
          positionsAccelerating[i][0] = positionsAccelerating80[i][0];
          positionsAccelerating[i][1] = positionsAccelerating80[i][1];
          AbsoluteAccelerateMaximum = 83.3333;
      }
      if( 84 < P && P < 101){
          numbersAccelerating[i] = numbers100erAccelerating[i];
          positionsAccelerating[i][0] = positionsAccelerating100[i][0];
          positionsAccelerating[i][1] = positionsAccelerating100[i][1];
          AbsoluteAccelerateMaximum = 100;
      }
      if(100 < P && P < 301){
          numbersAccelerating[i] = numbers300erAccelerating[i];
          positionsAccelerating[i][0] = positionsAccelerating300[i][0];
          positionsAccelerating[i][1] = positionsAccelerating300[i][1];
          AbsoluteAccelerateMaximum = 300;
      }
      if(300 < P && P < 321){
          numbersAccelerating[i] = numbers320erAccelerating[i];
          positionsAccelerating[i][0] = positionsAccelerating320[i][0];
          positionsAccelerating[i][1] = positionsAccelerating320[i][1];
          AbsoluteAccelerateMaximum = 320;
      }
      if(320 < P && P < 351){
          numbersAccelerating[i] = numbers350erAccelerating[i];
          positionsAccelerating[i][0] = positionsAccelerating350[i][0];
          positionsAccelerating[i][1] = positionsAccelerating350[i][1];
          AbsoluteAccelerateMaximum = 350;
      }
    }
    updateUnits();
}
void power::setAbsoluteBrakingMaximum(qint16 P){
    if(useAcceleratingRelative)P = 100;
    for(int i = 0; i < 8; i++){
        if(  0 < P && P <  101){
            numbersBraking[i] = numbers100erBraking[i];
            positionsBraking[i][0] = positionsBraking100[i][0];
            positionsBraking[i][1] = positionsBraking100[i][1];
            AbsoluteBrakingMaximum = 100;
        }
        if( 100 < P && P <  151){
            numbersBraking[i] = numbers150erBraking[i];
            positionsBraking[i][0] = positionsBraking150[i][0];
            positionsBraking[i][1] = positionsBraking150[i][1];
            AbsoluteBrakingMaximum = 150;
        }
        if( 150 < P && P <  251){
            numbersBraking[i] = numbers250erBraking[i];
            positionsBraking[i][0] = positionsBraking250[i][0];
            positionsBraking[i][1] = positionsBraking250[i][1];
            AbsoluteBrakingMaximum = 250;
         }
        if( 250 < P && P <  321){
            numbersBraking[i] = numbers320erBraking[i];
            positionsBraking[i][0] = positionsBraking320[i][0];
            positionsBraking[i][1] = positionsBraking320[i][1];
            AbsoluteBrakingMaximum = 320;
        }
    }
    updateUnits();
}
void power::paintEvent(QPaintEvent *){
    static QRectF centerUnitPos       (-60   , -32,120, 50);
    static QRectF driveModePos        (-30   , -0 , 60, 50);
    static QRectF descriptionPos1     (-110  , 160,100, 50);
    static QRectF descriptionPos2     (  14  , 160,100, 50);
    static QRectF unitBrakingPos      ( -40.5, 185, 30, 25);
    static QRectF uniteAcceleratingPos(  14  , 185, 70, 25);

    static const QPoint vSollTriangle[3] = {
        QPoint( beginTriangle,  0), QPoint( beginTriangle + deepSetTriangle,  -widthSetTriangleHalf), QPoint( beginTriangle + deepSetTriangle,   widthSetTriangleHalf)};
    side = qMin(width(), height());     // Use as reference the smaller side
    int t = int(dimensionMatrix) - widthPowerRing - widthSetTriangle;
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width() / 2, height() / 2);   // Moove zero to the middle
    painter.scale(side / dimensionMatrix, side / dimensionMatrix);  // Norm painter, independent of windowsize
    painter.rotate(startPosZero);
    // Draw bright background ===============================
    painter.save();
    painter.rotate(startPosM100);
    painter.setPen(QPen(era::grey, widthPowerRing, Qt::SolidLine, Qt::FlatCap,Qt::MiterJoin));
    painter.drawArc(-t/2, -t/2, t, t, 0, static_cast<int>(-(arcOpenTotal)*16));
    painter.restore();
    // Draw acceleration ===============================
    painter.save();
    if(arcAccel > 0){
        painter.setPen(QPen(QColor(0,0,234), widthPowerRing+1, Qt::SolidLine, Qt::FlatCap,Qt::MiterJoin));
    }
    else{
        painter.setPen(QPen(QColor(180,180,0), widthPowerRing+1, Qt::SolidLine, Qt::FlatCap,Qt::MiterJoin));
    }
    painter.drawArc(-t/2, -t/2, t, t, 0, static_cast<int>(-arcAccel*16));
    painter.restore();

    // Draw lines in acceleration part ========================================
    int tb = t / 2 - widthPowerRing / 2;    // Begin of section lines
    int te = tb + widthPowerRing;           // End of section lines
    painter.save();
    painter.setPen(QPen(era::darkBlue, 3));
    for (int j = 0; j < numAccLines; ++j) {
        painter.drawLine(te, 0, tb, 0);
        painter.rotate(-arcOpenLeft/numAccLines);
    }
    painter.restore();
  // Draw lines in braking part ========================================
    painter.save();
    painter.setPen(QPen(era::darkBlue, 3));
    for (int j = 0; j < numAccLines; ++j) {
        painter.drawLine(te, 0, tb, 0);
        painter.rotate(arcOpenLeft/numAccLines);
    }
    painter.restore();

    // Draw set triangle ===============================
    painter.save();
    painter.setBrush(era::grey);
    painter.setPen(Qt::NoPen);
    painter.rotate(arcAccelSet);
    painter.drawPolygon(vSollTriangle, 3);
    painter.restore();

    // Draw Center unit text ===============================
    painter.save();
    painter.setPen(Qt::white);
    painter.rotate(90);
    painter.setFont(fontNose);
    if(showDriveMode){
        painter.setFont(fontDescription);
        painter.drawText(centerUnitPos, Qt::AlignCenter ,"Stufe");
        painter.setFont(fontNose);
        painter.drawText(driveModePos, Qt::AlignCenter ,driveMode);
    }
    else{
        painter.drawText(centerUnitPos, Qt::AlignCenter ,unitUniversal);
    }
    painter.restore();

    // Draw unit seperator ========================================
    painter.save();
    painter.setPen(QPen(era::grey, widthPowerBrakeseperator, Qt::SolidLine, Qt::FlatCap,Qt::MiterJoin));
    painter.rotate(180);
    painter.drawLine(beginPowerBrakeseperator, 0, EndPowerBrakeseperator, 0);
    painter.restore();

    // Draw unit texts ========================================
    painter.save();
    painter.setPen(Qt::white);
    painter.rotate(90);
    painter.setFont(fontDescription);
    painter.drawText(descriptionPos1,Qt::AlignRight, "Bremsen");
    painter.drawText(descriptionPos2,Qt::AlignLeft, "Fahren");
    painter.drawText(unitBrakingPos, Qt::AlignRight, unitBraking);
    painter.drawText(uniteAcceleratingPos, Qt::AlignLeft, uniteAccelerating);
    painter.restore();

    // Draw braking numbers ========================================
    painter.save();
    painter.setPen(Qt::white);
    painter.rotate(90);
    painter.setFont(fontDial);
    for (int i = 0; i < 8; ++i) {
        if(numbersBraking[i] == "")break;
        painter.drawText(QPoint(positionsBraking[i][0],positionsBraking[i][1]),numbersBraking[i]);
    }
    // Draw Accelerating numbers ========================================
    for (int i = 0; i < 8; ++i) {
        if(numbersAccelerating[i] == "")break;
        QRect pos = QRect(QPoint(positionsAccelerating[i][0],positionsAccelerating[i][1]),QSize(57,35));
        painter.drawText(pos,Qt::AlignRight,numbersAccelerating[i]);
    }
    painter.restore();
}
void power::updateUnits(){
    if(useBrakingRelative && useAcceleratingRelative){ // Wenn beides rel., dann nur in der Mitte "%"
        if(showDriveMode){
            unitUniversal = "";
            unitBraking = "%";
            uniteAccelerating = "%";
        }
        else{
            unitUniversal = "%";
            unitBraking = "";
            uniteAccelerating = "";
        }
    }
    if(!useBrakingRelative && !useAcceleratingRelative){ // Wenn beides nicht rel., dann in der Mitte "kN" und rechts lokabhängig (kN oder kn/FM)
        if(showDriveMode){
            unitUniversal = "";
            unitBraking = "kN";
            uniteAccelerating = uniteAcceleratingAbsDef;
        }
        else{
            unitUniversal = "kN";
            unitBraking = "";
            uniteAccelerating = uniteAcceleratingAbsDef;
        }
    }
    if(useBrakingRelative && !useAcceleratingRelative){ // Wenn br. rel., dann links "%", rechts kn/FM und in der Mitte ""
        unitUniversal = "";
        unitBraking = "%";
        uniteAccelerating = "kN/FM";
    }
    if(!useBrakingRelative && useAcceleratingRelative){ // Wenn br. nicht rel., dann links lokabhängig (kN oder kn/FM), rechts "%" und in der Mitte ""
        unitUniversal = "";
        unitBraking = uniteBrakingAbsDef;
        uniteAccelerating = "%";
    }
}

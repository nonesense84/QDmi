#include "targetdistance.h"

targetDistance::targetDistance(QWidget *parent) : QWidget(parent){
    fontFactor = 96 / QApplication::screens().at(0)->logicalDotsPerInch();
    connect(attenuationTimer, SIGNAL(timeout()),this,SLOT(attenuationRoutine()));
    attenuationTimer->setInterval(80);
}

void targetDistance::setTargetDistance(quint16 distance, bool barVisible, bool digitalVisible, bool fromEtcs){
    targetDistanceFromEtcs      = fromEtcs;
    targetDistanceDest          = static_cast<quint16>(roundf((static_cast<float>(distance) / 10)) * 10);
    targetDistanceIs            = targetDistanceDest;
    targetDistanceBarVisible    = barVisible;
    targetDistanceDigitalVisible= digitalVisible;
    checkStyleType();
    if(!barVisible){
        targetDistanceDest = 0;
        targetDistanceGraph = 0;
    }
    if(distance > distanceScale){
        targetDistanceGraph = distanceScale;
        attenuationTimer->stop();
    }
    else{
        attenuationTimer->start();
    }
    update();
}

void targetDistance::attenuationRoutine(){
    bool updateNedded = false;
    qreal attenuation = 0.2;
    if(targetDistanceGraph < targetDistanceDest){
        targetDistanceGraph =targetDistanceDest;
        updateNedded = true;
    }
    if(targetDistanceGraph > targetDistanceDest){
        targetDistanceGraph = targetDistanceGraph - (targetDistanceGraph - targetDistanceDest) * attenuation;
        if((targetDistanceGraph - targetDistanceDest) < 0.25){
            targetDistanceGraph = targetDistanceDest;
            attenuationTimer->stop();
        }
        updateNedded = true;
    }
    if(updateNedded)update();
}

void targetDistance::setEraUse(bool useEra){
    useEraStyle = useEra;
    checkStyleType();
    update();
}

void targetDistance::paintEvent(QPaintEvent *){
    int borderThickness = qApp->property("borderThickness").toInt();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    // We fill the background with the predefined dark blue color
    painter.fillRect(rect(), era::darkBlue);
    paintFrame(&painter, era::shadow, era::black, borderThickness);
    if(targetDistanceBarVisible){
        svgActive.load(fileForDistanceScale);
        svgActive.render(&painter,rect());
    }
    painter.scale(static_cast<qreal>(width()) / 58.0, static_cast<qreal>(height()) / 221.0);

    QRect digitalDistPosition(8,5,48,18);
    if(useEraStyle || targetDistanceFromEtcs){
        painter.setBrush(era::grey);
        painter.setPen(era::grey);
        painter.setFont(QFont("FreeSans",
                                   static_cast<int>(12 * fontFactor),
                                   QFont::Normal,
                                   false));
    }
    else{
        painter.setBrush(era::darkYellow);
        painter.setPen(era::darkYellow);
        painter.setFont(QFont("Segment7",
                                   static_cast<int>(12 * fontFactor),
                                   QFont::Normal,
                                   false));
    }
    QRect textRect = painter.boundingRect(digitalDistPosition,Qt::AlignRight,QString::number(targetDistanceIs));
    digitalDistPosition.setLeft(45 - textRect.width());
    //quint16 targetDistanceAnalog = targetDistance;
    qreal tem = 0.0;
    if(useEraStyle || targetDistanceFromEtcs){
        bool digiVisib = targetDistanceDigitalVisible;
        if(targetDistanceFromEtcs && !targetDistanceBarVisible && targetDistanceDigitalVisible) // Assumption, we are in mode OS
            digiVisib = displayDigitalInOs;
        if( targetDistanceFromEtcs && digiVisib)                painter.drawText(digitalDistPosition,QString::number(targetDistanceIs));
        if(!targetDistanceFromEtcs && targetDistanceBarVisible) painter.drawText(digitalDistPosition,QString::number(targetDistanceIs));
        if(targetDistanceBarVisible){
            if(targetDistanceGraph < 1000)
                tem = -2.50582750582749E-10 * qPow(targetDistanceGraph,4)
                      +7.07459207459205E-07 * qPow(targetDistanceGraph,3)
                      -0.000815792540793 * qPow(targetDistanceGraph,2)
                      +0.545314685314686 * targetDistanceGraph
                      +0.300699300699198;
            else
                tem = 187;
            QRectF distBar(30,217,10,-tem);
            painter.drawRect(distBar);
        }
    }
    else{

        quint16 distRound = static_cast<quint16>(roundf((static_cast<float>(targetDistanceIs)/ 200)) * 200);
        if(targetDistanceDigitalVisible)painter.drawText(digitalDistPosition,QString::number(distRound));
        if(targetDistanceBarVisible){
            if(targetDistanceIs <= 100)
                tem = 0.19135135136 * targetDistanceGraph + 1.58882185807825E-15;
            if(targetDistanceGraph >  100 && targetDistanceGraph <= 1000)
                tem = 0.09567567568 * targetDistanceGraph + 9.56756756800004;
            if(targetDistanceGraph >  1000)
                tem = 0.02391891892 * targetDistanceGraph + 81.3243243279999;
            QRectF distBar(42,213,10,-tem);
            painter.drawRect(distBar);
        }
    }
}

void targetDistance::paintFrame(QPainter *framePainter, QColor bright, QColor dark,  int bt){
    // We draw the 3D sunken border
    int w = width();
    int h = height();
    QPoint brightBorder[6] = {
        QPoint(0, h),                           // Bottom left corner
        QPoint(w, h),                           // Bottom right corner
        QPoint(w, 0),                           // Top right corner
        QPoint(w - bt, bt),                     // Shifted top right corner
        QPoint(w - bt, h - bt),                 // Shifted bottom right corner
        QPoint(bt, h - bt)                      // Shifted bottom left corner
    };
    framePainter->setPen(Qt::NoPen);            // Set no outlin
    framePainter->setBrush(bright);             // Apply the shadow color
    framePainter->drawPolygon(brightBorder, 6); // Draw the original polygon
    QPoint brightBorderMirrored[6];             // Create a mirrored version of the polygon
    for (int i = 0; i < 6; i++) {               // (diagonal reflection)
        brightBorderMirrored[i] = QPoint(w - brightBorder[i].x(), h - brightBorder[i].y());
    }
    framePainter->setBrush(dark);               // Set the brush to black for the mirrored polygon
    framePainter->drawPolygon(brightBorderMirrored, 6);   // Draw the mirrored polygon
}
void targetDistance::checkStyleType(){
    if(useEraStyle || targetDistanceFromEtcs){
        fileForDistanceScale = ":/icons/targetDistEra1000m.svg";
        distanceScale = 1000;
    }
    else{
        fileForDistanceScale = ":/icons/targetDistDb4000m.svg";
        distanceScale = 4000;
    }
}
void targetDistance::mousePressEvent(QMouseEvent *event){
    setDigitalInOs(!displayDigitalInOs);
    emit activatedDigitalInOs(displayDigitalInOs);
    (void)event;
}

void targetDistance::setDigitalInOs(bool active){
    displayDigitalInOs = active;
    update();
}

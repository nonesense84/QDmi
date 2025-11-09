#include "timetoindication.h"

timetoindication::timetoindication(QWidget *parent) : QWidget(parent){
    connect(attenuationTimer, &QTimer::timeout,this,&timetoindication::attenuationRoutine);
    attenuationTimer->setInterval(80);
    attenuationTimer->start();
}

void timetoindication::paintEvent(QPaintEvent *){
    int borderThickness = qApp->property("borderThickness").toInt();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    // We fill the background with the predefined color
    if(ttiActive){
        painter.fillRect(rect(), era::darkGrey);
        painter.setBrush(era::grey);
        painter.setPen(Qt::NoPen);
        float widgetSize = qMin(width(), height());
        float rectSize = static_cast<float>(widgetSize * squareSmooth);
        float x = (width()  - rectSize) / 2;
        float y = (height() - rectSize) / 2;
        QRectF ttiCube(x, y, rectSize, rectSize);
        painter.drawRect(ttiCube);
    }
    else{
        painter.fillRect(rect(), era::darkBlue);
    }
    paintFrame(&painter, era::shadow, era::black, borderThickness);
}

void timetoindication::setTti(float tti, bool visible){
    /*
     * ZUSI-ETCS has to emit to us the time to indication [s].
     * German translation of TTI: "Zeit bis zum Bremseinsatzpunkt".
     * This time must be displayed as a square that grows when the time becomes more critical.
     * On top, we expect a flag that can disable the visualization of the TTI square.
     * The calculated square size will then smoothly adapt its size using a smoothing timer.
     */
    float lowScalBorder  = 0.09259f;        // Req. 8.2.2.5.3  5 *  5 cells at n = 1 (Related to total size of 54 * 54 cells)
    float highScalBorder = 0.92592f;        // Req. 8.2.2.5.3 50 * 50 cells at n =10 (Related to total size of 54 * 54 cells)
    quint8 T_driver = 4;                    // SUBSET-026-3 v360, A.3.1  List of Fixed Value Data: T_driver = 4s
    quint8 T_indication = 5 + T_driver;     // SUBSET-026-3 v360, 3.13.9.3.6.4 (3.13.9.3.6.2 not feasible because ZUSI does not deliver T_bs)
    if(tti > T_indication) visible = false; // Don't show TTI if TTI is greater than the TTI threshold
    if(ttiActive != visible){               // If TTI switches off, we have to update the painter once...
        ttiActive = visible;                // ...because the smoothing timer would not do that anymore.
        squareSmooth = lowScalBorder;       // Furthermore set square to its minimum size
        update();
    }
    if(!ttiActive) return;                  // We don't have to calculate the square size if TTI is disabled
    if(!attenuationTimer->isActive())       // Start smoothing timer if TTI gets switched on
        attenuationTimer->start();
    for(quint8 n=1; n<=10; n++){            // Req. 8.2.2.5.3
        float A = T_indication*((10.0f-n)/10.0f);
        float B = T_indication*((10.0f-(n-1)))/10.0f;
        if(A <= tti && tti < B){
            // Map the result (n = 1...10) to the actual square size using the above-defined max and min factor of total widget size
            squareDest = mapClamped(n, 1, 10, lowScalBorder, highScalBorder);
            break;
        }
    }
}
void timetoindication::attenuationRoutine(){
    constexpr float attenuation = 0.5f;
    constexpr float threshold = 0.0025f;

    float delta = squareDest - squareSmooth;
    if (qAbs(delta) < threshold) {
        squareSmooth = squareDest;
        attenuationTimer->stop();
    } else {
        squareSmooth += delta * attenuation;
        update();
    }
}

void timetoindication::paintFrame(QPainter *framePainter, QColor bright, QColor dark,  int bt){
    // We draw the 3D sunken border
    int w = width();
    int h = height();
    QPoint brightBorder[6] = {
        QPoint(0, h),                           // Bottom left corner
        QPoint(w, h),                           // Bottom right corner
        QPoint(w, 0),                           // Top right corner
        QPoint(w - bt, bt),                     // Shifted top right corner
        QPoint(w - bt, h - bt),                 //Shifted bottom right corner
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


float timetoindication::mapClamped(float x, float in_min, float in_max, float out_min, float out_max){
    // Clamp-Eingangswert auf in_min ... in_max
    x = qBound(in_min, x, in_max);

    // Lineare Skalierung
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

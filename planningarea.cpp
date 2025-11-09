#include "planningarea.h"

planningArea::planningArea(QWidget *parent) : QWidget(parent) {


}


void planningArea::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    int borderThickness = qApp->property("borderThickness").toInt();
    paintFrame(&painter, era::shadow, era::black, borderThickness);
    painter.setPen(era::grey);
    painter.drawText(rect(), Qt::AlignCenter, "Planning ara");
}

void planningArea::paintFrame(QPainter *framePainter, QColor bright, QColor dark,  int bt){
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

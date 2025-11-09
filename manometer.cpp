#include "manometer.h"

manometer::manometer(QWidget *parent) : QWidget(parent){
    setPressMaxDial(12);
    setPointer1color(Qt::red);
    setPointer2color(Qt::yellow);
    setPressure1(0);
    setPressure2(0);
}

void manometer::setDpi(qreal dpi){
    fontSizeDial = static_cast<int>(26.0 * 96 / dpi);
    fontSiceLabel = static_cast<int>(37.0 * 96 / dpi);
}
void manometer::setPressMaxDial(quint8 pressure){
    quint8 newPressMaxDial = 0;
    if (pressure < 11) newPressMaxDial = 10;
    if (pressure < 7)  newPressMaxDial = 6;
    if (pressure >= 11)newPressMaxDial = 12;
    if(pressMaxDial != newPressMaxDial){
        numNumbers = newPressMaxDial + 1;
        numShortLines = (newPressMaxDial * 5) + 1;
        pressMaxDial = newPressMaxDial;
        scaleDegStep = arcOpenTotal / (numNumbers - 1);
        scaleDegStepSmall = arcOpenTotal / (numShortLines - 1);
        if(pressMaxDial == 12){
            switchAngle = 174;
            switchPressure = 6;
            degOffset = 1;
        }
        else{
            degOffset = 0;
            switchAngle = arcOpenTotal / 2;
            switchPressure = pressMaxDial / 2;
        }
        update();
    }
}
void manometer::setPointer2using(bool use){
    if(pointer2used != use){
        pointer2used = use;
        update();
    }
}
void manometer::setPointer1color(QColor color){
    if(colorPointer1 != color){
        colorPointer1 = color;
        update();
    }
}
void manometer::setPointer2color(QColor color){
    if(colorPointer2 != color){
        colorPointer2 = color;
        update();
    }
}
void manometer::setPonter1Label(QString text){
    if(textLabel1 != text){
        textLabel1 = text;
        update();
    }
}
void manometer::setPonter2Label(QString text){
    if(textLabel2 != text){
        textLabel2 = text;
        update();
    }
}
void manometer::setPressure1(quint16 pressure) {
    qreal p = static_cast<qreal>(pressure) / 10;
    if(pressure1 != p){
        pressure1 = p;
        qreal hMp =   pressMaxDial / 2.0;    // half max pressure
        if(p < hMp){
            posNeedle1 = p / hMp * switchAngle;
        }
        else{
            qreal ra = arcOpenTotal - switchAngle;  // Rest angle
            qreal rp = p - switchPressure;  // Rest pressure
            posNeedle1 = switchAngle + rp / hMp * ra;
        }
        update();
    }
}
void manometer::setPressure2(quint16 pressure){
    qreal p = static_cast<qreal>(pressure) / 10;
    if(pressure2 != p){
        pressure2 = p;
        qreal hMp =   pressMaxDial / 2.0;    // half max pressure
        if(p < hMp){
            posNeedle2 = p / hMp * switchAngle;
        }
        else{
            qreal ra = arcOpenTotal - switchAngle;  // Rest angle
            qreal rp = p - switchPressure;  // Rest pressure
            posNeedle2 = switchAngle + rp / hMp * ra;
        }
        update();
    }
}
void manometer::setBackgroundColor(QColor color){
    if(colorBackground != color){
        colorBackground = color;
        update();
    }
}
void manometer::paintEvent(QPaintEvent *)
{
    quint8 diamNeedleHub = 56;
    quint8 diamNeedleHubInner = 40;
    static const QPoint airPointer[95] = {
        QPoint(	 362,	1	),
        QPoint(	-0	,	10	),
        QPoint(	-23	,	11	),
        QPoint(	-28	,	12	),
        QPoint(	-39	,	14	),
        QPoint(	-44	,	15	),
        QPoint(	-48	,	16	),
        QPoint(	-53	,	17	),
        QPoint(	-57	,	18	),
        QPoint(	-60	,	19	),
        QPoint(	-63	,	20	),
        QPoint(	-67	,	21	),
        QPoint(	-70	,	22	),
        QPoint(	-73	,	23	),
        QPoint(	-76	,	24	),
        QPoint(	-79	,	25	),
        QPoint(	-81	,	26	),
        QPoint(	-84	,	27	),
        QPoint(	-86	,	28	),
        QPoint(	-89	,	29	),
        QPoint(	-91	,	30	),
        QPoint(	-96	,	32	),
        QPoint(	-98	,	33	),
        QPoint(	-100,	34	),
        QPoint(	-102,	35	),
        QPoint(	-104,	36	),
        QPoint(	-106,	37	),
        QPoint(	-108,	38	),
        QPoint(	-110,	39	),
        QPoint(	-112,	40	),
        QPoint(	-115,	41	),
        QPoint(	-117,	42	),
        QPoint(	-119,	43	),
        QPoint(	-121,	44	),
        QPoint(	-123,	45	),
        QPoint(	-125,	46	),
        QPoint(	-128,	46	),
        QPoint(	-131,	45	),
        QPoint(	-133,	44	),
        QPoint(	-135,	43	),
        QPoint(	-137,	41	),
        QPoint(	-139,	38	),
        QPoint(	-140,	36	),
        QPoint(	-141,	33	),
        QPoint(	-142,	30	),
        QPoint(	-143,	25	),
        QPoint(	-144,	17	),
        QPoint(	-145,	0	),
        QPoint(	-144,	-17	),
        QPoint(	-143,	-25	),
        QPoint(	-142,	-30	),
        QPoint(	-141,	-33	),
        QPoint(	-140,	-36	),
        QPoint(	-139,	-38	),
        QPoint(	-137,	-41	),
        QPoint(	-135,	-43	),
        QPoint(	-133,	-44	),
        QPoint(	-131,	-45	),
        QPoint(	-128,	-46	),
        QPoint(	-125,	-46	),
        QPoint(	-123,	-45	),
        QPoint(	-121,	-44	),
        QPoint(	-119,	-43	),
        QPoint(	-117,	-42	),
        QPoint(	-115,	-41	),
        QPoint(	-112,	-40	),
        QPoint(	-110,	-39	),
        QPoint(	-108,	-38	),
        QPoint(	-106,	-37	),
        QPoint(	-104,	-36	),
        QPoint(	-102,	-35	),
        QPoint(	-100,	-34	),
        QPoint(	-98	,	-33	),
        QPoint(	-96	,	-32	),
        QPoint(	-91	,	-30	),
        QPoint(	-89	,	-29	),
        QPoint(	-86	,	-28	),
        QPoint(	-84	,	-27	),
        QPoint(	-81	,	-26	),
        QPoint(	-79	,	-25	),
        QPoint(	-76	,	-24	),
        QPoint(	-73	,	-23	),
        QPoint(	-70	,	-22	),
        QPoint(	-67	,	-21	),
        QPoint(	-63	,	-20	),
        QPoint(	-60	,	-19	),
        QPoint(	-57	,	-18	),
        QPoint(	-53	,	-17	),
        QPoint(	-48	,	-16	),
        QPoint(	-44	,	-15	),
        QPoint(	-39	,	-14	),
        QPoint(	-28	,	-12	),
        QPoint(	-23	,	-11	),
        QPoint(	-0	,	-10	),
        QPoint(  362,	-1	)

    };
    qreal side = qMin(width(), height());
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width() / 2, height() / 2);
    quint16 normFactor = 548;
    qreal dimension = side / normFactor;
    painter.scale(dimension, dimension);
    static QRectF labelUnit( -normFactor / 2 , normFactor / 3.4, normFactor, fontSizeDial * 1.5);
    static QRectF labelRect1(-normFactor / 2 , normFactor / 20, normFactor, fontSizeDial * 1.5);
    static QRectF labelRect2(-normFactor / 2 , normFactor / 8, normFactor, fontSizeDial * 1.5);

    painter.save();
    // Draw long lines ===============================================
    painter.setPen(QPen(Qt::white, 5));
    painter.rotate(startPosZero);
    temp = 0;
    for (int i = 0; i < numNumbers; ++i) {
        painter.drawLine(outEndLines - lenLongLines, 0, outEndLines, 0);
        if(i < (numNumbers / 2)){
            painter.rotate(scaleDegStep+degOffset*5);
            temp = temp + scaleDegStep+degOffset*5;
            //qDebug() << "groß " <<  temp;
        }
        else{
            painter.rotate(scaleDegStep-degOffset*5);
            temp = temp + scaleDegStep-degOffset*5;
            //qDebug() << "klein " <<  temp;
        }
    }
    painter.rotate(scaleDegStep * numNumbers * -1);
    painter.restore();

    // Draw short lines ===============================================
    painter.save();
    painter.rotate(startPosZero);
    painter.setPen(QPen(Qt::white, 3));
    for (int j = 0; j < numShortLines; ++j) {
        painter.drawLine(outEndLines - lenShortLines, 0, outEndLines, 0);
        if(j < (numShortLines / 2)){
            painter.rotate(scaleDegStepSmall+degOffset);
        }
        else{
            painter.rotate(scaleDegStepSmall-degOffset);
        }
    }
    painter.rotate(scaleDegStepSmall * numShortLines * -1 - startPosZero);
    painter.restore();
    // Draw Numbers ===============================================
    painter.setPen(Qt::white);
    painter.setFont(QFont("FreeSans",fontSizeDial,QFont::Bold,false));
  /*for (int i = 0; i < numNumbers; ++i) {
        if(pressMaxDial == 6)
            painter.drawText(numberPositions6[i],QString::number(i));
        if(pressMaxDial == 10)
            painter.drawText(numberPositions10[i],QString::number(i));
        if(pressMaxDial == 12)
            painter.drawText(numberPositions12[i],QString::number(i));
    }*/
    painter.save();
    qreal numberRadius = outEndLines - lenLongLines - 20; // Distance of numbers to long lines
    qreal angle = startPosZero;
    for (quint8 i = 0; i < numNumbers; ++i) {
        QString label = QString::number(i);
        quint8 wRect = 30;
        if (label.length() >= 2){
           // In case of two digits:
           // Distance of numbers to long lines bus be bigger and text rect must be wider
            numberRadius = outEndLines - lenLongLines - 30;
            wRect = 40;
        }
        qreal angleRad = qDegreesToRadians(angle);
        qreal x = numberRadius * qCos(angleRad);
        qreal y = numberRadius * qSin(angleRad);
        QRectF textRect(-wRect/2, -15, wRect, 30);
        painter.save();
        painter.translate(x, y);
        painter.drawText(textRect, Qt::AlignCenter, label);
        painter.restore();
        if (i < (numNumbers / 2)) {
            angle += (scaleDegStep + degOffset*5);
        } else {
            angle += (scaleDegStep - degOffset*5);
        }
    }
    painter.restore();

    // Draw unit texts ============================================
    painter.setFont(QFont("FreeSans",fontSizeDial,QFont::Normal,false));
    painter.drawText(labelUnit,Qt::AlignCenter, "bar");
    painter.setPen(colorPointer1);
    painter.drawText(labelRect1,Qt::AlignCenter, textLabel1);
    if(pointer2used){
        painter.setPen(colorPointer2);
        painter.drawText(labelRect2,Qt::AlignCenter, textLabel2);
    }
    // Draw needles ===============================================
    painter.rotate(startPosZero);
    painter.scale(0.69, 0.69);
    painter.setPen(Qt::NoPen);
    // Draw needle 2 ==============================================
    if(pointer2used){
        painter.save();
        painter.setBrush(colorPointer2);
        painter.rotate(static_cast<qreal>(posNeedle2));
        painter.drawConvexPolygon(airPointer, 95);
        painter.restore();
    }
    // Draw needle 1 ==============================================
    painter.setBrush(colorPointer1);
    painter.rotate(static_cast<qreal>(posNeedle1));
    painter.drawConvexPolygon(airPointer, 95);
    // Draw needle hub ============================================
    painter.drawEllipse(-diamNeedleHub/2,-diamNeedleHub/2,diamNeedleHub,diamNeedleHub);
    painter.setPen(colorPointer1.darker());
    painter.drawEllipse(-diamNeedleHubInner/2,-diamNeedleHubInner/2,diamNeedleHubInner,diamNeedleHubInner);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::darkGray);
    painter.drawEllipse(-diamNeedleHub/10,-diamNeedleHub/10,diamNeedleHub/5,diamNeedleHub/5);
}


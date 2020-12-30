#include "manometer.h"

manometer::manometer(QWidget *parent) : QWidget(parent){
    setPressMaxDial(12);
    setPointer1color(Qt::red);
    setPointer2color(Qt::yellow);
    setPressure1(0);
    setPressure2(0);
}

void manometer::setDpi(qreal dpi){
    fontSiceDial = static_cast<int>(26.0 * 96 / dpi);
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
        scaleDegStep = arcOpenTotal / (numNumbers - 1);
        scaleDegStepSmall = arcOpenTotal / (numShortLines - 1);
        pressMaxDial = newPressMaxDial;
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
void manometer::setPressure1(quint16 pressure){
    if(pressure1 != pressure){
        pressure1 = pressure;
        posNeedle1 = pressure / static_cast<float> (pressMaxDial * 10)
                   * static_cast<float> (arcOpenTotal);
        update();
    }
}
void manometer::setPressure2(quint16 pressure){
    if(pressure2 != pressure){
        pressure2 = pressure;
        posNeedle2 = pressure / static_cast<float> (pressMaxDial * 10)
                   * static_cast<float> (arcOpenTotal);
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
    static const QPoint numberPositions6[7] = {
        QPoint( -112, 154),   //0
        QPoint( -189, 31),    //1
        QPoint( -147, -104),  //2
        QPoint( -9, -166),    //3
        QPoint( 121, -107),   //4
        QPoint( 165, 29),     //5
        QPoint( 93, 149)      //6
    };
    static const QPoint numberPositions10[11] = {
        QPoint( -112, 154),   //0
        QPoint( -165, 85),    //1
        QPoint( -182, -2),    //2
        QPoint( -153, -87),   //3
        QPoint( -85, -144),   //4
        QPoint( -10, -162),   //5
        QPoint( 72, -144),    //6
        QPoint( 139, -87),    //7
        QPoint( 164, -2),     //8
        QPoint( 150, 85),     //9
        QPoint( 81, 147)      //10
    };
    static const QPoint numberPositions12[13] = {
        QPoint( -112, 154),   //0
        QPoint( -165, 99),    //1
        QPoint( -189, 31),    //2
        QPoint( -178, -40),   //3
        QPoint( -147, -104),  //4
        QPoint( -86, -144),   //5
        QPoint( -10, -166),   //6
        QPoint( 60, -150),    //7
        QPoint( 122, -108),   //8
        QPoint( 154, -44),    //9
        QPoint( 145, 25),     //10
        QPoint( 128, 93),     //11
        QPoint( 81, 147)      //12
    };
    qreal side = qMin(width(), height());
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width() / 2, height() / 2);
    quint16 normFactor = 548;
    qreal dimension = side / normFactor;
    painter.scale(dimension, dimension);
    static QRectF labelUnit( -normFactor / 2 , normFactor / 3.4, normFactor, fontSiceDial * 1.5);
    static QRectF labelRect1(-normFactor / 2 , normFactor / 20, normFactor, fontSiceDial * 1.5);
    static QRectF labelRect2(-normFactor / 2 , normFactor / 8, normFactor, fontSiceDial * 1.5);
    // Draw long lines ===============================================
    painter.save();
    painter.setPen(QPen(Qt::white, 5));
    painter.rotate(startPosZero);
    for (int i = 0; i < numNumbers; ++i) {
        painter.drawLine(outEndLines - lenLongLines, 0, outEndLines, 0);
        painter.rotate(scaleDegStep);
    }
    painter.rotate(scaleDegStep * numNumbers * -1);
    // Draw short lines ===============================================
    painter.setPen(QPen(Qt::white, 3));
    for (int j = 0; j < numShortLines; ++j) {
        painter.drawLine(outEndLines - lenShortLines, 0, outEndLines, 0);
        painter.rotate(scaleDegStepSmall);
    }
    painter.rotate(scaleDegStepSmall * numShortLines * -1 - startPosZero);
    // Draw Numbers ===============================================
    painter.setPen(Qt::white);
    painter.setFont(QFont("FreeSans",fontSiceDial,QFont::Bold,false));
    for (int i = 0; i < numNumbers; ++i) {
        if(pressMaxDial == 6)
            painter.drawText(numberPositions6[i],QString::number(i));
        if(pressMaxDial == 10)
            painter.drawText(numberPositions10[i],QString::number(i));
        if(pressMaxDial == 12)
            painter.drawText(numberPositions12[i],QString::number(i));
    }
    // Draw unit texts ============================================
    painter.setFont(QFont("FreeSans",fontSiceDial,QFont::Normal,false));
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
        painter.setBrush(colorPointer2);
        painter.rotate(static_cast<qreal>(posNeedle2));
        painter.drawConvexPolygon(airPointer, 95);
        painter.rotate(static_cast<qreal>(-posNeedle2));
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

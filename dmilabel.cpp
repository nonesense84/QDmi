#include "dmilabel.h"
#include <QDebug>
#include <QtSvg>

dmiLabel::dmiLabel(QWidget *parent) : QWidget(parent){
    fontFactor = 96 / QApplication::screens().at(0)->logicalDotsPerInch();
    setTargetDistance(100,false);
    QFontDatabase::addApplicationFont(":/fonts/FreeSans.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Segment7Standard.otf");
}

void dmiLabel::setDpi(qreal dpi){
}

void dmiLabel::mousePressEvent(QMouseEvent *event){
    if(isEnab){
        isPushed = true;
        update();
    }
}

void dmiLabel::mouseReleaseEvent(QMouseEvent *event){
    isPushed = false;
    if(isEnab){
        emit clicked(true);
        if(isButton){
            QMediaPlayer *click =new QMediaPlayer;          // workaround: QSound::play crash on some android devices
            click->setMedia(QUrl("qrc:/sounds/click.wav"));
            click->setVolume(50);
            click->play();
        }
        update();
        emit txtBtnClicked(emitText);
    }
    emit dataEntryBtnClicked(labelText, isEnab);
}

void dmiLabel::setAsButton(QString text){
    setAsButton(true, text, text);
}

void dmiLabel::setAsButton(bool enabled, QString text){
    setAsButton(enabled, text, text);
}

void dmiLabel::setAsButton(bool enabled, QString text, QString textToEmit){
    bool needUpdate = false;
    if(isEnab != enabled || labelText != text)needUpdate = true;
    isButton = true;
    isEnab = enabled;
    labelText = text;
    emitText = textToEmit;
    textStyle = QFont::Light;
    if(needUpdate)update();
}

void dmiLabel::setAsDataEntryLabel(QString text, bool isInputfield, bool isEnabled, bool isValid, bool isApplicable, bool fullGrid){
    if(!isApplicable){
        labelText = "";
        bgColor = era::darkBlue;
        borderColorTopAndLeft = era::darkBlue;
        borderColorButtomAndRight = era::darkBlue;
        isDataEntryButton = false;
        isButton = true;
        update();
        return;
    }
    textStyle = QFont::Light;
    isEnab = isEnabled;
    labelText = text;
    emitText = text;
    isDataEntryButton = true;
    if(isInputfield){
        if(!hasCustomAlignment)alignment = Qt::AlignLeft;
        isButton = true;
        setUnclosedFrame(!fullGrid && !isEnabled,false,false,false);
        if(isEnabled){
            bgColor = era::mediumGrey;
            labelTextColorEnab = era::black;
            borderColorTopAndLeft = era::darkGrey;
            borderColorButtomAndRight = era::white;
        }
        else{
            bgColor = era::darkGrey;
            borderColorTopAndLeft = era::mediumGrey;
            borderColorButtomAndRight = era::mediumGrey;
            if(isValid){
                labelTextColorDisab = era::white;
            }
            else{
                labelTextColorDisab = era::grey;
            }
        }

    }
    else{
        if(!hasCustomAlignment)alignment = Qt::AlignRight;
        setUnclosedFrame(false,true,false,false);
        bgColor = era::darkGrey;
        labelTextColorEnab  = era::grey;
        labelTextColorDisab = era::grey;
        borderColorTopAndLeft     = era::mediumGrey;
        borderColorButtomAndRight = era::mediumGrey;
    }
    update();
}

void dmiLabel::setTargetDistance(quint16 distance, bool visible){
    targetDistance = distance;
    targetDistanceVisible = visible;
    update();
}

void dmiLabel::setIsDistanceScale(){
    isTargetDistance = true;
    if(useEraStyle){
        fileForDistanceScale = ":/icons/targetDistEra1000m.svg";
        distanceScale = 1000;
    }
    else{
        fileForDistanceScale = ":/icons/targetDistDb4000m.svg";
        distanceScale = 4000;
    }
}

void dmiLabel::setEraUse(bool useEra){
    useEraStyle = useEra;
    if(isTargetDistance) setIsDistanceScale();
    updateLabel();
}

void dmiLabel::updateBlinking(){
    blinkerFast = !blinkerFast;
    if(blinkerFast) blinkerSlow = !blinkerSlow;
    if((blinkFrequency != 0 && fileNameIsSet)){
        update();
    }
}

void dmiLabel::updateLabel(){
    if(blinkFrequency == 0){
        update();
    }
}

void dmiLabel::setBorderThickness(int thickness){
    borderThickness = thickness;
}

void dmiLabel::setVisib(bool visible){
    isVisible = visible;
    updateLabel();
}

void dmiLabel::setWorking(bool enabled, quint8 blinkingFreq, bool inverse){
    if((enabled != isEnab) || (blinkingFreq != blinkFrequency) || (inverse != isInvert)){
        isEnab = enabled;
        blinkFrequency = blinkingFreq;
        isInvert = inverse;
        updateLabel();
    }
}

void dmiLabel::setIcon(QString filename){
    if(filenameIconActive != filename){
        if(mimeFile.suffixForFileName(filename) == "svg"){
            svgActive.load(filename);
            fileNameIsSet = filename != "";
        }
        filenameIconActive = filename;
        updateLabel();
    }
}

void dmiLabel::setIcon(QString filenameActive, QString filenameInactive){
    if((filenameIconActive != filenameActive)||(filenameIconInactive != filenameInactive)){
        svgActive.load(filenameActive);
        svgInactive.load(filenameInactive);
        fileNameIsSet = filenameActive != "";
        filenameIconActive = filenameActive;
        filenameIconInactive = filenameInactive;
        updateLabel();
    }
}

void dmiLabel::setCustomFontFactor(qreal factor){
    customFontFactor = factor;
}

void dmiLabel::setCustomFontFactor(qreal factor, Qt::Alignment customAlignment){
    customFontFactor = factor;
    hasCustomAlignment = true;
    alignment = static_cast<int>(customAlignment);
}

void dmiLabel::setText(QString text){
    if(labelText != text){
        labelText = text;
        update();
    }
}

void dmiLabel::setText(quint16 value){
    QString text = QString::number(value);
    if(labelText != text){
        labelText = text;
        update();
    }
}

void dmiLabel::setText(QString text, QColor textColorEnabled, QColor textColorDisabled, quint8 boldOrThin){
    if((labelText != text)||(labelTextColorEnab != textColorEnabled)||(labelTextColorDisab != textColorDisabled)||(textStyle != boldOrThin)){
        textStyle = boldOrThin; //Normal (Not bold)
        labelText = text;
        labelTextColorEnab = textColorEnabled;
        labelTextColorDisab = textColorDisabled;
        update();
    }
}

void dmiLabel::setText(QString text, QColor textColor, QColor backgroundColor){
    if((labelText != text)||(labelTextColorEnab != textColor)||(bgColor != backgroundColor)){
        labelText = text;
        labelTextColorEnab = textColor;
        bgColor = backgroundColor;
        update();
    }
}

void dmiLabel::setUnclosedFrame(bool openL, bool openR, bool openU, bool openD){
    borderLClosed = !openL;
    borderRClosed = !openR;
    borderTClosed = !openU;
    borderBClosed = !openD;
}

void dmiLabel::setTextFieldUsing(quint8 numFields){
    isTextField = true;
    numTextFields = numFields;
    if(!hasCustomAlignment)alignment = Qt::AlignLeft;
}

void dmiLabel::setTextFieldUsing(quint8 numFields, quint8 customAlignment){
    isTextField = true;
    numTextFields = numFields;
    hasCustomAlignment = true;
    alignment = customAlignment;
}

void dmiLabel::setSegmentDigitToUse(quint8 position){
    segmentPosition = position;
}

void dmiLabel::setSegmentText(quint16 value, bool textVisible){
    if(isSegment != textVisible || segmentValue != value){
        segmentValue = value;
        isSegment = textVisible;
        segmentText = QString::number(value);
        segmentText = segmentText.mid(segmentText.length() - segmentPosition,1);
        updateLabel();
    }
}

void dmiLabel::addTextMessage(QString text, QColor textColor, QColor bgColor, quint8 msgId){
    if(!isTextField)setTextFieldUsing(1);
    for(quint8 i=0; i<10; i++){
        if(messageIds[i] == msgId){
            return; // Message allready in use
        }
    }
    for(quint8 i=0; i<10; i++){
        if(messageIds[i] == 255){
            messageTexts[i] = text;
            messageBackQolors[i] = bgColor;
            messageTextColors[i] = textColor;
            messageIds[i] = msgId;
            update();
            highestTextMessgePosition = i;
          //qDebug() << "highestTextMessgePosition: " + QString::number(highestTextMessgePosition);
            if(i >= numTextFields)emit messaesOutOfView(true);
            return;
        }
    }
}

void dmiLabel::removeTextMessage(quint8 msgId){
    bool needUpdate = false;
    bool foundMessaesOutOfView = false;
    for(quint8 i=0; i<10; i++){
        if(messageIds[i] == msgId){
            messageTexts[i] = "";
            messageBackQolors[i] = era::darkBlue;
            messageTextColors[i] = era::darkBlue;
            messageIds[i] = 255;
            needUpdate  = true;
        }
        if(messageIds[i] != 255 && i > numTextFields){
            foundMessaesOutOfView = true;
            highestTextMessgePosition = i;
            if(textMessageOffset >= highestTextMessgePosition) textMessageOffset = highestTextMessgePosition - numTextFields + 1;
          //qDebug() << "highestTextMessgePosition: " + QString::number(highestTextMessgePosition);
        }
    }
    if(needUpdate)update();
    emit messaesOutOfView(foundMessaesOutOfView);
}

void dmiLabel::shiftTextMessageOffset(qint8 shift){
    textMessageOffset = textMessageOffset + shift;
    if(textMessageOffset < 0) textMessageOffset = 0;
    if(textMessageOffset >= highestTextMessgePosition) textMessageOffset = highestTextMessgePosition - numTextFields + 1;
    update();
  //qDebug() << "textMessageOffset: " + QString::number(textMessageOffset);
}

void dmiLabel::paintEvent(QPaintEvent *)
{
    if(isVisible){
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        QRect centralAreaLbl(  borderThickness * borderLClosed,
                               borderThickness * borderTClosed,
                               width() -  borderThickness * (borderRClosed + borderLClosed),
                               height() - borderThickness * (borderBClosed + borderTClosed));
        // First we have to draw the frame of the label.
        // In case of a data entry button, there is no frame arround,
        // so the buttom will be drawn by the frame itself, and without a frame if its pushed.
        // If it is a regular button, we have to draw the button INSIDE the label with swaped worder colors.
        // ...But only, when its not pushed, because the we only need the frame.
        if(isDataEntryButton && isPushed){ // Pushed data enrtry button. Entire label without a frame:
            paintFrame(&painter, centralAreaLbl, bgColor,  bgColor, 0);
        }
        else{ // Regualar frame:
         paintFrame(&painter, centralAreaLbl, borderColorButtomAndRight,  borderColorTopAndLeft, 0);
        }
        if(isButton && !isDataEntryButton){ // Regular button inside a frame, thats allready painted
            QRect centralAreaBtn(2*borderThickness, 2 * borderThickness, width() - 4 * borderThickness, height() - 4 * borderThickness);
            if(!isPushed){ // Regular bushed button inside a frame, thats allready painted
                paintFrame(&painter, centralAreaBtn, borderColorTopAndLeft, borderColorButtomAndRight,  borderThickness);
            }
            paintIcon(&painter, centralAreaBtn);
        }
        if(isTextField){
            paintTextMessages(&painter, centralAreaLbl);
        }
        if(isSegment){
            paintSegment(&painter, centralAreaLbl);
        }
        if(isTargetDistance && targetDistanceVisible){
            paintDistance(&painter, centralAreaLbl);
        }
        if(!isTargetDistance && !isSegment && !isButton){
            paintIcon(&painter, centralAreaLbl);
        }
        if(labelText != ""){
            paintText(&painter, centralAreaLbl);
        }
    }
}

void dmiLabel::paintText(QPainter *iconPainter, QRect centralArea){
    int lineHeight = centralArea.height();
    int lineWidth = centralArea.width();
    QRect field(centralArea.x(),centralArea.y(),lineWidth, lineHeight);
    if(isEnab){
        iconPainter->setPen(labelTextColorEnab);
    }
    else{
        iconPainter->setPen(labelTextColorDisab);
    }
    iconPainter->setFont(QFont("FreeSans",
                               static_cast<int>(fontFactor * customFontFactor * lineHeight),
                               textStyle,
                               false));

    QRectF textRect = iconPainter->boundingRect(field,alignment,labelText);
    if(alignment == Qt::AlignLeft){
        textRect.moveLeft(5*borderThickness);
        textRect.translate(0, field.height()/2 - textRect.height()/2);
    }
    if(alignment == Qt::AlignRight){
        textRect.translate(0, field.height()/2 - textRect.height()/2);
    }
    iconPainter->drawText(textRect,labelText);
}

void dmiLabel::paintDistance(QPainter *iconPainter, QRect centralArea){
    iconPainter->save();
    svgActive.load(fileForDistanceScale);
    svgActive.render(iconPainter,centralArea);
    iconPainter->scale(static_cast<qreal>(width()) / 58.0, static_cast<qreal>(height()) / 221.0);

    QRect digitalDistPosition(8,5,48,18);
    if(useEraStyle){
        iconPainter->setBrush(era::grey);
        iconPainter->setPen(era::grey);
        iconPainter->setFont(QFont("FreeSans",
                                   static_cast<int>(12 * fontFactor),
                                   QFont::Normal,
                                   false));
    }
    else{
        iconPainter->setBrush(era::darkYellow);
        iconPainter->setPen(era::darkYellow);
        iconPainter->setFont(QFont("Segment7",
                                   static_cast<int>(12 * fontFactor),
                                   QFont::Normal,
                                   false));
    }
    QRect textRect = iconPainter->boundingRect(digitalDistPosition,Qt::AlignRight,QString::number(targetDistance));
    digitalDistPosition.setLeft(45 - textRect.width());
    quint16 targetDistanceAnalog = targetDistance;
    if(targetDistance > distanceScale){ targetDistanceAnalog = distanceScale;}
    qreal tem = 0.0;
    if(useEraStyle){
        iconPainter->drawText(digitalDistPosition,QString::number(ceil((targetDistance / 10)) * 10));
        if(targetDistance < 1000)
            tem = -2.50582750582749E-10 * qPow(targetDistance,4)
                  +7.07459207459205E-07 * qPow(targetDistance,3)
                  -0.000815792540793 * qPow(targetDistance,2)
                  +0.545314685314686 * targetDistance
                  +0.300699300699198;
        else
            tem = 187;
        QRect distBar(30,217,10,-(static_cast<int>(tem)));
        iconPainter->drawRect(distBar);
    }
    else{
        if(targetDistance > 4000)iconPainter->drawText(digitalDistPosition,QString::number(ceil((targetDistance + 100)/ 200) * 200));
        if(targetDistance <= 100)
            tem = 0.19135135136 * targetDistanceAnalog + 1.58882185807825E-15;
        if(targetDistanceAnalog >  100 && targetDistanceAnalog <= 1000)
            tem = 0.09567567568 * targetDistanceAnalog + 9.56756756800004;
        if(targetDistanceAnalog >  1000)
            tem = 0.02391891892 * targetDistanceAnalog + 81.3243243279999;
        QRect distBar(42,213,10,-(static_cast<int>(tem)));
        iconPainter->drawRect(distBar);
    }
    iconPainter->restore();
}

void dmiLabel::paintSegment(QPainter *iconPainter, QRect centralArea){
    iconPainter->save();
    iconPainter->setPen(era::darkYellow);
    iconPainter->setFont(QFont("Segment7",
                               static_cast<int>(fontFactor * 0.6 * centralArea.height()),
                               QFont::Normal,
                               false));
    QRect textRect = iconPainter->boundingRect(centralArea,Qt::AlignLeft,segmentText);
    textRect.translate(centralArea.width()/1.5 - textRect.width()/1.5,
                      (centralArea.height()/1.3 - textRect.height()/1.3));
    iconPainter->drawText(textRect,segmentText);
    iconPainter->restore();
}

void dmiLabel::paintTextMessages(QPainter *iconPainter, QRect centralArea){
    int lineHeight = centralArea.height()/numTextFields;
    QRect field(centralArea.x(),centralArea.y(),centralArea.width(), lineHeight);
    iconPainter->save();
    for(quint8 i=0; i < numTextFields; i++){
        if(i == numTextFields - 1){
            field.setHeight(centralArea.height() - i * lineHeight);
        }
        iconPainter->setPen(Qt::NoPen);
        iconPainter->setBrush(messageBackQolors[i + textMessageOffset]);
        iconPainter->drawRect(field);
        iconPainter->setPen(messageTextColors[i + textMessageOffset]);
        iconPainter->setFont(QFont("FreeSans",
                                   static_cast<int>(fontFactor * 0.44 * lineHeight),
                                   QFont::Bold,
                                   false));
        QRect textRect = iconPainter->boundingRect(field,Qt::AlignLeft,messageTexts[i + textMessageOffset]);
        textRect.moveLeft(3*borderThickness);
        if(alignment == Qt::AlignLeft)textRect.translate(0, field.height()/2 - textRect.height()/2);
        if(alignment == Qt::AlignRight)textRect.translate(field.width()-textRect.width(), field.height()/2 - textRect.height()/2);
        if(alignment == Qt::AlignCenter)textRect.translate(field.width()/2-textRect.width()/2, field.height()/2 - textRect.height()/2);
        iconPainter->drawText(textRect,messageTexts[i + textMessageOffset]);
        field.translate(0,lineHeight);
    }
    iconPainter->restore();
}

void dmiLabel::paintIcon(QPainter *iconPainter, QRect centralArea){
    centralArea = calcOptimalRect(centralArea,svgActive.viewBox());
    if(((blinkFrequency == 1) &&  blinkerSlow && !isInvert) ||
       ((blinkFrequency == 1) && !blinkerSlow &&  isInvert) ||
       ((blinkFrequency == 2) &&  blinkerFast && !isInvert) ||
       ((blinkFrequency == 2) && !blinkerFast &&  isInvert) ||
       !isEnab){
       svgInactive.render(iconPainter, centralArea);

    }
    else{
        svgActive.render(iconPainter,centralArea);
    }
}

QRect dmiLabel::calcOptimalRect(QRect destArea, QRect iconArea){
    if(iconArea.width()>0 && isButton){
        qreal relDest = static_cast<qreal>(destArea.width()) / static_cast<qreal>(destArea.height()); // >1 Breiter als hoch. Je größer, desto Breiter.
        qreal relIcon = static_cast<qreal>(iconArea.width()) / static_cast<qreal>(iconArea.height());
        qreal destRatio = static_cast<qreal>(destArea.height()) / static_cast<qreal>(destArea.width());
        if(relDest > relIcon){   // Wenn das Widget breiter als das Icon
            int originWidth = destArea.width();
            destArea.setWidth(static_cast<int>(static_cast<qreal>(destArea.width())*destRatio));
            destArea.moveLeft(originWidth/2 -destArea.width()/2 + destArea.left());
        }
        if(relDest < relIcon){   // Wenn das Widget schmaler als das Icon
            int originHeight = destArea.height();
            destArea.setHeight(static_cast<int>(static_cast<qreal>(destArea.height())/destRatio));
            destArea.moveTop(originHeight/2 -destArea.height()/2 + destArea.top());
        }
    }
    return destArea;
}

void dmiLabel::paintFrame(QPainter *framePainter,QRect centralArea, QColor colorLefUp, QColor colorRightDown, int offset){
    int halfHeight = height()/2;
    int halfWidth = width()/2;
    QRect darkBorder(0 + offset, 0 + offset, width() - 2 * offset, height() - 2 * offset);
    QPoint brightBorder[5];
    brightBorder[0] = QPoint(width() - offset,  0        + offset);
    brightBorder[1] = QPoint(width() - offset,  height() - offset);
    brightBorder[2] = QPoint(0       + offset,  height() - offset);
    if(halfWidth > halfHeight){
        brightBorder[3] = QPoint(0 + halfHeight + offset,  halfHeight - offset);
        brightBorder[4] = QPoint( width() - halfHeight - offset, halfHeight + offset);
    }
     else{
        brightBorder[3] = QPoint(halfWidth + offset, height() - halfWidth - offset);
        brightBorder[4] = QPoint(halfWidth - offset, halfWidth + offset);
    }
    framePainter->setPen(Qt::NoPen);
    framePainter->setBrush(colorLefUp);
    framePainter->drawRect(darkBorder);
    framePainter->setBrush(colorRightDown);
    framePainter->drawPolygon(brightBorder, 5);
    framePainter->setBrush(bgColor);

    framePainter->drawRect(centralArea);
}

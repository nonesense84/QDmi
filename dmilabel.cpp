#include "dmilabel.h"
#include "qapplication.h"
#include <QDebug>
#include <QApplication>

dmiLabel::dmiLabel(QWidget *parent) : QWidget(parent){
    fontFactor = 96 / QApplication::screens().at(0)->logicalDotsPerInch();
    QFontDatabase::addApplicationFont(":/fonts/FreeSans.ttf");
    QFontDatabase::addApplicationFont(":/fonts/Segment7Standard.otf");
}

void dmiLabel::mousePressEvent(QMouseEvent *event){
    if(isEnab){
        isPushed = true;
        update();
    }
    (void)event;
}

void dmiLabel::mouseReleaseEvent(QMouseEvent *event){
    isPushed = false;
    if(isEnab){
        emit clicked();
        if(isButton && !isDataEntryButton){
            click1->stop();
            click1->play();
        }
        update();
        emit txtBtnClicked(emitText);
    }
    emit dataEntryBtnClicked(labelText, isEnab);
    (void)event;
}

void dmiLabel::setAsButton(QString text){
    setAsButton(true, text, text);
}

void dmiLabel::setAsButton(bool enabled, QString text){
    setAsButton(enabled, text, text);
}

void dmiLabel::setAsButton(bool enabled, QString text, QString textToEmit){
    if(click1 == nullptr){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        click1 = new QMediaPlayer(this);
        click1->setMedia(QUrl("qrc:/sounds/click.wav"));
        click1->setVolume(50);
#else
        click1 = new QMediaPlayer(this);
        auto audioOutput = new QAudioOutput(this);
        click1->setAudioOutput(audioOutput);
        audioOutput->setVolume(0.5);
        click1->setSource(QUrl("qrc:/sounds/click.wav"));
#endif
    }
    bool needUpdate = false;
    if(isEnab != enabled || labelText != text)needUpdate = true;
    isButton = true;
    isEnab = enabled;
    labelText = text;
    emitText = textToEmit;
    textStyle = QFont::Light;
    if(needUpdate)update();
}

void dmiLabel::setFrameless(){
    borderThickness = 0;
}
void dmiLabel::setAsDataEntryLabel(QString text, bool isInputfield, bool isEnabled, bool isValid, bool isApplicable){
    setAsDataEntryLabelFg(text, isInputfield, isEnabled, isValid, isApplicable, false);
}

void dmiLabel::setAsDataEntryLabelFg(QString text, bool isInputfield, bool isEnabled, bool isValid, bool isApplicable, bool fullGrid){
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
        setUnclosedFrame(!fullGrid && !isEnabled,false);
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
        setUnclosedFrame(false,true);
        bgColor = era::darkGrey;
        labelTextColorEnab  = era::grey;
        labelTextColorDisab = era::grey;
        borderColorTopAndLeft     = era::mediumGrey;
        borderColorButtomAndRight = era::mediumGrey;
    }
    update();
}

void dmiLabel::updateBlinking(){
    tickCounter++;
    blinkerSuperFast = !blinkerSuperFast;
    if (tickCounter % 2 == 0) blinkerFast = !blinkerFast;
    if (tickCounter % 4 == 0) blinkerSlow = !blinkerSlow;
    if ((blinkFrequency != 0 && fileNameIsSet) || acknowledgeFrameActive)
        update();
}

void dmiLabel::updateLabel(){
    if(blinkFrequency == 0){
        update();
    }
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
void dmiLabel::setEnabled(bool enabled){
    isEnab = enabled;
    updateLabel();
}


void dmiLabel::setIcon(QString filename){
    if(filenameIconActive != filename){
        if(mimeFile.suffixForFileName(filename) == "svg"){
            svgActive.load(filename);
            fileNameIsSet = filename != "";
            filenameIconActive = filename;
            filenameIconInactive = ":/icons/blanc.svg";
            svgInactive.load(filenameIconInactive);
        }
        updateLabel();
    }
}

void dmiLabel::setIcon(QString filenameActive, QString filenameInactive){
    if((filenameIconActive != filenameActive)||(filenameIconInactive != filenameInactive)){
        fileNameIsSet = filenameActive != "";
        if(filenameActive == "")
            filenameIconActive = ":/icons/blanc.svg";
        else
            filenameIconActive = filenameActive;

        if(filenameInactive == "")
            filenameIconInactive = ":/icons/blanc.svg";
        else
            filenameIconInactive = filenameInactive;
        svgActive.load(filenameIconActive);
        svgInactive.load(filenameIconInactive);
        updateLabel();
    }
}

void dmiLabel::setCustomFontFactor(qreal factor){
    customFontFactor = factor;
}

void dmiLabel::setCustomFontFactor(qreal factor, Qt::Alignment customAlignment){
    setCustomFontFactor(factor);
    hasCustomAlignment = true;
    alignment = static_cast<int>(customAlignment);
}

void dmiLabel::setCustomFontFactor(qreal factor, Qt::Alignment customAlignment, QFont::Weight style){
    textStyle = style;
    setCustomFontFactor(factor, customAlignment);
}

void dmiLabel::setText(QString text){
    if(labelText != text){
        labelText = text;
        update();
    }
}

void dmiLabel::setText(quint16 value){
    QString text = QString::number(value);
    if(value == 0) text = "";
    if(labelText != text){
        labelText = text;
        update();
    }
}

void dmiLabel::setText(QString text, QColor textColorEnabled, QColor textColorDisabled, QFont::Weight boldOrThin){
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

void dmiLabel::setUnclosedFrame(bool openL, bool openR){
    borderLOpen = openL;
    borderROpen = openR;
}
void dmiLabel::setAcklowedgeFrame(bool active){
    acknowledgeFrameActive = active;
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

void dmiLabel::setSegmentDigit(quint16 value, bool textVisible, bool fromEtcs){
    if(isSegment != textVisible || segmentValue != value){
        customFontFactor = 0.6;
        segmentValue = value;
        isSegment = textVisible;
        segmentText = QString::number(value);
        segmentText = segmentText.mid(segmentText.length() - segmentPosition,1);
        updateLabel();
    }
    (void)fromEtcs;
}

void dmiLabel::setSegmentText(quint16 value){
    isSegment = true;
    segmentText = QString::number(value);
    update();
}

void dmiLabel::addTextLzbMessage(QString text, QColor textColor, QColor bgColor, quint8 msgId){
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

void dmiLabel::setTextMessageOffset(qint8 offset){
    textMessageOffset = offset;
    if(textMessageOffset < 0) textMessageOffset = 0;
    if(textMessageOffset >= highestTextMessgePosition) textMessageOffset = highestTextMessgePosition - numTextFields + 1;
    update();
}

void dmiLabel::paintEvent(QPaintEvent *)
{
    if(isVisible){
        if(borderThickness > 0) borderThickness = qApp->property("borderThickness").toInt();
        QPainter painter(this);

        painter.setRenderHint(QPainter::Antialiasing);
        // Create a rect, that does not contain the frame
        QRect centralAreaLbl(  borderThickness * !borderLOpen,
                               borderThickness,
                               width() -  borderThickness * (!borderROpen + !borderLOpen),
                               height() - 2 * borderThickness);

        // First we have to draw the frame of the label.
        // In case of a data entry button, there is no frame arround,
        // so the buttom will be drawn by the frame itself, and without a frame if its pushed.
        // If it is a regular button, we have to draw the button INSIDE the label with swaped border colors.
        // ...But only, when its not pushed, because then we only need the frame.
        painter.fillRect(rect(), bgColor);
        if(!(isDataEntryButton && isPushed)){ // Regualar frame:
         paintFrame(&painter, borderColorTopAndLeft, borderColorButtomAndRight, borderThickness, false);
        }
        if(isButton && !isDataEntryButton){ // Regular button inside a frame, thats allready painted
            QRect centralAreaBtn(2*borderThickness, 2 * borderThickness, width() - 4 * borderThickness, height() - 4 * borderThickness);
            if(!isPushed){ // Regular bushed button inside a frame, thats allready painted
                paintFrame(&painter, borderColorButtomAndRight, borderColorTopAndLeft, borderThickness, true);
            }
            paintIcon(&painter, centralAreaBtn);
        }
        else{
            if(fileNameIsSet)paintIcon(&painter, centralAreaLbl);
        }
        if(acknowledgeFrameActive && blinkerSuperFast)      paintFrame(&painter, era::yellow, era::veryDarkYellow, borderThickness,false);
        if(isTextField)                                     paintTextMessages(&painter, centralAreaLbl);
        if(isSegment)                                       paintSegment(&painter, centralAreaLbl);
        if(labelText != "")                                 paintText(&painter, centralAreaLbl);
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

void dmiLabel::paintSegment(QPainter *iconPainter, QRect centralArea){
    iconPainter->save();
    iconPainter->setPen(era::darkYellow);
    iconPainter->setFont(QFont("Segment7",
                               static_cast<int>(fontFactor * customFontFactor * centralArea.height()),
                               QFont::Normal,
                               false));
    QRect textRect = iconPainter->boundingRect(centralArea,Qt::AlignLeft,segmentText);
    QPointF offset;
    if(alignment == Qt::AlignLeft){
        offset = QPointF(
            static_cast<qreal>(0),
            static_cast<qreal>(centralArea.height() - textRect.height()) / 1.3);
    }
    else if(alignment == Qt::AlignRight){
        offset = QPointF(
            static_cast<qreal>(centralArea.width() - textRect.width()) / 1.5,
            static_cast<qreal>(centralArea.height() - textRect.height()) / 1.3);
    }
    else{
        offset = QPointF(
            static_cast<qreal>(centralArea.width() - textRect.width()) / 1.5,
            static_cast<qreal>(centralArea.height() - textRect.height()) / 1.3);
    }
    textRect.translate(offset.toPoint());

    iconPainter->drawText(textRect,segmentText);
    iconPainter->restore();
}

void dmiLabel::paintTextMessages(QPainter *iconPainter, QRect centralArea){
    int lineHeight = centralArea.height()/numTextFields;
    QRect field(centralArea.x(),centralArea.y(),centralArea.width(), lineHeight);
    iconPainter->save();
    QFont font = QFont("FreeSans");
    font.setBold(true);
    font.setPointSizeF(fontFactor * 0.44 * lineHeight);
    iconPainter->setFont(font);
    for(quint8 i=0; i < numTextFields; i++){
        if(i == numTextFields - 1){
            field.setHeight(centralArea.height() - i * lineHeight);
        }
        iconPainter->setPen(Qt::NoPen);
        iconPainter->setBrush(messageBackQolors[i + textMessageOffset]);
        iconPainter->drawRect(field);
        iconPainter->setPen(messageTextColors[i + textMessageOffset]);
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
        if(fileNameIsSet)svgActive.render(iconPainter,centralArea);
    }
}

QRect dmiLabel::calcOptimalRect(QRect destArea, QRect iconArea){
    if (!isButton || iconArea.isEmpty())
        return destArea;
    // Fit size of icon (viewBox) proportionaly in destArea
    const QSizeF fitted = QSizeF(iconArea.size())
                              .scaled(destArea.size(), Qt::KeepAspectRatio);
    QRect result(QPoint(0,0), fitted.toSize());
    result.moveCenter(destArea.center());
    return result;
}

void dmiLabel::paintFrame(QPainter *framePainter, QColor bright, QColor dark,  int bt, bool isBtn){
    // We draw the 3D sunken border
    int w = width();
    int h = height();
    int bO = bt*isBtn;                          // Offset of widget corner, in case of painting a button
    bool lC = !borderLOpen;                     // We need to take care of labels with open frames
    bool rC = !borderROpen;
    QPoint brightBorder[6] = {
        QPoint(  - bO        , h - bO     ),    // Bottom left corner
        QPoint(w - bO        , h - bO     ),    // Bottom right corner
        QPoint(w - bO        ,     bO     ),    // Top right corner
        QPoint(w - bO - bt*rC,     bO + bt),    // Shifted top right corner
        QPoint(w - bO - bt*rC, h - bO - bt),    // Shifted bottom right corner
        QPoint(  - bO + bt*lC, h - bO - bt)     // Shifted bottom left corner
    };
    QPoint darkBorder[6] = {
        QPoint(    bO        , h - bO     ),    // Bottom left corner
        QPoint(    bO        ,     bO     ),    // Top left corner
        QPoint(w - bO        ,     bO     ),    // Top right corner
        QPoint(w - bO - bt*rC,     bO + bt),    // Shifted top right corner
        QPoint(    bO + bt*lC,     bO + bt),    // Shifted lop left corner
        QPoint(    bO + bt*lC, h - bO - bt)     // Shifted bottom left corner
    };
    framePainter->setPen(Qt::NoPen);            // Set no outlin
    framePainter->setBrush(bright);             // Apply the shadow color
    framePainter->drawPolygon(brightBorder, 6); // Draw the frame border at buttom and right
    framePainter->setBrush(dark);               // Set the brush to dark for the mirrored polygon
    framePainter->drawPolygon(darkBorder, 6);   // Draw the frame border at top and left
}

#ifndef DMILABEL_H
#define DMILABEL_H

#include <QWidget>
#include <QPainter>
#include "era.h"
#include <QSvgRenderer>
#include <QtSvg>
#include <QMimeDatabase>
//#include <QGraphicsSvgItem>   // Wird ggf gebraucht unter Qt5
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QAudioOutput>
#endif
#include <QSoundEffect>
#include <QTimer>

class dmiLabel : public QWidget
{
    Q_OBJECT
public:
    dmiLabel(QWidget *parent = nullptr);

public slots:
    void updateBlinking();
    void updateLabel();
    //void setAsButton(bool forDataEntry=false, bool enabled=true, QString text="");
    void setAsButton(QString text);
    void setAsButton(bool enabled, QString text, QString emitText);
    void setAsButton(bool enabled=true, QString text="");
    void setAsDataEntryLabel(QString text, bool isInputfield=false, bool isEnabled=false, bool isValid=false, bool isApplicable=true);
    void setAsDataEntryLabelFg(QString text, bool isInputfield=false, bool isEnabled=false, bool isValid=false, bool isApplicable=true, bool fullGrid=false);
    void setVisib(bool visible);
    void setIcon(QString filename);
    void setIcon(QString filenameActive, QString filenameInactive);
    void setCustomFontFactor(qreal factor);
    void setCustomFontFactor(qreal factor, Qt::Alignment customAlignment);
    void setCustomFontFactor(qreal factor, Qt::Alignment customAlignment, QFont::Weight style);
    void setText(QString text);
    void setText(quint16 text);
    void setText(QString text, QColor textColor, QColor textColorDisabled, QFont::Weight boldOrThin);
    void setText(QString text, QColor textColor, QColor backgroundColor);
    void setTextFieldUsing(quint8 numFields);
    void setTextFieldUsing(quint8 numFields, quint8 customAlignment);
    void setSegmentDigitToUse(quint8 position);
    void setSegmentDigit(quint16 value, bool textVisible, bool fromEtcs);
    void setSegmentText(quint16 value);
    void addTextLzbMessage(QString text, QColor textColor, QColor bgColor, quint8 msgId);
    void removeTextMessage(quint8 msgId);
    void shiftTextMessageOffset(qint8 shift);
    void setTextMessageOffset(qint8 offset);
    void setWorking(bool working, quint8 blinking, bool inverse);
    void setEnabled(bool enabled);
    void setUnclosedFrame(bool openL, bool openR);
    void setAcklowedgeFrame(bool active);
    void setFrameless();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:
    void clicked();
    void txtBtnClicked(QString value);
    void dataEntryBtnClicked(QString value, bool enabled);
    void messaesOutOfView(bool outOfView);

private:
    QSoundEffect *click1 =nullptr; // workaround: QSound::play crash on some android devices
    QTimer *attenuationTimer = new QTimer();
    bool fileNameIsSet;
    bool isTextField = false;
    bool isSegment = false;
    bool borderLOpen = false;
    bool borderROpen = false;
    QFont::Weight textStyle=QFont::Bold;   // Bold: 75, Normal 50
    QString labelText = "";
    QString emitText = "";
    QString filenameIconActive;
    QString filenameIconInactive;
    QColor labelTextColorEnab = era::grey;
    QColor labelTextColorDisab = era::darkGrey;
    QColor borderColorTopAndLeft = era::shadow;
    QColor borderColorButtomAndRight = era::black;
    QColor bgColor = era::darkBlue;
    quint8 segmentPosition;
    qreal fontFactor;
    qreal customFontFactor = 0.3;
    quint8 numTextFields = 0;
    quint8 highestTextMessgePosition = 0;
    quint8 indexHighestTextFields = 0;
    QString messageTexts[10];
    qint8 textMessageOffset = 0;
    QString segmentText;
    quint16 segmentValue = 0;
    QColor messageBackQolors[10] = {era::darkBlue,era::darkBlue,era::darkBlue,era::darkBlue,era::darkBlue,
                                    era::darkBlue,era::darkBlue,era::darkBlue,era::darkBlue,era::darkBlue,};
    QColor messageTextColors[10] = {era::darkBlue,era::darkBlue,era::darkBlue,era::darkBlue,era::darkBlue,
                                    era::darkBlue,era::darkBlue,era::darkBlue,era::darkBlue,era::darkBlue,};
    quint8 messageIds[10]={255,255,255,255,255,255,255,255,255,255};
    bool isPushed = false;
    QMimeDatabase mimeFile;
    int borderThickness = 1;
    bool isVisible = true;
    bool isEnab = true;
    QSvgRenderer svgActive;
    QSvgRenderer svgInactive;
    quint16 tickCounter = 0;
    quint8 blinkFrequency = 0;
    //bool isBlinking = false;
    bool isInvert = false;
    bool blinkerSlow = false;
    bool blinkerFast = false;
    bool blinkerSuperFast = false;
    bool isButton = false;
    bool isDataEntryButton = false;
    bool acknowledgeFrameActive = false;
    //bool alignLeft = false;
    //bool alignRight = false;
    bool hasCustomAlignment = false;
    int alignment = Qt::AlignCenter;
    void paintText(QPainter *iconPainter, QRect centralArea);
    void paintTextMessages(QPainter *iconPainter, QRect centralArea);
    void paintFrame(QPainter *framePainter, QColor bright, QColor dark,  int bt, bool isBtn);
    void paintIcon(QPainter *iconPainter, QRect centralArea);
    void paintSegment(QPainter *iconPainter, QRect centralArea);
    QRect calcOptimalRect(QRect destArea, QRect iconArea);

private slots:
    //void attenuationRoutine();
};

#endif // DMILABEL_H

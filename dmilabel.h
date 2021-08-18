#ifndef DMILABEL_H
#define DMILABEL_H

#include <QWidget>
#include <QPainter>
#include "era.h"
#include <QSvgRenderer>
#include <QMimeDatabase>
#include <QGraphicsSvgItem>
#include <QMediaPlayer>
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
    void setAsDataEntryLabel(QString text, bool isInputfield=false, bool isEnabled=false, bool isValid=false, bool isApplicable=true, bool fullGrid=false);
    void setVisib(bool visible);
    void setBorderThickness(int height);
    void setIcon(QString filename);
    void setIcon(QString filenameActive, QString filenameInactive);
    void setCustomFontFactor(qreal factor);
    void setCustomFontFactor(qreal factor, Qt::Alignment customAlignment);
    void setText(QString text);
    void setText(quint16 text);
    void setText(QString text, QColor textColor, QColor textColorDisabled, quint8 boldOrThin);
    void setText(QString text, QColor textColor, QColor backgroundColor);
    void setTextFieldUsing(quint8 numFields);
    void setTextFieldUsing(quint8 numFields, quint8 customAlignment);
    void setSegmentDigitToUse(quint8 position);
    void setSegmentText(quint16 value, bool textVisible);
    void addTextMessage(QString text, QColor textColor, QColor bgColor, quint8 msgId);
    void removeTextMessage(quint8 msgId);
    void shiftTextMessageOffset(qint8 shift);
    void setWorking(bool working, quint8 blinking, bool inverse);
    void setDpi(qreal dpi);
    void setTargetDistance(quint16 distance, bool visible);
    void setIsDistanceScale();
    void setEraUse(bool useEra);
    void setUnclosedFrame(bool openL, bool openR, bool openU, bool openD);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:
    void clicked(bool value);
    void txtBtnClicked(QString value);
    void dataEntryBtnClicked(QString value, bool enabled);
    void messaesOutOfView(bool outOfView);

private:
    QTimer *attenuationTimer = new QTimer();
    bool fileNameIsSet;
    bool isTextField = false;
    bool isSegment = false;
    bool isTargetDistance = false;
    bool targetDistanceVisible = false;
    bool useEraStyle = true;
    bool borderLClosed = true;
    bool borderRClosed = true;
    bool borderTClosed = true;
    bool borderBClosed = true;
    quint8 textStyle=QFont::Bold;   // Bold: 75, Normal 50
    QString labelText = "";
    QString emitText = "";
    QString filenameIconActive;
    QString filenameIconInactive;
    QColor labelTextColorEnab = era::grey;
    QColor labelTextColorDisab = era::darkGrey;
    QColor borderColorTopAndLeft = era::shadow;
    QColor borderColorButtomAndRight = era::black;
    QColor bgColor = era::darkBlue;
    //quint8 msgTextAlign;
    QString fileForDistanceScale;
    quint16 distanceScale;
    quint16 targetDistance;
    qreal targetDistanceGraph = 0;
    quint16 targetDistanceDest;
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
    quint8 blinkFrequency = 0;
    //bool isBlinking = false;
    bool isInvert = false;
    bool blinkerSlow = false;
    bool blinkerFast = false;
    bool isButton = false;
    bool isDataEntryButton = false;
    //bool alignLeft = false;
    //bool alignRight = false;
    bool hasCustomAlignment = false;
    int alignment = Qt::AlignCenter;
    void paintText(QPainter *iconPainter, QRect centralArea);
    void paintTextMessages(QPainter *iconPainter, QRect centralArea);
    void paintFrame(QPainter *framePainter, QRect centralArea, QColor colorLefUp, QColor colorRightDown, int offset);
    void paintIcon(QPainter *iconPainter, QRect centralArea);
    void paintSegment(QPainter *iconPainter, QRect centralArea);
    void paintDistance(QPainter *iconPainter, QRect centralArea);
    QRect calcOptimalRect(QRect destArea, QRect iconArea);

private slots:
    void attenuationRoutine();
};

#endif // DMILABEL_H

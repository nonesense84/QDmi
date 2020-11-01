#ifndef DMILABEL_H
#define DMILABEL_H

#include <QWidget>
#include <QPainter>
#include "era.h"
#include <QSvgRenderer>
#include <QMimeDatabase>
#include <QGraphicsSvgItem>
#include <QSound>

class dmiLabel : public QWidget
{
    Q_OBJECT
public:
    dmiLabel(QWidget *parent = nullptr);

public slots:
    void updateBlinking1();
    void updateBlinking2();
    void setAsButton(bool asButton);
    void setAsButton(bool asButton, bool forDataEntry);
    void setVisib(bool visible);
    void setBorderThickness(int height);
    void setIcon(QString filename);
    void setIcon(QString filenameActive, QString filenameInactive);
    void setCustomFontFactor(qreal factor);
    void setText(QString text);
    void setText(QString text, QColor textColor, QColor textColorDisabled, quint8 boldOrThin);
    void setTextFieldUsing(quint8 numFields);
    void setTextFieldUsing(quint8 numFields, quint8 alignment);
    void setSegmentDigitToUse(quint8 position);
    void setSegmentText(quint16 value, bool textVisible);
    void addTextMessage(QString text, QColor textColor, QColor bgColor, quint8 msgId);
    void removeTextMessage(quint8 msgId);
    void setWorking(bool working, quint8 blinking, bool inverse);
    void setDpi(qreal dpi);
    void setTargetDistance(quint16 distance, bool visible);
    void setIsDistanceScale();
    void setEraUse(bool useEra);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:
    void clicked(bool value);
    void txtBtnClicked(QString value);

private:
    QString tmpFileName;
    bool isTextField = false;
    bool isSegment = false;
    bool isTargetDistance = false;
    bool targetDistanceVisible = false;
    bool useEraStyle = true;
    quint8 textStyle=QFont::Bold;   // Bold: 75, Normal 50
    QString labelText;
    QColor labelTextColorEnab = era::grey;
    QColor labelTextColorDisab = era::darkGrey;
    uint8_t msgTextAlign;
    QString fileForDistanceScale;
    quint16 distanceScale;
    quint16 targetDistance;
    quint8 segmentPosition;
    qreal fontFactor;
    qreal customFontFactor = 0.3;
    quint8 numTextFields = 0;
    QString messageTexts[10];
    QString segmentText;
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
    QPixmap iconActive;
    QPixmap iconInactive;
    QSvgRenderer svgActive;
    QSvgRenderer svgInactive;
    quint8 blinkFrequency;
    //bool isBlinking = false;
    bool isInvert = false;
    bool timeToBlink1 = false;
    bool timeToBlink2 = false;
    bool isButton = false;
    bool isDataEntryButton = false;
    bool alignLeft = false;
    bool useSvgIcon = false;
    void paintText(QPainter *iconPainter, QRect centralArea);
    void paintTextMessages(QPainter *iconPainter, QRect centralArea);
    void paintFrame(QPainter *framePainter, QRect centralArea, QColor colorLefUp, QColor colorRightDown, int offset);
    void paintIcon(QPainter *iconPainter, QRect centralArea);
    void paintSegment(QPainter *iconPainter, QRect centralArea);
    void paintDistance(QPainter *iconPainter, QRect centralArea);
    QRect calcOptimalRect(QRect destArea, QRect iconArea);

};

#endif // DMILABEL_H

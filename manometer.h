#ifndef MANOMETER_H
#define MANOMETER_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QtMath>
#include <QDebug>

class manometer : public QWidget
{
    Q_OBJECT
public:
    explicit manometer(QWidget *parent = nullptr);

public slots:
    void setDpi(qreal dpi);
    void setPressMaxDial(quint8 pressure);
    void setPointer2using(bool use);
    void setPointer1color(QColor color);
    void setPointer2color(QColor color);
    void setPonter1Label(QString text);
    void setPonter2Label(QString text);
    void setPressure1(quint16 pressure);
    void setPressure2(quint16 pressure);
    void setBackgroundColor(QColor color);

private:
    QColor colorPointer1 = Qt::yellow;
    QColor colorPointer2 = Qt::red;
    QColor colorBackground = Qt::black;
    QString textLabel1 = "";
    QString textLabel2 = "";
    qreal temp = 0;
    qreal posNeedle1;
    qreal posNeedle2;
    qreal switchAngle = 0;
    qreal switchPressure = 0;
    qreal pressure1;
    qreal pressure2;
    qreal pressMaxDial = 0;
    qreal scaleDegStep;
    qreal scaleDegStepSmall;
    qreal arcOpenTotal = 288;   // 288° 0 to Vmax
    qreal startPosZero = -234; //-(144 + 90)
    quint16 outEndLines = 250;
    quint16 lenShortLines = 30;
    quint16 lenLongLines = 50;
    int fontSizeDial = 26;
    int fontSiceLabel = 37;
    quint8 degOffset = 0; // For 12bar scale, whre 5bar must be at top
    quint8 numShortLines;
    quint8 numNumbers;
    bool pointer2used = true;

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // MANOMETER_H

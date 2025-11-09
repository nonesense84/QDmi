#ifndef TARGETDISTANCE_H
#define TARGETDISTANCE_H

#include <QWidget>
#include <QObject>
#include <QApplication>
#include <QTimer>
#include <QPainter>
#include <QtSvg>
#include "era.h"

class targetDistance : public QWidget
{
    Q_OBJECT
public:
    targetDistance(QWidget *parent = nullptr);

signals:
    void activatedDigitalInOs(bool active);

public slots:
    void setTargetDistance(quint16 dist, bool barVisible, bool digitalVisible, bool fromEtcs);
    void setEraUse(bool useEra);
    void setDigitalInOs(bool active);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    qreal fontFactor;
    qreal targetDistanceGraph = 0;
    QTimer *attenuationTimer = new QTimer(this);
    QSvgRenderer svgActive;
    QString fileForDistanceScale;
    int borderThickness = 1;
    quint16 distanceScale;
    quint16 targetDistanceIs;
    quint16 targetDistanceDest;
    bool targetDistanceBarVisible = false;
    bool targetDistanceDigitalVisible = false;
    bool targetDistanceFromEtcs = false;
    bool useEraStyle = true;
    bool displayDigitalInOs = false;

private slots:
    void attenuationRoutine();
    void paintFrame(QPainter *framePainter, QColor bright, QColor dark,  int bt);
    void checkStyleType();
};

#endif // TARGETDISTANCE_H

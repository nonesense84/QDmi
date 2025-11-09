#ifndef TIMETOINDICATION_H
#define TIMETOINDICATION_H

#include <QWidget>
#include <QObject>
#include <QApplication>
#include <QTimer>
#include <QPainter>
#include "era.h"
#include <QDebug>
class timetoindication : public QWidget
{
public:
    timetoindication(QWidget *parent = nullptr);

public slots:
    void setTti(float quint16, bool visible);
private:
    QTimer *attenuationTimer = new QTimer(this);
    float squareSmooth = 0.0;  // Value of TTI as relation to widget size (Actual square size)
    float squareDest = 0.0;    // Value of TTI as relation to widget size (Target square size)
    bool ttiActive = false;

private slots:
    void attenuationRoutine();
    void paintFrame(QPainter *framePainter, QColor bright, QColor dark,  int bt);
    float mapClamped(float x, float in_min, float in_max, float out_min, float out_max);

protected:
    void paintEvent(QPaintEvent *event) override;

};

#endif // TIMETOINDICATION_H

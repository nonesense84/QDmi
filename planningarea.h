#ifndef PLANNINGAREA_H
#define PLANNINGAREA_H

#include <QWidget>
#include <QPainter>
#include "era.h"
#include <QApplication>

class planningArea : public QWidget {
    Q_OBJECT
public:
    planningArea(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void paintFrame(QPainter *framePainter, QColor bright, QColor dark,  int bt);
};

#endif // PLANNINGAREA_H

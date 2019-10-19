#ifndef DMIBUTTON_H
#define DMIBUTTON_H

#include <QWidget>
#include <QPainter>
#include "era.h"

class dmiButton : public QWidget
{
    Q_OBJECT
public:
    dmiButton(QWidget *parent = nullptr);
    void setBorderThickness(int height);
    void setPixmapEnabled(QPixmap pme, QPixmap pmd);
    void setEnabled(bool en);
    void setUsed(bool used);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:
    void clicked(bool value);

private:
    int borderThickness = 1;
    bool pushed = false;
    QPixmap enabPixmap;
    QPixmap disaPixmap;
    bool enabledd;
    bool isUsed = true;


};

#endif // DMIBUTTON_H

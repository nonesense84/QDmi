#include "dmibutton.h"

dmiButton::dmiButton(QWidget *parent) : QWidget(parent)
{
}
void dmiButton::setUsed(bool used){
    isUsed = used;
}
void dmiButton::setPixmapEnabled(QPixmap pme, QPixmap pmd){
  enabPixmap = pme;
  disaPixmap = pmd;
}


void dmiButton::setEnabled(bool en){
    enabledd = en;
}

void dmiButton::setBorderThickness(int thickness){
    borderThickness = thickness;
}
void dmiButton::mousePressEvent(QMouseEvent *event){
    pushed = true;
    update();
    if(isUsed)
        emit clicked(true);
}

void dmiButton::mouseReleaseEvent(QMouseEvent *event){
    pushed = false;
    update();
}

void dmiButton::paintEvent(QPaintEvent *)
{
    if(isUsed){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    int halfHeight = height()/2;
    int halfWidth = width()/2;
    QRect darkBorder(0, 0, width(), height());
    QPoint brightBorder[5];
    if(halfWidth > halfHeight){
        brightBorder[0] = QPoint( width(),  0);
        brightBorder[1] = QPoint( width(),  height());
        brightBorder[2] = QPoint(0,         height());
        brightBorder[3] = QPoint(halfHeight,  halfHeight);
        brightBorder[4] = QPoint( width() - halfHeight, halfHeight);
    }
     else{
        brightBorder[0] = QPoint(width(),  0);
        brightBorder[1] = QPoint(width(),  height());
        brightBorder[2] = QPoint(0      ,  height());
        brightBorder[3] = QPoint(halfWidth, height() - halfWidth);
        brightBorder[4] = QPoint(halfWidth, halfWidth);
    }
    QRect centralArea(borderThickness, borderThickness, width()-2*borderThickness, height()-2*borderThickness);
    QRect centralAreaPush(2*borderThickness, 2*borderThickness, width()-4*borderThickness, height()- 4 * borderThickness);
    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setBrush(era::black);
    painter.drawRect(darkBorder);
    painter.setBrush(era::shadow);
    painter.drawPolygon(brightBorder, 5);
    painter.setBrush(era::darkBlue);
    painter.drawRect(centralArea);
    painter.drawPixmap(centralAreaPush,enabPixmap);
    painter.restore();

    if(!pushed){
        brightBorder[0] = QPoint(brightBorder[0].x() - borderThickness, brightBorder[0].y() + borderThickness);
        brightBorder[1] = QPoint(brightBorder[1].x() - borderThickness, brightBorder[1].y() - borderThickness);
        brightBorder[2] = QPoint(brightBorder[2].x() + borderThickness, brightBorder[2].y() - borderThickness);
        brightBorder[3] = QPoint(brightBorder[3].x() + borderThickness, brightBorder[3].y() - borderThickness);
        brightBorder[4] = QPoint(brightBorder[4].x() - borderThickness, brightBorder[4].y() + borderThickness);
        darkBorder = QRect(borderThickness,borderThickness,width() - 2 * borderThickness, height() - 2 * borderThickness);
        painter.save();
        painter.setPen(Qt::NoPen);
        painter.setBrush(era::shadow);
        painter.drawRect(darkBorder);
        painter.setBrush(era::black);
        painter.drawPolygon(brightBorder, 5);
        painter.setBrush(era::darkBlue);
        painter.drawRect(centralAreaPush);
        painter.drawPixmap(centralAreaPush,enabPixmap);
        painter.restore();
    }
    }
}

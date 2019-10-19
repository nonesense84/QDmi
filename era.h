#ifndef ERA_H
#define ERA_H
#include <QObject>
#include <QColor>
#include <QRect>
namespace {
class era : public QObject
{
    Q_OBJECT
public:

    static const QColor white;
    static const QColor black;
    static const QColor grey;
    static const QColor mediumGrey;
    static const QColor darkGrey;
    static const QColor darkBlue;
    static const QColor shadow;
    static const QColor yellow;
    static const QColor orange;
    static const QColor red;
    static const QColor paspDark;
    static const QColor paspLight;
    static const QColor darkYellow;
    static const QRect tachoDefault;
    static const QRect modeAreDefault;

};

const QColor era::white = QColor(255,255,255);
const QColor era::black = QColor(0,0,0);
const QColor era::grey = QColor(195, 195, 195);
const QColor era::mediumGrey = QColor(150,150,150);
const QColor era::darkGrey = QColor(85,85,85);
const QColor era::darkBlue = QColor(3,17,34);
const QColor era::shadow = QColor(8,24,57);
const QColor era::yellow = QColor(223,223,0);
const QColor era::orange = QColor(234,145,0);
const QColor era::red = QColor(191,0,2);
const QColor era::paspDark = QColor(33,45,74);
const QColor era::paspLight = QColor(41,74,107);
const QColor era::darkYellow = QColor(255,219,0); // Not Official from Era! Just looks good!

//const QColor era::black = QColor(255,0,0);
//const QColor era::shadow = QColor(0,127,127);

const QRect era::tachoDefault(3,12,274,274);
const QRect era::modeAreDefault(7,211,265,82);

}


#endif // ERA_H

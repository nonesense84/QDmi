#ifndef ERA_H
#define ERA_H
#include <QObject>
#include <QColor>
#include <QRect>
namespace era{
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
    static const QRect modeOverride;
};

const QStringList modeIcons = { // Order of icons acording to TCP protokol of Zusi 3
    ":/icons/blanc.svg", // 0   mode undefined
    ":/icons/MO_11.svg", // 1   mode FS
    ":/icons/MO_07.svg", // 2   mode OS
    ":/icons/MO_09.svg", // 3   mode SR
    ":/icons/MO_01.svg", // 4   mode SH
    ":/icons/MO_16.svg", // 5   mode UN
    ":/icons/MO_19.svg", // 6   mode SL
    ":/icons/MO_13.svg", // 7   mode SB
    ":/icons/MO_04.svg", // 8   mode TR
    ":/icons/MO_06.svg", // 9   mode PT
    ":/icons/MO_18.svg", // 10  mode SF
    ":/icons/MO_IS.svg", // 11  mode IS
    ":/icons/blanc.svg", // 12  mode NP
    ":/icons/MO_12.svg", // 13  mode NL
    ":/icons/blanc.svg", // 14  mode SE
    ":/icons/MO_19.svg", // 15  mode SN
    ":/icons/MO_14.svg", // 16  mode RV
    ":/icons/MO_21.svg", // 17  mode LS
    ":/icons/blanc.svg", // 18  mode PS
};
const QStringList modeOverrideIcon = {
     ":/icons/blanc.svg", // 0   mode undefined
     ":/icons/MO_03.svg", // 19  mode Override
};
const QStringList nextModeIcons = { // Order of icons acording to TCP protokol of Zusi 3
    ":/icons/blanc.svg", // 0   mode undefined
    ":/icons/blanc.svg", // 1   mode FS
    ":/icons/MO_08.svg", // 2   mode OS
    ":/icons/MO_10.svg", // 3   mode SR
    ":/icons/MO_02.svg", // 4   mode SH
    ":/icons/MO_17.svg", // 5   mode UN
    ":/icons/MO_20.svg", // 6   mode SL
    ":/icons/blanc.svg", // 7   mode SB
    ":/icons/MO_05.svg", // 8   mode TR
    ":/icons/blanc.svg", // 9   mode PT
    ":/icons/blanc.svg", // 10  mode SF
    ":/icons/blanc.svg", // 11  mode IS
    ":/icons/blanc.svg", // 12  mode NP
    ":/icons/blanc.svg", // 13  mode NL
    ":/icons/blanc.svg", // 14  mode SE
    ":/icons/MO_20.svg", // 15  mode SN
    ":/icons/MO_15.svg", // 16  mode RV
    ":/icons/MO_22.svg", // 17  mode LS
    ":/icons/blanc.svg", // 18  mode PS
};
const QStringList levelIcons = { // Order of icons acording to TCP protokol of Zusi 3
    ":/icons/blanc.svg", // level undefined  =  0,
    ":/icons/blanc.svg", // level Stm        =  1,
    ":/icons/LE_01.svg", // level 0          =  2,
    ":/icons/LE_03.svg", // level 1          =  3,
    ":/icons/LE_04.svg", // level 2          =  4,
    ":/icons/LE_05.svg", // level 3          =  5
    ":/icons/LE_02a.svg",// level PZB/LZB    =  6
};
const QStringList nextLevelIcons = { // Order of icons acording to TCP protokol of Zusi 3
    ":/icons/blanc.svg", // level undefined  =  0,
    ":/icons/blanc.svg", // level Stm        =  1,
    ":/icons/LE_06.svg", // level 0          =  2,
    ":/icons/LE_10.svg", // level 1          =  3,
    ":/icons/LE_12.svg", // level 2          =  4,
    ":/icons/LE_14.svg", // level 3          =  5
    ":/icons/LE_08a.svg",// level PZB/LZB    =  6
};
const QStringList nextLevelIconsAck = { // Order of icons acording to TCP protokol of Zusi 3
    ":/icons/blanc.svg", // level undefined  =  0
    ":/icons/blanc.svg", // level Stm        =  1,
    ":/icons/LE_07.svg", // level 0          =  2,
    ":/icons/LE_11.svg", // level 1          =  3,
    ":/icons/LE_13.svg", // level 2          =  4,
    ":/icons/LE_15.svg", // level 3          =  5
    ":/icons/LE_09a.svg",// level PZB/LZB    =  6
};
const QStringList emergencyBrake = {
    ":/icons/blanc.svg", // no brake            =  0,
    ":/icons/ST_01.svg", // brake active        =  1,
};
const QStringList adhesionReduced = {
    ":/icons/blanc.svg", // not reduced         =  0,
    ":/icons/ST_02.svg", // reduced             =  1,
};
const QStringList ordersAndAnnouncementofTrackConditionIcons = {
    ":/icons/blanc.svg",//
    ":/icons/TC01.svg", // Pantograph lowered, grey                               =  1,
    ":/icons/TC02.svg", // Lower pantograph, grey                                 =  2,
    ":/icons/TC03.svg", // Lower pantograph, yellow                               =  3,
    ":/icons/TC04.svg", // Raise pantograph, grey                                 =  4,
    ":/icons/TC05.svg", // Raise pantographyellow                                 =  5,
    ":/icons/TC06.svg", // Neutral section and Neutral section announcement, grey =  6,
    ":/icons/TC07.svg", // Neutral section announcement, yellow ;                 =  7,
    ":/icons/TC08.svg", // End of neutral section, grey ;                         =  8,
    ":/icons/TC09.svg", // End of neutral section, yellow ;                       =  9,

};
const QStringList nextLevelTextmessageAck = {
    "Levelwechsel bestätigen",             // level undefined  =  0,
    "Wechsel zu Level NTC bestätigen",     // level Stm        =  1,
    "Wechsel zu Level 0 bestätigen",       // level 0          =  2,
    "Wechsel zu Level 1 bestätigen",       // level 1          =  3,
    "Wechsel zu Level 2 bestätigen",       // level 2          =  4,
    "Wechsel zu Level 3 bestätigen",       // level 3          =  5
    "Wechsel zu Level PZB/LZB bestätigen"  // level PZB/LZB    =  6
};
const QStringList nextLevelTextmessage = {
    "Levelwechsel erwartet",               // level undefined  =  0,
    "Wechsel zu Level NTC erwartet",       // level Stm        =  1,
    "Wechsel zu Level 0 erwartet",         // level 0          =  2,
    "Wechsel zu Level 1 erwartet",         // level 1          =  3,
    "Wechsel zu Level 2 erwartet",         // level 2          =  4,
    "Wechsel zu Level 3 erwartet",         // level 3          =  5
    "Wechsel zu Level PZB/LZB erwartet"    // level PZB/LZB    =  6
};
const QStringList nextModeTextmessageAck = {
    "Moduswechsel bestätigen",        // 0   mode undefined
    "Wechsel zu Modus FS bestätigen", // 1   mode FS
    "Wechsel zu Modus OS bestätigen", // 2   mode OS
    "Wechsel zu Modus SR bestätigen", // 3   mode SR
    "Wechsel zu Modus SH bestätigen", // 4   mode SH
    "Wechsel zu Modus UN bestätigen", // 5   mode UN
    "Wechsel zu Modus SL bestätigen", // 6   mode SL
    "Wechsel zu Modus SB bestätigen", // 7   mode SB
    "Wechsel zu Modus TR bestätigen", // 8   mode TR
    "Wechsel zu Modus PT bestätigen", // 9   mode PT
    "Wechsel zu Modus SF bestätigen", // 10  mode SF
    "Wechsel zu Modus IS bestätigen", // 11  mode IS
    "Wechsel zu Modus NP bestätigen", // 12  mode NP
    "Wechsel zu Modus NL bestätigen", // 13  mode NL
    "Wechsel zu Modus SE bestätigen", // 14  mode SE
    "Wechsel zu Modus SN bestätigen", // 15  mode SN
    "Wechsel zu Modus RV bestätigen", // 16  mode RV
    "Wechsel zu Modus LS bestätigen", // 17  mode LS
    "Wechsel zu Modus PS bestätigen"  // 18  mode PS
};
const QStringList nextModeTextmessage = {
    "Moduswechsel bestätigt",       // 0   mode undefined
    "Wechsel zu Modus FS erwartet", // 1   mode FS
    "Wechsel zu Modus OS erwartet", // 2   mode OS
    "Wechsel zu Modus SR erwartet", // 3   mode SR
    "Wechsel zu Modus SH erwartet", // 4   mode SH
    "Wechsel zu Modus UN erwartet", // 5   mode UN
    "Wechsel zu Modus SL erwartet", // 6   mode SL
    "Wechsel zu Modus SB erwartet", // 7   mode SB
    "Wechsel zu Modus TR erwartet", // 8   mode TR
    "Wechsel zu Modus PT erwartet", // 9   mode PT
    "Wechsel zu Modus SF erwartet", // 10  mode SF
    "Wechsel zu Modus IS erwartet", // 11  mode IS
    "Wechsel zu Modus NP erwartet", // 12  mode NP
    "Wechsel zu Modus NL erwartet", // 13  mode NL
    "Wechsel zu Modus SE erwartet", // 14  mode SE
    "Wechsel zu Modus SN erwartet", // 15  mode SN
    "Wechsel zu Modus RV erwartet", // 16  mode RV
    "Wechsel zu Modus LS erwartet", // 17  mode LS
    "Wechsel zu Modus PS erwartet"  // 18  mode PS
};
const QStringList reasonEmgrBreakTextmessage = {
    "",                                            //  0
    "Zwangsbremsung Grund 1",                      //  1
    "Zwangsbremsung Grund 2",                      //  2
    "Zwangsbremsung Grund 3",                      //  3
    "Zwangsbremsung Grund 4",                      //  4
    "Zwangsbremsung Grund 5",                      //  5
    "V-Max überschritten",                         //  6
    "Funktionsprüfung",                            //  7
    "Zwangsbremsung Grund 8",                      //  8
    "Zwangsbremsung Grund 9",                      //  9
    "Rechnerausfall",                              // 10
    "ETCS-Nothalt überfahren",                     // 11
    "Zwangsbremsung Grund 12",                     // 12
    "Zwangsbremsung Grund 13",                     // 13
    "Zwangsbremsung Grund 14",                     // 14
    "ETCS-Halt überfahren",                        // 15
    "Stillstands-/ Rücklaufüberwachung ausgelöst", // 16
    "Nicht quittiert",                             // 17
    "Funkausfall",                                 // 18
    "Balisenstörung",                              // 19
    "Manueller Levelwechsel",                      // 20
    "Zwangsbremsung Grund 21",                     // 21
    "Zwangsbremsung Grund 22",                     // 22
    "Zwangsbremsung Grund 23",                     // 23
    "Zwangsbremsung Grund 24",                     // 24
    "Zwangsbremsung Grund 25",                     // 25
    "Zwangsbremsung Grund 26",                     // 26
    "Allgemeine Störung",                          // 27
    "Stromversorgung gestört"                      // 28
};
const QStringList traindataStateTextMessages = {
    "",                        //  0
    "Tfzf-Eingabe erwartet",   //  1
    "Start durch Tf erwartet", //  2
};


const QColor white = QColor(255,255,255);
const QColor black = QColor(0,0,0);
const QColor grey = QColor(195, 195, 195);
const QColor mediumGrey = QColor(150,150,150);
const QColor darkGrey = QColor(85,85,85);
const QColor darkBlue = QColor(3,17,34);
const QColor shadow = QColor(8,24,57);
const QColor yellow = QColor(223,223,0);
const QColor orange = QColor(234,145,0);
const QColor red = QColor(191,0,2);
const QColor paspDark = QColor(33,45,74);
const QColor paspLight = QColor(41,74,107);
const QColor darkYellow = QColor(255,219,0); // Not Official from Era! Just looks good!

//const QColor era::black = QColor(255,0,0);
//const QColor era::shadow = QColor(0,127,127);

const QRect tachoDefault(3,12,274,274);
const QRect modeAreDefault(7,211,265,82);

}


#endif // ERA_H

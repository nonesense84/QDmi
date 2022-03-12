#ifndef DB_H
#define DB_H
#include <QObject>
#include <QColor>
#include <QRect>
namespace db{
class db : public QObject
{
    Q_OBJECT
public:
    static const QString messages[71];
    static const QColor textBgColors[58];
    static const QColor textFontColors[58];
    static const QString indicatorFiles[18][2];
    static const quint8 numMessages = 71;
};
    const quint8 numMessages = 71;
    const QString messages[73] = {
        "Zwangsbremsung",
        "LZB-Halt überfahren",
        "LZB-Nothalt",
        "LZB-Nothalt überfahren",
        "LZB-Nothaltschalter betätigt",
        "Geschwindigkeitsüberschreitung",
        "Vmax 100 km/h",
        "V-Überwachung 150 km/h",
        "V-Überwachung 140 km/h",
        "V-Überwachung 130 km/h",
        "V-Überwachung 120 km/h",
        "V-Überwachung 110 km/h",
        "V-Überwachung 100 km/h",
        "V-Überwachung 90 km/h",
        "V-Überwachung 85 km/h",
        "V-Überwachung 80 km/h",
        "V-Überwachung 70 km/h",
        "V-Überwachung 60 km/h",
        "V-Überwachung 55 km/h",
        "V-Überwachung 50 km/h",
        "V-Überwachung 45 km/h",
        "V-Überwachung 40 km/h",
        "V-Überwachung 35 km/h",
        "V-Überwachung 30 km/h",
        "V-Überwachung 25 km/h",
        "V-Überwachung 20 km/h",
        "V-Überwachung 10 km/h",
        "Ggf. weitere Stromabn. beachten",
        "LZB-Ende",
        "LZB-Vorsichtauftrag",
        "LZB-Ersatzauftrag",
        "LZB-Gegengleisfahrauftrag ",
        "LZB mit Störschalter abgeschaltet",
        "PZB-/LZB-Luftabsperrhahn zu",
        "PZB-/LZB ausgeschaltet",
        "LZB-Störung",
        "LZB-Übertragungsausfall",
        "Funktionsprüfung läuft",
        "Fdl beteiligen",
        "Mit Betriebsbremsung anhalten",
        "Stromabnehmer senken",
        "Stromabnehmer heben",
        "Hauptschalter ausschalten",
        "Hauptschalter einschalten",
        "Wachsamkeitstaste betätigen",
        "Freitaste betätigen",
        "Zugdaten eingeben",
        "Abbremsen unter 85 km/h",
        "Abbremsen unter 80 km/h",
        "Abbremsen unter 70 km/h",
        "Abbremsen unter 60 km/h",
        "Abbremsen unter 55 km/h",
        "Abbremsen unter 50 km/h",
        "Abbremsen unter 40 km/h",
        "Abbremsen unter 30 km/h",
        "Abbremsen unter 20 km/h",
        "Abbremsen unter 10 km/h",
        "Signale und Fahrplan beachten",
        "Geschwindigkeitswechsel",
        "Grunddaten wirksam",
        "Wirksame Zugdaten",
        "Bremseinsatzpunkt erwarten",
        "BRH < 60 eingegeben ",
        "Unberechtigtes Befreien",
        "WT nicht zeitgerecht betätigt",
        "2000 Hz Beeinflussung",
        "PZB mit Störschalter abgeschaltet ",
        "PZB Störung ",
        "Zwangsbremsausgabe Störung",
        "Ersatzzugdaten wirksam",
        "V-Überwachung 95 km/h",    // Fuer I60. Nicht Teil des DB-LH.
        "V-Überwachung 75 km/h",    // Fuer I60. Nicht Teil des DB-LH.
    };
    const QColor textBgColors[74]{
        QColor(191,0,2), QColor(191,0,2), QColor(191,0,2), QColor(191,0,2), QColor(191,0,2), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0),
        QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0),
        QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0),
        QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), QColor(255,219,0), Qt::cyan, Qt::cyan, Qt::cyan, Qt::cyan, Qt::cyan,
        Qt::cyan, Qt::cyan, Qt::cyan, Qt::cyan, Qt::cyan, Qt::cyan, Qt::cyan, Qt::cyan, Qt::cyan, Qt::cyan,
        Qt::cyan, Qt::cyan, Qt::cyan, Qt::cyan, Qt::cyan, Qt::gray, Qt::gray, Qt::gray, Qt::gray,
        QColor(255,219,0),QColor(255,219,0),QColor(255,219,0),QColor(255,219,0),QColor(255,219,0),Qt::gray, QColor(191,0,2),Qt::gray, QColor(255,219,0), QColor(255,219,0), QColor(255,219,0)
    };
    const QColor textFontColors[74]{
        Qt::white, Qt::white, Qt::white, Qt::white, Qt::white, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black,
        Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black,Qt::black,
        Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black,Qt::black,
        Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black,
        Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black,
        Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black,
        Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black, Qt::black
    };
    const QString indicatorFiles[26][2] = {
        {   ":/icons/LmB_on.svg",       ":/icons/LmB_off.svg",},        // 0
        {   ":/icons/Lm85_on.svg",      ":/icons/Lm85_off.svg",},       // 1
        {   ":/icons/Lm70_on.svg",      ":/icons/Lm70_off.svg",},       // 2
        {   ":/icons/Lm55_on.svg",      ":/icons/Lm55_off.svg",},       // 3
        {   ":/icons/LmPzb_on.svg",     ":/icons/LmPzb_off.svg",},      // 4
        {   ":/icons/LmLzb_on.svg",     ":/icons/LmLzb_off.svg",},      // 5
        {   ":/icons/LmS_on.svg",       ":/icons/LmS_off.svg",},        // 6
        {   ":/icons/LmH_on.svg",       ":/icons/LmH_off.svg",},        // 7
        {   ":/icons/LmG_on.svg",       ":/icons/LmG_off.svg",},        // 8
        {   ":/icons/LmE40_on.svg",     ":/icons/LmE40_off.svg",},      // 9
        {   ":/icons/LmV40_on.svg",     ":/icons/LmV40_off.svg",},      // 10
        {   ":/icons/LmBef40_on.svg",   ":/icons/LmBef40_off.svg",},    // 11
        {   ":/icons/Lm1000Hz_on.svg",  ":/icons/Lm1000Hz_off.svg",},   // 12
        {   ":/icons/Lm500Hz_on.svg",   ":/icons/Lm500Hz_off.svg",},    // 13
        {   ":/icons/LmEL_on.svg",      ":/icons/LmEL_off.svg",},       // 14
        {   ":/icons/LmUe_on.svg",      ":/icons/LmUe_off.svg",},       // 15
        {   ":/icons/ST_01.svg",        ":/icons/ST_01.svg",},          // 16 Forced brake
        {   ":/icons/LmEnde_on.svg",     ":/icons/LmEnde_off.svg",},    // 17
        {   ":/icons/LmGnt_on.svg",     ":/icons/LmGnt_off.svg",},      // 18
        {   ":/icons/LmUe_Gnt_on.svg",     ":/icons/LmUe_Gnt_off.svg",},// 19
        {   ":/icons/LmG_Gnt_on.svg",     ":/icons/LmG_Gnt_off.svg",},  // 20
        {   ":/icons/LmS_Gnt_on.svg",     ":/icons/LmS_Gnt_off.svg",},  // 21
        {   ":/icons/Lm95_on.svg",      ":/icons/Lm95_off.svg",},       // 22
        {   ":/icons/Lm75_on.svg",      ":/icons/Lm75_off.svg",},       // 23
        {   ":/icons/Lm60_on.svg",      ":/icons/Lm60_off.svg",},       // 24
        {   ":/icons/LmIndusi_on.svg",  ":/icons/LmIndusi_on.svg",},    // 25
    };
}
#endif // DB_H

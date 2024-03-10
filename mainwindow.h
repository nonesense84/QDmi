#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QFontDatabase>
#include <QScreen>
#include <QTimer>
#include <QSettings>
#include <QMouseEvent>
#include <QDateTime>
#include <QNetworkInterface>
#include <QStandardItemModel>
#include "era.h"
#include "lzb.h"
#include "sep.h"
#include "mtd.h"
#include "zusi3tcp.h"
#include "zusitraindata.h"
#include "alphanumericinput.h"

#ifdef Q_OS_ANDROID
#include <QtAndroid>
#include <QAndroidJniEnvironment>
#endif

#define P_ABE_DefaultWindow  0
#define P_ABE_DataEntry      1
#define P_DG_DefaultWindow   0
#define P_DG_Settings        1
#define P_DG_System_Version  5
#define P_DG_QDmi            6
#define P_DG_Data_entry      7
#define P_DG_Main_menu       8
#define P_DG_Override_menu   9

#define P_entry_ZusiIp       0
#define P_entry_TrainData1   1
#define P_entry_TrainData2   2
#define P_Entry_Button_YesNo 3
#define P_entry_DriverId     4
#define P_entry_TrainNumber  5
#define P_entry_Level        6

#define P_Keyboard_Numeric          0
#define P_Keyboard_Alphanumeric     1
#define P_Keyboard_ValidYesNo       2
#define P_Keyboard_brakingRegimePzb 3
#define P_Keyboard_brakingRegimeLzb 4
#define P_Keyboard_TrainCategory    5
#define P_Keyboard_yesNo            6
#define P_Keyboard_axleLoad         7
#define P_Keyboard_loadingGauge     8
#define P_Keyboard_level            9

#define P_entry_complete  0
#define P_entry_not_complete  1

#define P_TrnSettingsAndPage_NonOfThem  0
#define P_TrnSettingsAndPage_TrnSettings  1
#define P_TrnSettingsAndPage_Page  2

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

//#define levelUndefined  10
#define levelPzbLzbNtc  6
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    quint8  showManometer = 1;

private slots:
    void setKeyboardType(quint8 type);
    void setKeyboardType(quint8 type, bool showDott);
    void TdeCompeteClicked();
    void setEtcsTexs(QStringList timeStamps, QStringList messages);
    void srollTextMessages(qint8 scroll);
    void openMainMenu();
    void fieldF2Clicked();
    void fieldF3Clicked();
    void fieldF4Clicked();
    void openSettings();
  //void arrowUpClicked();
  //void arrowDownClicked();
    void connectTimers();
    void process();
    void connectPzbIcons();
    void connectMtdIcons();
    void connectMtdPower();
    void connectTcpStuff();
    void gotTcpConnectionFeedback(QString feedback);
    void setEvcPresent(bool present);
    void setLevel(quint16 level);
    void swapTdePage(bool seccondPage);
    void openTrainDataEntry();
    void cabActivation(bool cabActivated, bool standstill);
    void openDriverIdEntry();
    void openTrainRunnimgNumberEntry();
    void openLevelSelection();
    void settingsBtn1Clicked();
    void settingsBtn2Clicked();
    void settingsBtn3Clicked();
    void openSystemVersionInfo();
    void openQDmiSettings();
    void openNetworkSettings();
    void enableTde(bool enable);
    void anableOverride(bool enable);
    void closeQDmi();
    void settingsBtn8Clicked();
    void geoPositionClicked();
    void setGeoPosition(qint32 geoPosition);
    void settingsCloseClicked();
    void addItemToData(QString item);
    void blinkCursor();
    void setEntryStrWithMaxLength(quint8 length, QString startString);
    void applyClicked(QString data, bool enabled);
    void applySettings();
    void configureSettingsWindow();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    #ifdef Q_OS_ANDROID
    void keepScreenOn();
    #endif
    void resizeMe();

private:
    QSettings *settings;
    Ui::MainWindow *ui;
    quint8 position;
    QTimer *shortTimer;
    lzb *myLzb;
    sep *mySep;
    mtd *myMtd;
    zusi3Tcp *myTcp;
    zusiTraindata *myZusiTrainData;
    alphaNumericInput *myDriverId;
    QThread* lzbThread = new QThread;
    QThread* sepThread = new QThread;
    QThread* mtdThread = new QThread;
    QThread* tcpThread = new QThread;
    bool useDistEraForLzb;
    bool useHookForLzb;
    bool useTextFromPzb;
  //bool MessaesOutOffView5to9 = false;
  //bool MessaesOutOffView8to9 = false;
    quint8 indexOffsetTextmessage = 0;
    quint8 numTextmessage = 0;
    QPoint lKilickPos;
    QString dataString = "";
    quint8 maxEntryStrLength;
    quint8 actKeyboardType = 0;
    QString activeDataEntryItem = "";
    uint16_t  actLevel = 0;
    bool evcPresent = false;

protected:
  void resizeEvent ( QResizeEvent * event );
  #ifdef Q_PROCESSOR_ARM
  void keyPressEvent(QKeyEvent *event);
  #endif

signals:
    void newDotsPerInch(qreal dpi);
    void newManometerUse(bool use);
    void tcpConnectionSettings(quint8 settings);
    void newZusiIp(QString ip);
    void newDriverId(QString number);
    void newTrainRunningNumber(QString number);
    void newLevelSelection(QString level);
    void newTextMessagesSettings(quint8  setting);
    void naivationArrowClick(qint8 direction);
};

#endif // MAINWINDOW_H

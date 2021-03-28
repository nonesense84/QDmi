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
#include "era.h"
#include "lzb.h"
#include "sep.h"
#include "mtd.h"
#include "zusi3tcp.h"
#ifdef Q_OS_ANDROID
#include <QtAndroid>
#include <QAndroidJniEnvironment>
#endif

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

#define modePzbLzbNtc  1
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    quint8  showManometer = 1;

private slots:
    void messaesOutOfViewHandling5to9(bool outOfView);
    void messaesOutOfViewHandling8to9(bool outOfView);
    void fieldF1Clicked();
    void fieldF4Clicked();
    void fieldF5Clicked();
    void arrowUpClicked();
    void arrowDownClicked();
    void connectTimers();
    void process();
    void connectPzbIcons();
    void connectMtdIcons();
    void connectMtdPower();
    void connectTcpStuff();
    void gotTcpConnectionFeedback(QString feedback);
    void setPzbLzbNtc();
    void settingsBtn1Clicked();
    void settingsBtn2Clicked();
    void settingsBtn3Clicked();
    void settingsBtn4Clicked();
    void settingsBtn5Clicked();
    void settingsBtn6Clicked();
    void settingsBtn7Clicked();
    void settingsBtn8Clicked();
    void settingsCloseClicked();
    void addItemToData(QString item);
    void blinkCursor();
    void applyClicked(QString data);
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
    QThread* lzbThread = new QThread;
    QThread* sepThread = new QThread;
    QThread* mtdThread = new QThread;
    QThread* tcpThread = new QThread;
    bool useDistEraForLzb;
    bool useHookForLzb;
    bool useTextFromPzb;
    bool MessaesOutOffView5to9 = false;
    bool MessaesOutOffView8to9 = false;
    QPoint lKilickPos;
    QString dataString = "";
    QString activeDataEntryItem = "";
    uint8_t  mode = 0;

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
    void newTextMessagesSettings(quint8  setting);
    void naivationArrowClick(qint8 direction);
};

#endif // MAINWINDOW_H

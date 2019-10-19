#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFontDatabase>
#include <QScreen>
#include <QTimer>
#include <QSettings>
#include <QMouseEvent>
#include <QDateTime>
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

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void arrowF4Clicked();
    void arrowF5Clicked();
    void arrowUpClicked();
    void arrowDownClicked();
    void connectTimers();
    void connectPzbIcons();
    void connectMtdIcons();
    void connectMtdPower();
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
    void configureSettingsWindow();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keepScreenOn();

private:
    QSettings *settings;
    Ui::MainWindow *ui;
    quint8 position;
    QTimer *longTimer;
    QTimer *shortTimer;
    lzb *myLzb;
    sep *mySep;
    mtd *myMtd;
    zusi3Tcp *myTcp;
    bool useDistEraForLzb;
    bool useHookForLzb;
    bool useTextFromPzb;
    QPoint lKilickPos;
    QString dataString = "";
    QString activeDataEntryItem = "";

protected:
  void resizeEvent ( QResizeEvent * event );
};

#endif // MAINWINDOW_H

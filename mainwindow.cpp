#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    #ifdef Q_PROCESSOR_ARM
  //QSettings::setPath(QSettings::IniFormat, QSettings::SystemScope, ".");
    #endif
    settings = new QSettings("QDmi", "QDmi");
  //settings->setPath(QSettings::IniFormat, QSettings::SystemScope, ".");
    this->setGeometry(0,0,settings->value("mainwindow/width").toInt(),settings->value("mainwindow/height").toInt());
  //this->setGeometry(0,0,768,480); // For 4:3 800*600. For 16:9 848*480. For 16:10 768*480
  //showFullScreen();
    #ifdef Q_OS_ANDROID
    QTimer::singleShot(1000,this,SLOT(showFullScreen()));
    QTimer::singleShot(500,this,SLOT(keepScreenOn()));
    #endif
    qApp->setProperty("borderThickness", 1);
    myLzb = new lzb();
    mySep = new sep();
    myMtd = new mtd();
    myTcp = new zusi3Tcp();
    myZusiTrainData = new zusiTraindata();
    myDriverId = new alphaNumericInput();
    // https://wiki.qt.io/QThreads_general_usage
    // https://mayaposch.wordpress.com/2011/11/01/how-to-really-truly-use-qthreads-the-full-explanation/
    myLzb->moveToThread(lzbThread);
    mySep->moveToThread(sepThread);
    myMtd->moveToThread(mtdThread);
    myTcp->moveToThread(tcpThread);
    connect(tcpThread, &QThread::started, myTcp, &zusi3Tcp::initialize);
    connect(tcpThread, &QThread::started, this, &MainWindow::initialize);
    connect(sepThread, &QThread::started, mySep, &sep::initialize);
    lzbThread->start();
    sepThread->start();
    mtdThread->start();
    tcpThread->start();
}
void MainWindow::reset(){
    ui->widgetTacho = new gauge();

}
void MainWindow::initialize(){
    qreal dotsPerInch = QApplication::screens().at(0)->logicalDotsPerInch();
    ui->widgetTacho->setDpi(dotsPerInch);
    ui->widgetPower->setDpi(dotsPerInch);
    ui->widgetPower->setDpi(dotsPerInch);
    ui->widgetMano1->setDpi(dotsPerInch);
    ui->widgetMano2->setDpi(dotsPerInch);
    ui->FieldELzbTexts->setTextFieldUsing(3);       // Bring PZB/LZB Inidicators in front.
    ui->fieldB6->setCustomFontFactor(0.45,Qt::AlignCenter, QFont::Thin);
    ui->settingsTitel->setFrameless();
    ui->settingsTitel->addTextLzbMessage(" Einstellungen",era::grey,era::black,0);
    ui->mainTitel->setFrameless();
    ui->mainTitel->addTextLzbMessage(" Hauptmenü",era::grey,era::black,0);
    ui->qdmiSettingsTitel->setFrameless();
    ui->qdmiSettingsTitel->addTextLzbMessage(" QDmi Einstellungen",era::grey,era::black,0);
    ui->zusiIpTitel->setFrameless();
    ui->zusiIpTitel->addTextLzbMessage(" Zusi 3 IP-Adresse",era::grey,era::black,1);
    ui->driverIdTitel->setFrameless();
    ui->driverIdTitel->addTextLzbMessage(" Triebfahrzeugführernummer",era::grey,era::black,1);
    ui->trainRunningNumberTitel->setFrameless();
    ui->trainRunningNumberTitel->addTextLzbMessage(" Zugnummer",era::grey,era::black,1);

    ui->levelTitel->setFrameless();
    ui->levelTitel->addTextLzbMessage(" Level",era::grey,era::black,1);
    ui->overrideTitel->setFrameless();
    ui->overrideTitel->addTextLzbMessage(" Override",era::grey,era::black,1);

    ui->dataEntryCompleteQuestion->setFrameless();
    ui->dataEntryCompleteQuestion->setTextFieldUsing(1, Qt::AlignCenter);
    ui->dataEntryCompleteQuestion->addTextLzbMessage("Zugdateneingabe vollständig?",era::grey, era::darkBlue,1);

    ui->trainDataTitel1->setTextFieldUsing(1, Qt::AlignRight);
    ui->trainDataTitel2->setTextFieldUsing(1, Qt::AlignRight);
    ui->trainDataTitel1->addTextLzbMessage("Zugdateneingabe ",era::grey,era::black,1);
    ui->trainDataTitel1->addTextLzbMessage("Zugdateneingabe (1/2) ",era::grey,era::black,2);
    ui->trainDataTitel1->addTextLzbMessage("Zugdateneingabe (2/2) ",era::grey,era::black,3);
    ui->trainDataTitel2->addTextLzbMessage("Zugdaten bestätigen ",era::grey,era::black,1);

    ui->trainDataTitel1->setFrameless();
    ui->trainDataTitel2->setFrameless();
    ui->dataValidLabel0->setFrameless();
    ui->dataValidLabel1->setFrameless();
    ui->dataValidLabel2->setFrameless();
    ui->dataValidLabel3->setFrameless();
    ui->dataValidLabel4->setFrameless();
    ui->dataValidLabel5->setFrameless();
    ui->dataValidLabel6->setFrameless();
    ui->dataValidLabel7->setFrameless();
    ui->dataValidLabel8->setFrameless();
    ui->dataValidLabel9->setFrameless();
    ui->dataValidLabel10->setFrameless();
    ui->dataValidLabel11->setFrameless();
    ui->dataValidLabel12->setFrameless();
    ui->dataValidLabel0->setCustomFontFactor( 0.5,Qt::AlignRight);
    ui->dataValidLabel1->setCustomFontFactor( 0.4,Qt::AlignRight);
    ui->dataValidLabel2->setCustomFontFactor( 0.4,Qt::AlignRight);
    ui->dataValidLabel3->setCustomFontFactor( 0.4,Qt::AlignRight);
    ui->dataValidLabel4->setCustomFontFactor( 0.4,Qt::AlignRight);
    ui->dataValidLabel5->setCustomFontFactor( 0.5,Qt::AlignRight);
    ui->dataValidLabel6->setCustomFontFactor( 0.4,Qt::AlignRight);
    ui->dataValidLabel7->setCustomFontFactor( 0.4,Qt::AlignRight);
    ui->dataValidLabel8->setCustomFontFactor( 0.4,Qt::AlignRight);
    ui->dataValidLabel9->setCustomFontFactor( 0.4,Qt::AlignRight);
    ui->dataValidLabel10->setCustomFontFactor(0.4,Qt::AlignRight);
    ui->dataValidLabel11->setCustomFontFactor(0.4,Qt::AlignRight);
    ui->dataValidLabel12->setCustomFontFactor(0.4,Qt::AlignRight);
    ui->dataValidLabel0->setText("PZB / LZB", era::white, era::white, QFont::Bold);
    ui->dataValidLabel1->setText("Bremsart ");
    ui->dataValidLabel2->setText("Bremshundertstel ");
    ui->dataValidLabel3->setText("Zuglänge ");
    ui->dataValidLabel4->setText("Höchstgeschwindigkeit ");

    ui->dataValidLabel5->setText("EVC", era::white, era::white, QFont::Bold);
    ui->dataValidLabel6->setText("Bremshundertstel ");
    ui->dataValidLabel7->setText("Zuglänge ");
    ui->dataValidLabel8->setText("Höchstgeschwindigkeit ");
    ui->dataValidLabel9->setText("Zugkategorie ");
    ui->dataValidLabel10->setText("Achslaskkategorie ");
    ui->dataValidLabel11->setText("Luftdicht ");
    ui->dataValidLabel12->setText("Lademaß ");


    ui->dataValidValue0->setFrameless();
    ui->dataValidValue1->setFrameless();

    ui->dataValidValue2->setFrameless();
    ui->dataValidValue3->setFrameless();
    ui->dataValidValue4->setFrameless();
    ui->dataValidValue5->setFrameless();
    ui->dataValidValue6->setFrameless();
    ui->dataValidValue7->setFrameless();
    ui->dataValidValue8->setFrameless();
    ui->dataValidValue9->setFrameless();
    ui->dataValidValue10->setFrameless();
    ui->dataValidValue11->setFrameless();
    ui->dataValidValue12->setFrameless();
    ui->dataValidValue1->setCustomFontFactor(0.5,Qt::AlignLeft);
    ui->dataValidValue2->setCustomFontFactor(0.5,Qt::AlignLeft);
    ui->dataValidValue3->setCustomFontFactor(0.5,Qt::AlignLeft);
    ui->dataValidValue4->setCustomFontFactor(0.5,Qt::AlignLeft);
    ui->dataValidValue5->setCustomFontFactor(0.4,Qt::AlignLeft);
    ui->dataValidValue6->setCustomFontFactor(0.4,Qt::AlignLeft);
    ui->dataValidValue7->setCustomFontFactor(0.4,Qt::AlignLeft);
    ui->dataValidValue8->setCustomFontFactor(0.4,Qt::AlignLeft);
    ui->dataValidValue9->setCustomFontFactor(0.4,Qt::AlignLeft);
    ui->dataValidValue10->setCustomFontFactor(0.4,Qt::AlignLeft);
    ui->dataValidValue11->setCustomFontFactor(0.4,Qt::AlignLeft);
    ui->dataValidValue12->setCustomFontFactor(0.4,Qt::AlignLeft);
    ui->dataEntryCompleteButton->setCustomFontFactor(0.3, Qt::AlignCenter);
    ui->systemVersionTitel->setFrameless();
    ui->systemVersionTitel->addTextLzbMessage(" System Version",era::grey,era::black,1);
    ui->systemVersionComp1Name->setFrameless();
    ui->systemVersionComp1Name->setTextFieldUsing(1, Qt::AlignRight);
    ui->systemVersionComp1Name->addTextLzbMessage("QDmi ",era::grey,era::darkBlue,1);
    ui->systemVersionComp2Name->setFrameless();
    ui->systemVersionComp2Name->setTextFieldUsing(1, Qt::AlignRight);
    ui->systemVersionComp2Name->addTextLzbMessage("github.com/nones",era::grey,era::darkBlue,1);
    ui->systemVersionComp1Version->setFrameless();
    ui->systemVersionComp1Version->addTextLzbMessage("1.4.0",era::grey,era::darkBlue,1);
    ui->systemVersionComp2Version->setFrameless();
    ui->systemVersionComp2Version->addTextLzbMessage("ense84/QDmi",era::grey,era::darkBlue,1);
    qRegisterMetaType< QVector<quint8> >("QVector<quint8>");
    qRegisterMetaType< QVector<qint16> >("QVector<qint16>");
    connect(ui->fieldF1,    &dmiLabel::clicked, this, &MainWindow::openMainMenu);
    connect(ui->fieldF2,    &dmiLabel::clicked, this, &MainWindow::fieldF2Clicked);
    connect(ui->fieldF3,    &dmiLabel::clicked, this, &MainWindow::fieldF3Clicked);
    connect(ui->fieldF4,    &dmiLabel::clicked, this, &MainWindow::fieldF4Clicked);
    connect(ui->fieldF5,    &dmiLabel::clicked, this, &MainWindow::openSettings);
    connect(ui->fieldE10,   &dmiLabel::clicked, ui->FieldE8to9, &EtcsTextWidget::scrollUp);
    connect(ui->fieldE11,   &dmiLabel::clicked, ui->FieldE8to9, &EtcsTextWidget::scrollDown);
    connect(ui->FieldE8to9, &EtcsTextWidget::enableScrollUp, ui->fieldE10, &dmiLabel::setEnabled);
    connect(ui->FieldE8to9, &EtcsTextWidget::enableScrollDown, ui->fieldE11, &dmiLabel::setEnabled);
    connect(ui->settingsBtn1, &dmiLabel::clicked, this, &MainWindow::settingsBtn1Clicked);
    connect(ui->settingsBtn2, &dmiLabel::clicked, this, &MainWindow::settingsBtn2Clicked);
    connect(ui->settingsBtn3, &dmiLabel::clicked, this, &MainWindow::settingsBtn3Clicked);
    connect(ui->settingsBtn4, &dmiLabel::clicked, this, &MainWindow::openSystemVersionInfo);
    connect(ui->settingsBtn5, &dmiLabel::clicked, this, &MainWindow::openQDmiSettings);
    connect(ui->settingsBtn6, &dmiLabel::clicked, this, &MainWindow::openNetworkSettings);
    connect(ui->settingsBtn7, &dmiLabel::clicked, this, &MainWindow::closeQDmi);
    connect(ui->settingsBtn8, &dmiLabel::clicked, this, &MainWindow::settingsBtn8Clicked);
    connect(ui->dataEntryPrevPage, &dmiLabel::clicked, this, &MainWindow::setFirstTdePage);
    connect(ui->dataEntryNextPage, &dmiLabel::clicked, this, &MainWindow::setSecondTdePage);
    connect(ui->mainBtn1, &dmiLabel::clicked, myTcp, &zusi3Tcp::sendEtcsStart);
    connect(ui->mainBtn2, &dmiLabel::clicked, this, &MainWindow::openDriverIdEntry);
    connect(ui->mainBtn3, &dmiLabel::clicked, this, &MainWindow::openTrainDataEntry);
    connect(ui->mainBtn5, &dmiLabel::clicked, this, &MainWindow::openLevelSelection);
    connect(ui->mainBtn6, &dmiLabel::clicked, this, &MainWindow::openTrainRunnimgNumberEntry);
    connect(ui->fieldG12_geopos_H, &dmiLabel::clicked, this, &MainWindow::geoPositionClicked);
    connect(ui->fieldG12_geopos_T, &dmiLabel::clicked, this, &MainWindow::geoPositionClicked);
    connect(ui->fieldG12_btn, &dmiLabel::clicked, this, &MainWindow::geoPositionClicked);
    ui->fieldG12_btn->setWorking(false, false, false);
    ui->fieldG12_geopos_T->setUnclosedFrame(false, true);
    ui->fieldG12_geopos_H->setUnclosedFrame(true, false);
    ui->fieldG12_geopos_H->setCustomFontFactor(0.22, Qt::AlignLeft);
    ui->fieldG12_geopos_T->setCustomFontFactor(0.3,  Qt::AlignRight);
    connect(ui->settingsClose,      &dmiLabel::clicked, this, &MainWindow::settingsCloseClicked);
    connect(ui->qdmiSettingsClose,  &dmiLabel::clicked, this, &MainWindow::settingsCloseClicked);
    connect(ui->mainClose,          &dmiLabel::clicked, this, &MainWindow::settingsCloseClicked);
    connect(ui->overrideClose,      &dmiLabel::clicked, this, &MainWindow::settingsCloseClicked);
    connect(ui->fieldC1,            &dmiLabel::clicked, myTcp, &zusi3Tcp::sendEtcsAck);
    connect(ui->fieldC1l,           &dmiLabel::clicked, myTcp, &zusi3Tcp::sendEtcsAck);


    connect(ui->dataEntryCompleteButton, &dmiLabel::clicked, this,               &MainWindow::TdeCompeteClicked);
    connect(ui->dataValidButton,  &dmiLabel::txtBtnClicked, myZusiTrainData,     &zusiTraindata::dataValidButtonPressed);
    connect(ui->dataValidBtnNo,   &dmiLabel::txtBtnClicked, ui->dataValidButton, static_cast<void (dmiLabel::*)(QString)>(&dmiLabel::setAsButton));
    connect(ui->dataValidBtnYes,  &dmiLabel::txtBtnClicked, ui->dataValidButton, static_cast<void (dmiLabel::*)(QString)>(&dmiLabel::setAsButton));
    connect(ui->levelBtn1,        &dmiLabel::txtBtnClicked, ui->levelOkBtn,      static_cast<void (dmiLabel::*)(QString)>(&dmiLabel::setAsButton));
    connect(ui->levelBtn2,        &dmiLabel::txtBtnClicked, ui->levelOkBtn,      static_cast<void (dmiLabel::*)(QString)>(&dmiLabel::setAsButton));
    connect(ui->levelBtn4,        &dmiLabel::txtBtnClicked, ui->levelOkBtn,      static_cast<void (dmiLabel::*)(QString)>(&dmiLabel::setAsButton));
    connect(ui->levelBtn5,        &dmiLabel::txtBtnClicked, ui->levelOkBtn,      static_cast<void (dmiLabel::*)(QString)>(&dmiLabel::setAsButton));
    connect(ui->dataValidBtnYes,  &dmiLabel::txtBtnClicked, ui->levelOkBtn,      static_cast<void (dmiLabel::*)(QString)>(&dmiLabel::setAsButton));
    connect(ui->overrideBtn1,     &dmiLabel::clicked,       myTcp,               &zusi3Tcp::sendEtcsOverride);
    connect(ui->dataEntryClose,   &dmiLabel::clicked,       myZusiTrainData,     &zusiTraindata::finalizeDataEntry);
    connect(ui->numBtn0,          &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->numBtn1,          &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->numBtn2,          &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->numBtn3,          &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->numBtn4,          &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->numBtn5,          &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->numBtn6,          &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->numBtn7,          &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->numBtn8,          &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->numBtn9,          &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->numBtn10,         &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->numBtn11,         &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->alphaBtn0,        &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->alphaBtn1,        &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->alphaBtn2,        &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->alphaBtn3,        &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->alphaBtn4,        &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->alphaBtn5,        &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->alphaBtn6,        &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->alphaBtn7,        &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->alphaBtn8,        &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->alphaBtn9,        &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->alphaBtn10,       &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->alphaBtn11,       &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->braBtn1,          &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->braBtn2,          &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->braBtn3,          &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->braBtn4,          &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->braBtn6,          &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->braBtn8,          &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->braBtn9,          &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->noBtn,            &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->yesBtn,           &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->categoryBtn1,     &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->categoryBtn2,     &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->categoryBtn3,     &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->categoryBtn4,     &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->categoryBtn5,     &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->categoryBtn6,     &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->categoryBtn7,     &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->categoryBtn8,     &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->categoryBtn9,     &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->categoryBtn10,    &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->categoryBtn11,    &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->categoryBtn12,    &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->axleLoadBtn1,     &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->axleLoadBtn2,     &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->axleLoadBtn3,     &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->axleLoadBtn4,     &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->axleLoadBtn5,     &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->axleLoadBtn6,     &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->axleLoadBtn7,     &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->axleLoadBtn8,     &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->axleLoadBtn9,     &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->axleLoadBtn10,    &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->axleLoadBtn11,    &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->axleLoadBtn12,    &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->loadingGaugeBtn1, &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->loadingGaugeBtn2, &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->loadingGaugeBtn3, &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->loadingGaugeBtn4, &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);
    connect(ui->loadingGaugeBtn5, &dmiLabel::txtBtnClicked, this, &MainWindow::addItemToData);

    connect(ui->dataEntryClose,          &dmiLabel::clicked,             this, &MainWindow::settingsCloseClicked);
    connect(ui->dataEntrySettings,       &dmiLabel::clicked,             this, &MainWindow::openSettings);
    connect(ui->dataEntryTrn,            &dmiLabel::clicked,             this, &MainWindow::openTrainRunnimgNumberEntry);
    connect(ui->zusiIpOkBtn,             &dmiLabel::dataEntryBtnClicked, this, &MainWindow::applyClicked);
    connect(ui->driverIdOkBtn,           &dmiLabel::dataEntryBtnClicked, this, &MainWindow::applyClicked);
    connect(ui->trainRunningNumberOkBtn, &dmiLabel::dataEntryBtnClicked, this, &MainWindow::applyClicked);
    connect(ui->levelOkBtn,              &dmiLabel::dataEntryBtnClicked, this, &MainWindow::applyClicked);

    connect(ui->systemVersionClose, &dmiLabel::clicked,              this,               &MainWindow::settingsCloseClicked);
    connect(mySep,                  &sep::newLzbIndicators,          myLzb,              &lzb::setIndicators);
    connect(myTcp,                  &zusi3Tcp::newLzbIndicators,     myLzb,              &lzb::setIndicators);
    connect(myLzb,                  &lzb::newLzbTextMessage,         ui->FieldELzbTexts, &dmiLabel::addTextLzbMessage);
    connect(myLzb,                  &lzb::removeMessage,             ui->FieldELzbTexts, &dmiLabel::removeTextMessage);
    connect(myTcp->myIndicators,    &zusiIndicator::newAfbSoll,      ui->widgetTacho,    &gauge::setVSet);
    connect(mySep,                  &sep::newAfbSoll,                ui->widgetTacho,    &gauge::setVSet);
    connect(mySep,                  &sep::newFzgVmaxTacho,           ui->widgetTacho,    &gauge::setVMaxDial);
    connect(myTcp->myIndicators,    &zusiIndicator::newFzgVmaxTacho, ui->widgetTacho,    &gauge::setVMaxDial);
    connect(mySep,                  &sep::newLzbValues,              myLzb,              &lzb::setAnalogValues);
    connect(myTcp,                  &zusi3Tcp::newLzbValues,         myLzb,              &lzb::setAnalogValues);
    connect(mySep,                  &sep::newMtdIndicators,          myMtd,              &mtd::setStates);
    connect(myTcp,                  &zusi3Tcp::newMtdIndicators,     myMtd,              &mtd::setStates);
    connect(mySep,                  &sep::newSpeed,                  ui->widgetTacho,    &gauge::setVAct);
    connect(myTcp,                  &zusi3Tcp::newSpeed,             ui->widgetTacho,    &gauge::setVAct);
    connect(mySep,                  &sep::newSpeed,                  myLzb,              &lzb::setVAct);
    connect(myTcp,                  &zusi3Tcp::newSpeed,             myLzb,              &lzb::setVAct);
    connect(myLzb,                  &lzb::speedGt0,                  this,               &MainWindow::enableTde);
    connect(myLzb,                  &lzb::speedGt40,                 this,               &MainWindow::anableOverride);
    connect(myTcp,                  &zusi3Tcp::newHlb,               ui->widgetMano1,    &manometer::setPressure1);
    connect(myTcp,                  &zusi3Tcp::newHll,               ui->widgetMano1,    &manometer::setPressure2);
    connect(myTcp,                  &zusi3Tcp::newBrz,               ui->widgetMano2,    &manometer::setPressure1);
    connect(mySep,                  &sep::newHlb,                    ui->widgetMano1,    &manometer::setPressure1);
    connect(mySep,                  &sep::newHll,                    ui->widgetMano1,    &manometer::setPressure2);
    connect(mySep,                  &sep::newBrz,                    ui->widgetMano2,    &manometer::setPressure1);
    connect(myTcp,                  &zusi3Tcp::newKilometrierung,    this,               &MainWindow::setGeoPosition);
    connect(mySep,                  &sep::newGeoPos,                 this,               &MainWindow::setGeoPosition);
    connect(myTcp,                  &zusi3Tcp::newLzbBra,            myZusiTrainData,    &zusiTraindata::setActiveBra);
    connect(myTcp,                  &zusi3Tcp::newLzbBrh,            myZusiTrainData,    &zusiTraindata::setActiveBrh);
    connect(myTcp,                  &zusi3Tcp::newLzbZl,             myZusiTrainData,    &zusiTraindata::setActiveZl);
    connect(myTcp,                  &zusi3Tcp::newLzbVmz,            myZusiTrainData,    &zusiTraindata::setActiveVmz);
    connect(myTcp,                  &zusi3Tcp::changedTrain,         myZusiTrainData,    &zusiTraindata::finalizeDataEntry);
    connect(myTcp,                  &zusi3Tcp::changedTrain,         this,               &MainWindow::reset);
    connect(myTcp,                  &zusi3Tcp::changedTrain,         ui->widgetTacho,    &gauge::reset);
    connect(myTcp, &zusi3Tcp::newLzbBra, ui->dataValidValue1, &dmiLabel::setSegmentText);
    connect(myTcp, &zusi3Tcp::newLzbBrh, ui->dataValidValue2, &dmiLabel::setSegmentText);
    connect(myTcp, &zusi3Tcp::newLzbZl,  ui->dataValidValue3, &dmiLabel::setSegmentText);
    connect(myTcp, &zusi3Tcp::newLzbVmz, ui->dataValidValue4, &dmiLabel::setSegmentText);
    connect(myTcp, &zusi3Tcp::newEvcBrh, ui->dataValidValue6, static_cast<void (dmiLabel::*)(quint16)>(&dmiLabel::setText));
    connect(myTcp, &zusi3Tcp::newEvcZl,  ui->dataValidValue7, static_cast<void (dmiLabel::*)(quint16)>(&dmiLabel::setText));
    connect(myTcp, &zusi3Tcp::newEvcVmz, ui->dataValidValue8, static_cast<void (dmiLabel::*)(quint16)>(&dmiLabel::setText));

    connect(ui->widgetTacho,     &gauge::activatedSpeedHooks,                ui->fieldA3,         &targetDistance::setDigitalInOs);
    connect(ui->fieldA3,         &targetDistance::activatedDigitalInOs,      ui->widgetTacho,     &gauge::setBasicSpeedHooks);


    connect(myTcp->myEtcs,       &zusi3etcs::newActiveTct,                   ui->dataValidValue9, static_cast<void (dmiLabel::*)(QString)>(&dmiLabel::setText));
    connect(myTcp->myEtcs,       &zusi3etcs::newActiveAxl,                   ui->dataValidValue10,static_cast<void (dmiLabel::*)(QString)>(&dmiLabel::setText));
    connect(myTcp->myEtcs,       &zusi3etcs::newActiveAit,                   ui->dataValidValue11,static_cast<void (dmiLabel::*)(QString)>(&dmiLabel::setText));
    connect(myTcp->myEtcs,       &zusi3etcs::newActiveLdg,                   ui->dataValidValue12,static_cast<void (dmiLabel::*)(QString)>(&dmiLabel::setText));
    connect(myTcp->myEtcs,       &zusi3etcs::newTextMessages,                ui->FieldE8to9,      &EtcsTextWidget::setEtcsTextMessages);
    connect(myTcp,               &zusi3Tcp::newTextMessages,                 ui->FieldE8to9,      &EtcsTextWidget::setEtcsTextMessages);  
    connect(ui->FieldE8to9,      &EtcsTextWidget::sendAckEtcsTextMessage,    myTcp,               &zusi3Tcp::sendAckEtcsTextMessage);
    connect(myTcp,               &zusi3Tcp::newCabActive,                    this,                &MainWindow::cabActivation);
    connect(myTcp,               &zusi3Tcp::newTastaturkommando,             myZusiTrainData,     &zusiTraindata::setKeycommand);
    connect(myLzb,               &lzb::lzbAvailable,                         this,                &MainWindow::setLzbPresent);

    connect(myTcp->myEtcs,       &zusi3etcs::newBehavBtnStart,               ui->mainBtn1,        &dmiLabel::setEnabled);
    connect(myTcp->myEtcs,       &zusi3etcs::newBehavBtnLevel,               ui->mainBtn5,        &dmiLabel::setEnabled);
    connect(myTcp->myEtcs,       &zusi3etcs::newBehavBtnShunting,            ui->mainBtn7,        &dmiLabel::setEnabled);
    connect(myTcp->myEtcs,       &zusi3etcs::newBehavBtnNonLeading,          ui->mainBtn8,        &dmiLabel::setEnabled);
    connect(myTcp->myEtcs,       &zusi3etcs::newBehavBtndriverId,            ui->mainBtn2,        &dmiLabel::setEnabled);
    connect(myTcp->myEtcs,       &zusi3etcs::newBehavBtnTde,                 ui->mainBtn3,        &dmiLabel::setEnabled);
    connect(myTcp->myEtcs,       &zusi3etcs::newBehavBtnTrn,                 ui->mainBtn6,        &dmiLabel::setEnabled);
  //connect(myTcp->myEtcs,       &zusi3etcs::newBehavBtnExitShunting,        TBD,                 &dmiLabel::setEnabled); // Button not aimplemented jet
  //connect(myTcp->myEtcs,       &zusi3etcs::newBehavBtnMaintainShunting,    ui->mainBtn9,        &dmiLabel::setEnabled); // Button not aimplemented jet
  //connect(myTcp->myEtcs,       &zusi3etcs::newBehavBtnRadio,               ui->mainBtn10,       &dmiLabel::setEnabled); // Button not aimplemented jet
    connect(myTcp->myEtcs,       &zusi3etcs::newBehavBtnEoa,                 ui->overrideBtn1,    &dmiLabel::setEnabled);

    connect(myZusiTrainData,     &zusiTraindata::newTraindataLzb,            myTcp,               &zusi3Tcp::setTrainDataLzb);
    connect(myZusiTrainData,     &zusiTraindata::newTraindataEtcs,           myTcp,               &zusi3Tcp::setTraindataEtcs);
    connect(myZusiTrainData,     &zusiTraindata::newBehavBraBtn,             ui->dataEntryInput1, &dmiLabel::setAsDataEntryLabel);
    connect(myZusiTrainData,     &zusiTraindata::newBehavBrhBtn,             ui->dataEntryInput2, &dmiLabel::setAsDataEntryLabel);
    connect(myZusiTrainData,     &zusiTraindata::newBehavZlBtn,              ui->dataEntryInput3, &dmiLabel::setAsDataEntryLabel);
    connect(myZusiTrainData,     &zusiTraindata::newBehavVmzBtn,             ui->dataEntryInput4, &dmiLabel::setAsDataEntryLabel);
    connect(myZusiTrainData,     &zusiTraindata::newBehavTctBtn,             ui->dataEntryInput5, &dmiLabel::setAsDataEntryLabel);
    connect(myZusiTrainData,     &zusiTraindata::newBehavAxlBtn,             ui->dataEntryInput6, &dmiLabel::setAsDataEntryLabel);
    connect(myZusiTrainData,     &zusiTraindata::newBehavAitBtn,             ui->dataEntryInput7, &dmiLabel::setAsDataEntryLabel);
    connect(myZusiTrainData,     &zusiTraindata::newBehavLdgBtn,             ui->dataEntryInput8, &dmiLabel::setAsDataEntryLabel);
    connect(myZusiTrainData,     &zusiTraindata::newBehavZlLbl,              ui->dataEntryLabel3, &dmiLabel::setAsDataEntryLabel);
    connect(myZusiTrainData,     &zusiTraindata::newBehavVmzLbl,             ui->dataEntryLabel4, &dmiLabel::setAsDataEntryLabel);
    connect(myZusiTrainData,     &zusiTraindata::newBehavTctLbl,             ui->dataEntryLabel5, &dmiLabel::setAsDataEntryLabel);
    connect(myZusiTrainData,     &zusiTraindata::newBehavAxlLbl,             ui->dataEntryLabel6, &dmiLabel::setAsDataEntryLabel);
    connect(myZusiTrainData,     &zusiTraindata::newBehavAitLbl,             ui->dataEntryLabel7, &dmiLabel::setAsDataEntryLabel);
    connect(myZusiTrainData,     &zusiTraindata::newBehavLdgLbl,             ui->dataEntryLabel8, &dmiLabel::setAsDataEntryLabel);
    connect(myZusiTrainData,     &zusiTraindata::newBehavCmpltBtn,           ui->dataEntryCompleteButton, &dmiLabel::setAsDataEntryLabel);
    connect(myZusiTrainData,     &zusiTraindata::closeDataEntryWindow,       this,               &MainWindow::settingsCloseClicked);
    connect(myZusiTrainData,     &zusiTraindata::repeateDataEntry,           this,               &MainWindow::openTrainDataEntry);
    connect(myZusiTrainData,     &zusiTraindata::requestDataEntrStrInitials, this,               &MainWindow::setEntryStrWithMaxLength);
    connect(myZusiTrainData,     &zusiTraindata::requestSecondTdePage,       this,               &MainWindow::setTdePage);
    connect(myZusiTrainData,     &zusiTraindata::requestKeyboardLayout,      this,               static_cast<void (MainWindow::*)(quint8)>(&MainWindow::setKeyboardType));
    connect(ui->dataEntryInput1, &dmiLabel::dataEntryBtnClicked,             myZusiTrainData,    &zusiTraindata::setBra);
    connect(ui->dataEntryInput2, &dmiLabel::dataEntryBtnClicked,             myZusiTrainData,    &zusiTraindata::setBrh);
    connect(ui->dataEntryInput3, &dmiLabel::dataEntryBtnClicked,             myZusiTrainData,    &zusiTraindata::setZl);
    connect(ui->dataEntryInput4, &dmiLabel::dataEntryBtnClicked,             myZusiTrainData,    &zusiTraindata::setVmz);
    connect(ui->dataEntryInput5, &dmiLabel::dataEntryBtnClicked,             myZusiTrainData,    &zusiTraindata::setTct);
    connect(ui->dataEntryInput6, &dmiLabel::dataEntryBtnClicked,             myZusiTrainData,    &zusiTraindata::setAxl);
    connect(ui->dataEntryInput7, &dmiLabel::dataEntryBtnClicked,             myZusiTrainData,    &zusiTraindata::setAit);
    connect(ui->dataEntryInput8, &dmiLabel::dataEntryBtnClicked,             myZusiTrainData,    &zusiTraindata::setLdg);
    connect(ui->dataEntryCompleteButton, &dmiLabel::clicked,                 myZusiTrainData,    &zusiTraindata::TdeCompeteClicked);

    ui->widgetMano2->setPointer2using(false);
    ui->widgetMano1->setPointer1color(era::red);
    ui->widgetMano1->setPointer2color(era::yellow);
    ui->widgetMano2->setPointer1color(era::yellow);
    ui->widgetMano1->setPonter1Label("Hauptluftbehälter");
    ui->widgetMano1->setPonter2Label("Hauptluftleitung");
    ui->widgetMano2->setPonter1Label("Bremszylinder");
    ui->widgetMano2->setPressMaxDial(10);
    connect(myTcp, &zusi3Tcp::newSimTime,   ui->fieldG13, static_cast<void (dmiLabel::*)(QString)>(&dmiLabel::setText));
    connect(mySep, &sep::newSimTime,        ui->fieldG13, static_cast<void (dmiLabel::*)(QString)>(&dmiLabel::setText));
    connect(myTcp, &zusi3Tcp::newZugnummer, ui->fieldG11, static_cast<void (dmiLabel::*)(QString)>(&dmiLabel::setText));
    connect(mySep, &sep::newTrainnumber,    ui->fieldG11, static_cast<void (dmiLabel::*)(QString)>(&dmiLabel::setText));

    connect(mySep, &sep::tcpConnectionSettings,          myTcp, &zusi3Tcp::setAutoReconnect);
    connect(this,  &MainWindow::newManometerUse,         myTcp, &zusi3Tcp::setUseManometer);
    connect(this,  &MainWindow::tcpConnectionSettings,   myTcp, &zusi3Tcp::setAutoReconnect);
    connect(this,  &MainWindow::newTextMessagesSettings, myLzb, &lzb::setTextUsing);
    connect(this,  &MainWindow::newTextMessagesSettings, myTcp, &zusi3Tcp::setTextUsing);
    connect(this,  &MainWindow::newDriverId,             myTcp, &zusi3Tcp::setDriverId);
    connect(this,  &MainWindow::newTrainRunningNumber,   myTcp, &zusi3Tcp::setTrainRunningNumber);
    connect(this,  &MainWindow::newLevelSelection,       myTcp, &zusi3Tcp::sendEtcsLevel);

    connect(this,  &MainWindow::newDriverId,             myZusiTrainData, &zusiTraindata::setDriverId);
    connect(this,  &MainWindow::newTrainRunningNumber,   myZusiTrainData, &zusiTraindata::setTrainRunningNumber);

    ui->settingsBtn1->setIcon(":/icons/lang_ena.svg",":/icons/lang_dis.svg");
    ui->settingsBtn2->setIcon(":/icons/volume_ena.svg", ":/icons/volume_dis.svg");
    ui->settingsBtn3->setIcon(":/icons/bright_ena.svg", ":/icons/bright_dis.svg");
    ui->settingsBtn5->setIcon(":/icons/QDmi-Icon.svg", ":/icons/QDmi-Icon.svg");
    ui->settingsBtn6->setIcon(":/icons/netw_ena.svg", ":/icons/netw_dis.svg");
    ui->settingsBtn7->setIcon(":/icons/power_off.svg", ":/icons/power_off.svg");
    ui->fieldG12_btn->setIcon(":/icons/DR_03_ena.svg", ":/icons/DR_03_dis.svg");

    ui->dataEntryLabel1->setAsDataEntryLabel("Bremsart ");
    ui->dataEntryLabel2->setAsDataEntryLabel("Bremshundertstel ");
    ui->dataEntryLabel3->setAsDataEntryLabel("Zuglänge ");
    ui->dataEntryLabel4->setAsDataEntryLabel("Höchstgeschwindigkeit ");
    ui->dataEntryLabel5->setAsDataEntryLabel("Zugkategorie ");
    ui->dataEntryLabel6->setAsDataEntryLabel("Achslast ");
    ui->dataEntryLabel7->setAsDataEntryLabel("Luftdicht ");
    ui->dataEntryLabel8->setAsDataEntryLabel("Lademasse ");
    ui->dataEntryInput1->setAsDataEntryLabel("0",true);
    ui->dataEntryInput2->setAsDataEntryLabel("0",true);
    ui->dataEntryInput3->setAsDataEntryLabel("0",true);
    ui->dataEntryInput4->setAsDataEntryLabel("0",true);
    ui->dataEntryInput5->setAsDataEntryLabel("PASS 1",true);
    ui->dataEntryInput6->setAsDataEntryLabel("A",true);
    ui->dataEntryInput7->setAsDataEntryLabel("Nein",true);
    ui->dataEntryInput8->setAsDataEntryLabel("Out of GC",true);

    ui->dataEntryCompleteButton->setAsDataEntryLabelFg("Ja",true,false,false,true,true);
    ui->dataValidButton->setAsDataEntryLabelFg("Nein",true,true,false,true,true);
    ui->dataValidBtnNo->setAsButton(true, "Nein");
    ui->dataValidBtnYes->setAsButton(true, "Ja");
    ui->settingsClose->setIcon(":/icons/X.svg");
    ui->qdmiSettingsClose->setIcon(":/icons/X.svg");
    ui->mainClose->setIcon(":/icons/X.svg");
    ui->overrideClose->setIcon(":/icons/X.svg");
    ui->fieldE10->setIcon(":/icons/arrowUp_ena.svg",  ":/icons/arrowUp_dis.svg");
    ui->fieldE11->setIcon(":/icons/arrowDown_ena.svg",":/icons/arrowDown_dis.svg");
    ui->fieldE10->setAsButton();
    ui->fieldE11->setAsButton();
    ui->fieldE10->setWorking(true, false, false);
    ui->fieldE11->setWorking(true, false, false);
    ui->fieldC2->setVisib(false);
    ui->fieldC3->setVisib(false);
    ui->fieldC4->setVisib(false);
    ui->fieldC5->setVisib(false);
    ui->fieldC6->setVisib(false);
    ui->fieldC7->setVisib(false);
    ui->fieldF1->setAsButton(true, "Menü");
    ui->fieldF2->setAsButton(true);
    ui->fieldF2->setIcon(":/icons/override_ena.svg", ":/icons/override_dis.svg");
    ui->fieldF3->setAsButton(true);
    ui->fieldF4->setAsButton(true);
    ui->fieldF5->setAsButton();
    ui->fieldF5->setIcon(":/icons/tool.svg");
    ui->dataEntrySettings->setAsButton();
    ui->dataEntrySettings->setIcon(":/icons/tool_small.svg");
    ui->dataEntryTrn->setAsButton(true,"Zug");
    ui->dataEntryPrevPage->setAsButton(true);
    ui->dataEntryPrevPage->setWorking(false, true, true);
    ui->dataEntryNextPage->setAsButton(true);
    ui->dataEntryPrevPage->setIcon(":/icons/arrowLeft_ena.svg",  ":/icons/arrowLeft_dis.svg");
    ui->dataEntryNextPage->setIcon(":/icons/arrowRight_ena.svg",":/icons/arrowRight_dis.svg");
    ui->settingsBtn1->setAsButton(false);
    ui->settingsBtn2->setAsButton(false);
    ui->settingsBtn3->setAsButton(false);
    ui->settingsBtn4->setAsButton(true,"System version");
    ui->settingsBtn5->setAsButton();
    ui->settingsBtn6->setAsButton();
    ui->settingsBtn7->setAsButton();
    ui->settingsBtn8->setAsButton(false);
    ui->settingsClose->setAsButton();
    ui->qdmiSettingsClose->setAsButton();

    ui->mainBtn1->setAsButton(true, "Start");
    ui->mainBtn2->setAsButton(true, "TF-Nummer");
    ui->mainBtn3->setAsButton(true, "Zugdateneingabe");
    ui->mainBtn4->setAsButton(true, "Funktionsprüfung");
    ui->mainBtn5->setAsButton(true, "Level");
    ui->mainBtn6->setAsButton(true, "Zugnummer");
    ui->mainBtn7->setAsButton(true, "Rangieren");
    ui->mainBtn8->setAsButton(true, "Nicht führend");
    ui->mainBtn1->setCustomFontFactor(0.28);
    ui->mainBtn2->setCustomFontFactor(0.28);
    ui->mainBtn3->setCustomFontFactor(0.28);
    ui->mainBtn4->setCustomFontFactor(0.28);
    ui->mainBtn5->setCustomFontFactor(0.28);
    ui->mainBtn6->setCustomFontFactor(0.28);
    ui->mainBtn7->setCustomFontFactor(0.28);
    ui->mainBtn8->setCustomFontFactor(0.28);
    ui->mainClose->setAsButton();

    // Level buttons
    ui->levelBtn1->setAsButton(true, "Level 0");
    ui->levelBtn2->setAsButton(true, "Level 1");
    ui->levelBtn4->setAsButton(true, "Level 2");
    ui->levelBtn5->setAsButton(true, "PZB / LZB");
    // Override bunntons
    ui->overrideBtn1->setAsButton(true, "EOA");
    ui->overrideClose->setAsButton();

    // Numeric keyboard buttons
    ui->numBtn0->setAsButton(true, "0");
    ui->numBtn1->setAsButton(true, "1");
    ui->numBtn2->setAsButton(true, "2");
    ui->numBtn3->setAsButton(true, "3");
    ui->numBtn4->setAsButton(true, "4");
    ui->numBtn5->setAsButton(true, "5");
    ui->numBtn6->setAsButton(true, "6");
    ui->numBtn7->setAsButton(true, "7");
    ui->numBtn8->setAsButton(true, "8");
    ui->numBtn9->setAsButton(true, "9");
    ui->numBtn10->setAsButton(true, "Entf", "Del");
    ui->numBtn11->setAsButton(true, ".");

    ui->alphaBtn0->setAsButton(true, "0");
    ui->alphaBtn1->setAsButton(true, "1");
    ui->alphaBtn2->setIcon(":/icons/key_2abc.svg",  ":/icons/key_2abc.svg");
    ui->alphaBtn2->setAsButton(true, "", "2");
    ui->alphaBtn3->setIcon(":/icons/key_3def.svg",  ":/icons/key_3def.svg");
    ui->alphaBtn3->setAsButton(true, "", "3");
    ui->alphaBtn4->setIcon(":/icons/key_4ghi.svg",  ":/icons/key_4ghi.svg");
    ui->alphaBtn4->setAsButton(true, "", "4");
    ui->alphaBtn5->setIcon(":/icons/key_5jkl.svg",  ":/icons/key_5jkl.svg");
    ui->alphaBtn5->setAsButton(true, "", "5");
    ui->alphaBtn6->setIcon(":/icons/key_6mno.svg",  ":/icons/key_6mno.svg");
    ui->alphaBtn6->setAsButton(true, "", "6");
    ui->alphaBtn7->setIcon(":/icons/key_7pqrs.svg", ":/icons/key_7pqrs.svg");
    ui->alphaBtn7->setAsButton(true, "", "7");
    ui->alphaBtn8->setIcon(":/icons/key_8tuv.svg",  ":/icons/key_8tuv.svg");
    ui->alphaBtn8->setAsButton(true, "", "8");
    ui->alphaBtn9->setIcon(":/icons/key_9wxyz.svg", ":/icons/key_9wxyz.svg");
    ui->alphaBtn9->setAsButton(true, "", "9");
    ui->alphaBtn10->setAsButton(true, "Entf", "Del");
    ui->alphaBtn11->setAsButton(true, ".");

    // Brake regime buttons
    ui->braBtn1->setIcon(":/icons/key_BrakeType_1.svg");
    ui->braBtn2->setIcon(":/icons/key_BrakeType_2.svg");
    ui->braBtn3->setIcon(":/icons/key_BrakeType_3.svg");
    ui->braBtn4->setIcon(":/icons/key_BrakeType_4.svg");
    ui->braBtn6->setIcon(":/icons/key_BrakeType_6.svg");
    ui->braBtn8->setIcon(":/icons/key_BrakeType_8.svg");
    ui->braBtn9->setIcon(":/icons/key_BrakeType_9.svg");

    ui->braBtn1->setAsButton(true, "", "1");
    ui->braBtn2->setAsButton(true, "", "2");
    ui->braBtn3->setAsButton(true, "", "3");
    ui->braBtn4->setAsButton(true, "", "4");
    ui->braBtn6->setAsButton(true, "", "6");
    ui->braBtn8->setAsButton(true, "", "8");
    ui->braBtn9->setAsButton(true, "", "9");

    // Yes / No buttons
    ui->noBtn->setAsButton(true, "Nein", "Nein");
    ui->yesBtn->setAsButton(true, "Ja", "Ja");

    // Train category buttons
    ui->categoryBtn1->setAsButton(true,  "PASS1", "PASS1");
    ui->categoryBtn2->setAsButton(true,  "PASS2", "PASS2");
    ui->categoryBtn3->setAsButton(true,  "PASS3", "PASS3");
    ui->categoryBtn4->setAsButton(true,  "FP1",   "FP1"  );
    ui->categoryBtn5->setAsButton(true,  "FP2",   "FP2"  );
    ui->categoryBtn6->setAsButton(true,  "FP3",   "FP3"  );
    ui->categoryBtn7->setAsButton(true,  "FP4",   "FP4"  );
    ui->categoryBtn8->setAsButton(true,  "FG1",   "FG1"  );
    ui->categoryBtn9->setAsButton(true,  "FG2",   "FG2"  );
    ui->categoryBtn10->setAsButton(true, "FG3",   "FG3"  );
    ui->categoryBtn11->setAsButton(true, "FG4",   "FG4"  );
    ui->categoryBtn12->setAsButton(true, "TILT1", "TILT1");

    // Axle load buttons
    ui->axleLoadBtn1->setAsButton(true,  "A" ,   "A"   );
    ui->axleLoadBtn2->setAsButton(true,  "B1",   "B1"  );
    ui->axleLoadBtn3->setAsButton(true,  "B2",   "B2"  );
    ui->axleLoadBtn4->setAsButton(true,  "C2",   "C2"  );
    ui->axleLoadBtn5->setAsButton(true,  "C3",   "C3"  );
    ui->axleLoadBtn6->setAsButton(true,  "C4",   "C4"  );
    ui->axleLoadBtn7->setAsButton(true,  "D2",   "D2"  );
    ui->axleLoadBtn8->setAsButton(true,  "D3",   "D3"  );
    ui->axleLoadBtn9->setAsButton(true,  "D4",   "D4"  );
    ui->axleLoadBtn10->setAsButton(true, "D4XL", "D4XL");
    ui->axleLoadBtn11->setAsButton(true, "E4",   "E4"  );
    ui->axleLoadBtn12->setAsButton(true, "E5",   "E5"  );

    // Loading gauge buttons
    ui->loadingGaugeBtn1->setAsButton(true, "G1",        "G1"       );
    ui->loadingGaugeBtn2->setAsButton(true, "GA",        "GA"       );
    ui->loadingGaugeBtn3->setAsButton(true, "GB",        "GB"       );
    ui->loadingGaugeBtn4->setAsButton(true, "GC",        "GC"       );
    ui->loadingGaugeBtn5->setAsButton(true, "Out of GC", "Out of GC");

    ui->zusiIpOkBtn->setAsButton();
    ui->zusiIpOkBtn->setAsDataEntryLabel("",true,true);//Nach Eingabe hier tippen
    ui->driverIdOkBtn->setAsButton();
    ui->driverIdOkBtn->setAsDataEntryLabel("",true,true);
    ui->trainRunningNumberOkBtn->setAsButton();
    ui->trainRunningNumberOkBtn->setAsDataEntryLabel("",true,true);
    ui->levelOkBtn->setAsButton();
    ui->levelOkBtn->setAsDataEntryLabel("",true,true);
    ui->dataEntryClose->setAsButton();
    ui->dataEntryClose->setIcon(":/icons/X.svg");
    ui->numBtn0->setText("0",era::grey,era::darkGrey,QFont::Light);

    ui->systemVersionClose->setAsButton();
    ui->systemVersionClose->setIcon(":/icons/X.svg");
    ui->fieldG11->setText("",era::grey,era::darkGrey,QFont::Light);
    ui->fieldG11->setCustomFontFactor(0.24);
    ui->fieldG13->setText("--:--:--",era::grey,era::darkGrey,QFont::Light);
    ui->fieldG13->setCustomFontFactor(0.24);
    QSizePolicy sp = ui->FieldELzbTexts->sizePolicy();
    sp.setVerticalStretch(0);
    ui->FieldELzbTexts->setSizePolicy(sp);
    connectTimers();
    connectPzbIcons();
    connectMtdIcons();
    connectMtdPower();
    connectTcpStuff();
    //ui->fieldA3->setIsDistanceScale(); // obsolet
    ui->fieldA3->setTargetDistance(0,false, false, false);
    ui->widgetTacho->setVMaxDial(400);
    configureSettingsWindow();
    emit newManometerUse(showManometer > 0);
    setEvcPresent(false);
    resizeMe();
    update();
}

void MainWindow::connectPzbIcons(){
    connect(myLzb, &lzb::newIconBehav1,  ui->fieldCL1,&dmiLabel::setWorking);
    connect(myLzb, &lzb::newIconBehav2,  ui->fieldCL2,&dmiLabel::setWorking);
    connect(myLzb, &lzb::newIconBehav3,  ui->fieldCL3,&dmiLabel::setWorking);
    connect(myLzb, &lzb::newIconBehav4,  ui->fieldCL4,&dmiLabel::setWorking);
    connect(myLzb, &lzb::newIconBehav5,  ui->fieldCL5,&dmiLabel::setWorking);
    connect(myLzb, &lzb::newIconBehav6,  ui->fieldCL6,&dmiLabel::setWorking);
    connect(myLzb, &lzb::newIconBehav7,  ui->fieldCL7,&dmiLabel::setWorking);
    connect(myLzb, &lzb::newIconBehavC9, ui->fieldC9, &dmiLabel::setWorking);
    connect(myLzb, &lzb::newIconBehavG1, ui->fieldG1, &dmiLabel::setWorking);
    connect(myLzb, &lzb::newIcon1,  ui->fieldCL1, static_cast<void (dmiLabel::*)(QString, QString)>(&dmiLabel::setIcon));
    connect(myLzb, &lzb::newIcon2,  ui->fieldCL2, static_cast<void (dmiLabel::*)(QString, QString)>(&dmiLabel::setIcon));
    connect(myLzb, &lzb::newIcon3,  ui->fieldCL3, static_cast<void (dmiLabel::*)(QString, QString)>(&dmiLabel::setIcon));
    connect(myLzb, &lzb::newIcon4,  ui->fieldCL4, static_cast<void (dmiLabel::*)(QString, QString)>(&dmiLabel::setIcon));
    connect(myLzb, &lzb::newIcon5,  ui->fieldCL5, static_cast<void (dmiLabel::*)(QString, QString)>(&dmiLabel::setIcon));
    connect(myLzb, &lzb::newIcon6,  ui->fieldCL6, static_cast<void (dmiLabel::*)(QString, QString)>(&dmiLabel::setIcon));
    connect(myLzb, &lzb::newIcon7,  ui->fieldCL7, static_cast<void (dmiLabel::*)(QString, QString)>(&dmiLabel::setIcon));
    connect(myLzb, &lzb::newIconC9, ui->fieldC9,  static_cast<void (dmiLabel::*)(QString, QString)>(&dmiLabel::setIcon));
    connect(myLzb, &lzb::newIconG1, ui->fieldG1,  static_cast<void (dmiLabel::*)(QString, QString)>(&dmiLabel::setIcon));
    connect(myLzb, &lzb::newLzbTextMessage, ui->FieldELzbTexts, &dmiLabel::addTextLzbMessage);
    connect(myLzb, &lzb::removeMessage,     ui->FieldELzbTexts, &dmiLabel::removeTextMessage);
    ui->fieldVZile100->setSegmentDigitToUse(3);
    ui->fieldVZile10->setSegmentDigitToUse(2);
    ui->fieldVZile1->setSegmentDigitToUse(1);
    connect(myLzb, &lzb::newVTarget,       ui->fieldVZile100, &dmiLabel::setSegmentDigit);
    connect(myLzb, &lzb::newVTarget,       ui->fieldVZile10,  &dmiLabel::setSegmentDigit);
    connect(myLzb, &lzb::newVTarget,       ui->fieldVZile1,   &dmiLabel::setSegmentDigit);
    connect(myLzb, &lzb::newVTarget,       ui->widgetTacho,   &gauge::setVTarget);
    connect(myLzb, &lzb::newVPermit,       ui->widgetTacho,   &gauge::setVPerm);
    connect(myLzb, &lzb::newVOverspeed,    ui->widgetTacho,   &gauge::setVOverspeed);
    connect(myLzb, &lzb::newVMaxReducing,  ui->widgetTacho,   &gauge::setCsmReducing);
    connect(myLzb, &lzb::newOverspeed,     ui->widgetTacho,   &gauge::setOverspeed);
    connect(myLzb, &lzb::newIntervenation, ui->widgetTacho,   &gauge::setIntervenation);
    connect(myLzb, &lzb::newTarDist,       ui->fieldA3,       &targetDistance::setTargetDistance);

    connect(myTcp->myEtcs, &zusi3etcs::newVTarget,           ui->widgetTacho, &gauge::setVTarget);
    connect(myTcp->myEtcs, &zusi3etcs::newVPermit,           ui->widgetTacho, &gauge::setVPerm);
    connect(myTcp->myEtcs, &zusi3etcs::newReleaseSpeed,      ui->widgetTacho, &gauge::setVRelease);
    connect(myTcp->myEtcs, &zusi3etcs::newVOverspeed,        ui->widgetTacho, &gauge::setVOverspeed);
    connect(myTcp->myEtcs, &zusi3etcs::newReleaseSpeed,      ui->fieldB6,     static_cast<void (dmiLabel::*)(quint16)>(&dmiLabel::setText));
    connect(myTcp->myEtcs, &zusi3etcs::newIntervenation,     ui->widgetTacho, &gauge::setIntervenation);
    connect(myTcp->myEtcs, &zusi3etcs::newOverspeed,         ui->widgetTacho, &gauge::setOverspeed);
    connect(myTcp->myEtcs, &zusi3etcs::newVMaxReducing,      ui->widgetTacho, &gauge::setCsmReducing);
    connect(myTcp->myEtcs, &zusi3etcs::newTarDist,           ui->fieldA3,     &targetDistance::setTargetDistance);
    connect(myTcp->myEtcs, &zusi3etcs::newEvcPresent,        this,            &MainWindow::setEvcPresent);
    connect(myTcp->myEtcs, &zusi3etcs::newActiveLevelIcon,   ui->fieldC8,     static_cast<void (dmiLabel::*)(QString)>(&dmiLabel::setIcon));

    connect(myTcp->myEtcs, &zusi3etcs::newActiveModeIcon,    ui->fieldB7,     static_cast<void (dmiLabel::*)(QString)>(&dmiLabel::setIcon));
    connect(myTcp->myEtcs, &zusi3etcs::newActiveMode,        ui->widgetTacho, &gauge::setMode);
    connect(myTcp->myEtcs, &zusi3etcs::newNextLevelIcon,     ui->fieldC1,     static_cast<void (dmiLabel::*)(QString)>(&dmiLabel::setIcon));
    connect(myTcp->myEtcs, &zusi3etcs::newNextLevelIcon,     ui->fieldC1l,    static_cast<void (dmiLabel::*)(QString)>(&dmiLabel::setIcon));
    connect(myTcp->myEtcs, &zusi3etcs::newNextLevelAckFrame, ui->fieldC1,     &dmiLabel::setAcklowedgeFrame);
    connect(myTcp->myEtcs, &zusi3etcs::newNextLevelAckFrame, ui->fieldC1l,    &dmiLabel::setAcklowedgeFrame);
    connect(myTcp->myEtcs, &zusi3etcs::newRadioStateIcon,    ui->fieldE1,     static_cast<void (dmiLabel::*)(QString)>(&dmiLabel::setIcon));
    connect(myTcp->myEtcs, &zusi3etcs::newEmrOrSrvBrakeIcon, ui->fieldC9,     static_cast<void (dmiLabel::*)(QString)>(&dmiLabel::setIcon));
    connect(myTcp->myEtcs, &zusi3etcs::newEmrOrSrvBrakeState,myLzb,           &lzb::setBrakeFromEtcs);
    connect(myTcp->myEtcs, &zusi3etcs::newAdhesionIcon,      ui->fieldA4,     static_cast<void (dmiLabel::*)(QString)>(&dmiLabel::setIcon));
    connect(myTcp->myEtcs, &zusi3etcs::newOverrideIcon,      ui->fieldC7,     static_cast<void (dmiLabel::*)(QString)>(&dmiLabel::setIcon));
    connect(myTcp->myEtcs, &zusi3etcs::newOverrideActive,    ui->fieldC7,     &dmiLabel::setVisib);
    connect(myTcp->myEtcs, &zusi3etcs::newIconB5,            ui->fieldB5,     static_cast<void (dmiLabel::*)(QString)>(&dmiLabel::setIcon));
    connect(myTcp->myEtcs, &zusi3etcs::newTti,               ui->fieldA1,     &timetoindication::setTti);
  //connect(myTcp->myEtcs, &zusi3etcs::newNextLevelAckFrame, ui->FieldE8to9,  &EtcsTextWidget::setAcklowedgeFrame);
}
void MainWindow::connectMtdIcons(){
    connect(myMtd, &mtd::newIconBehavG10, ui->fieldG10,&dmiLabel::setWorking);
    connect(myMtd, &mtd::newIconBehavG2,  ui->fieldG2, &dmiLabel::setWorking);
    connect(myMtd, &mtd::newIconBehavG3,  ui->fieldG3, &dmiLabel::setWorking);
    connect(myMtd, &mtd::newIconBehavG4,  ui->fieldG4, &dmiLabel::setWorking);
    connect(myMtd, &mtd::newIconBehavG5,  ui->fieldG5, &dmiLabel::setWorking);
    connect(myMtd, &mtd::newIconG10,      ui->fieldG10,static_cast<void (dmiLabel::*)(QString, QString)>(&dmiLabel::setIcon));
    connect(myMtd, &mtd::newIconG2,       ui->fieldG2, static_cast<void (dmiLabel::*)(QString, QString)>(&dmiLabel::setIcon));
    connect(myMtd, &mtd::newIconG3,       ui->fieldG3, static_cast<void (dmiLabel::*)(QString, QString)>(&dmiLabel::setIcon));
    connect(myMtd, &mtd::newIconG4,       ui->fieldG4, static_cast<void (dmiLabel::*)(QString, QString)>(&dmiLabel::setIcon));
    connect(myMtd, &mtd::newIconG5,       ui->fieldG5, static_cast<void (dmiLabel::*)(QString, QString)>(&dmiLabel::setIcon));
}
void MainWindow::connectMtdPower(){
    connect(myTcp->myPower, &zusiPower::newPowerValues, myMtd, &mtd::setAnalogValues);
    connect(myMtd,          &mtd::newPowerRelative, ui->widgetPower, &power::setPowerRelative);
    connect(myMtd,          &mtd::newPowerRelativeSet, ui->widgetPower, &power::setPowerRelativeSet);
    connect(myMtd,          &mtd::newPowerAbsolute, ui->widgetPower, &power::setPowerAbsolute);
    connect(myMtd,          &mtd::newPowerAbsoluteSet, ui->widgetPower, &power::setPowerAbsoluteSet);
    connect(mySep,          &sep::newPowerRelative, ui->widgetPower, &power::setPowerRelative);
    connect(myTcp->myPower, &zusiPower::maxPowerPositiveLine, ui->widgetPower, &power::setAbsoluteAccelerateMaximum);
    connect(myTcp->myPower, &zusiPower::maxPowerNegativeLine, ui->widgetPower, &power::setAbsoluteBrakingMaximum);
    connect(myTcp->myPower, &zusiPower::unitAcceleratingText, ui->widgetPower, &power::setUnitAcceleratingText);
    connect(myTcp->myPower, &zusiPower::unitBrakingText, ui->widgetPower, &power::setUnitBrakingText);
    connect(myTcp->myPower, &zusiPower::unitBrakingText, ui->widgetPower, &power::setUnitBrakingText);
    connect(myTcp->myPower, &zusiPower::hasDriveModeDisplay, ui->widgetPower, &power::setModeDisplay);
    connect(myTcp->myPower, &zusiPower::newDriveMode, ui->widgetPower, &power::setDriveMode);
}
void MainWindow::connectTimers(){
    shortTimer  = new QTimer(this);
    shortTimer->setInterval(250);
        shortTimer->start();
    connect(shortTimer, &QTimer::timeout, ui->FieldE8to9,  &EtcsTextWidget::updateBlinking);
    connect(shortTimer, &QTimer::timeout, ui->fieldCL1,    &dmiLabel::updateBlinking);
    connect(shortTimer, &QTimer::timeout, ui->fieldCL2,    &dmiLabel::updateBlinking);
    connect(shortTimer, &QTimer::timeout, ui->fieldCL3,    &dmiLabel::updateBlinking);
    connect(shortTimer, &QTimer::timeout, ui->fieldCL4,    &dmiLabel::updateBlinking);
    connect(shortTimer, &QTimer::timeout, ui->fieldCL5,    &dmiLabel::updateBlinking);
    connect(shortTimer, &QTimer::timeout, ui->fieldCL6,    &dmiLabel::updateBlinking);
    connect(shortTimer, &QTimer::timeout, ui->fieldCL7,    &dmiLabel::updateBlinking);
    connect(shortTimer, &QTimer::timeout, ui->fieldC1,     &dmiLabel::updateBlinking);
    connect(shortTimer, &QTimer::timeout, ui->fieldC1l,    &dmiLabel::updateBlinking);
    connect(shortTimer, &QTimer::timeout, ui->fieldG2,     &dmiLabel::updateBlinking);
    connect(shortTimer, &QTimer::timeout, this,            &MainWindow::blinkCursor);
    connect(shortTimer, &QTimer::timeout, myZusiTrainData, &zusiTraindata::setCursorBlink);

}
void MainWindow::connectTcpStuff(){
    connect(myTcp, &zusi3Tcp::sendTcpConnectionFeedback, this, &MainWindow::gotTcpConnectionFeedback);
    connect(myTcp, &zusi3Tcp::sendDataSourceIsZusi, myLzb, &lzb::setZusiAsDataSource);
    connect(this,  &MainWindow::newZusiIp,             myTcp, &zusi3Tcp::setIpadress);
    connect(myTcp->myEtcs, &zusi3etcs::newActiveMode,  this,  &MainWindow::setMode);
}

void MainWindow::gotTcpConnectionFeedback(QString feedback){
    if(feedback.contains("."))settings->setValue("zusiIp",feedback);
    if(feedback == "-2")      emit newZusiIp(settings->value("zusiIp").toString());
}

void MainWindow::setLzbPresent(bool present){
    QMetaObject::invokeMethod(this, [this, present]() {
        myZusiTrainData->setLzbAvailable(present);
    }, Qt::QueuedConnection);
    ui->dataValidLabel3->setVisib(present);
    ui->dataValidLabel4->setVisib(present);
    ui->dataValidValue3->setVisib(present);
    ui->dataValidValue4->setVisib(present);
}

void MainWindow::setEvcPresent(bool present){
    evcPresent = present;
    setMode(zusi3etcs::etcsMode::mode_IS);
    QMetaObject::invokeMethod(this, [this, present]() {
        myZusiTrainData->setEtcsAvailable(present);
    }, Qt::QueuedConnection);
    if(!evcPresent)ui->fieldB7->setIcon(":/icons/MO_19.svg"); // Display Level NTC. if there is no EVC
    ui->dataValidLabel0->setVisib(present);
    ui->dataValidLabel5->setVisib(present); // ETCS Specific train data
    ui->dataValidLabel6->setVisib(present);
    ui->dataValidLabel7->setVisib(present);
    ui->dataValidLabel8->setVisib(present);
    ui->dataValidLabel9->setVisib(present);
    ui->dataValidLabel10->setVisib(present);
    ui->dataValidLabel11->setVisib(present);
    ui->dataValidLabel12->setVisib(present);
    ui->dataValidValue5->setVisib(present);
    ui->dataValidValue6->setVisib(present);
    ui->dataValidValue7->setVisib(present);
    ui->dataValidValue8->setVisib(present);
    ui->dataValidValue9->setVisib(present);
    ui->dataValidValue10->setVisib(present);
    ui->dataValidValue11->setVisib(present);
    ui->dataValidValue12->setVisib(present);
    if( evcPresent)ui->trainDataTitel1->setTextMessageOffset(1); // Two pages for ETCS train data entry
    if(!evcPresent)ui->trainDataTitel1->setTextMessageOffset(0); // One page only if no ETCS

    ui->mainBtn1->setEnabled(present);  // Button "Start"
    ui->mainBtn4->setEnabled(false);    // Button "Funktionspruefung"
    ui->mainBtn5->setEnabled(present);  // Button "Level"
    ui->mainBtn7->setEnabled(present);  // Button "Rangieren"
    ui->mainBtn8->setEnabled(present);  // Button "Nicht führend"
    ui->fieldF2->setAsButton(present);  // Button "Override"
}

void MainWindow::setMode(quint16 mode){
    using M = ::zusi3etcs::etcsMode;
    QSizePolicy sp = ui->FieldELzbTexts->sizePolicy();
    if(mode == M::mode_IS ||
       mode == M::mode_SN){                     // In these mode should PZB/LZB be displayed...
        sp.setVerticalStretch(3);               // ...show LZB text messages
        ui->fielBHolders->raise();              // ...the indicator for level change must be clickable on top
        ui->fieldC_holder->setCurrentIndex(1);  // ...and must be visible.
        ui->fieldC_holder2->setCurrentIndex(0);
        ui->fieldD->setCurrentIndex(0);         // Show power guage
    }
    else{                                       // Otherwise...
        sp.setVerticalStretch(0);               // ...hide LZB text messages
      //ui->widgetTacho->raise();               // ...keep spedo on top for basic speed hook activation
        ui->fielBHolders->lower();
        ui->fieldC_holder->setCurrentIndex(0);  // ...the 3 widgehts below the speedometer must be displayed
        ui->fieldC_holder2->setCurrentIndex(1);
        ui->fieldD->setCurrentIndex(1);         // Show planning area
    }
    ui->FieldELzbTexts->setSizePolicy(sp);
    actMode = mode;
    resizeMe();
}
void MainWindow::openMainMenu(){
    ui->fieldDG->setCurrentIndex(P_DG_Main_menu);                // Page with main menu
}

void MainWindow::setKeyboardType(quint8 type){
    setKeyboardType(type, false);
}
void MainWindow::setKeyboardType(quint8 type, bool showDott){
    actKeyboardType = type;
    ui->keyboards->setCurrentIndex(type);
    ui->numBtn11->setAsButton(showDott, ".");
    ui->alphaBtn11->setAsButton(showDott, ".");
}
void MainWindow::TdeCompeteClicked(){
    ui->keyboards->setCurrentIndex(P_Keyboard_ValidYesNo);              // Data entry page on the right keyboard "Yes" and "No" for validation
    ui->entryFields->setCurrentIndex(P_Entry_Button_YesNo);             // Data entry page on the right konfirm button for validation -> [Yes | No]
    ui->dataValidationHolder2->setCurrentIndex(P_entry_not_complete);   // Data entry page validation on the left buttom "Entry complerte? Yes" not visible
    ui->dataValidationHolder3->setCurrentIndex(P_entry_not_complete);   // Data entry page validation on the left top. Titel changess from data entry to data validation
}

void MainWindow::fieldF2Clicked(){
    ui->fieldDG->setCurrentIndex(P_DG_Override_menu);
}
void MainWindow::fieldF3Clicked(){
  //QMetaObject::invokeMethod(myTcp, "setEtcsBaseline", Qt::QueuedConnection);
}
void MainWindow::fieldF4Clicked(){
}
void MainWindow::openSettings(){
    ui->fieldDG->setCurrentIndex(P_DG_Settings);
    ui->TrnSettingsAndPageHolder->setCurrentIndex(P_TrnSettingsAndPage_NonOfThem);// In case previous window was driver ID, triggered by cab activation
}
void MainWindow::setTdePage(bool seccondPage){
    ui->dataEntryPrevPage->setWorking( seccondPage, false, false);
    ui->dataEntryNextPage->setWorking(!seccondPage, false, false);
    if( seccondPage){
        ui->trainDataTitel1->setTextMessageOffset(2);
        ui->entryFields->setCurrentIndex(P_entry_TrainData2);
    }
    if(!seccondPage){
        if(evcPresent)
            ui->trainDataTitel1->setTextMessageOffset(1);
        else
            ui->trainDataTitel1->setTextMessageOffset(0);
        ui->entryFields->setCurrentIndex(P_entry_TrainData1);
    }
}
void MainWindow::setFirstTdePage(){
    setTdePage(0);
}
void MainWindow::setSecondTdePage(){
    setTdePage(1);
}

void MainWindow::openTrainDataEntry(){
    ui->fieldDG->setCurrentIndex(P_DG_Data_entry);                  // Data entry page for all kind of data
    if( evcPresent) ui->TrnSettingsAndPageHolder->setCurrentIndex(P_TrnSettingsAndPage_Page);
    if(!evcPresent) ui->TrnSettingsAndPageHolder->setCurrentIndex(P_TrnSettingsAndPage_NonOfThem);
    ui->entryFields->setCurrentIndex(P_entry_TrainData1);           // Data entry page for LZB
    ui->dataValidationHolder2->setCurrentIndex(P_entry_complete);   // Data entry page validation on the left buttom "Entry complerte? Yes" visible
    ui->dataValidationHolder3->setCurrentIndex(P_entry_complete);   // Data entry page validation on the left top. Titel changess from data validation to data entry
    setKeyboardType(P_Keyboard_Numeric, false);                     // Numeric keyboard without dot
    ui->fieldABE->setCurrentIndex(P_ABE_DataEntry);                 // Data entry page on the left
    ui->dataValidButton->setAsButton(true, "Nein");
    activeDataEntryItem = "trainData";
    dataString = "";
}
void MainWindow::cabActivation(bool cabActivated, bool standstill){
    if(cabActivated && standstill){
        settingsCloseClicked();
        ui->TrnSettingsAndPageHolder->setCurrentIndex(P_TrnSettingsAndPage_TrnSettings);
        openDriverIdEntry();
    }
}
void MainWindow::openDriverIdEntry(){
    ui->fieldDG->setCurrentIndex(P_DG_Data_entry);                  // Data entry page for all kind of data
    ui->entryFields->setCurrentIndex(P_entry_DriverId);             // Data entry page for driver ID
    setKeyboardType(P_Keyboard_Alphanumeric);                       // Numeric keyboard without dot
    activeDataEntryItem = "driverId";
    myDriverId->clearText();
    myDriverId->hideText(true);
    setEntryStrWithMaxLength(16, "");
    ui->driverIdOkBtn->setText(myDriverId->getText(),era::black,era::black,QFont::Light);
}
void MainWindow::openTrainRunnimgNumberEntry(){
    ui->fieldDG->setCurrentIndex(P_DG_Data_entry);                  // Data entry page for all kind of data
    ui->entryFields->setCurrentIndex(P_entry_TrainNumber);          // Data entry page for train running number
    setKeyboardType(P_Keyboard_Numeric, false);                     // Numeric keyboard without dot
    activeDataEntryItem = "trainRunningNumber";
    setEntryStrWithMaxLength(16, "");
    ui->TrnSettingsAndPageHolder->setCurrentIndex(P_TrnSettingsAndPage_NonOfThem);// In case previous window was driver ID, triggered by cab activation
}
void MainWindow::openLevelSelection(){
    ui->fieldDG->setCurrentIndex(P_DG_Data_entry);                  // Data entry page for all kind of data
    ui->entryFields->setCurrentIndex(P_entry_Level);                // Data entry page level selction
    setKeyboardType(P_Keyboard_level, false);                       // keyboard with levels
    activeDataEntryItem = "level";
    setEntryStrWithMaxLength(16, "");
}

void MainWindow::settingsBtn1Clicked(){}
void MainWindow::settingsBtn2Clicked(){}
void MainWindow::settingsBtn3Clicked(){}
void MainWindow::openSystemVersionInfo(){ui->fieldDG->setCurrentIndex(P_DG_System_Version);}    // System Version
void MainWindow::openQDmiSettings(){ui->fieldDG->setCurrentIndex(P_DG_QDmi);}                   // QDmi
void MainWindow::openNetworkSettings(){                                     // Network
    QMetaObject::invokeMethod(this, [this]() {
        myTcp->setNwSettingsOpen(true);
    }, Qt::QueuedConnection);

    ui->fieldDG->setCurrentIndex(P_DG_Data_entry);                          // Data entry page for all kind of data
    setKeyboardType(P_Keyboard_Numeric, true);                              // Data entry page on the right numeric keyboard
    ui->entryFields->setCurrentIndex(P_entry_ZusiIp);                       // Data entry page on the right for IP adress
    dataString = settings->value("zusiIp").toString();
    activeDataEntryItem = "IP-Address";
    setEntryStrWithMaxLength(15, settings->value("zusiIp").toString());     // Limit entry string 15 characters and set start value from settings


}
void MainWindow::enableTde(bool enable){
    ui->mainBtn3->setAsButton(!enable, "Zugdateneingabe");
    settingsCloseClicked();
    settingsCloseClicked();
}
void MainWindow::anableOverride(bool enable){
    enable = enable && evcPresent;
    ui->fieldF2->setAsButton(!enable);
    if(ui->fieldDG->currentIndex() == P_DG_Override_menu){
       ui->fieldDG->setCurrentIndex(P_DG_DefaultWindow);
       return;
    }
}
void MainWindow::closeQDmi(){
    settings->setValue("mainwindow/height", this->height());
    settings->setValue("mainwindow/width", this->width());
    #ifdef Q_PROCESSOR_ARM
    system("shutdown -P now");
    #endif
    QApplication::quit();
}
void MainWindow::settingsBtn8Clicked(){}
void MainWindow::geoPositionClicked(){
    bool qurrentIndex = static_cast<bool>(ui->fieldG12_holder->currentIndex());
    ui->fieldG12_holder->setCurrentIndex(!qurrentIndex);
}

void MainWindow::setGeoPosition(qint32 geoPosition){
    ui->fieldG12_btn->setWorking(true, false, false);
    ui->fieldG12_geopos_T->setWorking(true,false,false);
    ui->fieldG12_geopos_H->setWorking(true,false,false);
    QString postionTextKilometer = QString::number(geoPosition / 1000);
    QString postionTextMeter = QString::number(geoPosition % 1000).rightJustified(3, '0');
    ui->fieldG12_geopos_T->setText(postionTextKilometer,era::black,era::grey);
    ui->fieldG12_geopos_H->setText(postionTextMeter,era::black,era::grey);
}
void MainWindow::settingsCloseClicked(){
    activeDataEntryItem = "";
    if(ui->fieldDG->currentIndex() == P_DG_Data_entry){// && (ui->entryFields->currentIndex() == 1 || ui->entryFields->currentIndex() == 2)){   // Data entry
        if(ui->entryFields->currentIndex() == P_entry_ZusiIp){
            ui->fieldDG->setCurrentIndex(P_DG_Settings);
            QMetaObject::invokeMethod(this, [this]() {
                myTcp->setNwSettingsOpen(false);
            }, Qt::QueuedConnection);
            return;
        }
        setTdePage(false); // Go back to first TDE page, in  case of beeing on ETCS page
        ui->fieldABE->setCurrentIndex(P_ABE_DefaultWindow);                 // Data entry page on the right
        ui->TrnSettingsAndPageHolder->setCurrentIndex(P_TrnSettingsAndPage_NonOfThem);
        ui->fieldDG->setCurrentIndex(P_DG_Main_menu);
        return;
    }

    if(ui->fieldDG->currentIndex() == P_DG_Main_menu){   // Main menu
       ui->fieldDG->setCurrentIndex(P_DG_DefaultWindow);
       return;
    }
    if(ui->fieldDG->currentIndex() == P_DG_Override_menu){   // Override menu
       ui->fieldDG->setCurrentIndex(P_DG_DefaultWindow);
       return;
    }
    if(ui->fieldDG->currentIndex() == P_DG_QDmi){
        settings->setValue("pulldown_gauge",ui->pulldown_gauge->currentIndex());
        settings->setValue("pulldown_targetdistance",ui->pulldown_targetdistance->currentIndex());
        settings->setValue("pulldown_showTextmessages",ui->pulldown_showTextmessages->currentIndex());
        settings->setValue("pulldown_showManometer",ui->pulldown_showManometer->currentIndex());
        settings->setValue("pulldown_tcpConnection",ui->pulldown_tcpConnection->currentIndex());
        applySettings();
        resizeMe();
    }
    if(ui->fieldDG->currentIndex()>1){
        ui->fieldDG->setCurrentIndex(P_DG_Settings);
    }
    else{
        ui->fieldDG->setCurrentIndex(P_DG_DefaultWindow);
    }
}
void MainWindow::applySettings(){
    ui->widgetTacho->setEraUse(settings->value("pulldown_gauge").toInt() == 0);
    ui->fieldVZile100->setVisib(settings->value("pulldown_gauge").toInt() == 1);
    ui->fieldVZile10->setVisib(settings->value("pulldown_gauge").toInt() == 1);
    ui->fieldVZile1->setVisib(settings->value("pulldown_gauge").toInt() == 1);
    ui->fieldA3->setEraUse(settings->value("pulldown_targetdistance").toInt() == 0);
    showManometer = static_cast<quint8>(settings->value("pulldown_showManometer").toUInt());
    emit newTextMessagesSettings(static_cast<quint8>(settings->value("pulldown_showTextmessages").toInt()));
    emit newManometerUse(showManometer);
    emit tcpConnectionSettings(static_cast<quint8>(settings->value("pulldown_tcpConnection").toUInt()));

}
void MainWindow::applyClicked(QString data, bool enabled){
    if(enabled){
        data.remove("_");
        if(activeDataEntryItem == "IP-Address"){
            emit newZusiIp(data);
        }
        if(activeDataEntryItem == "driverId"){
            emit newDriverId(data);
            openMainMenu();
        }
        if(activeDataEntryItem == "trainRunningNumber"){
            emit newTrainRunningNumber(data);
            openMainMenu();
        }
        if(activeDataEntryItem == "level"){
            if(data == "PZB / LZB") emit newLevelSelection(1);
            if(data == "Level 0") emit newLevelSelection(2);
            if(data == "Level 1") emit newLevelSelection(3);
            if(data == "Level 2") emit newLevelSelection(4);
            openMainMenu();
        }
    }
}
void MainWindow::addItemToData(QString item){
    bool hasUnderline = false;
    if(dataString.contains("_") or item == "_")
        hasUnderline = true;
    if(dataString.contains("_") and item == "_")
        hasUnderline = false;
    dataString.remove("_");
    if(item == "Del"){
        dataString.chop(1);
    }
    else{
        if((item != "_") && (dataString.length() < maxEntryStrLength)){
            dataString = dataString + item;
        }
        if((item != "_") && (maxEntryStrLength == 1))
            dataString = item;
        if(hasUnderline)
            dataString = dataString + "_";
    }
    if(activeDataEntryItem == "IP-Address")
        ui->zusiIpOkBtn->setText(dataString,era::black,era::black,QFont::Light);
    if(activeDataEntryItem == "trainData"){
        if(dataString == "0_" || dataString == "0")dataString = dataString.remove("0");
        if(item != "_") myZusiTrainData->setTextFromKeyboard(item);
    }
    if(activeDataEntryItem == "driverId"){
        if(item == "_") myDriverId->togleCursor();
        if(item == "Del") myDriverId->deleteLastChar();
        myDriverId->adItem(item);
        ui->driverIdOkBtn->setText(myDriverId->getText(),era::black,era::black,QFont::Light);
    }
    if(activeDataEntryItem == "trainRunningNumber")
        ui->trainRunningNumberOkBtn->setText(dataString,era::black,era::black,QFont::Light);

}
void MainWindow::blinkCursor(){
    addItemToData("_");
}
void MainWindow::setEntryStrWithMaxLength(quint8 length, QString startString){
    dataString = startString;
    maxEntryStrLength = length;
}

void MainWindow::resizeMe(){
    qreal windowRatio = 4.0 / 3.0;
    qreal defaultHeight = 480.0;
    qreal multi;
    qreal windowHeight = ui->centralWidget->height();
    qreal windowWidth = ui->centralWidget->width();
    qreal defaultWidth = 640.0;
    QSizePolicy manoSizePol = ui->DmiHolder->sizePolicy();
    qreal dpi = QApplication::screens().at(0)->logicalDotsPerInch();
    switch (showManometer){
        case 0:
        manoSizePol.setHorizontalStretch(0);
        windowRatio = 4.0 / 3.0;
        defaultWidth = 640.0;
        ui->manoHolder->setVisible(false);
        break;
        case 1:
        manoSizePol.setHorizontalStretch(53);
        manoSizePol.setVerticalStretch(54);
        windowRatio = 16.0 / 9.0;
        defaultWidth = 853.33;
        ui->manoHolder->setVisible(true);
        break;
        case 2:
        manoSizePol.setHorizontalStretch(32);
        manoSizePol.setVerticalStretch(224);
        windowRatio = 16.0 / 10.0;
        defaultWidth = 768.0;
        ui->manoHolder->setVisible(true);
        break;
        default:
        break;
    }
    ui->manoHolder->setSizePolicy(manoSizePol);
    ui->manoHolder_Spacer->setSizePolicy(manoSizePol);

    if ((windowWidth / windowHeight) < windowRatio){
        multi = windowWidth / defaultWidth;
    }
    else{
        multi = windowHeight / defaultHeight;
    }
    QFont settingsFont = QFont("FreeSans", static_cast<int>(multi * 1100.0 / dpi),QFont::Bold,false);
    ui->pulldown_gauge->setFont(            settingsFont);
    ui->pulldown_showManometer->setFont(    settingsFont);
    ui->pulldown_showTextmessages->setFont( settingsFont);
    ui->pulldown_targetdistance->setFont(   settingsFont);
    ui->pulldown_tcpConnection->setFont(    settingsFont);
    ui->label_qdmi_1->setFont(              settingsFont);
    ui->label_qdmi_2->setFont(              settingsFont);
    ui->label_qdmi_3->setFont(              settingsFont);
    ui->label_qdmi_4->setFont(              settingsFont);
    ui->label_qdmi_5->setFont(              settingsFont);
    ui->DmiManoHolder->setGeometry(0,0,static_cast<int>(defaultWidth * multi)
                                    ,static_cast<int>(defaultHeight * multi));
    int borderTickness = static_cast<int>(multi + 0.75);
    qApp->setProperty("borderThickness", borderTickness);
    QRect tachoRect;
    QRect fieldBHolderRect;
    tachoRect.setY(static_cast<int>(     era::tachoDefault.top() * multi));
    tachoRect.setX(static_cast<int>(     era::tachoDefault.left() *  multi));
    tachoRect.setWidth(static_cast<int>( era::tachoDefault.width() * multi));
    tachoRect.setHeight(static_cast<int>(era::tachoDefault.height() *multi));
    fieldBHolderRect.setY(static_cast<int>(     era::modeAreDefault.top() * multi));
    fieldBHolderRect.setX(static_cast<int>(     era::modeAreDefault.left() *  multi));
    fieldBHolderRect.setWidth(static_cast<int>( era::modeAreDefault.width() * multi));
    fieldBHolderRect.setHeight(static_cast<int>(era::modeAreDefault.height() *multi));

    ui->widgetTacho->setGeometry(tachoRect);
    ui->fielBHolders->setGeometry(fieldBHolderRect);


    #ifdef Q_PROCESSOR_ARM
    this->setCursor(Qt::BlankCursor);
    cursor().setPos(ui->centralWidget->width(), ui->centralWidget->height());
    #endif
}
void MainWindow::resizeEvent(QResizeEvent* event){
    resizeMe();
    (void)event;
}
void MainWindow::configureSettingsWindow(){
    QStringList tdList = { "1000m (ERTMS)", "4000m (DB)" };
    QStringList gaugeList = { "Haken (ERTMS)", "Dreieck (DB)" };
    QStringList showTextmessagesList = { "Ja", "Autom. (Nur Zusi)", "Nein" };
    QStringList showManometerList = { "nicht anzeigen", "anzeigen (16:9)", "anzeigen (16:10)" };
    QStringList tcpConnectionList = { "beim Start Verbinden", "immer Verbinden" };
    ui->pulldown_targetdistance->addItems(tdList);
    ui->pulldown_gauge->addItems(gaugeList);
    ui->pulldown_showTextmessages->addItems(showTextmessagesList);
    ui->pulldown_showManometer->addItems(showManometerList);
    ui->pulldown_tcpConnection->addItems(tcpConnectionList);
    ui->pulldown_targetdistance->setCurrentIndex(settings->value("pulldown_targetdistance").toInt());
    ui->pulldown_gauge->setCurrentIndex(settings->value("pulldown_gauge").toInt());
    ui->pulldown_showTextmessages->setCurrentIndex(settings->value("pulldown_showTextmessages").toInt());
    ui->pulldown_showManometer->setCurrentIndex(settings->value("pulldown_showManometer").toInt());
    ui->pulldown_tcpConnection->setCurrentIndex(settings->value("pulldown_tcpConnection").toInt());
    ui->pulldown_targetdistance->setItemData(0, era::darkGrey, Qt::BackgroundRole);
    ui->pulldown_targetdistance->setItemData(1, era::darkGrey, Qt::BackgroundRole);
    ui->pulldown_gauge->setItemData(0, era::darkGrey, Qt::BackgroundRole);
    ui->pulldown_gauge->setItemData(1, era::darkGrey, Qt::BackgroundRole);
    ui->pulldown_showTextmessages->setItemData(0, era::darkGrey, Qt::BackgroundRole);
    ui->pulldown_showTextmessages->setItemData(1, era::darkGrey, Qt::BackgroundRole);
    ui->pulldown_showTextmessages->setItemData(2, era::darkGrey, Qt::BackgroundRole);
    ui->pulldown_showManometer->setItemData(0, era::darkGrey, Qt::BackgroundRole);
    ui->pulldown_showManometer->setItemData(1, era::darkGrey, Qt::BackgroundRole);
    ui->pulldown_tcpConnection->setItemData(0, era::darkGrey, Qt::BackgroundRole);
    ui->pulldown_tcpConnection->setItemData(1, era::darkGrey, Qt::BackgroundRole);

    applySettings();
    emit newZusiIp(settings->value("zusiIp").toString());

}
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_Up:break;
        case Qt::Key_Down:break;
        case Qt::Key_P:
            ui->fieldD->setCurrentIndex(1);           // Show planning area
            break;
        case Qt::Key_K:
            ui->fieldD->setCurrentIndex(0);           // Show power guage
            break;
        case Qt::Key_Escape:
            #ifdef Q_PROCESSOR_ARM
            QApplication::quit();
            #endif
            break;
        default:
            QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::resetInput() {
    inputMode = None;
    inputBuffer.clear();
}

void MainWindow::mousePressEvent(QMouseEvent *event){
    #if defined(Q_OS_WIN32) || defined(Q_OS_LINUX)
    lKilickPos = event->pos();
    #endif
}
void MainWindow::mouseMoveEvent(QMouseEvent *event){
    #if not defined(Q_OS_ANDROID)
    if(event->buttons() & Qt::LeftButton){
        if (ui->fieldZ->underMouse()) {
            QPoint diff = event->pos() - lKilickPos;
            QPoint newPos = this->pos() + diff;
            this->move(newPos);
        }
        if(ui->fieldYResize->underMouse()){
            QPoint diff = event->pos();
            QRect newSize = this->geometry();
            newSize.setWidth(diff.x());
            newSize.setHeight(diff.y());
            this->setGeometry(newSize);
        }
    }
    #endif
}

#ifdef Q_OS_ANDROID
void MainWindow::keepScreenOn()
{
    QJniObject activity = QJniObject::callStaticObjectMethod(
        "org/qtproject/qt/android/QtNative",
        "activity",
        "()Landroid/app/Activity;"
        );

    if (activity.isValid()) {
        QJniObject window = activity.callObjectMethod(
            "getWindow",
            "()Landroid/view/Window;"
            );

        if (window.isValid()) {
            const int FLAG_KEEP_SCREEN_ON = 128;
            window.callMethod<void>(
                "addFlags",
                "(I)V",
                FLAG_KEEP_SCREEN_ON
                );
        }
    }

    QJniEnvironment env;
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
    }
}
#endif

#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    #ifdef Q_PROCESSOR_ARM
    //QSettings::setPath(QSettings::IniFormat, QSettings::SystemScope, ".");
    #endif
    settings = new QSettings("QDmi", "QDmi");
    //settings->setPath(QSettings::IniFormat, QSettings::SystemScope, ".");
    this->setGeometry(0,0,settings->value("mainwindow/width").toInt(),settings->value("mainwindow/height").toInt());
    this->setGeometry(0,0,768,480); // For 4:3 800*600. For 16:9 848*480. For 16:10 768*480
    #ifdef Q_OS_ANDROID
    QTimer::singleShot(1000,this,SLOT(showFullScreen()));
    QTimer::singleShot(500,this,SLOT(keepScreenOn()));
    #endif
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
    connect(tcpThread, SIGNAL(started()), myTcp, SLOT(process()));
    connect(tcpThread, SIGNAL(started()), this, SLOT(process()));
    connect(sepThread, SIGNAL(started()), mySep, SLOT(process()));
    lzbThread->start();
    sepThread->start();
    mtdThread->start();
    tcpThread->start();
//    myTcp->process();
//    mySep->process();
//    this->process();
}

void MainWindow::process(){
    qreal dotsPerInch = QApplication::screens().at(0)->logicalDotsPerInch();
    ui->widgetTacho->setDpi(dotsPerInch);
    ui->widgetPower->setDpi(dotsPerInch);
    ui->widgetPower->setDpi(dotsPerInch);
    ui->widgetMano1->setDpi(dotsPerInch);
    ui->widgetMano2->setDpi(dotsPerInch);
    ui->FieldE5to7->setTextFieldUsing(3);       // Bring PZB/LZB Inidicators in front.
    ui->FieldE5to9->setTextFieldUsing(5);
    ui->FieldE8to9->setTextFieldUsing(2);
    ui->settingsTitel->setBorderThickness(0);
    ui->settingsTitel->addTextMessage(" Einstellungen",era::grey,era::black,0);
    ui->mainTitel->setBorderThickness(0);
    ui->mainTitel->addTextMessage(" Hauptmenü",era::grey,era::black,0);
    ui->qdmiSettingsTitel->setBorderThickness(0);
    ui->qdmiSettingsTitel->addTextMessage(" QDmi Einstellungen",era::grey,era::black,0);
    ui->zusiIpTitel->setBorderThickness(0);
    ui->zusiIpTitel->addTextMessage(" Zusi 3 IP-Adresse",era::grey,era::black,1);
    ui->driverIdTitel->setBorderThickness(0);
    ui->driverIdTitel->addTextMessage(" Triebfahrzeugführernummer",era::grey,era::black,1);
    ui->trainRunningNumberTitel->setBorderThickness(0);
    ui->trainRunningNumberTitel->addTextMessage(" Zugnummer",era::grey,era::black,1);

    ui->trainDataTitel1->setBorderThickness(0);
    ui->trainDataTitel2->setBorderThickness(0);
    ui->dataEntryCompleteQuestion->setBorderThickness(0);
    ui->dataValidLabel1->setBorderThickness(0);
    ui->dataValidLabel2->setBorderThickness(0);
    ui->dataValidLabel3->setBorderThickness(0);
    ui->dataValidLabel4->setBorderThickness(0);
    ui->trainDataTitel1->setTextFieldUsing(1, Qt::AlignRight);
    ui->trainDataTitel2->setTextFieldUsing(1, Qt::AlignRight);
    ui->dataEntryCompleteQuestion->setTextFieldUsing(1, Qt::AlignCenter);
    ui->trainDataTitel1->addTextMessage("Zugdateneingabe ",era::grey,era::black,1);
    ui->trainDataTitel2->addTextMessage("Zugdaten bestätigen ",era::grey,era::black,1);
    ui->dataEntryCompleteQuestion->addTextMessage("Zugdateneingabe vollständig?",era::grey, era::darkBlue,1);

    ui->dataValidLabel1->setBorderThickness(0);
    ui->dataValidLabel2->setBorderThickness(0);
    ui->dataValidLabel3->setBorderThickness(0);
    ui->dataValidLabel4->setBorderThickness(0);
    ui->dataValidLabel1->setCustomFontFactor(0.4,Qt::AlignRight);
    ui->dataValidLabel2->setCustomFontFactor(0.4,Qt::AlignRight);
    ui->dataValidLabel3->setCustomFontFactor(0.4,Qt::AlignRight);
    ui->dataValidLabel4->setCustomFontFactor(0.4,Qt::AlignRight);
    ui->dataValidLabel1->setText("Bremsart ");
    ui->dataValidLabel2->setText("Bremshundertstel ");
    ui->dataValidLabel3->setText("Zuglänge ");
    ui->dataValidLabel4->setText("Höchstgeschwindigkeit ");

    ui->dataValidValue1->setBorderThickness(0);
    ui->dataValidValue2->setBorderThickness(0);
    ui->dataValidValue3->setBorderThickness(0);
    ui->dataValidValue4->setBorderThickness(0);
    ui->dataValidValue1->setCustomFontFactor(0.4,Qt::AlignLeft);
    ui->dataValidValue2->setCustomFontFactor(0.4,Qt::AlignLeft);
    ui->dataValidValue3->setCustomFontFactor(0.4,Qt::AlignLeft);
    ui->dataValidValue4->setCustomFontFactor(0.4,Qt::AlignLeft);

    ui->dataEntryCompleteButton->setCustomFontFactor(0.3, Qt::AlignCenter);
    ui->systemVersionTitel->setBorderThickness(0);
    ui->systemVersionTitel->addTextMessage(" System Version",era::grey,era::black,1);
    ui->systemVersionComp1Name->setBorderThickness(0);
    ui->systemVersionComp1Name->setTextFieldUsing(1, Qt::AlignRight);
    ui->systemVersionComp1Name->addTextMessage("QDmi ",era::grey,era::darkBlue,1);
    ui->systemVersionComp2Name->setBorderThickness(0);
    ui->systemVersionComp2Name->setTextFieldUsing(1, Qt::AlignRight);
    ui->systemVersionComp2Name->addTextMessage("github.com/nones",era::grey,era::darkBlue,1);
    ui->systemVersionComp1Version->setBorderThickness(0);
    ui->systemVersionComp1Version->addTextMessage("1.3T4",era::grey,era::darkBlue,1);
    ui->systemVersionComp2Version->setBorderThickness(0);
    ui->systemVersionComp2Version->addTextMessage("ense84/QDmi",era::grey,era::darkBlue,1);
    qRegisterMetaType< QVector<quint8> >("QVector<quint8>");
    qRegisterMetaType< QVector<qint16> >("QVector<qint16>");
    connect(ui->fieldF1,SIGNAL(clicked(bool)),this,SLOT(openMainMenu()));
    connect(ui->fieldF3,SIGNAL(clicked(bool)),this,SLOT(fieldF3Clicked()));
    connect(ui->fieldF4,SIGNAL(clicked(bool)),this,SLOT(fieldF4Clicked()));
    connect(ui->fieldF5,SIGNAL(clicked(bool)),this,SLOT(openSettings()));
    connect(ui->fieldE10,SIGNAL(clicked(bool)),this,SLOT(arrowUpClicked()));
    connect(ui->fieldE11,SIGNAL(clicked(bool)),this,SLOT(arrowDownClicked()));
    connect(ui->settingsBtn1,SIGNAL(clicked(bool)),this,SLOT(settingsBtn1Clicked()));
    connect(ui->settingsBtn2,SIGNAL(clicked(bool)),this,SLOT(settingsBtn2Clicked()));
    connect(ui->settingsBtn3,SIGNAL(clicked(bool)),this,SLOT(settingsBtn3Clicked()));
    connect(ui->settingsBtn4,SIGNAL(clicked(bool)),this,SLOT(openSystemVersionInfo()));
    connect(ui->settingsBtn5,SIGNAL(clicked(bool)),this,SLOT(openQDmiSettings()));
    connect(ui->settingsBtn6,SIGNAL(clicked(bool)),this,SLOT(openNetworkSettings()));
    connect(ui->settingsBtn7,SIGNAL(clicked(bool)),this,SLOT(closeQDmi()));
    connect(ui->settingsBtn8,SIGNAL(clicked(bool)),this,SLOT(settingsBtn8Clicked()));

    connect(ui->mainBtn1,SIGNAL(clicked(bool)),this,SLOT(openTrainDataEntry()));
    connect(ui->mainBtn2,SIGNAL(clicked(bool)),this,SLOT(openDriverIdEntry()));
    connect(ui->mainBtn3,SIGNAL(clicked(bool)),this,SLOT(openTrainRunnimgNumberEntry()));

    connect(ui->fieldG12_geopos_H,SIGNAL(clicked(bool)),this,SLOT(geoPositionClicked()));
    connect(ui->fieldG12_geopos_T,SIGNAL(clicked(bool)),this,SLOT(geoPositionClicked()));
    connect(ui->fieldG12_btn,SIGNAL(clicked(bool)),this,SLOT(geoPositionClicked()));
    ui->fieldG12_btn->setWorking(false, false, false);
    ui->fieldG12_geopos_T->setUnclosedFrame(false, true, false, false);
    ui->fieldG12_geopos_H->setUnclosedFrame(true, false, false, false);
    ui->fieldG12_geopos_H->setCustomFontFactor(0.22, Qt::AlignLeft);
    ui->fieldG12_geopos_T->setCustomFontFactor(0.3,  Qt::AlignRight);
    connect(ui->settingsClose,SIGNAL(clicked(bool)),this,SLOT(settingsCloseClicked()));
    connect(ui->settingsClose_6,SIGNAL(clicked(bool)),this,SLOT(settingsCloseClicked()));
    connect(ui->mainClose,SIGNAL(clicked(bool)),this,SLOT(settingsCloseClicked()));

    connect(ui->dataEntryCompleteButton,SIGNAL(clicked(bool)),this,SLOT(TdeCompeteClicked()));
    connect(ui->dataValidButton,SIGNAL(txtBtnClicked(QString)),myZusiTrainData,SLOT(dataValidButtonPressed(QString)));
    connect(ui->dataValidBtnNo,SIGNAL(txtBtnClicked(QString)),ui->dataValidButton,SLOT(setAsButton(QString)));
    connect(ui->dataValidBtnYes,SIGNAL(txtBtnClicked(QString)),ui->dataValidButton,SLOT(setAsButton(QString)));

    connect(ui->dataEntryClose,SIGNAL(clicked(bool)),myZusiTrainData,SLOT(finalizeDataEntry()));
    connect(ui->numBtn0,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->numBtn1,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->numBtn2,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->numBtn3,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->numBtn4,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->numBtn5,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->numBtn6,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->numBtn7,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->numBtn8,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->numBtn9,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->numBtn10,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->numBtn11,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->alphaBtn0,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->alphaBtn1,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->alphaBtn2,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->alphaBtn3,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->alphaBtn4,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->alphaBtn5,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->alphaBtn6,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->alphaBtn7,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->alphaBtn8,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->alphaBtn9,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->alphaBtn10,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->alphaBtn11,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->braBtn1,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->braBtn2,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->braBtn3,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->braBtn4,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->braBtn6,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->braBtn7,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->braBtn8,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->braBtn9,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->dataEntryClose, SIGNAL(clicked(bool)),this,SLOT(settingsCloseClicked()));
    connect(ui->dataEntrySettings,SIGNAL(clicked(bool)),this,SLOT(openSettings()));
    connect(ui->dataEntryTrn, SIGNAL(clicked(bool)),this,SLOT(openTrainRunnimgNumberEntry()));

    connect(ui->zusiIpOkBtn,SIGNAL(dataEntryBtnClicked(QString, bool)),this,SLOT(applyClicked(QString, bool)));
    connect(ui->driverIdOkBtn,SIGNAL(dataEntryBtnClicked(QString, bool)),this,SLOT(applyClicked(QString, bool)));
    connect(ui->trainRunningNumberOkBtn,SIGNAL(dataEntryBtnClicked(QString, bool)),this,SLOT(applyClicked(QString, bool)));
    connect(ui->systemVersionClose,SIGNAL(clicked(bool)),this,SLOT(settingsCloseClicked()));
    connect(ui->FieldE5to9,SIGNAL(messaesOutOfView(bool)),this,SLOT(messaesOutOfViewHandling5to9(bool)));
    connect(ui->FieldE8to9,SIGNAL(messaesOutOfView(bool)),this,SLOT(messaesOutOfViewHandling8to9(bool)));
    connect(this,SIGNAL(naivationArrowClick(qint8)),ui->FieldE5to9,SLOT(shiftTextMessageOffset(qint8)));
    connect(this,SIGNAL(naivationArrowClick(qint8)),ui->FieldE8to9,SLOT(shiftTextMessageOffset(qint8)));
    connect(mySep,SIGNAL(newLzbIndicators(QVector<quint8>)),myLzb,SLOT(setStates(QVector<quint8>)));
    connect(myTcp->myIndicators,SIGNAL(newLzbIndicators(QVector<quint8>)),myLzb,SLOT(setStates(QVector<quint8>)));
    connect(myTcp->myIndicators,SIGNAL(newTextMessage(QString, QColor, QColor, quint8)),ui->FieldE5to7,SLOT(addTextMessage(QString, QColor, QColor, quint8)));
    connect(myTcp->myIndicators,SIGNAL(newTechnicalMessage(QString, QColor, QColor, quint8)),ui->FieldE8to9,SLOT(addTextMessage(QString, QColor, QColor, quint8)));
    connect(myTcp->myIndicators,SIGNAL(removeMessage(quint8)),ui->FieldE5to7,SLOT(removeTextMessage(quint8)));
    connect(myTcp->myIndicators,SIGNAL(removeTechnicalMessage(quint8)),ui->FieldE8to9,SLOT(removeTextMessage(quint8)));
    connect(myTcp->myIndicators,SIGNAL(newAfbSoll(quint16, bool)),ui->widgetTacho,SLOT(setVSet(quint16, bool)));
    connect(mySep,SIGNAL(newAfbSoll(quint16, bool)),ui->widgetTacho,SLOT(setVSet(quint16, bool)));
    connect(mySep,SIGNAL(newFzgVmaxTacho(quint16)),ui->widgetTacho,SLOT(setVMaxDial(quint16)));
    connect(myTcp->myIndicators,SIGNAL(newFzgVmaxTacho(quint16)),ui->widgetTacho,SLOT(setVMaxDial(quint16)));
    connect(myTcp,SIGNAL(newTechnicalMessage(QString, QColor, QColor, quint8)),ui->FieldE8to9,SLOT(addTextMessage(QString, QColor, QColor, quint8)));
    connect(myTcp,SIGNAL(newTechnicalMessage(QString, QColor, QColor, quint8)),ui->FieldE5to9,SLOT(addTextMessage(QString, QColor, QColor, quint8)));
    connect(myTcp,SIGNAL(removeTechnicalMessage(quint8)),ui->FieldE5to9,SLOT(removeTextMessage(quint8)));
    connect(myTcp,SIGNAL(removeTechnicalMessage(quint8)),ui->FieldE8to9,SLOT(removeTextMessage(quint8)));
    connect(mySep,SIGNAL(newLzbValues(QVector<quint8>)),myLzb,SLOT(setAnalogValues(QVector<quint8>)));
    connect(myTcp->myIndicators,SIGNAL(newLzbValues(QVector<quint8>)),myLzb,SLOT(setAnalogValues(QVector<quint8>)));
    connect(mySep,SIGNAL(newMtdIndicators(QVector<quint8>)),myMtd,SLOT(setStates(QVector<quint8>)));
    connect(myTcp,SIGNAL(newMtdIndicators(QVector<quint8>)),myMtd,SLOT(setStates(QVector<quint8>)));
    connect(mySep,SIGNAL(newSpeed(quint16)),ui->widgetTacho,SLOT(setVAct(quint16)));
    connect(myTcp,SIGNAL(newSpeed(quint16)),ui->widgetTacho,SLOT(setVAct(quint16)));
    connect(mySep,SIGNAL(newSpeed(quint16)),myLzb,SLOT(setVAct(quint16)));
    connect(myTcp,SIGNAL(newSpeed(quint16)),myLzb,SLOT(setVAct(quint16)));
    connect(myTcp->myIndicators,SIGNAL(startDriving()),this,SLOT(disableTde()));
    connect(myTcp->myIndicators,SIGNAL(stopDriving ()),this,SLOT(enableTde()));
    connect(myTcp,SIGNAL(newHlb(quint16)),ui->widgetMano1,SLOT(setPressure1(quint16)));
    connect(myTcp,SIGNAL(newHll(quint16)),ui->widgetMano1,SLOT(setPressure2(quint16)));
    connect(myTcp,SIGNAL(newBrz(quint16)),ui->widgetMano2,SLOT(setPressure1(quint16)));
    connect(mySep,SIGNAL(newHlb(quint16)),ui->widgetMano1,SLOT(setPressure1(quint16)));
    connect(mySep,SIGNAL(newHll(quint16)),ui->widgetMano1,SLOT(setPressure2(quint16)));
    connect(mySep,SIGNAL(newBrz(quint16)),ui->widgetMano2,SLOT(setPressure1(quint16)));
    connect(myTcp,SIGNAL(newKilometrierung(qint32)),this,SLOT(setGeoPosition(qint32)));
    connect(mySep,SIGNAL(newGeoPos(qint32)),this,SLOT(setGeoPosition(qint32)));
    connect(myTcp,SIGNAL(newBra(quint16)),myZusiTrainData,SLOT(setActiveBra(quint16)));
    connect(myTcp,SIGNAL(newBrh(quint16)),myZusiTrainData,SLOT(setActiveBrh(quint16)));
    connect(myTcp,SIGNAL(newZl (quint16)),myZusiTrainData,SLOT(setActiveZl (quint16)));
    connect(myTcp,SIGNAL(newVmz(quint16)),myZusiTrainData,SLOT(setActiveVmz(quint16)));
    connect(myTcp,SIGNAL(changedTrain()),myZusiTrainData,SLOT(finalizeDataEntry()));
    connect(myTcp,SIGNAL(newBra(quint16)),ui->dataValidValue1,SLOT(setText(quint16)));
    connect(myTcp,SIGNAL(newBrh(quint16)),ui->dataValidValue2,SLOT(setText(quint16)));
    connect(myTcp,SIGNAL(newZl (quint16)),ui->dataValidValue3,SLOT(setText(quint16)));
    connect(myTcp,SIGNAL(newVmz(quint16)),ui->dataValidValue4,SLOT(setText(quint16)));
    connect(myTcp,SIGNAL(newCabActive(bool, bool)),this,SLOT(cabActivation(bool, bool)));
  //connect(myTcp,SIGNAL(newTastaturkommando(quint16)),myZusiTrainData,SLOT(setKeycommand(quint16))); // Not necessary, aslong I dont use 'Wachsam' for validation
    connect(myTcp->myIndicators,SIGNAL(lzbAvailable(bool)),myZusiTrainData,SLOT(setLzbAvailable(bool)));
    connect(myTcp->myIndicators,SIGNAL(lzbAvailable(bool)),ui->dataValidLabel3,SLOT(setVisib(bool)));
    connect(myTcp->myIndicators,SIGNAL(lzbAvailable(bool)),ui->dataValidLabel4,SLOT(setVisib(bool)));
    connect(myTcp->myIndicators,SIGNAL(lzbAvailable(bool)),ui->dataValidValue3,SLOT(setVisib(bool)));
    connect(myTcp->myIndicators,SIGNAL(lzbAvailable(bool)),ui->dataValidValue4,SLOT(setVisib(bool)));

    connect(myZusiTrainData,SIGNAL(newTraindata(quint16, quint16, quint16, quint16, bool)),myTcp,SLOT(setTrainData(quint16, quint16, quint16, quint16, bool)));
    connect(myZusiTrainData,SIGNAL(newBehavBraBtn(QString, bool, bool, bool, bool)),ui->dataEntryInput1,SLOT(setAsDataEntryLabel(QString, bool, bool, bool, bool)));
    connect(myZusiTrainData,SIGNAL(newBehavBrhBtn(QString, bool, bool, bool, bool)),ui->dataEntryInput2,SLOT(setAsDataEntryLabel(QString, bool, bool, bool, bool)));
    connect(myZusiTrainData,SIGNAL(newBehavZlBtn (QString, bool, bool, bool, bool)),ui->dataEntryInput3,SLOT(setAsDataEntryLabel(QString, bool, bool, bool, bool)));
    connect(myZusiTrainData,SIGNAL(newBehavVmzBtn(QString, bool, bool, bool, bool)),ui->dataEntryInput4,SLOT(setAsDataEntryLabel(QString, bool, bool, bool, bool)));
    connect(myZusiTrainData,SIGNAL(newBehavZlLbl (QString, bool, bool, bool, bool)),ui->dataEntryLabel3,SLOT(setAsDataEntryLabel(QString, bool, bool, bool, bool)));
    connect(myZusiTrainData,SIGNAL(newBehavVmzLbl(QString, bool, bool, bool, bool)),ui->dataEntryLabel4,SLOT(setAsDataEntryLabel(QString, bool, bool, bool, bool)));
    connect(myZusiTrainData,SIGNAL(newBehavCmpltBtn(QString, bool, bool, bool, bool, bool)),ui->dataEntryCompleteButton,SLOT(setAsDataEntryLabel(QString, bool, bool, bool, bool, bool)));
    connect(myZusiTrainData,SIGNAL(closeDataEntryWindow()), this,SLOT(settingsCloseClicked()));
    connect(myZusiTrainData,SIGNAL(repeateDataEntry()), this,SLOT(openTrainDataEntry()));
    connect(myZusiTrainData,SIGNAL(requestDataEntrStrInitials(quint8, QString)), this,SLOT(setEntryStrWithMaxLength(quint8, QString)));
    connect(ui->dataEntryCompleteButton,SIGNAL(clicked(bool)),myZusiTrainData,SLOT(TdeCompeteClicked()));
    connect(myZusiTrainData,SIGNAL(requestKeyboardLayout(quint8)), this,SLOT(setKeyboardType(quint8)));
    connect(ui->dataEntryInput1,SIGNAL(dataEntryBtnClicked(QString, bool)),myZusiTrainData,SLOT(setBra(QString, bool)));
    connect(ui->dataEntryInput2,SIGNAL(dataEntryBtnClicked(QString, bool)),myZusiTrainData,SLOT(setBrh(QString, bool)));
    connect(ui->dataEntryInput3,SIGNAL(dataEntryBtnClicked(QString, bool)),myZusiTrainData,SLOT(setZl (QString, bool)));
    connect(ui->dataEntryInput4,SIGNAL(dataEntryBtnClicked(QString, bool)),myZusiTrainData,SLOT(setVmz(QString, bool)));

    ui->widgetMano2->setPointer2using(false);
    ui->widgetMano1->setPointer1color(era::red);
    ui->widgetMano1->setPointer2color(era::yellow);
    ui->widgetMano2->setPointer1color(era::yellow);
    ui->widgetMano1->setPonter1Label("Hauptluftbehälter");
    ui->widgetMano1->setPonter2Label("Hauptluftleitung");
    ui->widgetMano2->setPonter1Label("Bremszylinder");
    connect(this,SIGNAL(newManometerUse(bool)),myTcp,SLOT(setUseManometer(bool)));
    connect(this,SIGNAL(tcpConnectionSettings(quint8)),myTcp,SLOT(setAutoReconnect(quint8)));
    connect(this,SIGNAL(newTextMessagesSettings(quint8)),myLzb,SLOT(setTextUsing(quint8)));
    connect(this,SIGNAL(newTextMessagesSettings(quint8)),myTcp,SLOT(setTextUsing(quint8)));
    connect(myTcp,SIGNAL(newSimTime(QString)),ui->fieldG13,SLOT(setText(QString)));
    connect(mySep,SIGNAL(newSimTime(QString)),ui->fieldG13,SLOT(setText(QString)));
    connect(myTcp,SIGNAL(newZugnummer(QString)),ui->fieldG11,SLOT(setText(QString)));
    connect(this,SIGNAL(newDriverId(QString)),myTcp,SLOT(setDriverId(QString)));
    connect(this,SIGNAL(newTrainRunningNumber(QString)),myTcp,SLOT(setTrainRunningNumber(QString)));
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
    ui->dataEntryInput1->setAsDataEntryLabel("0",true);
    ui->dataEntryInput2->setAsDataEntryLabel("0",true);
    ui->dataEntryInput3->setAsDataEntryLabel("0",true);
    ui->dataEntryInput4->setAsDataEntryLabel("0",true);
    ui->dataEntryCompleteButton->setAsDataEntryLabel("Ja",true,false,false,true,true);
    ui->dataValidButton->setAsDataEntryLabel("Nein",true,true,false,true,true);
    ui->dataValidBtnNo->setAsButton(true, "Nein");
    ui->dataValidBtnYes->setAsButton(true, "Ja");
    ui->settingsClose->setIcon(":/icons/X.svg");
    ui->settingsClose_6->setIcon(":/icons/X.svg");
    ui->mainClose->setIcon(":/icons/X.svg");
    ui->fieldE10->setIcon(":/icons/arrowUp_ena.svg",  ":/icons/arrowUp_dis.svg");
    ui->fieldE11->setIcon(":/icons/arrowDown_ena.svg",":/icons/arrowDown_dis.svg");
    ui->fieldE10->setAsButton();
    ui->fieldE11->setAsButton();
    ui->fieldE10->setWorking(false, false, false);
    ui->fieldE11->setWorking(true, false, false);
    ui->fieldF6->setVisib(false);
    ui->fieldF7->setVisib(false);
    ui->fieldF8->setVisib(false);
    ui->fieldF9->setVisib(false);
    ui->fieldC2->setVisib(false);
    ui->fieldC3->setVisib(false);
    ui->fieldC4->setVisib(false);
    ui->fieldC5->setVisib(false);
    ui->fieldC6->setVisib(false);
    ui->fieldC7->setVisib(false);
    ui->fieldF1->setAsButton(true, "Menü");
    ui->fieldF2->setAsButton();
    ui->fieldF3->setAsButton();
    ui->fieldF4->setAsButton();
    ui->fieldF5->setAsButton();
    ui->fieldF5->setIcon(":/icons/tool.svg");
    ui->dataEntrySettings->setAsButton();
    ui->dataEntrySettings->setIcon(":/icons/tool_small.svg");
    ui->dataEntryTrn->setAsButton(true,"Zug");
    ui->settingsBtn1->setAsButton(false);
    ui->settingsBtn2->setAsButton(false);
    ui->settingsBtn3->setAsButton(false);
    ui->settingsBtn4->setAsButton(true,"System version");
    ui->settingsBtn5->setAsButton();
    ui->settingsBtn6->setAsButton();
    ui->settingsBtn7->setAsButton();
    ui->settingsBtn8->setAsButton(false);
    ui->settingsClose->setAsButton();
    ui->settingsClose_6->setAsButton();

    ui->mainBtn1->setAsButton(true, "Zugdateneingabe");
    ui->mainBtn2->setAsButton(true, "TF-Nummer");
    ui->mainBtn3->setAsButton(true, "Zugnummer");
    ui->mainBtn1->setCustomFontFactor(0.28);
    ui->mainBtn2->setCustomFontFactor(0.28);
    ui->mainBtn3->setCustomFontFactor(0.28);
    ui->mainClose->setAsButton();


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
    ui->braBtn1->setAsButton(true, "G", "1");
    ui->braBtn2->setAsButton(true, "G KlBr", "2");
    ui->braBtn3->setAsButton(true, "G SchBr", "3");
    ui->braBtn4->setAsButton(true, "P", "4");
    ui->braBtn6->setAsButton(true, "P/R + KlBr", "6");
    ui->braBtn7->setAsButton(true, "P/R + SchBr SW", "7");
    ui->braBtn8->setAsButton(true, "P/R ", "8");
    ui->braBtn9->setAsButton(true, "P/R + SchBr", "9");

    ui->zusiIpOkBtn->setAsButton();
    ui->zusiIpOkBtn->setAsDataEntryLabel("Nach Eingabe hier tippen",true,true);
    ui->driverIdOkBtn->setAsButton();
    ui->driverIdOkBtn->setAsDataEntryLabel("",true,true);
    ui->trainRunningNumberOkBtn->setAsButton();
    ui->trainRunningNumberOkBtn->setAsDataEntryLabel("",true,true);
    ui->dataEntryClose->setAsButton();
    ui->dataEntryClose->setIcon(":/icons/X.svg");
    ui->numBtn0->setText("0",era::grey,era::darkGrey,QFont::Light);

    ui->systemVersionClose->setAsButton();
    ui->systemVersionClose->setIcon(":/icons/X.svg");
    ui->fieldG11->setText("",era::grey,era::darkGrey,QFont::Light);
    ui->fieldG11->setCustomFontFactor(0.24);
    ui->fieldG13->setText("--:--:--",era::grey,era::darkGrey,QFont::Light);
    ui->fieldG13->setCustomFontFactor(0.24);
    connectTimers();
    connectPzbIcons();
    connectMtdIcons();
    connectMtdPower();
    connectTcpStuff();
    ui->fieldA3->setIsDistanceScale();
    ui->fieldA3->setTargetDistance(0,false);
    ui->widgetTacho->setVMaxDial(400);
    configureSettingsWindow();
    emit newManometerUse(showManometer > 0);
    resizeMe();
}

void MainWindow::connectPzbIcons(){
    connect(myLzb,SIGNAL(newIconBehav1(bool,quint8,bool)),ui->fieldCL1,SLOT(setWorking(bool,quint8,bool)));
    connect(myLzb,SIGNAL(newIconBehav2(bool,quint8,bool)),ui->fieldCL2,SLOT(setWorking(bool,quint8,bool)));
    connect(myLzb,SIGNAL(newIconBehav3(bool,quint8,bool)),ui->fieldCL3,SLOT(setWorking(bool,quint8,bool)));
    connect(myLzb,SIGNAL(newIconBehav4(bool,quint8,bool)),ui->fieldCL4,SLOT(setWorking(bool,quint8,bool)));
    connect(myLzb,SIGNAL(newIconBehav5(bool,quint8,bool)),ui->fieldCL5,SLOT(setWorking(bool,quint8,bool)));
    connect(myLzb,SIGNAL(newIconBehav6(bool,quint8,bool)),ui->fieldCL6,SLOT(setWorking(bool,quint8,bool)));
    connect(myLzb,SIGNAL(newIconBehav7(bool,quint8,bool)),ui->fieldCL7,SLOT(setWorking(bool,quint8,bool)));
    connect(myLzb,SIGNAL(newIconBehavC9(bool,quint8,bool)),ui->fieldC9,SLOT(setWorking(bool,quint8,bool)));
    connect(myLzb,SIGNAL(newIconBehavG10(bool,quint8,bool)),ui->fieldG10,SLOT(setWorking(bool,quint8,bool)));
    connect(myLzb,SIGNAL(newIcon1(QString,QString)),ui->fieldCL1,SLOT(setIcon(QString,QString)));
    connect(myLzb,SIGNAL(newIcon2(QString,QString)),ui->fieldCL2,SLOT(setIcon(QString,QString)));
    connect(myLzb,SIGNAL(newIcon3(QString,QString)),ui->fieldCL3,SLOT(setIcon(QString,QString)));
    connect(myLzb,SIGNAL(newIcon4(QString,QString)),ui->fieldCL4,SLOT(setIcon(QString,QString)));
    connect(myLzb,SIGNAL(newIcon5(QString,QString)),ui->fieldCL5,SLOT(setIcon(QString,QString)));
    connect(myLzb,SIGNAL(newIcon6(QString,QString)),ui->fieldCL6,SLOT(setIcon(QString,QString)));
    connect(myLzb,SIGNAL(newIcon7(QString,QString)),ui->fieldCL7,SLOT(setIcon(QString,QString)));
    connect(myLzb,SIGNAL(newIconC9(QString,QString)),ui->fieldC9,SLOT(setIcon(QString,QString)));
    connect(myLzb,SIGNAL(newIconG10(QString,QString)),ui->fieldG10,SLOT(setIcon(QString,QString)));
    connect(myLzb,SIGNAL(newTextMessage(QString, QColor, QColor, quint8)),ui->FieldE5to7,SLOT(addTextMessage(QString, QColor, QColor, quint8)));
    connect(myLzb,SIGNAL(removeMessage(quint8)),ui->FieldE5to7,SLOT(removeTextMessage(quint8)));
    ui->fieldVZile100->setSegmentDigitToUse(3);
    ui->fieldVZile10->setSegmentDigitToUse(2);
    ui->fieldVZile1->setSegmentDigitToUse(1);
    connect(myLzb,SIGNAL(newVTarget(quint16, bool)),ui->fieldVZile100,SLOT(setSegmentText(quint16, bool)));
    connect(myLzb,SIGNAL(newVTarget(quint16, bool)),ui->fieldVZile10,SLOT(setSegmentText(quint16, bool)));
    connect(myLzb,SIGNAL(newVTarget(quint16, bool)),ui->fieldVZile1,SLOT(setSegmentText(quint16, bool)));
    connect(myLzb,SIGNAL(newVTarget(quint16, bool)),ui->widgetTacho,SLOT(setVTarget(quint16, bool)));
    connect(myLzb,SIGNAL(newVPermit(quint16, bool)),ui->widgetTacho,SLOT(setVPerm(quint16, bool)));
    connect(myLzb,SIGNAL(newVMaxReducing(bool)),ui->widgetTacho,SLOT(setCsmReducing(bool)));
    connect(myLzb,SIGNAL(newOverspeed(bool)),ui->widgetTacho,SLOT(setOverspeed(bool)));
    connect(myLzb,SIGNAL(newIntervenation(bool)),ui->widgetTacho,SLOT(setIntervenation(bool)));
    connect(myLzb,SIGNAL(newTarDist(quint16, bool)),ui->fieldA3,SLOT(setTargetDistance(quint16, bool)));
}
void MainWindow::connectMtdIcons(){
    connect(myMtd,SIGNAL(newIconBehavE3(bool,quint8,bool)),ui->fieldE3,SLOT(setWorking(bool,quint8,bool)));
    connect(myMtd,SIGNAL(newIconBehavG1(bool,quint8,bool)),ui->fieldG1,SLOT(setWorking(bool,quint8,bool)));
    connect(myMtd,SIGNAL(newIconBehavG2(bool,quint8,bool)),ui->fieldG2,SLOT(setWorking(bool,quint8,bool)));
    connect(myMtd,SIGNAL(newIconBehavG3(bool,quint8,bool)),ui->fieldG3,SLOT(setWorking(bool,quint8,bool)));
    connect(myMtd,SIGNAL(newIconBehavG4(bool,quint8,bool)),ui->fieldG4,SLOT(setWorking(bool,quint8,bool)));
    connect(myMtd,SIGNAL(newIconBehavG5(bool,quint8,bool)),ui->fieldG5,SLOT(setWorking(bool,quint8,bool)));
    connect(myMtd,SIGNAL(newIconE3(QString,QString)),ui->fieldE3,SLOT(setIcon(QString,QString)));
    connect(myMtd,SIGNAL(newIconG1(QString,QString)),ui->fieldG1,SLOT(setIcon(QString,QString)));
    connect(myMtd,SIGNAL(newIconG2(QString,QString)),ui->fieldG2,SLOT(setIcon(QString,QString)));
    connect(myMtd,SIGNAL(newIconG3(QString,QString)),ui->fieldG3,SLOT(setIcon(QString,QString)));
    connect(myMtd,SIGNAL(newIconG4(QString,QString)),ui->fieldG4,SLOT(setIcon(QString,QString)));
    connect(myMtd,SIGNAL(newIconG5(QString,QString)),ui->fieldG5,SLOT(setIcon(QString,QString)));
}
void MainWindow::connectMtdPower(){
    connect(myTcp->myPower,SIGNAL(newPowerValues (QVector<qint16>)),myMtd,SLOT(setAnalogValues(QVector<qint16>)));
    connect(myMtd,SIGNAL(newPowerRelative   (qint16)),ui->widgetPower,SLOT(setPowerRelative   (qint16)));
    connect(myMtd,SIGNAL(newPowerRelativeSet(qint16)),ui->widgetPower,SLOT(setPowerRelativeSet(qint16)));
    connect(myMtd,SIGNAL(newPowerAbsolute(qint16)),ui->widgetPower,SLOT(setPowerAbsolute(qint16)));
    connect(myMtd,SIGNAL(newPowerAbsoluteSet(qint16)),ui->widgetPower,SLOT(setPowerAbsoluteSet(qint16)));
    connect(mySep,SIGNAL(newPowerRelative(qint16)),ui->widgetPower,SLOT(setPowerRelative(qint16)));
    connect(myTcp->myPower,SIGNAL(maxPowerPositiveLine (qint16)),ui->widgetPower,SLOT(setAbsoluteAccelerateMaximum(qint16)));
    connect(myTcp->myPower,SIGNAL(maxPowerNegativeLine (qint16)),ui->widgetPower,SLOT(setAbsoluteBrakingMaximum(qint16)));
    connect(myTcp->myPower,SIGNAL(unitAcceleratingText(QString)),ui->widgetPower,SLOT(setUnitAcceleratingText(QString)));
    connect(myTcp->myPower,SIGNAL(unitBrakingText(QString)),ui->widgetPower,SLOT(setUnitBrakingText(QString)));
    connect(myTcp->myPower,SIGNAL(unitBrakingText(QString)),ui->widgetPower,SLOT(setUnitBrakingText(QString)));
    connect(myTcp->myPower,SIGNAL(hasDriveModeDisplay(bool)),ui->widgetPower,SLOT(setModeDisplay(bool)));
    connect(myTcp->myPower,SIGNAL(newDriveMode(quint8)),ui->widgetPower,SLOT(setDriveMode(quint8)));
}
void MainWindow::connectTimers(){
    shortTimer  = new QTimer(this);
    shortTimer->setInterval(500);
    shortTimer->start();
    connect(shortTimer,SIGNAL(timeout()),ui->fieldCL1,SLOT(updateBlinking()));
    connect(shortTimer,SIGNAL(timeout()),ui->fieldCL2,SLOT(updateBlinking()));
    connect(shortTimer,SIGNAL(timeout()),ui->fieldCL3,SLOT(updateBlinking()));
    connect(shortTimer,SIGNAL(timeout()),ui->fieldCL4,SLOT(updateBlinking()));
    connect(shortTimer,SIGNAL(timeout()),ui->fieldCL5,SLOT(updateBlinking()));
    connect(shortTimer,SIGNAL(timeout()),ui->fieldCL6,SLOT(updateBlinking()));
    connect(shortTimer,SIGNAL(timeout()),ui->fieldCL7,SLOT(updateBlinking()));
    connect(shortTimer,SIGNAL(timeout()),ui->fieldG2,SLOT(updateBlinking()));       // Doors
    //connect(shortTimer,SIGNAL(timeout()),ui->fieldG13,SLOT(updateBlinking()));
    connect(shortTimer,SIGNAL(timeout()),this,SLOT(blinkCursor()));
}
void MainWindow::connectTcpStuff(){
    connect(myTcp,SIGNAL(sendTcpConnectionFeedback(QString)),this,SLOT(gotTcpConnectionFeedback(QString)));
    connect(myTcp,SIGNAL(sendDataSourceIsZusi(bool)),myLzb,SLOT(setZusiAsDataSource(bool)));
    connect(this,SIGNAL(newZusiIp(QString)),myTcp,SLOT(setIpadress(QString)));
    connect(mySep,SIGNAL(newLevelInforamtion(quint8)),this,SLOT(setLevel(quint8)));
    connect(myTcp->myIndicators,SIGNAL(newLevelInforamtion(quint8)),this,SLOT(setLevel(quint8)));
}

void MainWindow::gotTcpConnectionFeedback(QString feedback){
    if(feedback.contains("."))settings->setValue("zusiIp",feedback);
    if(feedback == "-2")      emit newZusiIp(settings->value("zusiIp").toString());

}
MainWindow::~MainWindow(){delete ui;}
void MainWindow::setLevel(quint8 level){
    if(level == actLevel) return;
    if(level == levelPzbLzbNtc){
        ui->fieldB7->setIcon(":/icons/MO_19.svg");
        ui->fieldC8->setIcon(":/icons/LE_02a.svg");
        ui->fieldE_holder2->setCurrentIndex(1);
        ui->fieldC_holder->setCurrentIndex(1);
        ui->fieldC_holder2->setCurrentIndex(0);
        actLevel = levelPzbLzbNtc;
        messaesOutOfViewHandling8to9(MessaesOutOffView8to9);
    }
    if(level == levelUndefined){
        ui->fieldB7->setIcon(":/icons/blanc.svg");
        ui->fieldC8->setIcon(":/icons/blanc.svg");
        ui->fieldE_holder2->setCurrentIndex(0);
        ui->fieldC_holder->setCurrentIndex(0);
        ui->fieldC_holder2->setCurrentIndex(1);
        actLevel = levelUndefined;
        messaesOutOfViewHandling8to9(MessaesOutOffView8to9);
    }
}
void MainWindow::messaesOutOfViewHandling5to9(bool outOfView){
    MessaesOutOffView5to9 = outOfView;
    if(actLevel != levelPzbLzbNtc){
        if(outOfView){
            ui->fieldE10->setWorking(true, false, false);
            ui->fieldE11->setWorking(true, false, false);
        }
        else{
            ui->fieldE10->setWorking(false, false, false);
            ui->fieldE11->setWorking(false, false, false);
        }
    }
}
void MainWindow::messaesOutOfViewHandling8to9(bool outOfView){
    MessaesOutOffView8to9 = outOfView;
    if(actLevel == levelPzbLzbNtc){
        if(outOfView){
            ui->fieldE10->setWorking(true, false, false);
            ui->fieldE11->setWorking(true, false, false);
        }
        else{
            ui->fieldE10->setWorking(false, false, false);
            ui->fieldE11->setWorking(false, false, false);
        }
    }
}
void MainWindow::openMainMenu(){
    ui->fieldDG->setCurrentIndex(P_DG_Main_menu);                // Page with main menu
}
void MainWindow::fieldF3Clicked(){}
void MainWindow::setKeyboardType(quint8 type){
    setKeyboardType(type, false);
}
void MainWindow::setKeyboardType(quint8 type, bool showDott){
    actKeyboardType = type;
    ui->keyboards->setCurrentIndex(type);
    ui->numBtn11->setAsButton(showDott, ".");
}
void MainWindow::TdeCompeteClicked(){
    ui->keyboards->setCurrentIndex(P_Keyboard_YesNo);                   // Data entry page on the right keyboard "Yes" and "No" for validation
    ui->entryFields->setCurrentIndex(P_EntryButton_YesNo);              // Data entry page on the right konfirm button for validation -> [Yes | No]
    ui->dataValidationHolder2->setCurrentIndex(P_entry_not_complete);   // Data entry page validation on the left buttom "Entry complerte? Yes" not visible
    ui->dataValidationHolder3->setCurrentIndex(P_entry_not_complete);   // Data entry page validation on the left top. Titel changess from data entry to data validation
}
void MainWindow::fieldF4Clicked(){}
void MainWindow::openSettings(){
    ui->fieldDG->setCurrentIndex(P_DG_Settings);
    ui->TrnSettingsHolder->setCurrentIndex(0);                      // In case previous window was driver ID, triggered by cab activation
}
void MainWindow::openTrainDataEntry(){
    ui->fieldDG->setCurrentIndex(P_DG_Data_entry);                  // Data entry page for all kind of data
    ui->entryFields->setCurrentIndex(P_entry_TrainData);            // Data entry page for LZB
    ui->dataValidationHolder2->setCurrentIndex(P_entry_complete);   // Data entry page validation on the left buttom "Entry complerte? Yes" visible
    ui->dataValidationHolder3->setCurrentIndex(P_entry_complete);   // Data entry page validation on the left top. Titel changess from data validation to data entry
    setKeyboardType(P_Keyboard_Numeric, false);                     // Numeric keyboard without dot
    ui->fieldABE->setCurrentIndex(P_ABE_DataEntry);                 // Data entry page on the left
    ui->dataValidButton->setAsButton(true, "Nein");
    activeDataEntryItem = "trainData";
    dataString = "";
}
void MainWindow::cabActivation(bool cabActivated, bool standstill){
    if(cabActivated && standstill)ui->TrnSettingsHolder->setCurrentIndex(1);
    if(cabActivated && standstill)openDriverIdEntry();
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
    ui->TrnSettingsHolder->setCurrentIndex(0);                      // In case previous window was driver ID, triggered by cab activation
}
void MainWindow::arrowUpClicked(){
    emit naivationArrowClick(-1);
}
void MainWindow::arrowDownClicked(){
    emit naivationArrowClick(1);
}
void MainWindow::settingsBtn1Clicked(){}
void MainWindow::settingsBtn2Clicked(){}
void MainWindow::settingsBtn3Clicked(){}
void MainWindow::openSystemVersionInfo(){ui->fieldDG->setCurrentIndex(P_DG_System_Version);}    // System Version
void MainWindow::openQDmiSettings(){ui->fieldDG->setCurrentIndex(P_DG_QDmi);}                   // QDmi
void MainWindow::openNetworkSettings(){                                     // Network
    ui->fieldDG->setCurrentIndex(P_DG_Data_entry);                          // Data entry page for all kind of data
    setKeyboardType(P_Keyboard_Numeric, true);                              // Data entry page on the right numeric keyboard
    ui->entryFields->setCurrentIndex(P_entry_ZusiIp);                       // Data entry page on the right for IP adress
    dataString = settings->value("zusiIp").toString();
    activeDataEntryItem = "IP-Address";
    setEntryStrWithMaxLength(15, settings->value("zusiIp").toString());     // Limit entry string 15 characters and set start value from settings
    ui->FieldE5to9->addTextMessage(" Auf eingegebene IP-Adresse tippen", era::grey, era::darkBlue, 8);
    ui->FieldE8to9->addTextMessage(" Auf eingegebene IP-Adresse tippen", era::grey, era::darkBlue, 8);
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    ui->FieldE5to9->addTextMessage(" IP-Adressen dieses Gerätes:", era::grey, era::darkBlue, 100);
    ui->FieldE8to9->addTextMessage(" IP-Adressen dieses Gerätes:", era::grey, era::darkBlue, 100);
    quint8 i = 0;
    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost){
            ui->FieldE5to9->addTextMessage(" " + address.toString(), era::grey, era::darkBlue, 101 + i);
            ui->FieldE8to9->addTextMessage(" " + address.toString(), era::grey, era::darkBlue, 101 + i);
            i++;
        }
    }

}

void MainWindow::disableTde(){
    ui->fieldF1->setAsButton(false, "Menü");
    settingsCloseClicked();
    settingsCloseClicked();
}

void MainWindow::enableTde(){
    ui->fieldF1->setAsButton(true, "Menü");
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
       ui->fieldABE->setCurrentIndex(P_ABE_DefaultWindow);                 // Data entry page on the right
       ui->TrnSettingsHolder->setCurrentIndex(0);
       if(ui->entryFields->currentIndex() == P_entry_ZusiIp){
           ui->FieldE5to9->removeTextMessage(8);
           ui->FieldE8to9->removeTextMessage(8);
           ui->FieldE5to9->removeTextMessage(100);
           ui->FieldE8to9->removeTextMessage(100);
           ui->FieldE5to9->removeTextMessage(101);
           ui->FieldE8to9->removeTextMessage(101);
           ui->FieldE5to9->removeTextMessage(102);
           ui->FieldE8to9->removeTextMessage(102);
           ui->FieldE5to9->removeTextMessage(103);
           ui->FieldE8to9->removeTextMessage(103);
           ui->fieldDG->setCurrentIndex(P_DG_Settings);
           return;
       }
       ui->fieldDG->setCurrentIndex(P_DG_Main_menu);
       return;
    }
    if(ui->fieldDG->currentIndex() == P_DG_Main_menu){   // Main menu
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
        myZusiTrainData->setTextFromKeyboard(dataString);
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
    }
    ui->manoHolder->setSizePolicy(manoSizePol);
    ui->manoHolder_Spacer->setSizePolicy(manoSizePol);

    if ((windowWidth / windowHeight) < windowRatio){
        multi = windowWidth / defaultWidth;
    }
    else{
        multi = windowHeight / defaultHeight;
    }
    QFont settingsFont = QFont("FreeSans", multi * 1100.0 / dpi,QFont::Bold,false);
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
    ui->fieldA1->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldA3->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldA4->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldB->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldB3->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldB4->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldB5->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldB6->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldB7->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldC1->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldC1l->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldC2->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldC3->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldC4->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldC5->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldC6->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldC7->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldVZile1->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldVZile10->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldVZile100->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldCL1->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldCL2->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldCL3->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldCL4->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldCL5->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldCL6->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldCL7->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldC8->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldC9->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldE1->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldE2->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldE3->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldE4->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->FieldE5to9->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->FieldE5to7->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->FieldE8to9->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldE10->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldE11->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldD_planning->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldD_power->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldF1->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldF2->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldF3->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldF4->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldF5->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldF6->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldF7->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldF8->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldF9->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldG1->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldG2->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldG3->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldG4->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldG5->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldG6->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldG7->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldG8->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldG9->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldG10->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldG11->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldG12_btn->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldG12_geopos_T->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldG12_geopos_H->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->fieldG13->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->settingsBtn1->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->settingsBtn2->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->settingsBtn3->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->settingsBtn4->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->settingsBtn5->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->settingsBtn6->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->settingsBtn7->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->settingsBtn8->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->settingsClose->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->settingsClose_6->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->dataEntrySettings->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->dataEntryTrn->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->mainBtn1->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->mainBtn2->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->mainBtn3->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->mainClose->setBorderThickness(static_cast<int>(multi + 0.75));
    // Data entry buttons
    ui->numBtn0->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->numBtn1->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->numBtn2->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->numBtn3->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->numBtn4->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->numBtn5->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->numBtn6->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->numBtn7->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->numBtn8->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->numBtn9->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->numBtn10->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->numBtn11->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->alphaBtn0->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->alphaBtn1->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->alphaBtn2->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->alphaBtn3->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->alphaBtn4->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->alphaBtn5->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->alphaBtn6->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->alphaBtn7->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->alphaBtn8->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->alphaBtn9->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->alphaBtn10->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->alphaBtn11->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->dataEntryClose->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->zusiIpOkBtn->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->driverIdOkBtn->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->trainRunningNumberOkBtn->setBorderThickness(static_cast<int>(multi + 0.75));

    ui->systemVersionClose->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->dataEntryLabel1->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->dataEntryLabel2->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->dataEntryLabel3->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->dataEntryLabel4->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->dataEntryInput1->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->dataEntryInput2->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->dataEntryInput3->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->dataEntryInput4->setBorderThickness(static_cast<int>(multi + 0.75));

    ui->dataEntryCompleteButton->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->dataValidButton->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->dataValidBtnYes->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->dataValidBtnNo->setBorderThickness(static_cast<int>(multi + 0.75));

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
    applySettings();
    emit newZusiIp(settings->value("zusiIp").toString());

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
#ifdef Q_PROCESSOR_ARM
void MainWindow::keyPressEvent(QKeyEvent *event){
     if(event->key() == Qt::Key_Escape) QApplication::quit();
}
#endif
#ifdef Q_OS_ANDROID
void MainWindow::keepScreenOn()
{
    QAndroidJniObject activity = QtAndroid::androidActivity();
    if (activity.isValid()) {
        QAndroidJniObject window = activity.callObjectMethod("getWindow", "()Landroid/view/Window;");
        if (window.isValid()) {
            const int FLAG_KEEP_SCREEN_ON = 128;
            window.callMethod<void>("addFlags", "(I)V", FLAG_KEEP_SCREEN_ON);
        }
    }
    QAndroidJniEnvironment env;
        if (env->ExceptionCheck())
        {
            env->ExceptionClear();
        }
}
#endif

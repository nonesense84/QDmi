#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    settings = new QSettings("QDmi", "QDmi");
    this->setGeometry(0,0,settings->value("mainwindow/width").toInt(),settings->value("mainwindow/height").toInt());
    this->setGeometry(0,0,800,600);
    #ifdef Q_OS_ANDROID
    QTimer::singleShot(1000,this,SLOT(showFullScreen()));
    QTimer::singleShot(500,this,SLOT(keepScreenOn()));
    #endif
    myLzb = new lzb();
    mySep = new sep();
    myMtd = new mtd();
    myTcp = new zusi3Tcp();
    qreal dotsPerInch = QApplication::screens().at(0)->logicalDotsPerInch();
    ui->widgetTacho->setDpi(dotsPerInch);
    ui->widgetPower->setDpi(dotsPerInch);
    ui->FieldE5to7->setTextFieldUsing(3);       // Bring PZB/LZB Inidicators in front.
    ui->FieldE5to9->setTextFieldUsing(5);
    ui->FieldE8to9->setTextFieldUsing(2);
    ui->settingsTitel->setBorderThickness(0);
    ui->settingsTitel->addTextMessage(" Einstellungen",era::grey,era::black,0);
    ui->zusiIpTitel->setBorderThickness(0);
    ui->zusiIpTitel->addTextMessage(" Zusi3 IP-Adresse",era::grey,era::black,1);
    ui->systemVersionTitel->setBorderThickness(0);
    ui->systemVersionTitel->addTextMessage(" System Version",era::grey,era::black,1);
    ui->systemVersionComp1Name->setBorderThickness(0);
    ui->systemVersionComp1Name->setTextFieldUsing(1, Qt::AlignRight);
    ui->systemVersionComp1Name->addTextMessage("QDmi ",era::grey,era::darkBlue,1);
    ui->systemVersionComp2Name->setBorderThickness(0);
    ui->systemVersionComp2Name->setTextFieldUsing(1, Qt::AlignRight);
    ui->systemVersionComp2Name->addTextMessage("github.com/nones",era::grey,era::darkBlue,1);
    ui->systemVersionComp1Version->setBorderThickness(0);
    ui->systemVersionComp1Version->addTextMessage("1.0",era::grey,era::darkBlue,1);
    ui->systemVersionComp2Version->setBorderThickness(0);
    ui->systemVersionComp2Version->addTextMessage("ense84/QDmi",era::grey,era::darkBlue,1);
    connect(ui->fieldF4,SIGNAL(clicked(bool)),this,SLOT(arrowF4Clicked()));
    connect(ui->fieldF5,SIGNAL(clicked(bool)),this,SLOT(arrowF5Clicked()));
    connect(ui->fieldE10,SIGNAL(clicked(bool)),this,SLOT(arrowUpClicked()));
    connect(ui->fieldE11,SIGNAL(clicked(bool)),this,SLOT(arrowDownClicked()));
    connect(ui->settingsBtn1,SIGNAL(clicked(bool)),this,SLOT(settingsBtn1Clicked()));
    connect(ui->settingsBtn2,SIGNAL(clicked(bool)),this,SLOT(settingsBtn2Clicked()));
    connect(ui->settingsBtn3,SIGNAL(clicked(bool)),this,SLOT(settingsBtn3Clicked()));
    connect(ui->settingsBtn4,SIGNAL(clicked(bool)),this,SLOT(settingsBtn4Clicked()));
    connect(ui->settingsBtn5,SIGNAL(clicked(bool)),this,SLOT(settingsBtn5Clicked()));
    connect(ui->settingsBtn6,SIGNAL(clicked(bool)),this,SLOT(settingsBtn6Clicked()));
    connect(ui->settingsBtn7,SIGNAL(clicked(bool)),this,SLOT(settingsBtn7Clicked()));
    connect(ui->settingsBtn8,SIGNAL(clicked(bool)),this,SLOT(settingsBtn8Clicked()));
    connect(ui->settingsClose,SIGNAL(clicked(bool)),this,SLOT(settingsCloseClicked()));
    connect(ui->settingsClose_6,SIGNAL(clicked(bool)),this,SLOT(settingsCloseClicked()));
    connect(ui->zusiIpClose,SIGNAL(clicked(bool)),this,SLOT(settingsCloseClicked()));
    connect(ui->zusiIpBtn0,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->zusiIpBtn1,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->zusiIpBtn2,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->zusiIpBtn3,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->zusiIpBtn4,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->zusiIpBtn5,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->zusiIpBtn6,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->zusiIpBtn7,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->zusiIpBtn8,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->zusiIpBtn9,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->zusiIpBtn10,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->zusiIpBtn11,SIGNAL(txtBtnClicked(QString)),this,SLOT(addItemToData(QString)));
    connect(ui->zusiIpOkBtn,SIGNAL(txtBtnClicked(QString)),this,SLOT(applyClicked(QString)));
    connect(ui->systemVersionClose,SIGNAL(clicked(bool)),this,SLOT(settingsCloseClicked()));
    connect(mySep,SIGNAL(newLzbIndicators(QVector<quint8>)),myLzb,SLOT(setStates(QVector<quint8>)));
    connect(myTcp->myIndicators,SIGNAL(newLzbIndicators(QVector<quint8>)),myLzb,SLOT(setStates(QVector<quint8>)));
    connect(myTcp->myIndicators,SIGNAL(newTextMessage(QString, QColor, QColor, quint8)),ui->FieldE5to7,SLOT(addTextMessage(QString, QColor, QColor, quint8)));
    connect(myTcp->myIndicators,SIGNAL(newTechnicalMessage(QString, QColor, QColor, quint8)),ui->FieldE8to9,SLOT(addTextMessage(QString, QColor, QColor, quint8)));
    connect(myTcp->myIndicators,SIGNAL(removeMessage(quint8)),ui->FieldE5to7,SLOT(removeTextMessage(quint8)));
    connect(myTcp->myIndicators,SIGNAL(removeTechnicalMessage(quint8)),ui->FieldE8to9,SLOT(removeTextMessage(quint8)));
    connect(myTcp->myIndicators,SIGNAL(newAfbSoll(quint16, bool)),ui->widgetTacho,SLOT(setVSet(quint16, bool)));
    connect(myTcp->myIndicators,SIGNAL(newFzgVmaxTacho(quint16)),ui->widgetTacho,SLOT(setVMaxDial(quint16)));
    connect(myTcp,SIGNAL(newTechnicalMessage(QString, QColor, QColor, quint8)),ui->FieldE8to9,SLOT(addTextMessage(QString, QColor, QColor, quint8)));
    connect(myTcp,SIGNAL(newTechnicalMessage(QString, QColor, QColor, quint8)),ui->FieldE5to9,SLOT(addTextMessage(QString, QColor, QColor, quint8)));
    connect(myTcp,SIGNAL(removeTechnicalMessage(quint8)),ui->FieldE5to9,SLOT(removeTextMessage(quint8)));
    connect(myTcp,SIGNAL(removeTechnicalMessage(quint8)),ui->FieldE8to9,SLOT(removeTextMessage(quint8)));
    connect(mySep,SIGNAL(newLzbValues(QVector<quint8>)),myLzb,SLOT(setAnalogValues(QVector<quint8>)));
    connect(myTcp,SIGNAL(newLzbValues(QVector<quint8>)),myLzb,SLOT(setAnalogValues(QVector<quint8>)));
    connect(mySep,SIGNAL(newMtdIndicators(QVector<quint8>)),myMtd,SLOT(setStates(QVector<quint8>)));
    connect(myTcp,SIGNAL(newMtdIndicators(QVector<quint8>)),myMtd,SLOT(setStates(QVector<quint8>)));
    connect(mySep,SIGNAL(newSpeed(qreal)),ui->widgetTacho,SLOT(setVAct(qreal)));
    connect(myTcp,SIGNAL(newSpeed(qreal)),ui->widgetTacho,SLOT(setVAct(qreal)));
    connect(mySep,SIGNAL(newSpeed(qreal)),myLzb,SLOT(setVAct(qreal)));
    connect(myTcp,SIGNAL(newSpeed(qreal)),myLzb,SLOT(setVAct(qreal)));

    ui->settingsBtn1->setIcon(":/icons/lang_ena.svg",":/icons/lang_dis.svg");
    ui->settingsBtn2->setIcon(":/icons/volume_ena.svg", ":/icons/volume_dis.svg");
    ui->settingsBtn3->setIcon(":/icons/bright_ena.svg", ":/icons/bright_dis.svg");
    ui->settingsBtn4->setText("System version",era::grey,era::darkGrey,QFont::Light);
    ui->settingsBtn5->setText("QDmi",era::grey,era::darkGrey,QFont::Light);
    ui->settingsBtn6->setIcon(":/icons/netw_ena.svg", ":/icons/netw_dis.svg");
    ui->settingsBtn1->setWorking(false, false, false);
    ui->settingsBtn2->setWorking(false, false, false);
    ui->settingsBtn3->setWorking(false, false, false);
    ui->settingsBtn4->setWorking(true, false, false);

    ui->settingsClose->setIcon(":/icons/X.svg");
    ui->settingsClose_6->setIcon(":/icons/X.svg");
    ui->fieldE10->setIcon(":/icons/arrowUp_ena.svg",  ":/icons/arrowUp_dis.svg");
    ui->fieldE11->setIcon(":/icons/arrowDown_ena.svg",":/icons/arrowDown_dis.svg");
    ui->fieldE10->setAsButton(true);
    ui->fieldE11->setAsButton(true);
    ui->fieldE10->setWorking(false, false, false);
    ui->fieldE11->setWorking(false, false, false);
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
    ui->fieldF1->setAsButton(true);
    ui->fieldF2->setAsButton(true);
    ui->fieldF3->setAsButton(true);
    ui->fieldF4->setAsButton(true);
    ui->fieldF4->setIcon(":/icons/X.svg");
    ui->fieldF5->setAsButton(true);
    ui->fieldF5->setIcon(":/icons/tool.svg");
    ui->settingsBtn1->setAsButton(true);
    ui->settingsBtn2->setAsButton(true);
    ui->settingsBtn3->setAsButton(true);
    ui->settingsBtn4->setAsButton(true);
    ui->settingsBtn5->setAsButton(true);
    ui->settingsBtn6->setAsButton(true);
    ui->settingsBtn7->setAsButton(true);
    ui->settingsBtn8->setAsButton(true);
    ui->settingsClose->setAsButton(true);
    ui->settingsClose_6->setAsButton(true);
    ui->zusiIpBtn0->setAsButton(true);
    ui->zusiIpBtn1->setAsButton(true);
    ui->zusiIpBtn2->setAsButton(true);
    ui->zusiIpBtn3->setAsButton(true);
    ui->zusiIpBtn4->setAsButton(true);
    ui->zusiIpBtn5->setAsButton(true);
    ui->zusiIpBtn6->setAsButton(true);
    ui->zusiIpBtn7->setAsButton(true);
    ui->zusiIpBtn8->setAsButton(true);
    ui->zusiIpBtn9->setAsButton(true);
    ui->zusiIpBtn10->setAsButton(true);
    ui->zusiIpBtn11->setAsButton(true);
    ui->zusiIpOkBtn->setAsButton(true, true);
    ui->zusiIpClose->setAsButton(true);
    ui->zusiIpBtn0->setText("0",era::grey,era::darkGrey,QFont::Light);
    ui->zusiIpBtn1->setText("1",era::grey,era::darkGrey,QFont::Light);
    ui->zusiIpBtn2->setText("2",era::grey,era::darkGrey,QFont::Light);
    ui->zusiIpBtn3->setText("3",era::grey,era::darkGrey,QFont::Light);
    ui->zusiIpBtn4->setText("4",era::grey,era::darkGrey,QFont::Light);
    ui->zusiIpBtn5->setText("5",era::grey,era::darkGrey,QFont::Light);
    ui->zusiIpBtn6->setText("6",era::grey,era::darkGrey,QFont::Light);
    ui->zusiIpBtn7->setText("7",era::grey,era::darkGrey,QFont::Light);
    ui->zusiIpBtn8->setText("8",era::grey,era::darkGrey,QFont::Light);
    ui->zusiIpBtn9->setText("9",era::grey,era::darkGrey,QFont::Light);
    ui->zusiIpBtn10->setText("Del",era::grey,era::darkGrey,QFont::Light);
    ui->zusiIpBtn11->setText(".",era::grey,era::darkGrey,QFont::Light);
    ui->zusiIpClose->setIcon(":/icons/X.svg");
    ui->zusiIpBtn0->setText("0",era::grey,era::darkGrey,QFont::Light);
    ui->systemVersionClose->setAsButton(true);
    ui->systemVersionClose->setIcon(":/icons/X.svg");
    connectTimers();
    connectPzbIcons();
    connectMtdIcons();
    connectMtdPower();
    ui->fieldA3->setIsDistanceScale();
    ui->fieldA3->setTargetDistance(0,false);
    ui->widgetTacho->setVMaxDial(400);
    configureSettingsWindow();
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
    connect(myLzb,SIGNAL(newIcon1(QString,QString)),ui->fieldCL1,SLOT(setIcon(QString,QString)));
    connect(myLzb,SIGNAL(newIcon2(QString,QString)),ui->fieldCL2,SLOT(setIcon(QString,QString)));
    connect(myLzb,SIGNAL(newIcon3(QString,QString)),ui->fieldCL3,SLOT(setIcon(QString,QString)));
    connect(myLzb,SIGNAL(newIcon4(QString,QString)),ui->fieldCL4,SLOT(setIcon(QString,QString)));
    connect(myLzb,SIGNAL(newIcon5(QString,QString)),ui->fieldCL5,SLOT(setIcon(QString,QString)));
    connect(myLzb,SIGNAL(newIcon6(QString,QString)),ui->fieldCL6,SLOT(setIcon(QString,QString)));
    connect(myLzb,SIGNAL(newIcon7(QString,QString)),ui->fieldCL7,SLOT(setIcon(QString,QString)));
    connect(myLzb,SIGNAL(newIconC9(QString,QString)),ui->fieldC9,SLOT(setIcon(QString,QString)));
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
    connect(myLzb,SIGNAL(newOverspeed(qreal, bool, bool)),ui->widgetTacho,SLOT(setOverspeed(qreal, bool, bool)));
    connect(myLzb,SIGNAL(newTarDist(quint16, bool)),ui->fieldA3,SLOT(setTargetDistance(quint16, bool)));
    connect(myLzb,SIGNAL(gotLzbMessage()),this,SLOT(setPzbLzbNtc()));
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
    connect(myTcp->myPower,SIGNAL(maxPowerPositiveLine (qint16)),ui->widgetPower,SLOT(setAbsoluteAccelerateMaximum(qint16)));
    connect(myTcp->myPower,SIGNAL(maxPowerNegativeLine (qint16)),ui->widgetPower,SLOT(setAbsoluteBrakingMaximum(qint16)));
    connect(myTcp->myPower,SIGNAL(unitAcceleratingText(QString)),ui->widgetPower,SLOT(setUnitAcceleratingText(QString)));
    connect(myTcp->myPower,SIGNAL(unitBrakingText(QString)),ui->widgetPower,SLOT(setUnitBrakingText(QString)));
}
void MainWindow::connectTimers(){
    longTimer  = new QTimer(this);
    longTimer->setInterval(1000);
    longTimer->start();
    shortTimer  = new QTimer(this);
    shortTimer->setInterval(500);
    shortTimer->start();
    connect(longTimer, SIGNAL(timeout()),ui->fieldCL1,SLOT(updateBlinking1()));
    connect(shortTimer,SIGNAL(timeout()),ui->fieldCL1,SLOT(updateBlinking2()));
    connect(longTimer, SIGNAL(timeout()),ui->fieldCL2,SLOT(updateBlinking1()));
    connect(shortTimer,SIGNAL(timeout()),ui->fieldCL2,SLOT(updateBlinking2()));
    connect(longTimer, SIGNAL(timeout()),ui->fieldCL3,SLOT(updateBlinking1()));
    connect(shortTimer,SIGNAL(timeout()),ui->fieldCL3,SLOT(updateBlinking2()));
    connect(longTimer, SIGNAL(timeout()),ui->fieldCL4,SLOT(updateBlinking1()));
    connect(shortTimer,SIGNAL(timeout()),ui->fieldCL4,SLOT(updateBlinking2()));
    connect(longTimer, SIGNAL(timeout()),ui->fieldCL5,SLOT(updateBlinking1()));
    connect(shortTimer,SIGNAL(timeout()),ui->fieldCL5,SLOT(updateBlinking2()));
    connect(longTimer, SIGNAL(timeout()),ui->fieldCL6,SLOT(updateBlinking1()));
    connect(shortTimer,SIGNAL(timeout()),ui->fieldCL6,SLOT(updateBlinking2()));
    connect(longTimer, SIGNAL(timeout()),ui->fieldCL7,SLOT(updateBlinking1()));
    connect(shortTimer,SIGNAL(timeout()),ui->fieldCL7,SLOT(updateBlinking2()));
    connect(shortTimer,SIGNAL(timeout()),ui->fieldG2,SLOT(updateBlinking2()));
    connect(shortTimer,SIGNAL(timeout()),this,SLOT(blinkCursor()));
}
MainWindow::~MainWindow(){delete ui;}
void MainWindow::setPzbLzbNtc(){
    ui->fieldB7->setIcon(":/icons/MO_19.svg");
    ui->fieldC8->setIcon(":/icons/LE_02a.svg");
    ui->fieldE_holder2->setCurrentIndex(1);
    ui->fieldC_holder->setCurrentIndex(1);
    ui->fieldC_holder2->setCurrentIndex(0);
}
void MainWindow::arrowF4Clicked(){
    settings->setValue("mainwindow/height", this->height());
    settings->setValue("mainwindow/width", this->width());
    this->close();
}
void MainWindow::arrowF5Clicked(){ui->fieldDG->setCurrentIndex(1);}
void MainWindow::arrowUpClicked(){}
void MainWindow::arrowDownClicked(){}
void MainWindow::settingsBtn1Clicked(){}
void MainWindow::settingsBtn2Clicked(){}
void MainWindow::settingsBtn3Clicked(){}
void MainWindow::settingsBtn4Clicked(){ui->fieldDG->setCurrentIndex(5);}    // System Version
void MainWindow::settingsBtn5Clicked(){ui->fieldDG->setCurrentIndex(6);}    // QDmi
void MainWindow::settingsBtn6Clicked(){                                     // Network
    ui->fieldDG->setCurrentIndex(7);
    dataString = settings->value("zusiIp").toString();
    activeDataEntryItem = "IP-Address";
}
void MainWindow::settingsBtn7Clicked(){this->showFullScreen();}
void MainWindow::settingsBtn8Clicked(){keepScreenOn();}
void MainWindow::settingsCloseClicked(){
    if(ui->fieldDG->currentIndex() == 6){
        settings->setValue("pulldown_gauge",ui->pulldown_gauge->currentIndex());
        settings->setValue("pulldown_targetdistance",ui->pulldown_targetdistance->currentIndex());
        ui->widgetTacho->setEraUse(settings->value("pulldown_gauge").toInt() == 0);
        ui->fieldVZile100->setVisib(settings->value("pulldown_gauge").toInt() == 1);
        ui->fieldVZile10->setVisib(settings->value("pulldown_gauge").toInt() == 1);
        ui->fieldVZile1->setVisib(settings->value("pulldown_gauge").toInt() == 1);
        ui->fieldA3->setEraUse(settings->value("pulldown_targetdistance").toInt() == 0);
    }
    if(ui->fieldDG->currentIndex()>1){
        ui->fieldDG->setCurrentIndex(1);
    }
    else{
        ui->fieldDG->setCurrentIndex(0);
    }
}
void MainWindow::applyClicked(QString data){
    data.remove("_");
    if(activeDataEntryItem == "IP-Address"){
        ui->zusiIpOkBtn->setText("",era::grey,era::black,QFont::Light);
        if (myTcp->setIpadress(data) == 0)
            settings->setValue("zusiIp",data);
    }
    dataString = "";
}
void MainWindow::addItemToData(QString item){
    bool hasUnderline = false;
    if(dataString.contains("_") or item == "_")
        hasUnderline = true;
    if(dataString.contains("_") and item == "_")
        hasUnderline = false;
    dataString.remove("_");
    if(item == "Del"){
        dataString = dataString.left(dataString.length() - 1);
    }
    else{
        if(item != "_")
            dataString = dataString + item;
        if(hasUnderline)
            dataString = dataString + "_";
    }
    if(activeDataEntryItem == "IP-Address")
        ui->zusiIpOkBtn->setText(dataString,era::black,era::black,QFont::Light);
    //qDebug() << dataString;
}
void MainWindow::blinkCursor(){
    addItemToData("_");
}
void MainWindow::resizeEvent(QResizeEvent* event)
{
    qreal multi;
    int windowHeight = ui->centralWidget->height();
    int windowWidth = ui->centralWidget->width();
    if ((static_cast<qreal>(windowWidth) / static_cast<qreal>(windowHeight)) < (4.0/3.0)){
        multi = static_cast<qreal>(windowWidth) / 640.0;
    }
    else{
        multi = static_cast<qreal>(windowHeight) / 480.0;
    }
    ui->centralView->setGeometry(0,0,static_cast<int>(640*multi)
                                    ,static_cast<int>(480*multi));
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
    ui->fieldG12->setBorderThickness(static_cast<int>(multi + 0.75));
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
    ui->zusiIpBtn0->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->zusiIpBtn1->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->zusiIpBtn2->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->zusiIpBtn3->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->zusiIpBtn4->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->zusiIpBtn5->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->zusiIpBtn6->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->zusiIpBtn7->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->zusiIpBtn8->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->zusiIpBtn9->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->zusiIpBtn10->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->zusiIpBtn11->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->zusiIpClose->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->zusiIpOkBtn->setBorderThickness(static_cast<int>(multi + 0.75));
    ui->systemVersionClose->setBorderThickness(static_cast<int>(multi + 0.75));
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
}
void MainWindow::configureSettingsWindow(){
    QStringList tdList = { "1000m ERA", "4000m DB" };
    QStringList gaugeList = { "Hacken ERA", "Dreieck DB" };
    ui->pulldown_targetdistance->addItems(tdList);
    ui->pulldown_gauge->addItems(gaugeList);
    ui->pulldown_targetdistance->setCurrentIndex(settings->value("pulldown_targetdistance").toInt());
    ui->pulldown_gauge->setCurrentIndex(settings->value("pulldown_gauge").toInt());
    ui->widgetTacho->setEraUse(settings->value("pulldown_gauge").toInt() == 0);
    ui->fieldVZile100->setVisib(settings->value("pulldown_gauge").toInt() == 1);
    ui->fieldVZile10->setVisib(settings->value("pulldown_gauge").toInt() == 1);
    ui->fieldVZile1->setVisib(settings->value("pulldown_gauge").toInt() == 1);
    ui->fieldA3->setEraUse(settings->value("pulldown_targetdistance").toInt() == 0);
    myTcp->setIpadress(settings->value("zusiIp").toString());
}
void MainWindow::mousePressEvent(QMouseEvent *event){
    #ifdef Q_OS_WIN32
    lKilickPos = event->pos();
    #endif
}
void MainWindow::mouseMoveEvent(QMouseEvent *event){
    //#ifdef Q_OS_LINUX
    #ifdef Q_OS_WIN32
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
void MainWindow::keepScreenOn()
{
    #ifdef Q_OS_ANDROID
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
    #endif
}

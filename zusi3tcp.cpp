#include "zusi3tcp.h"
#include <QDebug>

zusi3Tcp::zusi3Tcp(){}

void zusi3Tcp::initialize(){
    client = new QTcpSocket(this);
    myIndicators = new zusiIndicator(this);
    myEtcs = new zusi3etcs(this);
    myPower = new zusiPower(this);
    mtdLmsToDecoder.resize(13);
    connect(client, &QTcpSocket::readyRead, this, &zusi3Tcp::clientReadReady);
    connect(client, &QTcpSocket::disconnected, this, &zusi3Tcp::connectToZusi);
    connect(client, &QTcpSocket::connected, this, &zusi3Tcp::subscribeZusiData);
    connect(client, &QTcpSocket::stateChanged, this, &zusi3Tcp::checkClientConnection);
}//connect(client,SIGNAL(errorOccurred(QAbstractSocket::SocketError)),this,SLOT(handleConnectionError(QAbstractSocket::SocketError)));

void zusi3Tcp::setNwSettingsOpen(bool open){
    if(!open){
        messageListMenueInfo.clear();
        emit newTextMessages(messageListMenueInfo);
        return;
    }
    date = QDateTime::currentDateTime();
    messageListMenueInfo.append({1, date, 0, "", "Auf eingegebene IP-Adresse tippen", "", 0});
    messageListMenueInfo.append({2, date, 0, "", "IP-Adressen dieses Gerätes:", "", 0});
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost){
            messageListMenueInfo.append({3, date, 0, "", address.toString(), "", 0});
        }
    }
    emit newTextMessages(messageListMenueInfo);
}
void zusi3Tcp::reconnect(){
    if (client->state() != QAbstractSocket::UnconnectedState) {
        client->waitForDisconnected(3000); // wartet max. 3 Sekunden
    }
    if (client->state() != QAbstractSocket::UnconnectedState) {
        client->abort();
    }
    reconnectOnes = true;
}
void zusi3Tcp::disconnectFromZusi(){
    qDebug() << "disconnectFromZusi ";
    reconnectOnes = false;
    client->disconnectFromHost();
    removeTechniccalMessages(); // To remove tcp state info that may persists
}
void zusi3Tcp::setAutoReconnect(quint8 reconnect){
   autoReconnect = reconnect > 0;
   reconnectOnes = false;
}
void zusi3Tcp::setIpadress(QString address){
    date = QDateTime::currentDateTime();
    messageList = messageListMenueInfo;
    if(address.isEmpty()) emit sendTcpConnectionFeedback("-1");
    QStringList slist = address.split(".");
    int s = slist.size();
    if(s>4){
        messageList.append({4, date, 0, "", "IP hat zu viele Elemente", "", 0});
        emit newTextMessages(messageList);
        emit sendTcpConnectionFeedback("-1");
        return;
    }
    bool emptyGroup = false;
    for(int i=0;i<s;i++){
        bool ok;
        if(slist[i].isEmpty()){
            emptyGroup = true;
            continue;
        }
        int val = slist[i].toInt(&ok);
        if(!ok || val<0 || val>255){
            messageList.append({4, date, 0, "", "Ungültige Werte in IP", "", 0});
            emit newTextMessages(messageList);
            emit sendTcpConnectionFeedback("-1");
            return;
        }
    }
    if(s<4 || emptyGroup){
        messageList.append({4, date, 0, "", "IP hat zu wenig Elemente", "", 0});
        emit newTextMessages(messageList);
        emit sendTcpConnectionFeedback("-1");
        return;
    }
    zusiPc = QHostAddress(address);
    ipAddress = address;
    if(client->state() != QAbstractSocket::UnconnectedState){
        reconnect();
    }
    else{
        connectToZusi();
    }
}
void zusi3Tcp::connectToZusi(){
    incommingData.clear();
    if (client->state() != QAbstractSocket::UnconnectedState)
        return;
    client->connectToHost(zusiPc,1436);
}
void zusi3Tcp::checkClientConnection(QAbstractSocket::SocketState state){
    date = QDateTime::currentDateTime();
    messageList = messageListMenueInfo;
    switch(state){
    case QAbstractSocket::UnconnectedState:
        messageList.append({3, date, 0, "", "Verbindungsaufbau gescheitert", "", 0});
        emit sendDataSourceIsZusi(false);
        if(autoReconnect || reconnectOnes)
            QTimer::singleShot(5000, this, SLOT(connectToZusi()));
        else
            disconnectFromZusi();
        break;
    case QAbstractSocket::HostLookupState:
    case QAbstractSocket::ConnectingState:
        messageList.append({3, date, 0, "", "Verbindungsaufbau zu Zusi3", "", 0});
        break;
    case QAbstractSocket::ConnectedState:
        messageList.append({3, date, 0, "", "Mit Zusi3 Verbunden", "", 0});
        emit sendDataSourceIsZusi(true);
        QTimer::singleShot(2500,this,SLOT(removeTechniccalMessages()));
        reconnectOnes = false;
        break;
    case QAbstractSocket::BoundState:break;
    case QAbstractSocket::ClosingState:break;
    case QAbstractSocket::ListeningState:break;
    }
    emit newTextMessages(messageList);
}
/*void zusi3Tcp::handleConnectionError(QAbstractSocket::SocketError socketError){
    qDebug() << "handleConnectionError";
    qDebug() << socketError;
}*/

void zusi3Tcp::subscribeZusiData(){
    QVector<unsigned char> anmeldung;
    QVector<unsigned char> abfrage;

    addKnotenAnfang(&anmeldung, 0x01);  //<Kn>  Verbindungsaufbau
    addKnotenAnfang(&anmeldung, 0x01);  //<Kn>  Befehl HELLO
    addAtribut(&anmeldung, 0x01, 0x02); // Protokoll-Version
    addAtribut(&anmeldung, 0x02, 0x02); // Client-Typ: [1: Zusi| 2: Fahrpult]
    addTextAtribut(&anmeldung, 0x03, "QDmi");
    addTextAtribut(&anmeldung, 0x04, "1.4.0");
    addKnotenEnde(&anmeldung, 2);
    QByteArray anmeldArr;
    for(int i = 0; i < anmeldung.length(); i++)
        anmeldArr.append(static_cast<char>(anmeldung[i]));
    client->write(anmeldArr);

    addKnotenAnfang(&abfrage, 0x02);// <Kn>    // Client-Anwendung 02
    addKnotenAnfang(&abfrage, 0x03);// <Kn>    // Befehl NEEDED_DATA
    addKnotenAnfang(&abfrage, 0x0A);// <Kn>    // Untergruppe Führerstandsanzeigen
    addAtribut(&abfrage, 0x01);  // Geschwindigkeit m/s
    if(useManometer)addAtribut(&abfrage, 0x02);  // Druck Hauptluftleitung
    if(useManometer)addAtribut(&abfrage, 0x03);  // Druck Bremszylinder
    if(useManometer)addAtribut(&abfrage, 0x04);  // Druck Hauptluftbehälter
  //addAtribut(&abfrage, 0x09);  // Zugkraft gesammt
    addAtribut(&abfrage, 0x0A);  // Zugkraft pro Achse
  //addAtribut(&abfrage, 0x0B);  // Zugkraft-soll gesammt
    addAtribut(&abfrage, 0x0C);  // Zugkraft-Soll pro Achse
  //addAtribut(&abfrage, 0x7C);  // Steuerwagen: Zugkraft gesammt
    addAtribut(&abfrage, 0x7D);  // Steuerwagen: Zugkraft pro Achse
  //addAtribut(&abfrage, 0x7E);  // Steuerwagen: Zugkraft-soll gesammt
    addAtribut(&abfrage, 0x7F);  // Steuerwagen: Zugkraft-Soll pro Achse
  //addAtribut(&abfrage, 0x90);  // Zug- und Brems-Gesamtkraftsoll normiert
  //addAtribut(&abfrage, 0x91);  // Steuerwagen: Zug- und Brems-Gesamtkraftsoll normiert
  //addAtribut(&abfrage, 0x93);  // Zug- und Brems-Gesamtkraftsoll absolut normiert
  //addAtribut(&abfrage, 0x94);  // Steuerwagen: Zug- und Brems-Gesamtkraftsoll absolut normiert
  //addAtribut(&abfrage, 0x0E);  // Fahrleitungsspannung
    addAtribut(&abfrage, 0x23);  // Zeit
    addAtribut(&abfrage, 0x13);  // Hauptschalter
    addAtribut(&abfrage, 0x15);  // Fahrstufe
    addAtribut(&abfrage, 0x85);  // Steuerwagen: Fahrstufe
    addAtribut(&abfrage, 0x17);  // AFB-Sollgeschwindigkeit
  //addAtribut(&abfrage, 0x20);  // LM Hochabbremsung Aus/Ein
  //addAtribut(&abfrage, 0x1b);  // LM Schleudern
  //addAtribut(&abfrage, 0x1c);  // LM Gleiten
    addAtribut(&abfrage, 0x36);  // AFB an
    addAtribut(&abfrage, 0x55);  // Stromabnehmer
    addAtribut(&abfrage, 0x61);  // Kilometrierung
    addAtribut(&abfrage, 0x64);  // SIFA
    addAtribut(&abfrage, 0x65);  // Zugsicherung
    addAtribut(&abfrage, 0x66);  // Türen
    addAtribut(&abfrage, 0x88);  // Steuerwagen: Stromabnehmer
    addAtribut(&abfrage, 0x8e);  // Status Zugverband
    addAtribut(&abfrage, 0xac);  // Führerstand deaktiviert
    addKnotenEnde(&abfrage, 1);
  //addKnotenAnfang(&abfrage, 0x0B); // DATA_OPERATION
  //addAtribut(&abfrage, 0x01);  // Betätigungsvorgang
  //addKnotenEnde(&abfrage);
    addKnotenAnfang(&abfrage, 0x0C); // DATA_PROG
    addAtribut(&abfrage, 0x02);  // Aktuelle Zugnummer
    addAtribut(&abfrage, 0x05);  // 1: Zug neu übernommen
    addKnotenEnde(&abfrage, 3);
    QByteArray abfrageArr;
    for(int i = 0; i < abfrage.length(); i++)
        abfrageArr.append(static_cast<char>(abfrage[i]));
    client->write(abfrageArr);
    emit sendTcpConnectionFeedback(ipAddress);
}
void zusi3Tcp::setEtcsBaseline(){
    sendEtcsSettingsByte(0x12, zusi3etcs::baseline_3_6_0); // Baseline des EVC
}
void zusi3Tcp::setDriverId(QString driverID){
    if(client->state() != QAbstractSocket::ConnectedState)return;
    QVector<unsigned char> traindata;
    addKnotenAnfang(&traindata, 0x02      );  // <Kn>  Client-Anwendung 02
    addKnotenAnfang(&traindata, 0x010A    );  // <Kn>  Befehl INPUT
    addKnotenAnfang(&traindata, 0x02      );  // <Kn>  Zugbeeinflussung einstellen
    addKnotenAnfang(&traindata, 0x02      );  // <Kn>  System aus der Indusi-Familie
    addAtribut(&traindata, 0x02, driverID );  // Tf-Nummer
    addKnotenEnde(&traindata, 4);             // 4 Knotenende
    addKnotenAnfang(&traindata, 0x02      );  // <Kn>  Client-Anwendung 02
    addKnotenAnfang(&traindata, 0x010A    );  // <Kn>  Befehl INPUT
    addKnotenAnfang(&traindata, 0x02      );  // <Kn>  Zugbeeinflussung einstellen
    addKnotenAnfang(&traindata, 0x04      );  // <Kn>  System aus der ETCS-Familie - Einstellungen und Interaktionen
    addKnotenAnfang(&traindata, 0x03      );  // <Kn>  Zugdaten
    addAtribut(&traindata, 0x07, driverID );  // Triebfahrzeugführernummer
    addKnotenEnde(&traindata, 5);             // 5 Knotenende
    QByteArray traindataArr;
    for(int i = 0; i < traindata.length(); i++)traindataArr.append(static_cast<char>(traindata[i]));
    client->write(traindataArr);
}
void zusi3Tcp::setTrainRunningNumber(QString trn){
    if(client->state() != QAbstractSocket::ConnectedState)return;
    QVector<unsigned char> traindata;
    addKnotenAnfang(&traindata, 0x02      );  // <Kn>  Client-Anwendung 02
    addKnotenAnfang(&traindata, 0x010A    );  // <Kn>  Befehl INPUT
    addKnotenAnfang(&traindata, 0x02      );  // <Kn>  Zugbeeinflussung einstellen
    addKnotenAnfang(&traindata, 0x02      );  // <Kn>  System aus der Indusi-Familie
    addAtribut(&traindata,      0x03, trn );  // Zugnummer
    addKnotenEnde(&traindata, 4);             // 4 Knotenende
    addKnotenAnfang(&traindata, 0x02      );  // <Kn>  Client-Anwendung 02
    addKnotenAnfang(&traindata, 0x010A    );  // <Kn>  Befehl INPUT
    addKnotenAnfang(&traindata, 0x02      );  // <Kn>  Zugbeeinflussung einstellen
    addKnotenAnfang(&traindata, 0x04      );  // <Kn>  System aus der ETCS-Familie - Einstellungen und Interaktionen
    addKnotenAnfang(&traindata, 0x03      );  // <Kn>  Zugdaten
    addAtribut(&traindata, 0x06, trn      );  // Zugnummer
    addKnotenEnde(&traindata, 5);             // 5 Knotenende
    QByteArray traindataArr;
    for(int i = 0; i < traindata.length(); i++)traindataArr.append(static_cast<char>(traindata[i]));
    client->write(traindataArr);
}
void zusi3Tcp::setTrainDataLzb(quint16 BRA, quint16 BRH, quint16 ZL, quint16 VMZ, bool validated){
    if(client->state() != QAbstractSocket::ConnectedState)return;
    QVector<unsigned char> traindata;
    addKnotenAnfang(&traindata, 0x02      );  // <Kn>  Client-Anwendung 02
    addKnotenAnfang(&traindata, 0x010A    );  // <Kn>  Befehl INPUT
    addKnotenAnfang(&traindata, 0x02      );  // <Kn>  Zugbeeinflussung einstellen
    addKnotenAnfang(&traindata, 0x02      );  // <Kn>  System aus der Indusi-Familie
    addKnotenAnfang(&traindata, 0x06      );  // <Kn>  Aktive Zugdaten
    addAtribut(&traindata, 0x01, BRH      );  // BRH-Wert (Bremshundertstel)
    addAtribut(&traindata, 0x02, BRA      );  // BRA-Wert (Bremsart)
    addAtribut(&traindata, 0x03, ZL       );  // ZL-Wert (Zuglänge) in m
    addAtribut(&traindata, 0x04, VMZ      );  // VMZ-Wert (Höchstgeschwindigkeit) in km/h
    addAtribut(&traindata, 0x05, 1        );  // Zugart. 1 = Zugart muss noch bestimmt werden
    addAtribut(&traindata, 0x06, VMZ      );  // Modus 6 = Normalbetrieb
    addKnotenEnde(&traindata, 5);             // 5 Knotenende
    QByteArray traindataArr;
    for(int i = 0; i < traindata.length(); i++)traindataArr.append(static_cast<char>(traindata[i]));
    client->write(traindataArr);

    //QVector<unsigned char> traindata2;
    //addKnotenAnfang(&traindata2, 0x02      );  // <Kn>  Client-Anwendung 02
    //addKnotenAnfang(&traindata2, 0x010A    );  // <Kn>  Befehl INPUT
    //addFloatAtribut(&traindata2, 0x19, BRH );  // BRH-Wert (Bremshundertstel)
    //for(quint8 i= 0; i<2 ; i++)addKnotenEnde(&traindata2); // 2 Knotenende
    //QByteArray traindataArr2;
    //for(int i = 0; i < traindata2.length(); i++)traindataArr2.append(static_cast<char>(traindata2[i]));
    //client->write(traindataArr2);

}
void zusi3Tcp::setTraindataEtcs(quint16 BRH, quint16 TCT, quint16 ZL, quint16 VMZ, quint16 AXL, QString AIT, QString LDG){
    if(client->state() != QAbstractSocket::ConnectedState)return;
    QVector<unsigned char> traindata;
    addKnotenAnfang(&traindata, 0x02      );  // <Kn>  Client-Anwendung 02
    addKnotenAnfang(&traindata, 0x010A    );  // <Kn>  Befehl INPUT
    addKnotenAnfang(&traindata, 0x02      );  // <Kn>  Zugbeeinflussung einstellen
    addKnotenAnfang(&traindata, 0x04      );  // <Kn>  System aus der ETCS-Familie - Einstellungen und Interaktionen
    addKnotenAnfang(&traindata, 0x03      );  // <Kn>  Zugdaten
    addAtribut     (&traindata, 0x01, BRH );  // BRH-Wert (Bremshundertstel)
    addAtribut     (&traindata, 0x02, TCT );  // Zugkategorie
    addAtribut     (&traindata, 0x03, ZL  );  // Zuglänge in m
    addAtribut     (&traindata, 0x04, VMZ );  // Höchstgeschwindigkeit in km/h
    addAtribut     (&traindata, 0x05, AXL );  // Achslast
    addAtribut32   (&traindata, 0x08, 0   );  // RBC-Nummer
    addTextAtribut (&traindata, 0x09, ""  );  // RBC-Telefonnummer
    addAtribut32   (&traindata, 0x0A, 0   );  // RBC-ID
    addAtribut32   (&traindata, 0x0B, 0   );  // RBC-Land
    addAtribut32   (&traindata, 0x0C, 0   );  // GSM-R-Netz (Radio Network ID)
    addKnotenEnde(&traindata, 5);             // 5 Knotenende
    QByteArray traindataArr;
    for(int i = 0; i < traindata.length(); i++)traindataArr.append(static_cast<char>(traindata[i]));
    client->write(traindataArr);
    myEtcs->setAirTight(AIT);       // WORKAROUND: Since we do not get that value from Zusi, we mirror this value directly from data entry to validation fields
    myEtcs->setLoadingGauge(LDG);   // WORKAROUND: Since we do not get that value from Zusi, we mirror this value directly from data entry to validation fields
}
void zusi3Tcp::sendEtcsAck(){           // ETCS-Quittierschalter
    // Tastaturzuordnung Zugbeeinflussung, Tastaturkommando ETCSQuittieren_Down, Schalterposition 1
    // Tastaturzuordnung Zugbeeinflussung, Tastaturkommando ETCSQuittieren_Up,   Schalterposition 0
    sendKeyboardCommand(0x0F, 0x7F, 0x01);
    QTimer::singleShot(500, this, [this]() {
    sendKeyboardCommand(0x0F, 0x80, 0x00);
    });
}
void zusi3Tcp::sendEtcsOverride(){      // Override Auslösung durch Client
    sendEtcsSettingsByte(0x09, 0x01);   // 1: Override angefordert
  //sendEtcsSettingsByte(0x09, 0x02);   // 2: Grundstellung
}
void zusi3Tcp::sendEtcsStart(){         // Start (nur Client -> Zusi)
    sendEtcsSettingsByte(0x0A, 0x01);   // 1: Startkommando
    sendEtcsSettingsByte(0x0A, 0x02);   // 2: Grundstellung
}
void zusi3Tcp::sendEtcsLevel(quint8 level){// Level einstellen/anfordern (nur Client -> Zusi)
    if(client->state() != QAbstractSocket::ConnectedState)return;
    QVector<unsigned char> traindata;
    addKnotenAnfang(&traindata, 0x02      );  // <Kn>  Client-Anwendung 02
    addKnotenAnfang(&traindata, 0x010A    );  // <Kn>  Befehl INPUT
    addKnotenAnfang(&traindata, 0x02      );  // <Kn>  Zugbeeinflussung einstellen
    addKnotenAnfang(&traindata, 0x04      );  // <Kn>  System aus der ETCS-Familie - Einstellungen und Interaktionen
    addAtribut(&traindata, 0x0C, 0        );  // Wenn STM ausgewählt: Index des ausgewählten STM-Systems
    addAtribut(&traindata, 0x0B, level    );  // Level einstellen/
    addKnotenEnde(&traindata, 4);             // 4 Knotenende
    QByteArray traindataArr;
    for(int i = 0; i < traindata.length(); i++)traindataArr.append(static_cast<char>(traindata[i]));
    client->write(traindataArr);
}
void zusi3Tcp::sendEtcsIndexStm(quint16 index){ // Wenn STM ausgewählt: Index desausgewählten STM-Systems
    sendEtcsSettingsWord(0x0C, index);
}
void zusi3Tcp::sendEtcsMode(quint16 mode){ //Modus einstellen/anfordern (nur Client -> Zusi)
    sendEtcsSettingsWord(0x0D, mode); // 00: undef, 01: FS, 02: OS, 03: SR, 04: SH, 05: UN, 06: SL, 07: SB, 08: TR, 09: PT, 10: SF, 11: IS, 12: NP, 13: NL, 14: SE, 15: SN, 16: RV, 17: LS, 18: PS
}
void zusi3Tcp::sendEtcsTafMode(quint16 tafMode){
    sendEtcsSettingsWord(0x0E, tafMode); // 1: TAF quittiert, 2: Grundstellung, 3: TAF abgelehnt
}
void zusi3Tcp::sendEtcsTestrun(quint8 testrun){ // Funktionspruefung starten (nur Client -> Zusi)
    sendEtcsSettingsByte(0x11, testrun); //1: Zusi soll testrun starten, 2: Testrun im Display als OK quittiert, 3: Testrun im Display als nicht i.O. quittiert
}
void zusi3Tcp::sendKeyboardCommand(quint16 mapping, quint8  command, qint16 position){
    if(client->state() != QAbstractSocket::ConnectedState)return;
    QVector<unsigned char> genericMessageVector;
    addKnotenAnfang(&genericMessageVector, 0x02     );      // <Kn>  Client-Anwendung 02
    addKnotenAnfang(&genericMessageVector, 0x010A   );      // <Kn>  Befehl INPUT
    addKnotenAnfang(&genericMessageVector, 0x01     );      // <Kn>  Tastatureingaben
    addAtribut(     &genericMessageVector, 0x01,  mapping); // Tastaturzuordnung
    addAtribut(     &genericMessageVector, 0x02,  command); // Tastaturkommando
    addAtribut(     &genericMessageVector, 0x03,     0x07); // Tastaturaktion (7=Angabe einer absoluten Raste)
    addAtribut(     &genericMessageVector, 0x04, position); // Schalterposition
    addKnotenEnde(&genericMessageVector, 3);                // 3 </Kn> Knotenende
    QByteArray genericMessageArr;
    for(int i = 0; i < genericMessageVector.length(); i++)genericMessageArr.append(static_cast<char>(genericMessageVector[i]));
    client->write(genericMessageArr);
}
void zusi3Tcp::sendEtcsSettingsByte(quint8 ID, quint8  value){
    if(client->state() != QAbstractSocket::ConnectedState)return;
    QVector<unsigned char> genericMessageVector;
    addKnotenAnfang(&genericMessageVector, 0x02     );  // <Kn>  Client-Anwendung 02
    addKnotenAnfang(&genericMessageVector, 0x010A   );  // <Kn>  Befehl INPUT
    addKnotenAnfang(&genericMessageVector, 0x02     );  // <Kn>  Zugbeeinflussung einstellen
    addKnotenAnfang(&genericMessageVector, 0x04     );  // <Kn>  System aus der ETCS-Familie - Einstellungen und Interaktionen
    addByteAtribut( &genericMessageVector, ID, value);
    addKnotenEnde(&genericMessageVector, 4);            // 4 </Kn> Knotenende
    QByteArray genericMessageArr;
    for(int i = 0; i < genericMessageVector.length(); i++)genericMessageArr.append(static_cast<char>(genericMessageVector[i]));
    client->write(genericMessageArr);
    qDebug() << "sendEtcsSettingsByte " << ID << " " << value;
}
void zusi3Tcp::sendEtcsSettingsWord(quint8 ID, quint16 value){
    QVector<unsigned char> genericMessageVector;
    addKnotenAnfang(&genericMessageVector, 0x02     );  // <Kn>  Client-Anwendung 02
    addKnotenAnfang(&genericMessageVector, 0x010A   );  // <Kn>  Befehl INPUT
    addKnotenAnfang(&genericMessageVector, 0x02     );  // <Kn>  Zugbeeinflussung einstellen
    addKnotenAnfang(&genericMessageVector, 0x04     );  // <Kn>  System aus der ETCS-Familie - Einstellungen und Interaktionen
    addAtribut(     &genericMessageVector, ID, value);
    addKnotenEnde(&genericMessageVector, 4);            // 4 </Kn> Knotenende
    QByteArray genericMessageArr;
    for(int i = 0; i < genericMessageVector.length(); i++)genericMessageArr.append(static_cast<char>(genericMessageVector[i]));
    client->write(genericMessageArr);
}
void zusi3Tcp::sendAckEtcsTextMessage(quint32 id){
    sendEtcsAck();
    QVector<unsigned char> genericMessageVector;
    addKnotenAnfang(&genericMessageVector, 0x02     );  // <Kn>  Client-Anwendung 02
    addKnotenAnfang(&genericMessageVector, 0x010A   );  // <Kn>  Befehl INPUT
    addKnotenAnfang(&genericMessageVector, 0x02     );  // <Kn>  Zugbeeinflussung einstellen
    addKnotenAnfang(&genericMessageVector, 0x04     );  // <Kn>  System aus der ETCS-Familie - Einstellungen und Interaktionen
    addKnotenAnfang(&genericMessageVector, 0x1A     );  // <Kn>  Textmeldung quittiert (nur Client → Zusi)
    addAtribut32(     &genericMessageVector, 0x01, id);
    addKnotenEnde(&genericMessageVector, 5);            // 5 </Kn> Knotenende
    QByteArray genericMessageArr;
    for(int i = 0; i < genericMessageVector.length(); i++)genericMessageArr.append(static_cast<char>(genericMessageVector[i]));
    client->write(genericMessageArr);
}
void zusi3Tcp::addKnotenAnfang(QVector<unsigned char> *vector, quint16 knoten){
    unsigned char knotenHb = static_cast<unsigned char>(knoten >> 8);
    unsigned char knotenLb = static_cast<unsigned char>(knoten & 0x00ff);
    vector->append({0x00, 0x00, 0x00, 0x00});
    vector->append(knotenLb);
    vector->append(knotenHb);
}
void zusi3Tcp::addKnotenEnde(QVector<unsigned char> *vector, quint8 numberNodes=1){
    for(quint8 i= 0; i<numberNodes ; i++)vector->append({0xFF, 0xFF, 0xFF, 0xFF});
}
void zusi3Tcp::addByteAtribut(QVector<unsigned char> *vector, unsigned char id, quint8 atribut){
    vector->append({0x03, 0x00, 0x00, 0x00});
    vector->append(id);
    vector->append(0x00);
    vector->append(atribut);
}
void zusi3Tcp::addAtribut(QVector<unsigned char> *vector, unsigned char id, quint16 atribut){
    unsigned char atributHb = static_cast<unsigned char>(atribut >> 8);
    unsigned char atributLb = static_cast<unsigned char>(atribut & 0x00ff);
    vector->append({0x04, 0x00, 0x00, 0x00});
    vector->append(id);
    vector->append(0x00);
    vector->append(atributLb);
    vector->append(atributHb);
}
void zusi3Tcp::addAtribut32(QVector<unsigned char> *vector, unsigned char id, quint32 atribut){
    unsigned char atributHb = static_cast<unsigned char>( atribut >> 24);
    unsigned char atributhb = static_cast<unsigned char>((atribut >> 16) & 0x000000ff);
    unsigned char atributlb = static_cast<unsigned char>((atribut >> 8)  & 0x000000ff);
    unsigned char atributLb = static_cast<unsigned char>( atribut        & 0x000000ff);
    vector->append({0x06, 0x00, 0x00, 0x00});
    vector->append(id);
    vector->append(0x00);
    vector->append(atributLb);
    vector->append(atributlb);
    vector->append(atributhb);
    vector->append(atributHb);
}
void zusi3Tcp::addFloatAtribut(QVector<unsigned char> *vector, unsigned char id, float atribut){
    quint32 raw;// = *reinterpret_cast<quint32*>(&atribut);
    static_assert(sizeof(atribut) == sizeof(raw), "Size mismatch");
    std::memcpy(&raw, &atribut, sizeof(raw));
    unsigned char atributHb = static_cast<unsigned char>( raw >> 24);
    unsigned char atributhb = static_cast<unsigned char>((raw >> 16) & 0x000000ff);
    unsigned char atributlb = static_cast<unsigned char>((raw >> 8)  & 0x000000ff);
    unsigned char atributLb = static_cast<unsigned char>( raw        & 0x000000ff);
    vector->append({0x06, 0x00, 0x00, 0x00});
    vector->append(id);
    vector->append(0x00);
    vector->append(atributLb);
    vector->append(atributlb);
    vector->append(atributhb);
    vector->append(atributHb);
}
void zusi3Tcp::addAtribut(QVector<unsigned char> *vector, unsigned char id, QString atribut){
    quint32 packet_length = static_cast<quint32>(atribut.length()) + 2;
    vector->append(packet_length         & 0x000000ff);
    vector->append((packet_length >>  8) & 0x000000ff);
    vector->append((packet_length >> 16) & 0x000000ff);
    vector->append((packet_length >> 24) & 0x000000ff);
    vector->append(id);
    vector->append(0x00);
    for(int i = 0; i < atribut.length(); i++){
        vector->append(atribut.at(i).cell());
    }
}
void zusi3Tcp::addTextAtribut(QVector<unsigned char> *vector, quint16 id, QString text){
    quint32 len = static_cast<quint32>(text.length()) + 2;
    vector->append(static_cast< unsigned char>( len & 0x000000ff));
    vector->append(static_cast< unsigned char>((len & 0x0000ff00) >> 8));
    vector->append(static_cast< unsigned char>((len & 0x00ff0000) >> 16));
    vector->append(static_cast< unsigned char>((len & 0xff000000) >> 24));
    vector->append(static_cast< unsigned char>(  id & 0x00ff));
    vector->append(static_cast< unsigned char>(( id & 0xff00) >> 8));
    for(int i = 0; i < text.length(); i++){
        vector->append(static_cast<unsigned char>(text.at(i).toLatin1()));
    }
}
void zusi3Tcp::addAtribut(QVector<unsigned char> *vector, unsigned char atribut){
    vector->append({0x04, 0x00, 0x00, 0x00, 0x01, 0x00});
    vector->append(atribut);
    vector->append(0x00);
}
void zusi3Tcp::clientReadReady(){
    // This function is used to isolate a zusi telegram.
    // If we can find an entire one, we cut it for decoding, and leave the rest, to be appended to new data.
    bool nodesChanged = false;
    int nodeCount = 0;
    int packetLength = 0;
    incommingData.append(client->readAll());        // Add arrived data to the QByteArray to be parsed
    for(int i=0; i <= incommingData.size()-3;i++){
      packetLength = readIntegerInRawAtPos(i);      // Get length of accordin zusi docu 11.3.1.1
      switch (packetLength){
      case 0:                                       // 0x0000 means node begin
          nodesChanged = true;
          nodeCount ++;                             // On after node begin, we can jump forward:
          i = i + 5;                                // 3 for nodes + 2 for following ID
          break;
      case -1:                                      // 0xffff (= -1) means node end
          nodesChanged = true;
          nodeCount --;
          i = i + 3;
          break;
      default:
          i = i + packetLength + 3;
      }
      if((nodesChanged == true) &&nodeCount == 0){
            compZusiPacket = incommingData.left(i+1);
            incommingData.remove(0,i+1);
            //qDebug() <<  "com =" + compZusiPacket.toHex();
            //qDebug() <<  "rst =" + incommingData.toHex();
            cutZusiTelegram();
            i = -1;
            nodesChanged = false;
      }
    }
}
int32_t zusi3Tcp::readIntegerInRawAtPos(int pos){
    DataLng.chr[0] = incommingData[pos    ];
    DataLng.chr[1] = incommingData[pos + 1];
    DataLng.chr[2] = incommingData[pos + 2];
    DataLng.chr[3] = incommingData[pos + 3];
    return DataLng.integer;
}
int32_t zusi3Tcp::readIntegerAtPos(int pos){
    DataLng.chr[0] = compZusiPacket[pos    ];
    DataLng.chr[1] = compZusiPacket[pos + 1];
    DataLng.chr[2] = compZusiPacket[pos + 2];
    DataLng.chr[3] = compZusiPacket[pos + 3];
    return DataLng.integer;
}
uint16_t zusi3Tcp::readIdAtPos(int pos){
    dataGroup.chr[0] =  compZusiPacket[pos    ];
    dataGroup.chr[1] =  compZusiPacket[pos + 1];
    return dataGroup.word;
}
void zusi3Tcp::cutZusiTelegram(){
    int packetLength = 0;
    nodeIds[0] = nodeIds[1] = nodeIds[2] = nodeIds[3] = nodeIds[4] = nodeIds[5] = atributeId = 0;
    for(int i=0; i <= compZusiPacket.size()-3;i++){
        packetLength = readIntegerAtPos(i);
        switch (packetLength){
        case 0: //Länge = 0 kennzeichnet den Knoten im Unterschied zum Attribut
            lastWasNewNode = true;
            layer ++;
            //qDebug() << "NodeID: " + QString::number(readIdAtPos(i+4));
            nodeIds[layer-1] = readIdAtPos(i+4); //ID zur Codierung der Funktion des Knotens (Word)
            i = i + 5;  // 3 für Knoten + 2 für folgende ID
            break;
        case -1: //Kennzeichnung des Knoten-Endes
            if(lastWasNewNode && nodeIds[0] == 0x0002 && nodeIds[1] == 0x000A){
                //zusiDecoderFahrpult();
            }
            else{
                zusiDecoderSecondaryInfos();
            }
            lastWasNewNode=false;
            layer --;
            i = i + 3;
            break;
        default:
            atributeId = readIdAtPos(i+4); //ID zur Codierung der Funktion des Attributs (Word)
            nodeIds[layer] = readIdAtPos(i+4);
            switch ((packetLength - 2)){
            case 8:
                useData8Byte.chr[0] = compZusiPacket[i + 6];
                useData8Byte.chr[1] = compZusiPacket[i + 7];
                useData8Byte.chr[2] = compZusiPacket[i + 8];
                useData8Byte.chr[3] = compZusiPacket[i + 9];
                useData8Byte.chr[4] = compZusiPacket[i +10];
                useData8Byte.chr[5] = compZusiPacket[i +11];
                useData8Byte.chr[6] = compZusiPacket[i +12];
                useData8Byte.chr[7] = compZusiPacket[i +13];
                break;
            case 4:
                useData4Byte.chr[0] = compZusiPacket[i + 6];
                useData4Byte.chr[1] = compZusiPacket[i + 7];
                useData4Byte.chr[2] = compZusiPacket[i + 8];
                useData4Byte.chr[3] = compZusiPacket[i + 9];
                break;
            case 2:
                useData2Byte.chr[0] = compZusiPacket[i + 6];
                useData2Byte.chr[1] = compZusiPacket[i + 7];
                break;
            case 1:
                useData2Byte.chr[0] = compZusiPacket[i + 6];
                useData2Byte.chr[1] = 0;
                break;
            default: break;
            }
            useDataComplex = compZusiPacket.mid(i+6,packetLength-2);
            if (nodeIds[0] == 0x0002){ // "Client-Anwendung 02" and "Befehl DATA_FTD - Führerstandsanzeigen"
                zusiDecoderFahrpult();
            }
            else{
                zusiDecoderSecondaryInfos();
            }
            i = i + packetLength + 3;
        }
    }
    if(dataContainsEtcs)myEtcs->doStuffAfterFinishOfZusiTelegram();dataContainsEtcs = false;
    transmitMtdIndicators();
    composeLzbInfos();
}
void zusi3Tcp::zusiDecoderFahrpult(){
    switch(nodeIds[1]){
    case 0x000A:
        switch(nodeIds[2]){
        case 0x0001:      // Geschwindigkeit Meter/Sekunde
            if (checkHysterise(&VIst, mPerSecToKmh(useData4Byte.Single))){
                myEtcs->setSpeed(useData4Byte.Single);
                emit newSpeed(VIst);
                myPower->setVIst(VIst);
                if(VIst > 0) trainHasBenMovedSinceLastNewTrainNumber = true;
            }
            return;
        case 0x0002: if (checkHysterise(&drHll, useData4Byte.Single * 10)){emit newHll(drHll);}return;  // Druck Hauptluftleitungreturn;
        case 0x0003: if (checkHysterise(&drBrz, useData4Byte.Single * 10)){emit newBrz(drBrz);}return;  // Druck Bremszylinder
        case 0x0004: if (checkHysterise(&drHlb, useData4Byte.Single * 10)){emit newHlb(drHlb);}return;  // Druck Hauptluftbehälter
        case 0x0061:                                                                                    // Kilometrierung
            if (checkHysterise(&kilometer, static_cast<float>(floor(static_cast<double>(useData4Byte.Single) * 10.0))*100)){emit newKilometrierung(kilometer);}
            return;
        case 0x0065:    // 11.3.3.3.4 Status Zugbeeinﬂussung
            switch (nodeIds[3]){
            case 0x0001:
                lzSys = myIndicators->checkPlzbType(QString::fromLatin1(useDataComplex));return;// Grundblock 11.3.3.3.4.1 Bauart Zugbeeinflussungssystem als Text // Obsolet
            case 0x0002:        // 11.3.3.3.4.2 System aus der Indusi-Familie - Einstellungen
                plzbDataWasReceived = true;
                switch(nodeIds[4]) {
                case 0x0001: pzZa=zusiZugartToSep(useData2Byte.byte[0]);return;                             // Zugart
              //case 0x0002: qDebug() << "Tf-Nummer: " + QString::fromLatin1(useDataComplex);return;        // Tf-Nummer
                case 0x0003: setZugnummer(QString::fromLatin1(useDataComplex), "PZB");return;               // Zugnummer
              /*case 0x0004:   // Werte der Grunddaten
                    switch(nodeIds[5]){
                    case 0x0001: qDebug() << "BRH Grund.: " + QString::number(useData2Byte.Word);return;    // BRH-Wert (Bremshundertstel).
                    case 0x0002: qDebug() << "BRA Grund.: " + QString::number(useData2Byte.Word);return;    // BRA-Wert (Bremsart).
                    case 0x0003: qDebug() << "ZL Grund:   " + QString::number(useData2Byte.Word);return;    // ZL-Wert (Zuglänge) in m
                    case 0x0004: qDebug() << "VMZ Grund.: " + QString::number(useData2Byte.Word);return;    // VMZ-Wert (Höchstgeschwindigkeit)in km/h
                    case 0x0005: qDebug() << "ZA Grund.:  " + QString::number(useData2Byte.byte[0]);return; // Zugehörige Zugart. Werte der Ersatzzugdaten
                    }
                    return;*/
              /*case 0x0005:   // Werte der Ersatzzugdaten
                    switch(nodeIds[5]) {
                    case 0x0001: emit newErsatzBrh(useData2Byte.Word);return;                               // BRH-Wert (Bremshundertstel)
                    case 0x0002: emit newErsatzBra(useData2Byte.Word); return;                              // BRA-Wert (Bremsart).
                    case 0x0003: emit newErsatzZl(useData2Byte.Word);return;                                // ZL-Wert (Zuglänge) in m
                    case 0x0004: emit newErsatzVmz(useData2Byte.Word);return;                               // VMZ-Wert (Höchstgeschwindigkeit)in km/h
                    case 0x0005: return;                                                                    // Zugehörige Zugart.
                    }
                    return;*/
                case 0x0006:   // Aktive Zugdaten
                    switch(nodeIds[5]) {
                    case 0x0001: emit newLzbBrh(useData2Byte.Word);return;                                  // BRH-Wert (Bremshundertstel). Werte der Ersatzzugdaten
                    case 0x0002: emit newLzbBra(useData2Byte.Word);return;                                  // BRA-Wert (Bremsart). Werte der Ersatzzugdaten
                    case 0x0003: emit newLzbZl(useData2Byte.Word);return;                                   // ZL-Wert (Zuglänge) in m
                    case 0x0004: emit newLzbVmz(useData2Byte.Word);return;                                  // VMZ-Wert (Höchstgeschwindigkeit)in km/h
                    case 0x0005: return;                                                                    // Zugehörige Zugart. Werte der Ersatzzugdaten
                    case 0x0006:typAktZugDa = useData2Byte.byte[0];return;                                  // Modus (Grunddaten / Ersatzzugdaten / Normalbetrieb)
                    }
                    return;
              //case 0x0007: return;                                                                        // Hauptschalter qDebug() << "Indusi-HS: " + QString::number(useData2Byte.byte[0]);
                case 0x0008: pzbStoersch = useData2Byte.byte[0];return;                                     // Indusi Störschalter qDebug() << "Indusi-SS: " + QString::number(useData2Byte.byte[0]);
                case 0x0009: lzbStoersch = useData2Byte.byte[0];return;                                     // LZB Störschalter
                case 0x000A: plzbLAH = useData2Byte.byte[0];return;                                         // Indusi-LuftabsperrhahnqDebug() << "Indusi-LH: " + QString::number(useData2Byte.byte[0]);
                case 0x000B: zusiKtp = useData2Byte.byte[0];return;                                         // Klartextmeldungen
                case 0x0011: sysStateLzb = useData2Byte.byte[0];return;                                     // Systemstatus LZB:// 3 = Aktiv
                case 0x000D: lmPzb = useData2Byte.byte[0]==3;                                               // Systemstatus Indusi:// 3 = Aktiv
                             sysStatePzb = useData2Byte.byte[0];
                }
                return;
            case 0x0003:        // 11.3.3.3.4.2 System aus der Indusi-Familie - Betriebsdaten
                plzbDataWasReceived = true;
                switch(nodeIds[4]) {
                case 0x0002: zstZugb = useData2Byte.byte[0]; return;                                        // Zustand Zugbeeinflussung
                case 0x0003: brGr = useData2Byte.byte[0]; return;                                           // Zwangsbremsung aktiv wegen
              //case 0x0004: return;                                                                        // Grund der Zwangsbremsung als Text
                case 0x000C: melderbild = useData2Byte.byte[0]; pzb90=true; return;                         // Melderbild
                case 0x000D: zustLzb = useData2Byte.byte[0]; return;                                        // LZB-Zustand
                case 0x000E: if (nodeIds[5] == 0x0001) endeVerf = useData2Byte.byte[0]; return;             // Ende-Verfahren
                case 0x000F: ersAuf = 1; return;                                                            // Ersatzauftrag aktiv
                case 0x0010: if (nodeIds[5] == 0x0001) falAuf = useData2Byte.byte[0]; return;               // Falschfahrauftrag aktiv
                case 0x0011: if (nodeIds[5] == 0x0001) vorAuf = useData2Byte.byte[0]; return;               // Vorsichtauftrag aktiv
                case 0x0012: fUeLHpB=1; return;                                                             // Fahrt über LZB-Halt per Befehl
                case 0x0013:
                    if (nodeIds[5] == 0x0001) ueAusfVz = mPerSecToIntKmh(useData4Byte.Single);              // Übertragungsausfall
                    if (nodeIds[5] == 0x0002) ueAusf = useData2Byte.byte[0]; return;                        // Übertragungsausfall
                case 0x0014: if (nodeIds[5] == 0x0001) noHa   = useData2Byte.byte[0]; return;               // LZB-Nothalt
                case 0x0016: if (nodeIds[5] == 0x0001) elAuf  = useData2Byte.byte[0]; return;               // LZB-EL-Auftrag
                case 0x0021: lzVS = mPerSecToIntKmh(useData4Byte.Single);return;                            // Sollgeschwindigkeit in m/s // Obsolet
                case 0x0022: lzVZ = mPerSecToIntKmh(useData4Byte.Single);                                   // Zielgeschwindigkeit in m/s (Wert<0 → dunkel)
                             lzbShow = useData4Byte.Single >=0;return;
                case 0x0023: lzZE = static_cast<quint16>(qMax(0, static_cast<int>(qCeil(useData4Byte.Single - 0.5f))));return;// Zielweg in m (Wert<0 → dunkel)
                case 0x0024: lmG  = zusiEnumToSepEnum(useData2Byte.byte[0]); return;                        // Status Melder G
                case 0x002f: lmTH = zusiEnumToSepEnum(useData2Byte.byte[0]); return;                        // Status Melder „1000 Hz“
                case 0x0030: lm85 = zusiEnumToSepEnum(useData2Byte.byte[0]);  qDebug() << "lm85 " << lm85; return;                        // Status Melder „Zugart O“
                case 0x0031: lm70 = zusiEnumToSepEnum(useData2Byte.byte[0]); return;                        // Status Melder „Zugart M“
                case 0x0032: lm55 = zusiEnumToSepEnum(useData2Byte.byte[0]); return;                        // Status Melder „Zugart U“
                case 0x0033: lmFH = zusiEnumToSepEnum(useData2Byte.byte[0]); return;                        // Status Melder „500 Hz“
                case 0x0034: lmBF = zusiEnumToSepEnum(useData2Byte.byte[0]); return;                        // Status Melder „Befehl“
                case 0x0038: lmH  = zusiEnumToSepEnum(useData2Byte.byte[0]); return;                        // Status Melder H
                case 0x0039: lmE4 = zusiEnumToSepEnum(useData2Byte.byte[0]); return;                        // Status Melder E40
                case 0x003A: lmEn = zusiEnumToSepEnum(useData2Byte.byte[0]); return;                        // Status Melder Ende
                case 0x003B: lmB  = zusiEnumToSepEnum(useData2Byte.byte[0]); return;                        // Status Melder B
                case 0x003C: lmUe = zusiEnumToSepEnum(useData2Byte.byte[0]); return;                        // Status Melder Ü
                case 0x003D: lmEl = zusiEnumToSepEnum(useData2Byte.byte[0]); return;                        // Status Melder EL
                case 0x003E: lmV4 = zusiEnumToSepEnum(useData2Byte.byte[0]); return;                        // Status Melder V40
                case 0x003F: lmS  = zusiEnumToSepEnum(useData2Byte.byte[0]); qDebug() << "S " << lmS; return;                        // Status Melder S
            }
                return;
            case 0x0004:        // 11.3.3.3.4.9 System aus der ETCS-Familie - Einstellungen und Interaktionen
                dataContainsEtcs = true;
                switch(nodeIds[4]) {
                case 0x0001: myEtcs->setTdeState(useData2Byte.byte[0]);return;// Zustand
                case 0x0002:// verfügbares STM-System (nur Zusi → Client)
                    static quint16 index;
                    switch (nodeIds[5]){
                    case 0x0001: index = useData2Byte.Word;return;                                          // Index des STM-Systems, von 1 beginnend gemäß Reihenfolge in der ftd-Datei
                    case 0x0002: myEtcs->addNtcToList(index, QString::fromLatin1(useDataComplex));return;   // Name des STM-Systems als Text
                    }
                    return;
                case 0x0003:                                                // Zugdaten
                    switch (nodeIds[5]){
                    case 0x0001: myEtcs->setBrakingPercentage(useData2Byte.Word);               // Bremshundertstel in %
                                 emit newEvcBrh(useData2Byte.Word);return;
                    case 0x0002: myEtcs->setTrainCategory(useData2Byte.Word);return;            // Zugkategorie
                    case 0x0003: myEtcs->setTrainLength(useData2Byte.Word);
                                 emit newEvcZl(useData2Byte.Word);return;
                    case 0x0004: myEtcs->setMaxSpeed(useData2Byte.Word);
                                 emit newEvcVmz(useData2Byte.Word);return;
                    case 0x0005: myEtcs->setAxleLoad(useData2Byte.Word);return;
                    case 0x0006: myEtcs->setTrainNumber(QString::fromLatin1(useDataComplex));return;
                    case 0x0007: myEtcs->setDriverId(QString::fromLatin1(useDataComplex));return;
                    case 0x0008: myEtcs->setRbcNumber(useData4Byte.Cardinal);return;
                    case 0x0009: myEtcs->setRbcTelNumber(QString::fromLatin1(useDataComplex));return;
                    case 0x000A: myEtcs->setRbcId(useData4Byte.Cardinal);return;
                    case 0x000B: myEtcs->setRbcCountry(useData4Byte.Cardinal);return;
                    case 0x000C: myEtcs->setRadioNetworkId(useData4Byte.Cardinal);return;
                    case 0x000D: myEtcs->setRadioNetworkIdIsFix(useData2Byte.byte[0]);return;
                    }
                case 0x0004:                                                                    // Spec
                    switch (nodeIds[5]){
                    case 0x0001: myEtcs->setAdhesionValue(useData2Byte.byte[0]);return;         // Reibwert vermindert
                    }
                case 0x0005: myEtcs->setEtcsPassiveSwitch(useData2Byte.byte[0]);return;         // ETCS-Passivschalter
                case 0x0006: myEtcs->setEtcsFaultySwitch(useData2Byte.byte[0]);return;          // ETCS-Störschalter (CEA)
                case 0x0007: myEtcs->setAirShutOff(useData2Byte.byte[0]);return;                // Luftabsperrhahn
                case 0x0008: myEtcs->setEtcsAcknowledger(useData2Byte.byte[0]);return;          // ETCS-Quittierschalter
                case 0x0009: myEtcs->setOverrideRequested(useData2Byte.byte[0]);return;         // Override Anforderung, Auslösung durch Client
                case 0x000F: myEtcs->setTrainRestart(useData2Byte.byte[0]);return;              // Zugneustart
                case 0x0012: //myEtcs->setMaxBaseline(useData2Byte.byte[0]);return;               // Maximal verfügbare Baseline des EVC
                {
                    QString BL = QString::fromLatin1(useDataComplex);
                    if(BL != "3.6.0")setEtcsBaseline();
                    return;
                 }
                case 0x0013: myEtcs->setVehicleHasEtcsCB(useData2Byte.byte[0]);return;          // Fahrzeug hat einen ETCS-LSS
                case 0x0014: myEtcs->setVehicleHasEtcsPS(useData2Byte.byte[0]);return;          // Fahrzeug hat einen Passivschalter
                case 0x0015: myEtcs->setVehicleHasEtcsRSW(useData2Byte.byte[0]);return;         // Fahrzeug hat einen ETCS-Reset-Schalter
                case 0x0016: myEtcs->setVehicleHasEtcsRSK(useData2Byte.byte[0]);return;         // Fahrzeug hat einen ETCS-Reset-Softkey
                case 0x0017: myEtcs->setEtcsCBState(useData2Byte.byte[0]);return;               // ETCS-LSS
                case 0x0019: myEtcs->setEvcTyte(QString::fromLatin1(useDataComplex));return;    // Bauart Zugbeeinflussungssystem als Text
                }
            case 0x0005:                                                                        // 11.3.3.3.4.9 System aus der ETCS-Familie – Betriebsdaten
                dataContainsEtcs = true;
                switch(nodeIds[4]) {
                case 0x0001: myEtcs->setActiveLevel(useData2Byte.Word);return;                  // Aktiver Level 0: ETCS Level undefiniert 1: ETCS Level STM 2: ETCS Level 0 3: ETCS Level 1 4: ETCS Level 2 5: ETCS Level 3
                case 0x0002: myEtcs->setActiveMode(useData2Byte.Word);return;                   // Aktiver ETCS-Modus 00: undefiniert 01: FS 02: OS 03: SR 04: SH 05: UN 06: SL 07: SB 08: TR 09: PT 10: SF 11: IS 12: NP 13: NL 14: SE 15: SN 16: RV 17: LS 18: PS
                case 0x0003: myEtcs->setReasonOfEmrBreakEnum(useData2Byte.Word);return;         // Grund der Zwangs- oder Betriebszwangsbremsung 0: Keine Zwangsbremsung 6: v-Max-Überwachung 7: Funktionsprüfung 10: Rechnerausfall 11: ETCS-Nothalt überfahren 15: ETCS-Halt überfahren 16: ETCS: Stillstands-/Rücklaufüberwachung ausgelöst 17: ETCS: nicht quittiert 18: ETCS: Funkausfall 19: ETCS: Balisenstörung 20: ETCS: manueller Levelwechsel 27: Allgemeine Störung 28: Stromversorgung fehlt
                case 0x0004:                                                                    // Grund der Zwangs- oder Betriebszwangsbremsung als Text
                    myEtcs->setReasonOfEmrBreakText(QString::fromLatin1(useDataComplex));return;
                case 0x0005:                                                                    // STM-Info
                    switch(nodeIds[5]) {
                    case 0x0001: myEtcs->setIndexOfActiveStm(useData2Byte.Word);return;         // Index des aktiven STM-System, von 1 beginnend gemäß Reihenfolge in der ftd-Datei
                    }
                case 0x0006:                                                                    // Angekündigter oder neu aktivierter Level
                    static quint16 level;
                    switch(nodeIds[5]) {
                    case 0x0001:level = useData2Byte.Word;return;                               // Neuer Level gemäß Paket 00 01
                    case 0x0002:myEtcs->setNextLevel(level, useData2Byte.byte[0]);return;       // Quittierung
                    }
                case 0x0007:                                                                    // Angekündigter Modus
                    static quint16 mode;
                    switch(nodeIds[5]) {
                    case 0x0001: mode = useData2Byte.Word;return;                               // Neuer Modus gemäß Paket 00 02
                    case 0x0002: myEtcs->setNextMode(mode, useData2Byte.byte[0]);return;        //  Quittierung Modus 1: noch keine Quittierung nötig 2: Quittierung nötig, aber noch nicht alle Voraussetzungen gegeben 3: Quittierung nötig 4: quittiert 5: wirksam
                    }
                case 0x0008:                                                                    // Funkstatus
                    switch(nodeIds[5]) {
                    case 0x0001:myEtcs->setRadioState(useData2Byte.byte[0]);return;             // Zustand
                    }
                case 0x0009: myEtcs->setTargetSpeed(useData4Byte.Single);return;                // Zielgeschwindigkeit in m/s (Wert<0 → dunkel)
                case 0x000A: myEtcs->setTargetDistance(useData4Byte.Single);return;             // Zielweg in m (Wert<0 → dunkel)
                case 0x000B: myEtcs->setBrakeApplicationPointDistance(useData4Byte.Single);return;// Abstand Bremseinsatzpunkt in m
                case 0x000C: myEtcs->setReleaseSpeed(useData4Byte.Single);return;               // Entlassungsgeschwindigkeit in m/s
                case 0x000D: myEtcs->setPermittedSpeed(useData4Byte.Single);return;             // Sollgeschwindigkeit in m/s
                case 0x000E: myEtcs->setAlertSpeed(useData4Byte.Single);return;                 // Warngeschwindigkeit in m/s
                case 0x000F: myEtcs->setServBreakSpeed(useData4Byte.Single);return;             // Bremsgeschwindigkeit in m/s
                case 0x0010: myEtcs->setEmergencyBreakSpeed(useData4Byte.Single);return;        // Zwangsbremsgeschwindigkeit in m/s
                case 0x0011: myEtcs->setTargetSpeedMonitoring(useData2Byte.byte[0]);return;     // Bremskurve läuft ab 0: nein 1: ja
                case 0x0012:                                                                    // Vorschaupunkt
                    static uint16_t  origin;
                    static float     speed, distance, altitude;
                    switch(nodeIds[5]) {
                        case 0x0001: origin = useData2Byte.Word;return;                         // Herkunft
                        case 0x0002: speed    = mPerSecToKmh(useData4Byte.Single);return;       // Geschwindigkeit in m/s (-1 bedeutet ETCS-Ende)
                        case 0x0003: distance = useData4Byte.Single;return;                     // Abstand in m
                        case 0x0004: altitude = useData4Byte.Single;return;                     // Höhenwert in m
                        case 0x0005:                                                            // Parameter
                            myEtcs->addPlanningInfo(origin, speed, distance, altitude, useData2Byte.Word);return;
                    }
                case 0x0013: myEtcs->setTrackAheadFreeRequestState(useData2Byte.byte[0]);return;// TAF-Status: 0: kein TAF aktiv 1: TAF wird nötig 2: TAF angefordert 3: TAF quittiert 4: TAF abgelehnt
                case 0x0014: myEtcs->setOverrideActive(useData2Byte.byte[0]);return;            // Override aktiv (nur Zusi → Client) 1: Override aktiv
                case 0x0015: myEtcs->setEmergencyStop(useData2Byte.byte[0]);return;             // Nothalt-Status 0: kein Nothalt 1: bedingter Nothalt aktiv 2: unbedingter Nothalt aktiv
                case 0x0016: myEtcs->setReasonServBrake(useData2Byte.Word);return;              // Betriebszwangsbremsung Wert siehe 00 03
                case 0x0017:                                                                    // ETCS-EL-Auftrag
                    switch(nodeIds[5]) {
                        case 0x0001: myEtcs->setElOrder(useData2Byte.byte[0]);return;
                    }
                case 0x0018:                                                                    // ETCS-Funktionsprüfung läuft
                    switch(nodeIds[5]) {
                    case 0x0001: myEtcs->setEtcsTestRunnState(useData2Byte.byte[0]);return;     // Zustand
                    }
                case 0x0019:                                                                    // Bootvorgang ETCS-Gerät läuft
                    switch(nodeIds[5]) {
                        case 0x0001: myEtcs->setEvcBootupState(useData2Byte.byte[0]);return;    // Zustand
                    }

                case 0x001A:
                    static uint32_t msgID;
                    static double timeStamp;
                    static uint16_t zusiKennung;
                    static QString msgTypeEtcsSpec, msgText, fillingText;
                    static uint8_t ackNeeded;
                    switch(nodeIds[5]) {                                                        // ETCS-Textmeldung, es folgt entweder Attribut 1 oder 2
                    case 0x0001: msgID = useData4Byte.Cardinal; return;
                    case 0x0002: timeStamp = useData8Byte.DoubleFloat;return;
                    case 0x0003: zusiKennung = useData2Byte.Word;return;
                    case 0x0004: msgTypeEtcsSpec = QString::fromLatin1(useDataComplex);return;
                    case 0x0005: msgText = QString::fromLatin1(useDataComplex);return;
                    case 0x0006: fillingText = QString::fromLatin1(useDataComplex);return;
                    case 0x0007:
                        ackNeeded = useData2Byte.byte[0];
                        myEtcs->setTextMessage(msgID, convertZusiTime(timeStamp), zusiKennung, msgTypeEtcsSpec, msgText, ackNeeded);
                        return;
                    }
                }
            case 0x0007:        // 11.3.3.3.4.10 ZUB
                //plzbDataWasReceived = true;
                switch(nodeIds[4]) {
                case 0x0001: lmGn   = zusiEnumToSepEnum(useData2Byte.byte[0]);return;       // Status Melder GNT
                case 0x0007: lmGnUe = zusiEnumToSepEnum(useData2Byte.byte[0]);return;       // Status Melder GNT Ü
                case 0x0008: lmGnG  = zusiEnumToSepEnum(useData2Byte.byte[0]);return;       // Status Melder GNT G
                case 0x0009: lmGnS  = zusiEnumToSepEnum(useData2Byte.byte[0]);return;       // Status Melder GNT S
                }
                return;
            }
            return;
            case 0x064:                                                                                     // 11.3.3.3.3 Status Sifa
                switch (nodeIds[3]){
              //case 0x0001: return;                                                                        // Bauart Sifasystem als Text
                case 0x0002: setMtdIndicator(useData2Byte.byte[0],1);return;                                // Status Sifa-Leuchtmelder
                case 0x0003: setMtdIndicator(useData2Byte.byte[0],2);return;                                // Status Sifa-Hupe
              //case 0x0004: return;                                                                        // Sifa-Hauptschalter
                case 0x0005: setMtdIndicator(useData2Byte.byte[0] % 2 ,0); return;                          // Sifa-Störschalter // 2 = Sifa on, 1 = Sifa off
                }
            return;
        case 0x066:     // 11.3.3.3.5 Status Tueren
            switch(nodeIds[3]) {
            case 0x0001:      // Bezeichnung des Türsystems als Text
                if (QString::fromLatin1(useDataComplex).contains("SAT")){setMtdIndicator(2,9);return;}                  // Türsystem SAT
                if (QString::fromLatin1(useDataComplex).contains("TB0")){setMtdIndicator(3,9);return;}                  // Türsystem TB0
                if (QString::fromLatin1(useDataComplex).contains("TAV")){setMtdIndicator(4,9);return;}                  // Türsystem TAV
                if (QString::fromLatin1(useDataComplex).contains("SST")){setMtdIndicator(5,9);return;}                  // Türsystem SST
                setMtdIndicator(1,9);                                                                       // Türsystem undefiniert
                return;
            case 0x0002: setMtdIndicator(useData2Byte.byte[0],10);return;                                   // Status linke Seite
            case 0x0003: setMtdIndicator(useData2Byte.byte[0],11);return;                                   // Status rechte Seite
          //case 0x0004:return;   // Traktionssperre aktiv
          //case 0x0005:return;   // Freigabestatus (Seitenwahlschalter)
          //case 0x0006:return;   // Melder links an
          //case 0x0007:return;   // Melder rechts an
          //case 0x0008:return;   // Status Melder links
          //case 0x0009:return;   // Status Melder rechts
          //case 0x000A:return;   // Melder „(Zwangs)schließen“ an
          //case 0x000B:return;   // Status Melder „(Zwangs)schließen“
          //case 0x000C:return;   // Melder „Türen links+rechts“ an
          //case 0x000D:return;   // Status Melder „Türen links+rechts“
          //case 0x000E:return;   // „Zentrales Öffnen links“ an
          //case 0x000F:return;   // „Zentrales Öffnen rechts an
          //case 0x0010:return;   // Status Melder „Zentrales Öffnen links“
          //case 0x0011:return;   // Status Melder „Zentrales Öffnen rechts“
          //case 0x0012:return;   // Melder „Grünschleife“ an
            }
        return;
      //case 0x0002: return;   // Druck Hauptluftleitung
      //case 0x0003: return;   // Druck Bremszylinder
      //case 0x0004: return;   // Druck Hauptluftbehälter
      //case 0x0009: myPower->setZugkraft(useData4Byte.Single);  return;                     //              Zugkraft gesammt
        case 0x000A: myPower->setZugkraftProAchse(useData4Byte.Single);                     //              Zugkraft pro Achse
            zugkraftProAchs = useData4Byte.Single; return; //guesTractionType(); return;
      //case 0x000B: myPower->setZugkraftSollGesammt(useData4Byte.Single); return;           //              Zugkraft-Soll gesammt
        case 0x000C: myPower->setZugkraftSollProAchse(useData4Byte.Single); return;          //              Zugkraft-Soll pro Achse
      //case 0x007C: myPower->setZugkraftGesammtSteuerwagen(useData4Byte.Single); return;    // Steuerwagen: Zugkraft gesammt
        case 0x007D: myPower->setZugkraftProAchseSteuerwagen(useData4Byte.Single);           // Steuerwagen: Zugkraft pro Achse
            zugkraftProAchsSteuerwagen = useData4Byte.Single; return;//guesTractionType(); return;
      //case 0x007E: myPower->setZugkraftSollGesammtSteuerwagen(useData4Byte.Single); return;// Steuerwagen: Zugkraft-soll gesammt
        case 0x007F: myPower->setZugkraftSollProAchseSteuerwagen(useData4Byte.Single); return;// Steuerwagen: Zugkraft-Soll gesamt pro Achse
      //case 0x0090: myPower->setZugkraftSollNormiert(useData4Byte.Single); return;          //              Zug- und Brems-Gesamtkraft soll normiert
      //case 0x0091: myPower->setZugkraftSollNormiertSteuerwagen(useData4Byte.Single); return;// Steuerwagen: Zug- und Brems-Gesamtkraft soll normiert
      //case 0x0093: myPower->setZugkraftNormiert(useData4Byte.Single); return;              //              Zug- und Brems-Gesamtkraft absolut normiert
      //case 0x0094: myPower->setZugkraftNormiertSteuerwagen(useData4Byte.Single); return;// Steuerwagen: Zug- und Brems-Gesamtkraft absolut normiert
      //case 0x000E: fahrlSpng = useData4Byte.Single; guesTractionType(); return;
        case 0x0013:   // Hauptschalter
            hauptschalter = useData4Byte.Single > 0;
            myIndicators->setLmHauptschalter(hauptschalter);
            setMtdIndicator(!hauptschalter ,7);
            if(useData4Byte.Single < 1 && istReisezug){    // ZS kommt nich von Zusi. Einfach verzögert von HS an und kein Güterz.
                QTimer::singleShot(5000, this, SLOT(setSammelschine()));
            }
            else{
                setMtdIndicator(1,8);
            }
            return;
        case 0x0015: // Fahrstufe
        case 0x0085: // Steuerwagen: Fahrstufe
            myPower->setFahrstufe(useData4Byte.Single);
            return;
          case 0x0017:   // AFB-Sollgeschwindigkeit
            if(checkHysterise(&VSoll, mPerSecToKmh(useData4Byte.Single)))
                myIndicators->setAfbSoll(VSoll);
            return;
      //case 0x0020: return;  // Hohe Abbrems
        case 0x001b: myIndicators->setLmSchleudern(useData4Byte.Single > 0);return;// LM Schleudern
        case 0x001c: myIndicators->setLmGleiten(useData4Byte.Single > 0);return;// LM Gleiten
        case 0x0023: emit newSimTime(   QTime::fromMSecsSinceStartOfDay(static_cast<int>(useData4Byte.Single * 86400000)).toString());
                     myEtcs->setSimTime(QTime::fromMSecsSinceStartOfDay(static_cast<int>(useData4Byte.Single * 86400000)).toString("hh:mm"));
                     return;
        case 0x0036: myIndicators->setAfbAn(useData4Byte.Single > 0);return;
        case 0x0055:   //             Stromabnehmer
            stromabnehmerLok = static_cast<quint8>(useData4Byte.Single);
            if(stromabnehmerSteuerwagen > stromabnehmerLok){
              setMtdIndicator(stromabnehmerSteuerwagen ,6);
              myIndicators->setStatusStromabnehmer(stromabnehmerSteuerwagen);
            }
            else{
              setMtdIndicator(stromabnehmerLok ,6);
              myIndicators->setStatusStromabnehmer(stromabnehmerLok);
            }
            return;
        case 0x0088:   // Steuerwagen Stromabnehmer
            stromabnehmerSteuerwagen = static_cast<quint8>(useData4Byte.Single);
            if(stromabnehmerSteuerwagen > stromabnehmerLok){
                setMtdIndicator(stromabnehmerSteuerwagen ,6);
                myIndicators->setStatusStromabnehmer(stromabnehmerSteuerwagen);
            }
            else{
                setMtdIndicator(stromabnehmerLok ,6);
                myIndicators->setStatusStromabnehmer(stromabnehmerLok);
            }
            return;
        case 0x008E:  // 11.3.3.3.7 Status Zugverband
            switch (nodeIds[3]){
            case 0x0001:    // Fahrzeug
                switch (nodeIds[4]){
                case 0x0005:    // Fahrzeug
                    if(checkHysterise(&VMFzg, mPerSecToKmh(useData4Byte.Single)) && istVMaxErstesFahrzeug){
                        myIndicators->setFzgVMax(VMFzg);
                        istVMaxErstesFahrzeug = false;
                        QTimer::singleShot(2000, this, SLOT(resetVehicleBlocking()));
                    }
                    return;
                case 0x0006: myPower->setBaureihe(QString::fromLatin1(useDataComplex));return;
                case 0x0025:
                    switch (nodeIds[5]){
                    case 0x0001:tractionType = useData2Byte.byte[0];return;
                    case 0x0002:
                        currentType = useData2Byte.byte[0];
                        setTractionType();
                        return;
                    }
                }
                return;
            case 0x0002:
                istReisezug = useData2Byte.byte[0];
                if(!istReisezug){
                    setMtdIndicator(0,9);
                }
                setMtdIndicator(!hauptschalter ,7);
                if(istReisezug)QTimer::singleShot(5000, this, SLOT(setSammelschine()));
                return;
            }
            return;
        case 0x00AC:  // Führerstand deaktiviert
            if(fstAktiv != (useData4Byte.Single < 1)){
                fstAktiv = useData4Byte.Single < 1;
                emit newCabActive(fstAktiv, VIst == 0);
            }
            return;
        default:
            return;
        }

        case 0x000B:
        switch(nodeIds[2]){
        case 0x0001:
            switch(nodeIds[3]){
            case 0x0002: emit newTastaturkommando(useData2Byte.Word);return;   // Tastaturzuordnung
            }
        }
        return;
    case 0x000C:
        switch(nodeIds[2]){
        case 0x0002: setZugnummer(QString::fromLatin1(useDataComplex), "Fahrplan");return; // Aktuelle Zugnummer
      //case 0x0003: return; // Status Ladepause, 0: Ende Ladepause (Start der Simulation)
        case 0x0005: // 1: Zug neu übernommen
            if(useData2Byte.byte[0] > 0){
                emit changedTrain();
                reconnect();
                myEtcs->reset();
            }
            return;
        }
    default:
        return;
    }
}

void zusi3Tcp::zusiDecoderSecondaryInfos(){
    // Verbindung
    if ((nodeIds[0] == 0x0001) && (nodeIds[1] == 0x0002)){
        switch(nodeIds[2]){
        case 0x0001:{
            QStringList zusiVersion = QString::fromLatin1(useDataComplex).split(".");
            QString minReqZusiVersion = " Mindestens Zusi " + QString::number(zusiMajor) + "." + QString::number(zusiMinor) + "." + QString::number(zusiPatch) + " erforderlich!";
            if(zusiVersion[0].toInt() != zusiMajor){
                myEtcs->setImmediatTextMessage(minReqZusiVersion);
                return;
            }
            if(zusiVersion[1].toInt() <  zusiMinor){
                myEtcs->setImmediatTextMessage(minReqZusiVersion);
                return;
            }
            if(zusiVersion[1].toInt() == zusiMinor){
                if(zusiVersion[2].toInt() <  zusiPatch){
                    myEtcs->setImmediatTextMessage(minReqZusiVersion);
                    return;
                }
            }
            return;
        }
        case 0x0002: return; //qDebug() << "Zusi-Verbindungsinfo: " + QString::fromLatin1(useDataComplex);
        case 0x0005: return; //qDebug() << "Version des TCP-Protokolls: " + QString::fromLatin1(useDataComplex);
        }
    }
    if ((nodeIds[0] == 0x0001) && (nodeIds[1] == 0x0002) && (nodeIds[2] == 0x0003)){
      //qDebug() << "Der Client wurde akzeptiert " + QString::number(useData2Byte.chr[0]);
    }
}
float zusi3Tcp::mPerSecToKmh(float input){
    if(input <= 0) return 0;
    return std::roundf(input * 3.6f);
}
quint16 zusi3Tcp::mPerSecToIntKmh(float input){
    if(input <= 0) return 0;
    return static_cast<quint16>(std::roundf(input * 3.6f));
}
bool zusi3Tcp::checkHysterise(int32_t *output, float input){
    qint32 tmp = static_cast<qint32>(input);
    if(tmp != *output){
        *output = tmp;
        return true;
    }
    return false;
}
bool zusi3Tcp::checkHysterise(quint16 *output, float input){
    quint16 tmp = static_cast<quint16>(qFabs(input));
    if(tmp != *output){
        *output = tmp;
        return true;
    }
    return false;
}
bool zusi3Tcp::checkHysterise(float *output, float input, bool isRelative){
    if(isRelative && input > 10){
        //input = input / 1000;
    }
    if(input != *output){
        *output = input;
        return true;
    }
    return false;
}
void zusi3Tcp::setMtdIndicator(uint8_t value, uint8_t pos){
    if(mtdLmsToDecoder[pos] != value){
        mtdLmsToDecoder[pos] = value;
    }
}
void zusi3Tcp::transmitMtdIndicators(){
    if (mtdLmsToDecoderOld != mtdLmsToDecoder){
        emit newMtdIndicators(mtdLmsToDecoder);
        mtdLmsToDecoderOld = mtdLmsToDecoder;
    }
}
void zusi3Tcp::setZugnummer(QString nummer, QString fromSystem){
    if(nummer == "")return;
    QString zugnummerAnzeige = nummer;
    emit newZugnummer(zugnummerAnzeige.replace('_', '\n'));
    zugnummer = nummer;
    (void)fromSystem;
}
void zusi3Tcp::setSammelschine(){
    setMtdIndicator(0,8);
}
void zusi3Tcp::resetVehicleBlocking(){
    istVMaxErstesFahrzeug = true;
}
void zusi3Tcp::setTractionType(){
  //qDebug() << "Antriebstyp: " + QString::number(tractionType);
  //qDebug() << "Stromtyp:    " + QString::number(currentType);
    if((tractionType == 6 || tractionType == 7) && (currentType == 2 || currentType == 3 || currentType == 4 || currentType == 6)){
        setMtdIndicator(1, 12);
    }
    else{
        setMtdIndicator(2, 12);
    }
}
void zusi3Tcp::setUseManometer(bool use){
    if(useManometer != use){
        useManometer = use;
        reconnect();
    }
}
void zusi3Tcp::setTextUsing(quint8 useAutomText){
    reconnect();
    (void)useAutomText;
}
void zusi3Tcp::removeTechniccalMessages(){
    messageList.clear();
    messageListMenueInfo.clear();
    emit newTextMessages(messageList);
}
QDateTime zusi3Tcp::convertZusiTime(double daysSince2000) {
    QDateTime epoch(QDate(2000, 1, 1), QTime(0, 0, 0), Qt::UTC);
    qint64 seconds = static_cast<qint64>(daysSince2000 * 86400); // 1 Tag = 86400 Sekunden
    return epoch.addSecs(seconds);
}
quint8 zusi3Tcp::zusiEnumToSepEnum(quint8 zusiEnum){
    switch(zusiEnum){
        case 0: return  0;   // Aus
        case 1: return  1;   // An
        case 2: return  5;   // Blinkend
        case 3: return 13;   // Gegenläufig blinkend
        case 4: return  0;   // Aus erzwingend
        default: return  0;  // Aus erzwingend
    }
}
quint8 zusi3Tcp::zusiZugartToSep(quint8 zusiEnum){
    if(zusiEnum==5)return 0;
    return zusiEnum-1;
}
void zusi3Tcp::composeLzbInfos(){
    if(!plzbDataWasReceived)return;
    lm95     = lm85  * !pzb90;
    lm75     = lm70  * !pzb90;
    lm60     = lm55  * !pzb90;
    lmIndusi = lm55  * !pzb90;
    lm85     = lm85  *  pzb90;
    lm70     = lm70  *  pzb90;
    lm55     = lm55  *  pzb90;
    lmPzb    = lmPzb *  pzb90;
    if(lmUe == 4 || lmUe == 0)endeVerf = 0; // Endeverfahren zu ende, wenn Ü aus geht.
    QVector<quint8> sepIndicatorTelegram =
    {lmB, lm85,lm70, lm55,lmPzb, lmS, lmH,lmG, lmE4,lmV4, lmBF,lmTH, lmFH,lmEl, lmUe,           // LMs, wie sie auch im SEP kommen
     lmEn,lmGn, lmGnUe,lmGnG, lmGnS,lm95, lm75,lm60, lmIndusi, lzSys, pzZa,
     true,                                                                                      // Flag, das anzeigt, dass die Daten von Zusi kommen.
      typAktZugDa, pzbStoersch, lzbStoersch, plzbLAH, sysStateLzb, sysStatePzb, zstZugb, zustLzb,// Zusätliche States
      elAuf, endeVerf, ersAuf, fUeLHpB, falAuf, vorAuf, brGr, ueAusf, ueAusfVz, zusiKtp, melderbild
    };
    emit newLzbIndicators(sepIndicatorTelegram);
    QVector<quint8> sepAnalogLzbValues = {
        lzbShow,
        static_cast<quint8>(lzVZ & 0xFF),static_cast<quint8>(lzVZ >> 8),
        static_cast<quint8>(lzVS & 0xFF),static_cast<quint8>(lzVS >> 8),
        static_cast<quint8>(lzZE & 0xFF),static_cast<quint8>(lzZE >> 8)
    };
    emit newLzbValues(sepAnalogLzbValues);
    plzbDataWasReceived = false;
    ersAuf=0; falAuf=0; vorAuf=0; elAuf=0; ueAusf=0; fUeLHpB=0; pzb90=0;
}

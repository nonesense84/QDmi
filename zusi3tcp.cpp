#include "zusi3tcp.h"
#include <QDebug>

zusi3Tcp::zusi3Tcp(){}

void zusi3Tcp::process(){
    client = new QTcpSocket();
    myIndicators = new zusiIndicator();
    myEtcs = new zusi3etcs();
    myPower = new zusiPower();
    mtdLmsToDecoder.resize(13);
    connect(client,SIGNAL(readyRead()),this,SLOT(clientReadReady()));
    connect(client,SIGNAL(disconnected()),this,SLOT(connectToZusi()));
    connect(client,SIGNAL(connected()),this,SLOT(subscribeZusiData()));
    connect(client,SIGNAL(stateChanged(QAbstractSocket::SocketState )),this,SLOT(checkClientConnection(QAbstractSocket::SocketState)));
  //connect(client,SIGNAL(errorOccurred(QAbstractSocket::SocketError)),this,SLOT(handleConnectionError(QAbstractSocket::SocketError)));
}
void zusi3Tcp::reconnect(){
    reconnectOnes = true;
    disconnectFromZusi();
}
void zusi3Tcp::disconnectFromZusi(){
   client->disconnectFromHost();
}
void zusi3Tcp::setAutoReconnect(quint8 reconnect){
   autoReconnect = reconnect > 0;
   reconnectOnes = false;
}
void zusi3Tcp::setIpadress(QString address){
    QDateTime date = QDateTime::currentDateTime();
    QString formattedTime = date.toString("hh:mm");
    if(address.isEmpty()) emit sendTcpConnectionFeedback("-1");
    QStringList slist = address.split(".");
    int s = slist.size();
    if(s>4){
        emit newTechnicalMessage({formattedTime}, {"IP hat zu viele Elemente"});
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
            emit newTechnicalMessage({formattedTime}, {"Ungültige Werte in IP"});
            emit sendTcpConnectionFeedback("-1");
            return;
        }
    }
    if(s<4 || emptyGroup){
        emit newTechnicalMessage({formattedTime}, {"IP hat zu wenig Elemente"});
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
    client->connectToHost(zusiPc,1436);
}
void zusi3Tcp::subscribeZusiData(){
    QVector<unsigned char> anmeldung;
    QVector<unsigned char> abfrage;

    addKnotenAnfang(&anmeldung, 0x01);  //<Kn>  Verbindungsaufbau
    addKnotenAnfang(&anmeldung, 0x01);  //<Kn>  Befehl HELLO
    addAtribut(&anmeldung, 0x01, 0x02); // Protokoll-Version
    addAtribut(&anmeldung, 0x02, 0x02); // Client-Typ: [1: Zusi| 2: Fahrpult]
    addTextAtribut(&anmeldung, 0x03, "QDmi");
    addTextAtribut(&anmeldung, 0x04, "1.3.0");
    addKnotenEnde(&anmeldung);
    addKnotenEnde(&anmeldung);
    QByteArray anmeldArr;
    for(int i = 0; i < anmeldung.length(); i++){
        anmeldArr.append(static_cast<char>(anmeldung[i]));
    }
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
    addKnotenEnde(&abfrage);
  //addKnotenAnfang(&abfrage, 0x0B); // DATA_OPERATION
  //addAtribut(&abfrage, 0x01);  // Betätigungsvorgang
  //addKnotenEnde(&abfrage);
    addKnotenAnfang(&abfrage, 0x0C); // DATA_PROG
    addAtribut(&abfrage, 0x02);  // Aktuelle Zugnummer
    addAtribut(&abfrage, 0x05);  // 1: Zug neu übernommen
    addKnotenEnde(&abfrage);
    addKnotenEnde(&abfrage);
    addKnotenEnde(&abfrage);
    QByteArray abfrageArr;
    for(int i = 0; i < abfrage.length(); i++){
        abfrageArr.append(static_cast<char>(abfrage[i]));
    }
    client->write(abfrageArr);
    emit sendTcpConnectionFeedback(ipAddress);
}
void zusi3Tcp::setDriverId(QString driverID){
    if(client->state() != QAbstractSocket::ConnectedState)return;
    QVector<unsigned char> traindata;
    addKnotenAnfang(&traindata, 0x02      );  // <Kn>  Client-Anwendung 02
    addKnotenAnfang(&traindata, 0x010A    );  // <Kn>  Befehl INPUT
    addKnotenAnfang(&traindata, 0x02      );  // <Kn>  Zugbeeinflussung einstellen
    addKnotenAnfang(&traindata, 0x02      );  // <Kn>  System aus der Indusi-Familie
    addAtribut(&traindata, 0x02, driverID );  // Tf-Nummer
    for(quint8 i= 0; i<5 ; i++)addKnotenEnde(&traindata); // 4 Knotenende
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
    for(quint8 i= 0; i<5 ; i++)addKnotenEnde(&traindata); // 4 Knotenende
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
    addAtribut(&traindata, 0x05, validated);  // Zugehörige Zugart (0:Automatisch | 1: Noch_unbestimmt | 2:U | 3:M | 4:O | 5:S-Bahn)
    addAtribut(&traindata, 0x06, 0        );  // Modus (0:undefiniert | 5:Ersatzzugdaten | 6:Normalbetrieb)
    for(quint8 i= 0; i<5 ; i++)addKnotenEnde(&traindata); // 5 Knotenende
    QByteArray traindataArr;
    for(int i = 0; i < traindata.length(); i++)traindataArr.append(static_cast<char>(traindata[i]));
    client->write(traindataArr);
}
void zusi3Tcp::newTraindataEtcs(quint16 BRH, QString TCT, quint16 ZL, quint16 VMZ, QString AXL, QString AIT, QString LDG, bool validated){
    if(client->state() != QAbstractSocket::ConnectedState)return;
    QVector<unsigned char> traindata;
    addKnotenAnfang(&traindata, 0x02      );  // <Kn>  Client-Anwendung 02
    addKnotenAnfang(&traindata, 0x010A    );  // <Kn>  Befehl INPUT
    addKnotenAnfang(&traindata, 0x02      );  // <Kn>  Zugbeeinflussung einstellen
    addKnotenAnfang(&traindata, 0x04      );  // <Kn>  System aus der ETCS-Familie - Einstellungen und Interaktionen
    addKnotenAnfang(&traindata, 0x03      );  // <Kn>  Zugdaten
    addAtribut(&traindata, 0x01, BRH      );  // BRH-Wert (Bremshundertstel)
    if(TCT == "PASS1") addAtribut(&traindata, 0x02,  1);// Zugkategorie
    if(TCT == "PASS2") addAtribut(&traindata, 0x02,  2);// FIXME: I'ts unknown, which categorys are supported and how the are associated
    if(TCT == "PASS3") addAtribut(&traindata, 0x02,  3);
    if(TCT == "FP1"  ) addAtribut(&traindata, 0x02,  4);
    if(TCT == "FP2"  ) addAtribut(&traindata, 0x02,  5);
    if(TCT == "FP3"  ) addAtribut(&traindata, 0x02,  6);
    if(TCT == "FP4"  ) addAtribut(&traindata, 0x02,  7);
    if(TCT == "FG1"  ) addAtribut(&traindata, 0x02,  8);
    if(TCT == "FG2"  ) addAtribut(&traindata, 0x02,  9);
    if(TCT == "FG3"  ) addAtribut(&traindata, 0x02, 10);
    if(TCT == "FG4"  ) addAtribut(&traindata, 0x02, 11);
    if(TCT == "TILT1") addAtribut(&traindata, 0x02, 12);
    addAtribut(&traindata, 0x03, ZL       );  // Zuglänge in m
    addAtribut(&traindata, 0x04, VMZ      );  // Höchstgeschwindigkeit in km/h
    if(TCT == "A"   ) addAtribut(&traindata, 0x02,  1);// Achslast in kg
    if(TCT == "B1"  ) addAtribut(&traindata, 0x02,  2);// FIXME: I'ts unknown, which categorys are supported and how the are associated
    if(TCT == "B2"  ) addAtribut(&traindata, 0x02,  3);
    if(TCT == "C2"  ) addAtribut(&traindata, 0x02,  4);
    if(TCT == "C3"  ) addAtribut(&traindata, 0x02,  5);
    if(TCT == "C4"  ) addAtribut(&traindata, 0x02,  6);
    if(TCT == "D2"  ) addAtribut(&traindata, 0x02,  7);
    if(TCT == "D3"  ) addAtribut(&traindata, 0x02,  8);
    if(TCT == "D4"  ) addAtribut(&traindata, 0x02,  9);
    if(TCT == "D4XL") addAtribut(&traindata, 0x02, 10);
    if(TCT == "E4"  ) addAtribut(&traindata, 0x02, 11);
    if(TCT == "E5"  ) addAtribut(&traindata, 0x02, 12);
    for(quint8 i= 0; i<5 ; i++)addKnotenEnde(&traindata); // 5 Knotenende
    QByteArray traindataArr;
    for(int i = 0; i < traindata.length(); i++)traindataArr.append(static_cast<char>(traindata[i]));
    client->write(traindataArr);
    myEtcs->setAirTight(AIT);       // WORKAROUND: Since we do not get that value from Zusi, we mirror this value directly from data entry to validation fields
    myEtcs->setLoadingGauge(LDG);   // WORKAROUND: Since we do not get that value from Zusi, we mirror this value directly from data entry to validation fields
}
void zusi3Tcp::sendEtcsAck(){           // ETCS-Quittierschalter
    sendKeyboardCommand(0x0F, 0x7F, 0x01);  // Tastaturzuordnung Zugbeeinflussung, Tastaturkommando ETCSQuittieren_Down, Tastaturaktion Down
    sendKeyboardCommand(0x0F, 0x08, 0x02);  // Tastaturzuordnung Zugbeeinflussung, Tastaturkommando ETCSQuittieren_Up, Tastaturaktion Up
}

void zusi3Tcp::sendEtcsOverride(){      // Override Auslösung durch Client
    sendEtcsSettingsByte(0x09, 0x01);   // 1: Override angefordert
  //sendEtcsSettingsByte(0x09, 0x02);   // 2: Grundstellung
}

void zusi3Tcp::sendEtcsStart(){         // Start (nur Client -> Zusi)
    sendEtcsSettingsByte(0x0A, 0x01);   // 1: Startkommando
    sendEtcsSettingsByte(0x0A, 0x02);   // 2: Grundstellung
}

void zusi3Tcp::sendEtcsLevel(QString level){// Level einstellen/anfordern (nur Client -> Zusi)
    // ETCS-Level (STM, 0, 1, usw.): 0: undf, 1: Stm, 2: L0, 3: L1, 4: L2, 5: L3
    if(level == "PZB / LZB"){
        sendEtcsSettingsWord(0x0B, 1);
        sendEtcsSettingsWord(0x0C, 0);
    }
    if(level == "Level 0")     sendEtcsSettingsWord(0x0B, 2);
    if(level == "Level 1")     sendEtcsSettingsWord(0x0B, 3);
    if(level == "Level 2")     sendEtcsSettingsWord(0x0B, 4);
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

void zusi3Tcp::sendKeyboardCommand(quint16 mapping, quint8  command, quint16 action){
    if(client->state() != QAbstractSocket::ConnectedState)return;
    QVector<unsigned char> genericMessageVector;
    addKnotenAnfang(&genericMessageVector, 0x02     );  // <Kn>  Client-Anwendung 02
    addKnotenAnfang(&genericMessageVector, 0x010A   );  // <Kn>  Befehl INPUT
    addKnotenAnfang(&genericMessageVector, 0x01     );  // <Kn>  Tastatureingaben
    addAtribut( &genericMessageVector, 0x01, mapping);  // Tastaturzuordnung
    addAtribut( &genericMessageVector, 0x02, command);  // Tastaturkommando
    addAtribut( &genericMessageVector, 0x03, action);  // Tastaturaktion
    for(quint8 i= 0; i<3 ; i++)addKnotenEnde(&genericMessageVector); // 3 </Kn> Knotenende
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
    for(quint8 i= 0; i<4 ; i++)addKnotenEnde(&genericMessageVector); // 4 </Kn> Knotenende
    QByteArray genericMessageArr;
    for(int i = 0; i < genericMessageVector.length(); i++)genericMessageArr.append(static_cast<char>(genericMessageVector[i]));
    client->write(genericMessageArr);
}

void zusi3Tcp::sendEtcsSettingsWord(quint8 ID, quint16 value){
    QVector<unsigned char> genericMessageVector;
    addKnotenAnfang(&genericMessageVector, 0x02     );  // <Kn>  Client-Anwendung 02
    addKnotenAnfang(&genericMessageVector, 0x010A   );  // <Kn>  Befehl INPUT
    addKnotenAnfang(&genericMessageVector, 0x02     );  // <Kn>  Zugbeeinflussung einstellen
    addKnotenAnfang(&genericMessageVector, 0x04     );  // <Kn>  System aus der ETCS-Familie - Einstellungen und Interaktionen
    addAtribut(     &genericMessageVector, ID, value);
    for(quint8 i= 0; i<4 ; i++)addKnotenEnde(&genericMessageVector); // 4 </Kn> Knotenende
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
void zusi3Tcp::addKnotenEnde(QVector<unsigned char> *vector){
    vector->append({0xFF, 0xFF, 0xFF, 0xFF});
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
void zusi3Tcp::checkClientConnection(QAbstractSocket::SocketState state){
    QDateTime date = QDateTime::currentDateTime();
    QString formattedTime = date.toString("hh:mm");
    switch(state){
    case QAbstractSocket::UnconnectedState:
        emit newTechnicalMessage({formattedTime}, {"Verbindungsaufbau gescheitert"});
        emit sendDataSourceIsZusi(false);
        if(autoReconnect || reconnectOnes)QTimer::singleShot(5000, this, SLOT(connectToZusi()));
        break;
    case QAbstractSocket::HostLookupState:
        emit newTechnicalMessage({formattedTime}, {"Verbindungsaufbau zu Zusi3"});
        break;
    case QAbstractSocket::ConnectingState:break;
    case QAbstractSocket::ConnectedState:
        emit newTechnicalMessage({formattedTime}, {"Mit Zusi3 Verbunden"});//myEtcs->setTextMessageByString("Mit Zusi3 Verbunden");
        emit sendDataSourceIsZusi(true);
        QTimer::singleShot(2500,this,SLOT(removeTechniccalMessages()));
        reconnectOnes = false;
        break;
    case QAbstractSocket::BoundState:break;
    case QAbstractSocket::ClosingState:break;
    case QAbstractSocket::ListeningState:break;
    }
}
/*void zusi3Tcp::handleConnectionError(QAbstractSocket::SocketError socketError){
    qDebug() << "handleConnectionError";
    qDebug() << socketError;
}*/

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
      if((nodesChanged = true) &&nodeCount == 0){
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
    plainningCounter = 0;
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
    myIndicators->makeLzbLmDatagram();
    transmitMtdIndicators();
}

void zusi3Tcp::zusiDecoderFahrpult(){
    switch(nodeIds[1]){
    case 0x000A:
        switch(nodeIds[2]){
        case 0x0001:      // Geschwindigkeit Meter/Sekunde
            if (checkHysterise(&VIst, mPerSecToKmh(useData4Byte.Single))){
                emit newSpeed(VIst);
                myIndicators->setVIst(VIst);
                myPower->setVIst(VIst);
                myEtcs->setSpeed(VIst);
                if(VIst > 0) trainHasBenMovedSinceLastNewTrainNumber = true;
            }
            return;
        case 0x0002:    // Druck Hauptluftleitung
            if (checkHysterise(&drHll, useData4Byte.Single * 10)){
                emit newHll(drHll);
            }
            return;
        case 0x0003:    // Druck Bremszylinder
            if (checkHysterise(&drBrz, useData4Byte.Single * 10)){
                emit newBrz(drBrz);
            }
            return;
        case 0x0004:    // Druck Hauptluftbehälter
            if (checkHysterise(&drHlb, useData4Byte.Single * 10)){
                emit newHlb(drHlb);
            }
            return;
        case 0x0061:    // Kilometrierung
            if (checkHysterise(&kilometer, static_cast<float>(floor(static_cast<double>(useData4Byte.Single) * 10.0))*100)){
                emit newKilometrierung(kilometer);
            }
            return;
        case 0x0065:    // 11.3.3.3.4 Status Zugbeeinﬂussung
            switch (nodeIds[3]){
            case 0x0001: myIndicators->setZugbeeinflussungssystem(QString(useDataComplex));return;          // Grundblock 11.3.3.3.4.1 Bauart Zugbeeinflussungssystem als Text
            case 0x0002:        // 11.3.3.3.4.2 System aus der Indusi-Familie - Einstellungen
                switch(nodeIds[4]) {
                case 0x0001: myIndicators->setZugart(useData2Byte.byte[0]);return;                          // Zugart
              //case 0x0002: qDebug() << "Tf-Nummer: " + QString(useDataComplex);return;                    // Tf-Nummer
                case 0x0003: setZugnummer(QString(useDataComplex), "PZB");return;                           // Zugnummer
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
                    case 0x0001: emit newBrh(useData2Byte.Word);return;                                     // BRH-Wert (Bremshundertstel). Werte der Ersatzzugdaten
                    case 0x0002: emit newBra(useData2Byte.Word);return;                                     // BRA-Wert (Bremsart). Werte der Ersatzzugdaten
                    case 0x0003: emit newZl(useData2Byte.Word);return;                                      // ZL-Wert (Zuglänge) in m
                    case 0x0004: emit newVmz(useData2Byte.Word);return;                                     // VMZ-Wert (Höchstgeschwindigkeit)in km/h
                    case 0x0005: return;                                                                    // Zugehörige Zugart. Werte der Ersatzzugdaten
                    case 0x0006: myIndicators->setGrunddatenWirksam(useData2Byte.byte[0] == 4, VIst <= 50); // Modus (Grunddaten / Ersatzzugdaten / Normalbetrieb)
                                 myIndicators->setErsatzdatenWirksam(useData2Byte.byte[0] == 5, VIst <= 50);
                                 return;
                    }
                    return;
              //case 0x0007: return;                                                                        // Hauptschalter qDebug() << "Indusi-HS: " + QString::number(useData2Byte.byte[0]);
                case 0x0008: myIndicators->setIndusiStoerschalter(useData2Byte.byte[0]);return;             // Indusi Störschalter qDebug() << "Indusi-SS: " + QString::number(useData2Byte.byte[0]);
                case 0x0009: myIndicators->setLzbStoerschalter(useData2Byte.byte[0]);return;                // LZB Störschalter
                case 0x000A: myIndicators->setPlzbLuftabsperrhahn(useData2Byte.byte[0]);return;              // Indusi-LuftabsperrhahnqDebug() << "Indusi-LH: " + QString::number(useData2Byte.byte[0]);
                case 0x000B: zusiTextMessagesPossible = useData2Byte.byte[0];                                // Klartextmeldungen
                             myIndicators->setKlartextmeldungen(zusiTextMessagesPossible, forceTextmessages);
                             return;
                case 0x0011: myIndicators->setSystemstatusLzb(useData2Byte.byte[0]); return;                // Systemstatus LZB:// 3 = Aktiv
                case 0x000D: myIndicators->setSystemstatusPzb(useData2Byte.byte[0]); return;                // Systemstatus Indusi:// 3 = Aktiv
                }
                return;
            case 0x0003:        // 11.3.3.3.4.2 System aus der Indusi-Familie - Betriebsdaten
                switch(nodeIds[4]) {
                case 0x0002: myIndicators->setZustandZugsicherung(useData2Byte.Word);return;                // Zustand Zugbeeinflussung
                case 0x0003: myIndicators->setGrundZwangsbrmnsung(useData2Byte.Word);return;                // Zwangsbremsung aktiv wegen
              //case 0x0004: return;                                                                        // Grund der Zwangsbremsung als Text
                case 0x0005: myIndicators->setLm1000Hz(useData2Byte.byte[0]);return;                        // Melder 1000 Hz
                case 0x000A: myIndicators->setLm500Hz(useData2Byte.byte[0]);return;                         // Melder 500 Hz
                case 0x000B: myIndicators->setLmBefehl(useData2Byte.byte[0]);return;                        // Melder Befehl
                case 0x000C: myIndicators->setMelderbild(useData2Byte.byte[0]);return;                      // Zusatzinfo Melderbild
                case 0x000D: myIndicators->setLzbZustand(useData2Byte.byte[0]);return;                      // LZB-Zustand
                case 0x000E:
                    switch (nodeIds[5]){
                    case 0x0001:myIndicators->setEndeverfahren(useData2Byte.byte[0]);return;
                    }
                    return;
                case 0x000F:
                    myIndicators->setErsatzauftrag(1);return;
                case 0x0010:
                    switch (nodeIds[5]){
                        case 0x0001: myIndicators->setFalschfahrauftrag(useData2Byte.byte[0]);return;
                    }
                    return;
                case 0x0011:
                    switch (nodeIds[5]){
                        case 0x0001:myIndicators->setVorsichtsauftrag(useData2Byte.byte[0]);return;
                    }
                    return;
                case 0x0012:
                    myIndicators->setFahrtUeberLlzbHaltPerBefehl(1);return;
                case 0x0013:
                    switch (nodeIds[5]){
                        case 0x0002: myIndicators->setUebertragungsausfall(useData2Byte.Word);return;
                    }
                    return;
                case 0x0016:
                    switch (nodeIds[5]){
                    case 0x0001: myIndicators->setLzbElAuftrag(useData2Byte.byte[0]);return;
                    }
                    return;
                case 0x0018: myIndicators->setLmE40(useData2Byte.byte[0]);return;                           // Melder E40 an
                case 0x001A: myIndicators->setLmB(useData2Byte.byte[0]);return;                             // Melder B an
                case 0x001B: myIndicators->setLmUe(useData2Byte.byte[0]);return;                            // Melder Ü an
                case 0x0024: myIndicators->setLmG(useData2Byte.byte[0]);return;                             // Melder G   // Ersetzt 0x001C
                case 0x001D: myIndicators->setLmEl(useData2Byte.byte[0]);return;                            // Melder EL an
                case 0x001E: myIndicators->setLmV40(useData2Byte.byte[0]);return;                           // Melder V40 an
                case 0x001F: myIndicators->setLmS(useData2Byte.byte[0]);return;                             // Melder S an
                case 0x0025: myIndicators->setLmPruefStoer(useData2Byte.byte[0]);return;                    // Melder Prüf/Stör // Ersetzt 0x0020
                case 0x0021: myIndicators->setVSoll(qFabs(ceil(useData4Byte.Single * 3.6 - 0.5)));return;   // Sollgeschwindigkeit in m/s
                case 0x0022: myIndicators->setVZiel(qFabs(ceil(useData4Byte.Single * 3.6 - 0.5)));return;   // Zielgeschwindigkeit in m/s (Wert<0 → dunkel)
                case 0x0023: myIndicators->setZielweg(ceil(useData4Byte.Single - 0.5));return;              // Zielweg in m (Wert<0 → dunkel)
                }
                return;
            case 0x0004:        // 11.3.3.3.4.9 System aus der ETCS-Familie - Einstellungen und Interaktionen
                dataContainsEtcs = true;
                switch(nodeIds[4]) {
                case 0x0001: myEtcs->setTdeState(useData2Byte.byte[0]);return;// Zustand
                case 0x0002:// verfügbares STM-System (nur Zusi → Client)
                    switch (nodeIds[5]){
                    case 0x0001: myEtcs->addNtcToListIndex(useData2Byte.Word);return;   // Index des STM-Systems, von 1 beginnend gemäß Reihenfolge in der ftd-Datei
                    case 0x0002: myEtcs->addNtcToListName(useDataComplex);return;       // Name des STM-Systems als Text
                    }
                    return;
                case 0x0003:                                                // Zugdaten
                    switch (nodeIds[5]){
                    case 0x0001: myEtcs->setBrakingPercentage(useData2Byte.Word);return;    // Bremshundertstel in %
                    case 0x0002: myEtcs->setTrainCategory(useData2Byte.Word);return;        // Zugkategorie
                    case 0x0003: myEtcs->setTrainLength(useData2Byte.Word);return;
                    case 0x0004: myEtcs->setMaxSpeed(useData2Byte.Word);return;
                    case 0x0005: myEtcs->setAxleLoad(useData2Byte.Word);return;
                    case 0x0006: myEtcs->setTrainNumber(useDataComplex);return;
                    case 0x0007: myEtcs->setDriverId(useDataComplex);return;
                    case 0x0008: myEtcs->setRbcNumber(useData4Byte.Cardinal);return;
                    case 0x0009: myEtcs->setRbcTelNumber(useDataComplex);return;
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
                case 0x0012: myEtcs->setMaxBaseline(useData2Byte.byte[0]);return;               // Maximal verfügbare Baseline des EVC
                case 0x0013: myEtcs->setVehicleHasEtcsCB(useData2Byte.byte[0]);return;          // Fahrzeug hat einen ETCS-LSS
                case 0x0014: myEtcs->setVehicleHasEtcsPS(useData2Byte.byte[0]);return;          // Fahrzeug hat einen Passivschalter
                case 0x0015: myEtcs->setVehicleHasEtcsRSW(useData2Byte.byte[0]);return;         // Fahrzeug hat einen ETCS-Reset-Schalter
                case 0x0016: myEtcs->setVehicleHasEtcsRSK(useData2Byte.byte[0]);return;         // Fahrzeug hat einen ETCS-Reset-Softkey
                case 0x0017: myEtcs->setEtcsCBState(useData2Byte.byte[0]);return;               // ETCS-LSS
                case 0x0019: myEtcs->setEvcTyte(useDataComplex);return;                         // Bauart Zugbeeinflussungssystem als Text
                }
            case 0x0005:                                                                        // 11.3.3.3.4.9 System aus der ETCS-Familie – Betriebsdaten
                dataContainsEtcs = true;
                switch(nodeIds[4]) {
                case 0x0001:
                    myEtcs->setActiveLevel(useData2Byte.Word);return;                  // Aktiver Level 0: ETCS Level undefiniert 1: ETCS Level STM 2: ETCS Level 0 3: ETCS Level 1 4: ETCS Level 2 5: ETCS Level 3
                case 0x0002: myEtcs->setActiveMode(useData2Byte.Word);return;                   // Aktiver ETCS-Modus 00: undefiniert 01: FS 02: OS 03: SR 04: SH 05: UN 06: SL 07: SB 08: TR 09: PT 10: SF 11: IS 12: NP 13: NL 14: SE 15: SN 16: RV 17: LS 18: PS
                case 0x0003: myEtcs->setReasonOfEmrBreakEnum(useData2Byte.Word);return;         // Grund der Zwangs- oder Betriebszwangsbremsung 0: Keine Zwangsbremsung 6: v-Max-Überwachung 7: Funktionsprüfung 10: Rechnerausfall 11: ETCS-Nothalt überfahren 15: ETCS-Halt überfahren 16: ETCS: Stillstands-/Rücklaufüberwachung ausgelöst 17: ETCS: nicht quittiert 18: ETCS: Funkausfall 19: ETCS: Balisenstörung 20: ETCS: manueller Levelwechsel 27: Allgemeine Störung 28: Stromversorgung fehlt
                case 0x0004: myEtcs->setReasonOfEmrBreakText(useDataComplex);return;            // Grund der Zwangs- oder Betriebszwangsbremsung als Text
                case 0x0005:                                                                    // STM-Info
                    switch(nodeIds[5]) {
                    case 0x0001: myEtcs->setIndexOfActiveStm(useData2Byte.Word);return;         // Index des aktiven STM-System, von 1 beginnend gemäß Reihenfolge in der ftd-Datei
                    }
                case 0x0006:                                                                    // Angekündigter oder neu aktivierter Level
                    switch(nodeIds[5]) {
                    case 0x0001:myEtcs->setNextLevel(useData2Byte.Word, 0xff);return;           // Neuer Level gemäß Paket 00 01
                    case 0x0002:myEtcs->setNextLevel(0xffff, useData2Byte.byte[0]);return;      // Quittierung
                    }
                case 0x0007:                                                                    // Angekündigter Modus
                    switch(nodeIds[5]) {
                    case 0x0001: myEtcs->setNextMode(useData2Byte.Word, 0xff);return;           // Neuer Modus gemäß Paket 00 02
                    case 0x0002: myEtcs->setNextMode(0xffff, useData2Byte.byte[0]);return;      //  Quittierung Modus 1: noch keine Quittierung nötig 2: Quittierung nötig, aber noch nicht alle Voraussetzungen gegeben 3: Quittierung nötig 4: quittiert 5: wirksam
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
                case 0x0011: myEtcs->setPermittedSpeedReducing(useData2Byte.byte[0]);return;    // Bremskurve läuft ab 0: nein 1: ja
                case 0x0012:                                                                    // Vorschaupunkt
                    switch(nodeIds[5]) {
                        case 0x0001: myEtcs->addPlanningInfoOrigin(useData2Byte.Word, plainningCounter);            // Herkunft
                                     plainningCounter++;
                                     return;
                        case 0x0002: myEtcs->addPlanningInfoSpeed(useData4Byte.Single, plainningCounter);return;    // Geschwindigkeit in m/s (-1 bedeutet ETCS-Ende)
                        case 0x0003: myEtcs->addPlanningInfoDistance(useData4Byte.Single, plainningCounter);return; // Abstand in m
                        case 0x0004: myEtcs->addPlanningGradient(useData4Byte.Single, plainningCounter);return;     // Höhenwert in m
                        case 0x0005: myEtcs->addPlanningParameter(useData2Byte.Word, plainningCounter);return;      // Parameter
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
                    switch(nodeIds[5]) {                                                        // ETCS-Textmeldung, es folgt entweder Attribut 1 oder 2
                    case 0x0001: myEtcs->setTextMessageByEnum(useData2Byte.Word);return;        // Grund einer Zwangsbremsung gemaess 00 02 → 00 0A → 00 65 →00 05 → 00 03
                    case 0x0002: myEtcs->setTextMessageByString(useDataComplex);return;         // Freier Meldungstext
                    }
                }
            case 0x0007:        // 11.3.3.3.4.10 ZUB
                switch(nodeIds[4]) {
                case 0x0001: myIndicators->setLmGnt(useData2Byte.Word);return;                              // Status Melder GNT
                case 0x0002: myIndicators->setLmGntUe(useData2Byte.Word);return;                            // Status Melder Ü
                case 0x0003: myIndicators->setLmGntG(useData2Byte.Word);return;                             // Status Melder G
                case 0x0004: myIndicators->setLmGntS(useData2Byte.Word);return;                             // Status Melder S
              /*case 0x0007: myIndicators->setLmGntUe(useData2Byte.Word);return;                            // Status Melder GNT Ü
                case 0x0008: myIndicators->setLmGntG(useData2Byte.Word);return;                             // Status Melder GNT G
                case 0x0009: myIndicators->setLmGntS(useData2Byte.Word);return;                             // Status Melder GNT S*/
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
                if (QString(useDataComplex).contains("SAT")){setMtdIndicator(2,9);return;}                  // Türsystem SAT
                if (QString(useDataComplex).contains("TB0")){setMtdIndicator(3,9);return;}                  // Türsystem TB0
                if (QString(useDataComplex).contains("TAV")){setMtdIndicator(4,9);return;}                  // Türsystem TAV
                if (QString(useDataComplex).contains("SST")){setMtdIndicator(5,9);return;}                  // Türsystem SST
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
        case 0x0023: emit newSimTime(QTime::fromMSecsSinceStartOfDay(useData4Byte.Single * 86400000).toString());
                     myIndicators->setSimTime(QTime::fromMSecsSinceStartOfDay(useData4Byte.Single * 86400000).toString("hh:mm"));
                     myEtcs->setSimTime(QTime::fromMSecsSinceStartOfDay(useData4Byte.Single * 86400000).toString("hh:mm"));
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
                case 0x0006: myPower->setBaureihe(QString(useDataComplex));return;
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
        case 0x0002: setZugnummer(QString(useDataComplex), "Fahrplan");return; // Aktuelle Zugnummer
      //case 0x0003: return; // Status Ladepause, 0: Ende Ladepause (Start der Simulation)
        case 0x0005: // 1: Zug neu übernommen
            if(useData2Byte.byte[0] > 0){
                emit changedTrain();
                myEtcs = new zusi3etcs();
                reconnect();
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
            QStringList zusiVersion = QString(useDataComplex).split(".");
            QString minReqZusiVersion = " Mindestens Zusi " + QString::number(zusiMajor) + "." + QString::number(zusiMinor) + "." + QString::number(zusiPatch) + " erforderlich!";
            if(zusiVersion[0].toInt() != zusiMajor){emit newTechnicalMessage({"a-:--"}, {minReqZusiVersion});return;}
            if(zusiVersion[1].toInt() <  zusiMinor){emit newTechnicalMessage({"b-:--"}, {minReqZusiVersion});return;}
            if(zusiVersion[1].toInt() == zusiMinor){
                if(zusiVersion[2].toInt() <  zusiPatch){emit newTechnicalMessage({"c-:--"}, {minReqZusiVersion});return;}
            }
            return;
        }
        case 0x0002: return; //qDebug() << "Zusi-Verbindungsinfo: " + QString(useDataComplex);
        case 0x0005: return; //qDebug() << "Version des TCP-Protokolls: " + QString(useDataComplex);
        }
    }
    if ((nodeIds[0] == 0x0001) && (nodeIds[1] == 0x0002) && (nodeIds[2] == 0x0003)){
      //qDebug() << "Der Client wurde akzeptiert " + QString::number(useData2Byte.chr[0]);
    }
}

float zusi3Tcp::mPerSecToKmh(float input){
    return ceil((input * 3.6 - 0.5));
}


bool zusi3Tcp::checkHysterise(int32_t *output, float input){
    qint32 tmp = static_cast<qint32>(input);
    if(tmp != *output){
        *output = tmp;
        return true;
    }
    return false;
}

bool zusi3Tcp::checkHysterise(uint16_t *output, float input){
    uint16_t tmp = qFabs(input);
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
/*void zusi3Tcp::guesTractionType(){
  if(fahrlSpng > 0){
        setMtdIndicator(1, 12);
    }
    if(fahrlSpng <= 0 && ((zugkraftProAchs > 0) || (zugkraftProAchsSteuerwagen > 0))){
        setMtdIndicator(2, 12);
    }
}*/
void zusi3Tcp::setUseManometer(bool use){
    if(useManometer != use){
        useManometer = use;
        reconnect();
    }
}
void zusi3Tcp::removeTechniccalMessages(){
    //myEtcs->setTextMessageByString("");
    emit newTechnicalMessage({""}, {""});
}
void zusi3Tcp::setTextUsing(quint8 useAutomText){
    if(forceTextmessages != useAutomText){  // 0: Allways, 1: Automatic, 2: Never
        forceTextmessages = useAutomText;
        myIndicators->setDefaults();
        reconnect();
    }
    myIndicators->setKlartextmeldungen(zusiTextMessagesPossible, forceTextmessages);
}

#include "zusi3tcp.h"
#include <QDebug>

zusi3Tcp::zusi3Tcp(){}

void zusi3Tcp::process(){
    client = new QTcpSocket();
    myIndicators = new zusiIndicator();
    myPower = new zusiPower();
    mtdLmsToDecoder.resize(13);
    connect(client,SIGNAL(readyRead()),this,SLOT(clientReadReady()));
}

void zusi3Tcp::disconnectFromZusi(){
   client->disconnectFromHost();
}

void zusi3Tcp::setIpadress(QString address){
    incommingData.clear();
    disconnectFromZusi();
    if(address.isEmpty()) emit sendTcpConnectionFeedback("-1");
    QStringList slist = address.split(".");
    int s = slist.size();
    if(s>4){
        emit newTechnicalMessage(" IP hat zu viele Elemente", era::grey, era::darkBlue, 9);
        QTimer::singleShot(2500,this,SLOT(remooveTechMessage9()));
         emit sendTcpConnectionFeedback("-1");
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
            emit newTechnicalMessage(" Ungültige Werte in IP", era::grey, era::darkBlue, 9);
            QTimer::singleShot(2500,this,SLOT(remooveTechMessage9()));
            emit sendTcpConnectionFeedback("-1");
        }
    }
    if(s<4 || emptyGroup){
        emit newTechnicalMessage(" IP hat zu wenig Elemente", era::grey, era::darkBlue, 9);
        QTimer::singleShot(2500,this,SLOT(remooveTechMessage9()));
        emit sendTcpConnectionFeedback("-1");
    }
    // qDebug() << address;
    QHostAddress zusiPc = QHostAddress(address);
    client->abort();
    client->connectToHost(zusiPc,1436);
    constexpr static const unsigned char  Anmeldung2[] = { 0x00, 0x00, 0x00, 0x00, 0x1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1, 0x00, 0x04, 0x00, 0x00, 0x00, 0x1, 0x00, 0x2, 0x00, 0x04, 0x00, 0x00, 0x00, 0x2, 0x00, 0x2, 0x00, 0xA, 0x00,
                            0x00, 0x00, 0x3, 0x00, 0x51, 0x44, 0x6D, 0x69, 0x0, 0x0, 0x0, 0x0, 0x5, 0x00, 0x00, 0x00, 0x04, 0x00, 0x32, 0x2E, 0x30, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
                          };
    constexpr static const unsigned char trainData[] =
             { 0x00, 0x00, 0x00, 0x00, 0x02, 0x00,              // <Kn> Client-Anwendung 02
               0x00, 0x00, 0x00, 0x00, 0x0A, 0x01,              // <Kn> Befehl INPUT
               0x00, 0x00, 0x00, 0x00, 0x02, 0x00,              // <Kn> Zugbeeinflussung einstellen
               0x00, 0x00, 0x00, 0x00, 0x02, 0x00,              // <Kn> System aus der Indusi-Familie
               0x00, 0x00, 0x00, 0x00, 0x04, 0x00,              // <Kn> Werte der Grunddaten
               0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x8C, 0x00,  // BRH-Wert (Bremshundertstel)
               0x04, 0x00, 0x00, 0x00, 0x02, 0x00, 0x09, 0x00,  // BRA-Wert (Bremsart)
               0x04, 0x00, 0x00, 0x00, 0x05, 0x00, 0x02, 0x00,  // Zugehörige Zugart
               0xFF, 0xFF, 0xFF, 0xFF,
               0x00, 0x00, 0x00, 0x00, 0x05, 0x00,              // <Kn> Werte der Ersatzzugdaten
               0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x8C, 0x00,  // BRH-Wert (Bremshundertstel)
               0x04, 0x00, 0x00, 0x00, 0x02, 0x00, 0x09, 0x00,  // BRA-Wert (Bremsart)
               0x04, 0x00, 0x00, 0x00, 0x05, 0x00, 0x02, 0x00,  // Zugehörige Zugart
               0x04, 0x00, 0x00, 0x00, 0x06, 0x00, 0x06, 0x00,  // Modus (Normalbetrieb, nicht Ersatzzugdaten)
               0xFF, 0xFF, 0xFF, 0xFF,
               0x00, 0x00, 0x00, 0x00, 0x06, 0x00,              // <Kn> Aktive Zugdaten
               0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x8C, 0x00,  // BRH-Wert (Bremshundertstel)
               0x04, 0x00, 0x00, 0x00, 0x02, 0x00, 0x09, 0x00,  // BRA-Wert (Bremsart)
               0x04, 0x00, 0x00, 0x00, 0x05, 0x00, 0x02, 0x00,  // Zugehörige Zugart
               0x04, 0x00, 0x00, 0x00, 0x06, 0x00, 0x06, 0x00,  // Modus (Normalbetrieb, nicht Ersatzzugdaten)
               0xFF, 0xFF, 0xFF, 0xFF,
               0xFF, 0xFF, 0xFF, 0xFF,
               0xFF, 0xFF, 0xFF, 0xFF,
               0xFF, 0xFF, 0xFF, 0xFF,
               0xFF, 0xFF, 0xFF, 0xFF
             };
    client->write((const char*)Anmeldung2,sizeof (Anmeldung2));
    //qDebug() << (" Rregistration required data");
    constexpr static const unsigned char Abfrage2[] = {
        0x00, 0x00, 0x00, 0x00, 0x02, 0x00,              // <Kn>
        0x00, 0x00, 0x00, 0x00, 0x03, 0x00,              // <Kn>
        0x00, 0x00, 0x00, 0x00, 0x0A, 0x00,              // <Kn>
        0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00,  // Geschwindigkeit m/s
      //0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x09, 0x00,  // Zugkraft gesammt
        0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x0A, 0x00,  // Zugkraft pro Achse
      //0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x0B, 0x00,  // Zugkraft-soll gesammt
        0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x0C, 0x00,  // Zugkraft-Soll pro Achse
      //0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x7C, 0x00,  // Steuerwagen: Zugkraft gesammt
        0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x7D, 0x00,  // Steuerwagen: Zugkraft pro Achse
      //0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x7E, 0x00,  // Steuerwagen: Zugkraft-soll gesammt
        0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x7F, 0x00,  // Steuerwagen: Zugkraft-Soll pro Achse
      //0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x90, 0x00,  // Zug- und Brems-Gesamtkraftsoll normiert
      //0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x91, 0x00,  // Steuerwagen: Zug- und Brems-Gesamtkraftsoll normiert
      //0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x93, 0x00,  // Zug- und Brems-Gesamtkraftsoll absolut normiert
      //0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x94, 0x00,  // Steuerwagen: Zug- und Brems-Gesamtkraftsoll absolut normiert
        0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x0E, 0x00,  // Fahrleitungsspannung
        0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x23, 0x00,  // Zeit
        0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x13, 0x00,  // Hauptschalter
        0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x15, 0x00,  // Fahrstufe
        0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x17, 0x00,  // AFB-Sollgeschwindigkeit
        //0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x20, 0x00,  // LM Hochabbremsung Aus/Ein
        //0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1b, 0x00,  // LM Schleudern
        //0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1c, 0x00,  // LM Gleiten
        0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x36, 0x00,  // AFB an
        0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x55, 0x00,  // Stromabnehmer
        0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x64, 0x00,  // SIFA
        0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x65, 0x00,  // Zugsicherung
        0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x66, 0x00,  // Türen
        0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x88, 0x00,  // Steuerwagen: Stromabnehmer
        0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x8e, 0x00,  // Status Zugverband716
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF
        };
    client->write((const char*)Abfrage2,sizeof (Abfrage2));
    QTimer::singleShot(0,this,SLOT(checkClientConnection()));
    emit sendTcpConnectionFeedback(address);
}

void zusi3Tcp::checkClientConnection(){
    switch(client->state()){
    case QAbstractSocket::UnconnectedState:
        removeTechnicalMessage(11);
        emit newTechnicalMessage("Verbindungsaufbau gescheitert", era::grey, era::darkBlue, 10);
        QTimer::singleShot(5000,this,SLOT(remooveTechMessage10()));
        break;
    case QAbstractSocket::HostLookupState:
        removeTechnicalMessage(10);
        emit newTechnicalMessage("Verbindungsaufbau zu Zusi3", era::grey, era::darkBlue, 11);
        QTimer::singleShot(10,this,SLOT(checkClientConnection()));
        break;
    case QAbstractSocket::ConnectingState:
        removeTechnicalMessage(10);
        emit newTechnicalMessage("Verbindungsaufbau zu Zusi3", era::grey, era::darkBlue, 11);
        QTimer::singleShot(10,this,SLOT(checkClientConnection()));
        break;
    case QAbstractSocket::ConnectedState:
        removeTechnicalMessage(10);
        removeTechnicalMessage(11);
        emit newTechnicalMessage(" Mit Zusi3 Verbunden", era::grey, era::darkBlue, 13);
        QTimer::singleShot(2500,this,SLOT(remooveTechMessage13()));
        break;
    case QAbstractSocket::BoundState:
        break;
    case QAbstractSocket::ClosingState:
        break;
    case QAbstractSocket::ListeningState:
        break;
    }
}
void zusi3Tcp::remooveTechMessage9(){
    removeTechnicalMessage(9);
}
void zusi3Tcp::remooveTechMessage10(){
    removeTechnicalMessage(10);
}
void zusi3Tcp::remooveTechMessage13(){
    removeTechnicalMessage(13);
}

void zusi3Tcp::clientReadReady(){
    // This function is used to isolate a zusi telegram.
    // If we can find an entire one, we cut it for decoding, and leave the rest, to be appended to new data.
    bool nodesChanged = false;
    int nodeCount = 0;
    int packetLength = 0;
    static quint8 errorcounter = 0;
    static qint64 incommingDataSizeOld = 0;
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
    // Workarround: Since using multithreading, some data, coming from zusi, get lost sporadically.
    // Adding a sleep helps, and if it does not help, we have here a mechanism, that checks if the
    // data to be decoded becomes bigger and bigger. If that is the case we emit error code "-2".
 /*   if(incommingData.size() >= incommingDataSizeOld && incommingData.size() > 0){
       errorcounter++;
       if(errorcounter > 3){
           incommingDataSizeOld = 0;
           //qDebug() << "================== Data loose ==================               " + QString::number(incommingData.size());
           emit sendTcpConnectionFeedback("-2");
       }
    }
    else{
        errorcounter = 0;
    }
    incommingDataSizeOld = incommingData.size();
    QThread::msleep(1);*/
    // Workarround end
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
                zusiDecoderFahrpult();
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
            switch ((packetLength - 2))
            case 4:
                useData4Byte.chr[0] = compZusiPacket[i + 6];
                useData4Byte.chr[1] = compZusiPacket[i + 7];
                useData4Byte.chr[2] = compZusiPacket[i + 8];
                useData4Byte.chr[3] = compZusiPacket[i + 9];
            case 2:
                useData2Byte.chr[0] = compZusiPacket[i + 6];
                useData2Byte.chr[1] = compZusiPacket[i + 7];
            case 1:
                useData2Byte.chr[0] = compZusiPacket[i + 6];
                useData2Byte.chr[1] = 0;
            useDataComplex = compZusiPacket.mid(i+6,packetLength-2);
            if (nodeIds[0] == 0x0002 && nodeIds[1] == 0x000A){ // "Client-Anwendung 02" and "Befehl DATA_FTD - Führerstandsanzeigen"
                zusiDecoderFahrpult();
            }
            else{
                zusiDecoderSecondaryInfos();
            }
            i = i + packetLength + 3;
        }
    }
}

void zusi3Tcp::zusiDecoderFahrpult(){
    qDebug() << "need to decode";
    switch(nodeIds[2]){
        case 0x0001:      // Geschwindigkeit Meter/Sekunde
            if (checkHysterise(&VIst, useData4Byte.Single)){
                emit newSpeed(VIst);
                myIndicators->setVIst(VIst);
                myPower->setVIst(VIst);
            }
            return;
        case 0x0065:    // 11.3.3.3.4 Status Zugbeeinﬂussung
            switch (nodeIds[3]){
                case 0x0001:      // Grundblock 11.3.3.3.4.1 Bauart Zugbeeinflussungssystem als Text
                    myIndicators->setZugbeeinflussungssystem(QString(useDataComplex));
                    return;
                case 0x0002:// 11.3.3.3.4.2 System aus der Indusi-Familie - Einstellungen
                    switch(nodeIds[4]) {
                        case 0x0001:      // Zugart
                            myIndicators->setZugart(useData2Byte.byte[0]);
                            return;
                        case 0x0002:   // Tf-Nummer
                            //qDebug() << "Tf-Nummer: " + QString(useDataComplex);
                            return;
                        case 0x0003:   // Zugnummer
                            //qDebug() << "Zugnummer: " + QString(useDataComplex);
                            return;
                        case 0x0004:
                            switch(nodeIds[5]){
                            case 0x0001:   // BRH-Wert (Bremshundertstel).
                                //qDebug() << "BRH Grund.: " + QString::number(useData2Byte.Word);
                                return;
                            case 0x0002:   // BRA-Wert (Bremsart).
                                //qDebug() << "BRA Grund.: " + QString::number(useData2Byte.Word);
                                return;
                            case 0x0003:   // ZL-Wert (Zuglänge) in m
                                //qDebug() << "ZL Grund:   " + QString::number(useData2Byte.Word);
                                return;
                            case 0x0004:   // VMZ-Wert (Höchstgeschwindigkeit)in km/h
                                //qDebug() << "VMZ Grund.: " + QString::number(useData2Byte.Word);
                                return;
                            case 0x0005:   // Zugehörige Zugart. Werte der Ersatzzugdaten
                                //qDebug() << "ZA Grund.:  " + QString::number(useData2Byte.byte[0]);
                                return;
                            }
                            return;
                        case 0x0005:
                            switch(nodeIds[5]) {
                                case 0x0001:   // BRH-Wert (Bremshundertstel)
                                    //qDebug() << "BRH Ers.: " + QString::number(useData2Byte.Word);
                                    return;
                                case 0x0002:   // BRA-Wert (Bremsart).
                                    //qDebug() << "BRA Ers.: " + QString::number(useData2Byte.Word);
                                    return;
                                case 0x0003:   // ZL-Wert (Zuglänge) in m
                                    //qDebug() << "ZL Ers.:  " + QString::number(useData2Byte.Word);
                                    return;
                                case 0x0004:   // VMZ-Wert (Höchstgeschwindigkeit)in km/h
                                    //qDebug() << "VMZ Ers.: " + QString::number(useData2Byte.Word);
                                    return;
                                case 0x0005:   // Zugehörige Zugart.
                                    //qDebug() << "ZA Ers.:  " + QString::number(useData2Byte.byte[0]);
                                    return;
                            }
                            return;
                        case 0x0006:
                            switch(nodeIds[5]) {
                                case 0x0001:   // BRH-Wert (Bremshundertstel). Werte der Ersatzzugdaten
                                    //qDebug() << "BRH Akt.:   " + QString::number(useData2Byte.Word);
                                    return;
                                case 0x0002:   // BRA-Wert (Bremsart). Werte der Ersatzzugdaten
                                    //qDebug() << "BRA Akt.:   " + QString::number(useData2Byte.Word);
                                    return;
                                case 0x0003:   // ZL-Wert (Zuglänge) in m
                                    //qDebug() << "ZL Akt.:    " + QString::number(useData2Byte.Word);
                                    return;
                                case 0x0005:   // Zugehörige Zugart. Werte der Ersatzzugdaten
                                    //qDebug() << "ZA Akt.:    " + QString::number(useData2Byte.byte[0]);
                                    return;
                                case 0x0006:   // Modus (Ersatzzugdaten / Normalbetrieb)
                                    //qDebug() << "Ers./Norm.: " + QString::number(useData2Byte.byte[0]);
                                    return;
                            }
                            return;
                        case 0x0007:   // Hauptschalter
                            //qDebug() << "Indusi-HS: " + QString::number(useData2Byte.byte[0]);
                            return;
                        case 0x0008:   // Indusi Störschalter
                            //qDebug() << "Indusi-SS: " + QString::number(useData2Byte.byte[0]);
                            return;
                        case 0x0009:   // LZB Störschalter
                            //qDebug() << "LZB SS   : " + QString::number(useData2Byte.byte[0]);
                            return;
                        case 0x000A:   // Indusi-Luftabsperrhahn
                            //qDebug() << "Indusi-LH: " + QString::number(useData2Byte.byte[0]);
                            return;
                        case 0x000B:   // Klartextmeldungen
                            myIndicators->setKlartextmeldungen(useData2Byte.byte[0]);
                            return;
                }
                case 0x0003:// 11.3.3.3.4.2 System aus der Indusi-Familie - Betriebsdaten
                    switch(nodeIds[4]) {
                        case 0x0002:  // Zustand Zugbeeinflussung
                            myIndicators->setZustandZugsicherung(useData2Byte.Word);
                            return;
                        case 0x0003:  // Zwangsbremsung aktiv wegen
                            myIndicators->setGrundZwangsbrmnsung(useData2Byte.Word);
                            return;
                        case 0x0004:  // Grund der Zwangsbremsung als Text
                            //qDebug() << "ZB wegen Tx: " + QString(useDataComplex);
                            return;
                        case 0x0005:  // Melder 1000 Hz
                            myIndicators->setLm1000Hz(useData2Byte.byte[0]);
                            return;
                        case 0x000A:  // Melder 500 Hz
                            myIndicators->setLm500Hz(useData2Byte.byte[0]);
                            return;
                        case 0x000B:  // Melder Befehl
                            myIndicators->setLmBefehl(useData2Byte.byte[0]);
                            return;
                        case 0x000C:  // Zusatzinfo Melderbild
                            myIndicators->setMelderbild(useData2Byte.byte[0]);
                            return;
                        case 0x000D:  // LZB-Zustand    // Möglicherweise geht das nicht mit "(bool)", sondern ich muss >0 abfragen
                            myIndicators->setLzbZustand(useData2Byte.byte[0]);
                            return;
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
                        case 0x0018:  // Melder E40 an
                            myIndicators->setLmE40(useData2Byte.byte[0]);
                            return;
                        case 0x001A:  // Melder B an
                            myIndicators->setLmB(useData2Byte.byte[0]);
                            return;
                        case 0x001B:  // Melder Ü an
                            myIndicators->setLmUe(useData2Byte.byte[0]);
                            return;
                        case 0x0024:  // Melder G   // Ersetzt 0x001C
                            myIndicators->setLmG(useData2Byte.byte[0]);
                            return;
                        case 0x001D:  // Melder EL an
                            myIndicators->setLmEl(useData2Byte.byte[0]);
                            return;
                        case 0x001E:  // Melder V40 an
                            myIndicators->setLmV40(useData2Byte.byte[0]);
                            return;
                        case 0x001F:  // Melder S an
                            myIndicators->setLmS(useData2Byte.byte[0]);
                            return;
                        case 0x0025:  // Melder Prüf/Stör // Ersetzt 0x0020
                            myIndicators->setLmPruefStoer(useData2Byte.byte[0]);
                            return;
                        case 0x0021:  // Sollgeschwindigkeit in m/s
                            myIndicators->setVSoll(qFabs(ceil(useData4Byte.Single * 3.6 - 0.5)));
                            //myIndicators->setLzbValue(useData4Byte.Single, 3);
                            return;
                        case 0x0022:  // Zielgeschwindigkeit in m/s (Wert<0 → dunkel)
                            myIndicators->setVZiel(qFabs(ceil(useData4Byte.Single * 3.6 - 0.5)));
                            //myIndicators->setLzbValue(useData4Byte.Single, 1);
                            return;
                        case 0x0023:  // Zielweg in m (Wert<0 → dunkel)
                            //if(useData4Byte.Single <0 ) useData4Byte.Single = 0;
                            myIndicators->setZielweg(ceil(useData4Byte.Single - 0.5));
                            //myIndicators->setLzbValue(useData4Byte.Single, 5);
                           return;
                        default:
                            return;
               }
                case 0x0007:// 11.3.3.3.4.10 ZUB
                    switch(nodeIds[4]) {
                        case 0x0001:   // Status Melder GNT
                            myIndicators->setLmGnt(useData2Byte.Word);
                            return;
                        case 0x0002:   // Status Melder Ü
                            myIndicators->setLmGntUe(useData2Byte.Word);
                            return;
                        case 0x0003:   // Status Melder G
                            myIndicators->setLmGntG(useData2Byte.Word);
                            return;
                        case 0x0004:   // Status Melder S
                            myIndicators->setLmGntS(useData2Byte.Word);
                            return;
                      /*case 0x0007:   // Status Melder GNT Ü
                            myIndicators->setLmGntUe(useData2Byte.Word);
                            return;
                        case 0x0008:   // Status Melder GNT G
                            myIndicators->setLmGntG(useData2Byte.Word);
                            return;
                        case 0x0009:   // Status Melder GNT S
                            myIndicators->setLmGntS(useData2Byte.Word);
                            return;*/
                    }
                    return;
            }
            return;
        case 0x064:     // 11.3.3.3.3 Status Sifa
            switch (nodeIds[3]){
                case 0x0001:      // Bauart Sifasystem als Text
                    //qDebug() << "Sifasystem: " + QString(useDataComplex);
                    return;
                case 0x0002:   // Status Sifa-Leuchtmelder
                    setMtdIndicator(useData2Byte.byte[0],1);
                    return;
                case 0x0003:   // Status Sifa-Hupe
                    setMtdIndicator(useData2Byte.byte[0],2);
                    return;
                case 0x0004:   // Sifa-Hauptschalter
                    return;
                case 0x0005:   // Sifa-Störschalter         //myIndicators->setSifaStoerschalter(useData2Byte.byte[0]);
                    //qDebug() << "Sifa-SS:    " + QString::number(useData2Byte.byte[0]);
                    setMtdIndicator(useData2Byte.byte[0] % 2 ,0); // 2 = Sifa on, 1 = Sifa off
                    return;
            }
            return;
        case 0x066:
            switch(nodeIds[3]) {
                case 0x0001:      // Bezeichnung des Türsystems als Text
                    if (QString(useDataComplex).contains("SAT")){
                        setMtdIndicator(2,9);
                        return;
                    }
                    if (QString(useDataComplex).contains("TB0")){
                        setMtdIndicator(3,9);
                        return;
                    }
                    if (QString(useDataComplex).contains("TAV")){
                        setMtdIndicator(4,9);
                        return;
                    }
                    if (QString(useDataComplex).contains("SST")){
                        setMtdIndicator(5,9);
                        return;
                    }
                    return;
                case 0x0002:      // Status linke Seite
                    setMtdIndicator(useData2Byte.byte[0],10);
                    return;
                case 0x0003:      // Status rechte Seite
                    setMtdIndicator(useData2Byte.byte[0],11);
                    return;
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
                     zugkraftProAchs = useData4Byte.Single; guesTractionType(); return;
      //case 0x000B: myPower->setZugkraftSollGesammt(useData4Byte.Single); return;           //              Zugkraft-Soll gesammt
        case 0x000C: myPower->setZugkraftSollProAchse(useData4Byte.Single); return;          //              Zugkraft-Soll pro Achse
      //case 0x007C: myPower->setZugkraftGesammtSteuerwagen(useData4Byte.Single); return;    // Steuerwagen: Zugkraft gesammt
        case 0x007D: myPower->setZugkraftProAchseSteuerwagen(useData4Byte.Single); return;   // Steuerwagen: Zugkraft pro Achse
      //case 0x007E: myPower->setZugkraftSollGesammtSteuerwagen(useData4Byte.Single); return;// Steuerwagen: Zugkraft-soll gesammt
        case 0x007F: myPower->setZugkraftSollProAchseSteuerwagen(useData4Byte.Single); return;// Steuerwagen: Zugkraft-Soll gesamt pro Achse
      //case 0x0090: myPower->setZugkraftSollNormiert(useData4Byte.Single); return;          //              Zug- und Brems-Gesamtkraft soll normiert
      //case 0x0091: myPower->setZugkraftSollNormiertSteuerwagen(useData4Byte.Single); return;// Steuerwagen: Zug- und Brems-Gesamtkraft soll normiert
      //case 0x0093: myPower->setZugkraftNormiert(useData4Byte.Single); return;              //              Zug- und Brems-Gesamtkraft absolut normiert
      //case 0x0094: myPower->setZugkraftNormiertSteuerwagen(useData4Byte.Single); return;// Steuerwagen: Zug- und Brems-Gesamtkraft absolut normiert
        case 0x000E: fahrlSpng = useData4Byte.Single; guesTractionType(); return;
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
        //case 0x0015: return;  // Fahrstufe
          case 0x0017:   // AFB-Sollgeschwindigkeit
            if(checkHysterise(&VSoll, useData4Byte.Single))
                myIndicators->setAfbSoll(VSoll);
            return;
        //case 0x0020: return;  // Hohe Abbrems
          case 0x001b: myIndicators->setLmSchleudern(useData4Byte.Single > 0);return;// LM Schleudern
          case 0x001c: myIndicators->setLmGleiten(useData4Byte.Single > 0);return;// LM Gleiten
          case 0x0023: emit newSimtime(QTime::fromMSecsSinceStartOfDay(useData4Byte.Single * 86400000).toString());return;
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
                            if(checkHysterise(&VMFzg, useData4Byte.Single) && istVMaxErstesFahrzeug){
                                myIndicators->setFzgVMax(VMFzg);
                                istVMaxErstesFahrzeug = false;
                                QTimer::singleShot(2000, this, SLOT(resetVehicleBlocking()));
                            }
                            return;
                        case 0x0006:
                            myPower->setBaureihe(QString(useDataComplex));
                            return;
                    }
                case 0x0002:
                    istReisezug = useData2Byte.byte[0];
                    if(!istReisezug)setMtdIndicator(1,9);
                    setMtdIndicator(!hauptschalter ,7);
                    if(istReisezug)QTimer::singleShot(5000, this, SLOT(setSammelschine()));
                    return;
            }
            return;
        default:
            return;
    }
}

void zusi3Tcp::zusiDecoderSecondaryInfos(){
    // Verbindung
    if ((nodeIds[0] == 0x0001) && (nodeIds[1] == 0x0002)){
        switch(nodeIds[2]) {
            case 0x0001:
                //qDebug() << "Zusi-Version: " + QString(useDataComplex);
                return;
            case 0x0002:
                //qDebug() << "Zusi-Verbindungsinfo: " + QString(useDataComplex);
                return;
        }
    }
    if ((nodeIds[0] == 0x0001) && (nodeIds[1] == 0x0002) && (nodeIds[2] == 0x0003)){
      //qDebug() << "Der Client wurde akzeptiert " + QString::number(useData2Byte.chr[0]);
    }
}

bool zusi3Tcp::checkHysterise(uint16_t *output, float input){
    uint16_t tmp = qFabs(ceil((input * 3.6 - 0.5)));
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


//emit newVTarget(static_cast<quint16>((values[2] << 8) + values[1]), (values[0] & 0x0f) > 0);
//emit newVPermit(static_cast<quint16>((values[4] << 8) + values[3]), (values[0] & 0x0f) > 0);
//emit newTarDist(static_cast<quint16>((values[6] << 8) + values[5]), (values[0] & 0x0f) > 0);
void zusi3Tcp::setMtdIndicator(uint8_t value, uint8_t pos){
    if(mtdLmsToDecoder[pos] != value){
        mtdLmsToDecoder[pos] = value;
        emit newMtdIndicators(mtdLmsToDecoder);
    }
}

void zusi3Tcp::setSammelschine(){
    setMtdIndicator(0,8);
}
void zusi3Tcp::resetVehicleBlocking(){
    istVMaxErstesFahrzeug = true;
}
void zusi3Tcp::guesTractionType(){
    if(fahrlSpng > 0){
        tractionType = 1;
        setMtdIndicator(tractionType, 12);
    }
    if(fahrlSpng <= 0 && zugkraftProAchs > 0){
        tractionType = 2;
        setMtdIndicator(tractionType, 12);
    }
}

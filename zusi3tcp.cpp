#include "zusi3tcp.h"
#include <QDebug>

zusi3Tcp::zusi3Tcp()
{
    client = new QTcpSocket();
    myIndicators = new zusiIndicator();
    myPower = new zusiPower();
    mtdLmsToDecoder.resize(13);
    lzbValuesToDecoder.resize(7);
    connect(client,SIGNAL(readyRead()),this,SLOT(clientReadReady()));
    client->waitForReadyRead(3000);
}

qint8 zusi3Tcp::setIpadress(QString address){
    if(address.isEmpty()) return -1;
    QStringList slist = address.split(".");
    int s = slist.size();
    if(s>4){
        emit newTechnicalMessage(" IP hat zu viele Elemente", era::grey, era::darkBlue, 9);
        QTimer::singleShot(2500,this,SLOT(remooveTechMessage9()));
        return -1;
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
            return -1;
        }
    }
    if(s<4 || emptyGroup){
        emit newTechnicalMessage(" IP hat zu wenig Elemente", era::grey, era::darkBlue, 9);
        QTimer::singleShot(2500,this,SLOT(remooveTechMessage9()));
        return -1;
    }
    //qDebug() << address;
    QHostAddress zusiPc = QHostAddress(address);
    client->connectToHost(zusiPc,1436);
    constexpr static const unsigned char  Anmeldung2[] = { 0x00, 0x00, 0x00, 0x00, 0x1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1, 0x00, 0x04, 0x00, 0x00, 0x00, 0x1, 0x00, 0x2, 0x00, 0x04, 0x00, 0x00, 0x00, 0x2, 0x00, 0x2, 0x00, 0xA, 0x00,
                            0x00, 0x00, 0x3, 0x00, 0x46, 0x61, 0x68, 0x72, 0x70, 0x75, 0x6C, 0x74, 0x5, 0x00, 0x00, 0x00, 0x04, 0x00, 0x32, 0x2E, 0x30, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
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
        0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x09, 0x00,  // Zugkraft gesammt
        0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x0A, 0x00,  // Zugkraft pro Achse
        0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x0B, 0x00,  // Zugkraft-soll gesammt
        0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x0C, 0x00,  // Zugkraft-Soll pro Achse
        0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x7C, 0x00,  // Steuerwagen: Zugkraft gesammt
        0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x7D, 0x00,  // Steuerwagen: Zugkraft pro Achse
        0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x7E, 0x00,  // Steuerwagen: Zugkraft-soll gesammt
        0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x7F, 0x00,  // Steuerwagen: Zugkraft-Soll pro Achse
        0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x90, 0x00,  // Zug- und Brems-Gesamtkraftsoll normiert
        0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x91, 0x00,  // Steuerwagen: Zug- und Brems-Gesamtkraftsoll normiert
        0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x93, 0x00,  // Zug- und Brems-Gesamtkraftsoll absolut normiert
        0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x94, 0x00,  // Steuerwagen: Zug- und Brems-Gesamtkraftsoll absolut normiert
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
        0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x8e, 0x00,  // Status Zugverband
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF
        };
    client->write((const char*)Abfrage2,sizeof (Abfrage2));
    QTimer::singleShot(0,this,SLOT(checkClientConnection()));
    return 0;
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
    bool nodesChanged = false;
    int nodeCount = 0;
    int packetLength = 0;
    incommingData.append(client->readAll());
  for(int i=0; i <= incommingData.size()-3;i++){
      packetLength = readIntegerInRawAtPos(i);
      switch (packetLength){
      case 0:
          nodesChanged = true;
          nodeCount ++;
          i = i + 5;  // 3 für Knoten + 2 für folgende ID
          break;
      case -1:
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
    nodeId1 = nodeId2 = nodeId3 = nodeId4 = nodeId5 = atributeId = 0;
    for(int i=0; i <= compZusiPacket.size()-3;i++){
        packetLength = readIntegerAtPos(i);
        switch (packetLength){
        case 0: //Länge = 0 kennzeichnet den Knoten im Unterschied zum Attribut
            lastWasNewNode = true;
            layer ++;
            //qDebug() << "NodeID: " + QString::number(readIdAtPos(i+4));
            if(layer == 1) nodeId1 = readIdAtPos(i+4); //ID zur Codierung der Funktion des Knotens (Word)
            if(layer == 2) nodeId2 = readIdAtPos(i+4);
            if(layer == 3) nodeId3 = readIdAtPos(i+4);
            if(layer == 4) nodeId4 = readIdAtPos(i+4);
            if(layer == 5) nodeId5 = readIdAtPos(i+4);
            if(layer == 6) nodeId6 = readIdAtPos(i+4);
            i = i + 5;  // 3 für Knoten + 2 für folgende ID
            break;
        case -1: //Kennzeichnung des Knoten-Endes
            if(lastWasNewNode)zusiDecoder();
            lastWasNewNode=false;
            layer --;
            i = i + 3;
            break;
        default:
            atributeId = readIdAtPos(i+4); //ID zur Codierung der Funktion des Attributs (Word)
            if((packetLength - 2) == 4){ //Datenbytes, Interpretation je nach Funktion
                useData4Byte.chr[0] = compZusiPacket[i + 6];
                useData4Byte.chr[1] = compZusiPacket[i + 7];
                useData4Byte.chr[2] = compZusiPacket[i + 8];
                useData4Byte.chr[3] = compZusiPacket[i + 9];
            }
            if((packetLength - 2) == 2){ //Datenbytes, Interpretation je nach Funktion
                useData2Byte.chr[0] = compZusiPacket[i + 6];
                useData2Byte.chr[1] = compZusiPacket[i + 7];
            }
            if((packetLength - 2) == 1){ //Datenbytes, Interpretation je nach Funktion
                useData2Byte.chr[0] = compZusiPacket[i + 6];
                useData2Byte.chr[1] = 0;
            }
            useDataComplex = compZusiPacket.mid(i+6,packetLength-2);
            //qDebug() <<  "atributeId: " + QString::number(atributeId) + " useData: " + useDataComplex.toHex();
            zusiDecoder();
            i = i + packetLength + 3;
        }
    }
}

void zusi3Tcp::zusiDecoder(){
    // Verbindung
    if ((layer == 2) && (nodeId1 == 0x0001) && (nodeId2 == 0x0002)){
        switch(atributeId) {
            case 0x0001:
                //qDebug() << "Zusi-Version: " + QString(useDataComplex);
                break;
            case 0x0002:
                //qDebug() << "Zusi-Verbindungsinfo: " + QString(useDataComplex);
                break;
        }
    }
    if ((layer == 2) && (nodeId1 == 0x0001) && (nodeId2 == 0x0002) && (atributeId == 0x0003)){
      //qDebug() << "Der Client wurde akzeptiert " + QString::number(useData2Byte.chr[0]);
    }
    // MTD
    if ((layer == 2) && (nodeId1 == 0x0002) && (nodeId2 == 0x000A)                          ){    
        switch(atributeId) {
            case 0x0001:      // Geschwindigkeit Meter/Sekunde
                if (checkHysterise(&VIst, useData4Byte.Single)){
                    emit newSpeed(static_cast<qreal>(VIst));
                    myIndicators->setVIst(VIst);
                    myPower->setVIst(VIst);
                }
                break;
            case 0x0002: break;   // Druck Hauptluftleitung
            case 0x0003: break;   // Druck Bremszylinder
            case 0x0004: break;   // Druck Hauptluftbehälter
            case 0x0009: myPower->setZugkraft(useData4Byte.Single);  break;                     //              Zugkraft gesammt
            case 0x000A: myPower->setZugkraftProAchse(useData4Byte.Single);break;               //              Zugkraft pro Achse
            case 0x000B: myPower->setZugkraftSollGesammt(useData4Byte.Single); break;           //              Zugkraft-Soll gesammt
            case 0x000C: myPower->setZugkraftSollProAchse(useData4Byte.Single); break;          //              Zugkraft-Soll pro Achse
            case 0x007C: myPower->setZugkraftGesammtSteuerwagen(useData4Byte.Single); break;    // Steuerwagen: Zugkraft gesammt
            case 0x007D: myPower->setZugkraftProAchseSteuerwagen(useData4Byte.Single); break;   // Steuerwagen: Zugkraft pro Achse
            case 0x007E: myPower->setZugkraftSollGesammtSteuerwagen(useData4Byte.Single); break;// Steuerwagen: Zugkraft-soll gesammt
            case 0x007F: myPower->setZugkraftSollProAchseSteuerwagen(useData4Byte.Single); break;// Steuerwagen: Zugkraft-Soll gesamt pro Achse
            case 0x0090: myPower->setZugkraftSollNormiert(useData4Byte.Single); break;          //              Zug- und Brems-Gesamtkraft soll normiert
            case 0x0091: myPower->setZugkraftSollNormiertSteuerwagen(useData4Byte.Single); break;// Steuerwagen: Zug- und Brems-Gesamtkraft soll normiert
            case 0x0093: myPower->setZugkraftNormiert(useData4Byte.Single); break;              //              Zug- und Brems-Gesamtkraft absolut normiert
            case 0x0094: myPower->setZugkraftNormiertSteuerwagen(useData4Byte.Single); break;// Steuerwagen: Zug- und Brems-Gesamtkraft absolut normiert
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
                //qDebug() << "Hauptschalter: " + QString::number(useData4Byte.Single > 0);
                break;
              case 0x0015: break;  // Fahrstufe
              case 0x0017:   // AFB-Sollgeschwindigkeit
                if(checkHysterise(&VSoll, useData4Byte.Single))
                    myIndicators->setAfbSoll(VSoll);
                break;
              case 0x0020: break;  // Hohe Abbrems
              case 0x001b:   // LM Schleudern
                myIndicators->setLmSchleudern(useData4Byte.Single > 0);
                break;
              case 0x001c:   // LM Gleiten
                myIndicators->setLmGleiten(useData4Byte.Single > 0);
                break;
              case 0x0036:   // AFB-An
                 myIndicators->setAfbAn(useData4Byte.Single > 0);
                 break;
              case 0x0055:   //             Stromabnehmer
              case 0x0088:   // Steuerwagen Stromabnehmer
                 //qDebug() << "Stromabnehmer A: " + QString::number(static_cast<quint8>(useData4Byte.Single));
                 setMtdIndicator(static_cast<quint8>(useData4Byte.Single) ,6);
                 break;
       }
    }
    // Notbremse
    if ((layer == 3) && (nodeId1 == 0x0002) && (nodeId2 == 0x000A) && (nodeId3 == 0x0022)){
        // Status Notbremssystem
    }
    // Sifa
    if ((layer == 3) && (nodeId1 == 0x0002) && (nodeId2 == 0x000A) && (nodeId3 == 0x0064)){
        switch(atributeId) {
            case 0x0001:      // Bauart Sifasystem als Text
                //qDebug() << "Sifasystem: " + QString(useDataComplex);
                break;
            case 0x0002:   // Status Sifa-Leuchtmelder
                myIndicators->setLmSifa(useData2Byte.byte[0]);
                setMtdIndicator(useData2Byte.byte[0],1);
                break;
            case 0x0003:   // Status Sifa-Hupe
                myIndicators->setSifaHupe(useData2Byte.byte[0]);
                setMtdIndicator(useData2Byte.byte[0],2);
                break;
            case 0x0004:   // Sifa-Hauptschalter
                myIndicators->setSifaHauptschalter(useData2Byte.byte[0]);
                break;
            case 0x0005:   // Sifa-Störschalter
                //qDebug() << "Sifa-SS:    " + QString::number(useData2Byte.byte[0]);
                myIndicators->setSifaStoerschalter(useData2Byte.byte[0]);
                setMtdIndicator(useData2Byte.byte[0],0);
                break;
            case 0x0006:   // Sifa-Luftabsperrhahn
                //qDebug() << "Sifa-LH:    " + QString::number(useData2Byte.byte[0]);
                myIndicators->setSifaStoerschalter(useData2Byte.byte[0]);
                break;
        }
    }
    // Bauart Zugbeeinflussungssystem
    if ((layer == 3) && (nodeId1 == 0x0002) && (nodeId2 == 0x000A) && (nodeId3 == 0x0065)){
        switch(atributeId) {
            case 0x0001:      // Bauart Zugbeeinflussungssystem als Text
                //qDebug() << "Zugbeeinflussungssystem: " + QString(useDataComplex);
                break;
        }
    }
    // PZB Basisinfo
    if ((layer == 4) && (nodeId1 == 0x0002) && (nodeId2 == 0x000A) && (nodeId3 == 0x0065) && (nodeId4 == 0x0002)){
        switch(atributeId) {
            case 0x0001:      // Zugart
                myIndicators->setZugart(useData2Byte.byte[0]);
                break;
            case 0x0002:      // Tf-Nummer
                //qDebug() << "Tf-Nummer: " + QString(useDataComplex);
                break;
            case 0x0003:      // Zugnummer
                //qDebug() << "Zugnummer: " + QString(useDataComplex);
                break;
            case 0x0007:   // Hauptschalter
                //qDebug() << "Indusi-HS: " + QString::number(useData2Byte.byte[0]);
                break;
            case 0x0008:   // Indusi Störschalter
                //qDebug() << "Indusi-SS: " + QString::number(useData2Byte.byte[0]);
                break;
            case 0x0009:   // LZB Störschalter
                //qDebug() << "LZB SS   : " + QString::number(useData2Byte.byte[0]);
                break;
            case 0x000A:   // Indusi-Luftabsperrhahn
                //qDebug() << "Indusi-LH: " + QString::number(useData2Byte.byte[0]);
                break;
            case 0x000B:   // Klartextmeldungen
                myIndicators->setKlartextmeldungen(useData2Byte.byte[0]);
                break;
        }
    }
    // PZB
    if ((layer == 4) && (nodeId1 == 0x0002) && (nodeId2 == 0x000A) && (nodeId3 == 0x0065) && (nodeId4 == 0x0003)){
        switch(atributeId) {
            case 0x0002:      // Zustand Zugbeeinflussung
                myIndicators->setZustandZugsicherung(useData2Byte.Word);
                break;
            case 0x0003:   // Zwangsbremsung aktiv wegen
                myIndicators->setGrundZwangsbrmnsung(useData2Byte.Word);
                break;
            case 0x0004:   // Grund der Zwangsbremsung als Text
                //qDebug() << "ZB wegen Tx: " + QString(useDataComplex);
                break;
            case 0x0005:  // Melder 1000 Hz
                myIndicators->setLm1000Hz(useData2Byte.byte[0]);
                break;
            case 0x0006:  // Melder U
                myIndicators->setLmZugartU(useData2Byte.byte[0]);
                break;
            case 0x0007:  // Melder M
                myIndicators->setLmZugartM(useData2Byte.byte[0]);
                break;
            case 0x0008:  // Melder O
                myIndicators->setLmZugartO(useData2Byte.byte[0]);
                break;
            case 0x000A:  // Melder 500 Hz
                myIndicators->setLm500Hz(useData2Byte.byte[0]);
                break;
            case 0x000B:  // Melder Befehl
                myIndicators->setLmBefehl(useData2Byte.byte[0]);
                break;
            case 0x000C:  // Zusatzinfo Melderbild
                myIndicators->setMelderbild(useData2Byte.byte[0]);
                break;
            case 0x000D:  // LZB-Zustand    // Möglicherweise geht das nicht mit "(bool)", sondern ich muss >0 abfragen
                myIndicators->setLzbZustand(useData2Byte.byte[0]);
                break;
            case 0x0017:  // Melder H an
                myIndicators->setLmH(useData2Byte.byte[0]);
                break;
            case 0x0018:  // Melder E40 an
                myIndicators->setLmE40(useData2Byte.byte[0]);
                break;
            case 0x0019:  // Melder Ende an
                myIndicators->setLmEnde(useData2Byte.byte[0]);
                break;
            case 0x001A:  // Melder B an
                myIndicators->setLmB(useData2Byte.byte[0]);
                break;
            case 0x001B:  // Melder Ü an
                myIndicators->setLmUe(useData2Byte.byte[0]);
                break;
            case 0x0024:  // Melder G   // Ersetzt 0x001C
                myIndicators->setLmG(useData2Byte.byte[0]);
                break;
            case 0x001D:  // Melder EL an
                myIndicators->setLmEl(useData2Byte.byte[0]);
                break;
            case 0x001E:  // Melder V40 an
                myIndicators->setLmV40(useData2Byte.byte[0]);
                break;
            case 0x001F:  // Melder S an
                myIndicators->setLmS(useData2Byte.byte[0]);
                break;
            case 0x0025:  // Melder Prüf/Stör // Ersetzt 0x0020
                myIndicators->setLmPruefStoer(useData2Byte.byte[0]);
                break;
            case 0x0021:  // Sollgeschwindigkeit in m/s
                myIndicators->setVSoll(useData4Byte.Single * float(3.6));
                setLzbValue(useData4Byte.Single, 3);
                break;
            case 0x0022:  // Zielgeschwindigkeit in m/s (Wert<0 → dunkel)
                  myIndicators->setVZiel(useData4Byte.Single * float(3.6));
                  setLzbValue(useData4Byte.Single, 1);
                break;
            case 0x0023:  // Zielweg in m (Wert<0 → dunkel)
               //if(useData4Byte.Single <0 ) useData4Byte.Single = 0;
               myIndicators->setZielweg(useData4Byte.Single);
               setLzbValue(useData4Byte.Single, 5);
               break;
            case 0x0026:  // CIR-ELKE-Modus
                myIndicators->setCirEelkeModus(useData2Byte.byte[0]);
                break;
            case 0x0027:  // ZDE Anzeigen
                myIndicators->setMfaAnzeigemodusZugdaten(useData2Byte.byte[0]);
                break;
       }
    }
    if ((layer == 5) && (nodeId1 == 0x0002) && (nodeId2 == 0x000A) && (nodeId3 == 0x0065) && (nodeId4 == 0x0003) && (nodeId5 == 0x000E) && (atributeId == 0x0001)){
        myIndicators->setEndeverfahren(useData2Byte.byte[0]);
    }
    if ((layer == 5) && (nodeId1 == 0x0002) && (nodeId2 == 0x000A) && (nodeId3 == 0x0065) && (nodeId4 == 0x0003) && (nodeId5 == 0x000F)){
        myIndicators->setErsatzauftrag(1);
    }
    if ((layer == 5) && (nodeId1 == 0x0002) && (nodeId2 == 0x000A) && (nodeId3 == 0x0065) && (nodeId4 == 0x0003) && (nodeId5 == 0x0010) && (atributeId == 0x0001)){
        myIndicators->setFalschfahrauftrag(useData2Byte.byte[0]);
    }
    if ((layer == 5) && (nodeId1 == 0x0002) && (nodeId2 == 0x000A) && (nodeId3 == 0x0065) && (nodeId4 == 0x0003) && (nodeId5 == 0x0011) && (atributeId == 0x0001)){
        myIndicators->setVorsichtsauftrag(useData2Byte.byte[0]);
    }
    if ((layer == 5) && (nodeId1 == 0x0002) && (nodeId2 == 0x000A) && (nodeId3 == 0x0065) && (nodeId4 == 0x0003) && (nodeId5 == 0x0012) && (atributeId == 0x0002)){
        myIndicators->setFahrtUeberLlzbHaltPerBefehl(useData2Byte.Word);
    }
    if ((layer == 5) && (nodeId1 == 0x0002) && (nodeId2 == 0x000A) && (nodeId3 == 0x0065) && (nodeId4 == 0x0003) && (nodeId5 == 0x0013) && (atributeId == 0x0002)){
        myIndicators->setUebertragungsausfall(useData2Byte.Word);
    }
    if ((layer == 5) && (nodeId1 == 0x0002) && (nodeId2 == 0x000A) && (nodeId3 == 0x0065) && (nodeId4 == 0x0003) && (nodeId5 == 0x0016) && (atributeId == 0x0001)){
        myIndicators->setLzbElAuftrag(useData2Byte.byte[0]);
    }
/*  if ((layer == 5) && (nodeId1 == 0x0002) && (nodeId2 == 0x000A) && (nodeId3 == 0x0065) && (nodeId4 == 0x0003) && (nodeId5 == 0x0025) && (nodeId6 == 0x0002)){
          qDebug() << "Anzeige der Führungsgrößen: " + QString::number(useData2Byte.Word);
  }*/
    // Zugdaten
    if ((layer == 5) && (nodeId1 == 0x0002) && (nodeId2 == 0x000A) && (nodeId3 == 0x0065) && (nodeId4 == 0x0002) && (nodeId5 == 0x0004)){
        switch(atributeId) {
            case 0x0001:   // BRH-Wert (Bremshundertstel). Werte der Ersatzzugdaten
                //qDebug() << "BRH (Ers.): " + QString::number(useData2Byte.Word);
                break;
            case 0x0002:   // BRA-Wert (Bremsart). Werte der Ersatzzugdaten
                //qDebug() << "BRA (Ers.): " + QString::number(useData2Byte.Word);
                break;
            case 0x0003:   // ZL-Wert (Zuglänge) in m
                //qDebug() << "ZL:         " + QString::number(useData2Byte.Word);
                break;
            case 0x0004:   // VMZ-Wert (Höchstgeschwindigkeit)in km/h
                //qDebug() << "VMZ:        " + QString::number(useData2Byte.Word);
                break;
            case 0x0005:   // Zugehörige Zugart. Werte der Ersatzzugdaten
                //qDebug() << "ZA (Ers.):  " + QString::number(useData2Byte.byte[0]);
                break;
        }
    }
    if ((layer == 5) && (nodeId1 == 0x0002) && (nodeId2 == 0x000A) && (nodeId3 == 0x0065) && (nodeId4 == 0x0002) && (nodeId5 == 0x0005)){
        switch(atributeId) {
            case 0x0001:   // BRH-Wert (Bremshundertstel). Werte der Ersatzzugdaten
                //qDebug() << "BRH (Ers.): " + QString::number(useData2Byte.Word);
                break;
            case 0x0002:   // BRA-Wert (Bremsart). Werte der Ersatzzugdaten
                //qDebug() << "BRA (Ers.): " + QString::number(useData2Byte.Word);
                break;
            case 0x0005:   // Zugehörige Zugart. Werte der Ersatzzugdaten
                //qDebug() << "ZA (Ers.):  " + QString::number(useData2Byte.byte[0]);
                break;
        }
    }
    if ((layer == 5) && (nodeId1 == 0x0002) && (nodeId2 == 0x000A) && (nodeId3 == 0x0065) && (nodeId4 == 0x0002) && (nodeId5 == 0x0006)){
        switch(atributeId) {
        case 0x0001:   // BRH-Wert (Bremshundertstel). Werte der Ersatzzugdaten
            //qDebug() << "BRH:         " + QString::number(useData2Byte.Word);
            break;
        case 0x0002:   // BRA-Wert (Bremsart). Werte der Ersatzzugdaten
            //qDebug() << "BRA:         " + QString::number(useData2Byte.Word);
            break;
        case 0x0005:   // Zugehörige Zugart. Werte der Ersatzzugdaten
            //qDebug() << "ZA:         " + QString::number(useData2Byte.byte[0]);
            break;
        case 0x0006:   // Modus (Ersatzzugdaten / Normalbetrieb)
            //qDebug() << "Ers./Norm.: " + QString::number(useData2Byte.byte[0]);
            break;
    }
     }
    // Türen
    if ((layer == 3) && (nodeId1 == 0x0002) && (nodeId2 == 0x000A) && (nodeId3 == 0x0066)){
        switch(atributeId) {
            case 0x0001:      // Bezeichnung des Türsystems als Text
            //qDebug() << "System:    " + QString(useDataComplex);
            tuerSystem = QString(useDataComplex);
            if (tuerSystem.toStdString().find("SAT") != std::string::npos) setMtdIndicator(2,9);
            if (tuerSystem.toStdString().find("TB0") != std::string::npos) setMtdIndicator(3,9);
            if (tuerSystem.toStdString().find("TAV") != std::string::npos) setMtdIndicator(4,9);
            if (tuerSystem.toStdString().find("SST") != std::string::npos) setMtdIndicator(5,9);
                break;
            case 0x0002:      // Status linke Seite
                //qDebug() << "Stat. Li.: " + QString::number(useData2Byte.byte[0]);
                setMtdIndicator(useData2Byte.byte[0],10);
                break;
            case 0x0003:      // Status rechte Seite
                //qDebug() << "Stat. Re.: " + QString::number(useData2Byte.byte[0]);
                setMtdIndicator(useData2Byte.byte[0],11);
                break;
            case 0x0004:break;   // Traktionssperre aktiv
            case 0x0005:break;   // Freigabestatus (Seitenwahlschalter)
            case 0x0006:break;   // Melder links an
            case 0x0007:break;   // Melder rechts an
            case 0x0008:break;   // Status Melder links
            case 0x0009:break;   // Status Melder rechts
            case 0x000A:break;   // Melder „(Zwangs)schließen“ an
            case 0x000B:break;   // Status Melder „(Zwangs)schließen“
            case 0x000C:break;   // Melder „Türen links+rechts“ an
            case 0x000D:break;   // Status Melder „Türen links+rechts“
            case 0x000E:break;   // „Zentrales Öffnen links“ an
            case 0x000F:break;   // „Zentrales Öffnen rechts an
            case 0x0010:break;   // Status Melder „Zentrales Öffnen links“
            case 0x0011:break;   // Status Melder „Zentrales Öffnen rechts“
            case 0x0012:break;   // Melder „Grünschleife“ an
                setMtdIndicator(useData2Byte.byte[0],3);
                break;
        }
    }
    // Status Zugverband
    if ((layer == 4) && (nodeId1 == 0x0002) && (nodeId2 == 0x000A) && (nodeId3 == 0x008E) && (nodeId4 == 0x0001) && (atributeId == 0x0005)){
        //qDebug() << "setFzgVMax    " + QString::number(useData4Byte.Single);
        if(checkHysterise(&VMFzg, useData4Byte.Single) && istVMaxErstesFahrzeug){
            myIndicators->setFzgVMax(VMFzg);
            istVMaxErstesFahrzeug = false;
            QTimer::singleShot(2000, this, SLOT(resetVehicleBlocking()));
        }
    }
    // Baureihe
    if ((layer == 4) && (nodeId1 == 0x0002) && (nodeId2 == 0x000A) && (nodeId3 == 0x008E) && (nodeId4 == 0x0001) && (atributeId == 0x0006)){
        //qDebug() << "Fahrzeug: " + QString(useDataComplex);
        myPower->setBaureihe(QString(useDataComplex));
    }
    // Reisezug oder Güterzug
    if ((layer == 3) && (nodeId1 == 0x0002) && (nodeId2 == 0x000A) && (nodeId3 == 0x008E) && (atributeId == 0x0002)){
        istReisezug = useData2Byte.byte[0];
        //qDebug() << "istReisezug: " + QString::number(istReisezug);
        if(!istReisezug)setMtdIndicator(1,9);
        setMtdIndicator(!hauptschalter ,7);
        if(istReisezug)QTimer::singleShot(5000, this, SLOT(setSammelschine()));
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

void zusi3Tcp::setLzbValue(float input, uint8_t pos){
    union { //Pos1/3 VZiel/Soll , //Pos5Entfernung
      uint8_t byte[2];
      uint16_t word;
    } tmp;
    if(pos == 5){
        tmp.word = qFabs(ceil(input - 0.5));
        lzbValuesToDecoder[0] = input > 0;  // Flag setzten, ob Führungsgrößen angezeigt werden
    }
    else{
        tmp.word = qFabs(ceil(input * 3.6 - 0.5));
    }//qDebug() << "setLzbValue(input " + QString::number(tmp.word) + ", pos " + QString::number(pos) + ")";
    if(tmp.byte[0] != lzbValuesToDecoder[pos] || tmp.byte[1] != lzbValuesToDecoder[pos + 1]){
        lzbValuesToDecoder[pos    ] = tmp.byte[0];
        lzbValuesToDecoder[pos + 1] = tmp.byte[1];
        emit newLzbValues(lzbValuesToDecoder);
    }
}
//emit newLzbValues(lzbValuesToDecoder);
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

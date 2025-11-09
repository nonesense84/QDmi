#include "zusiindicator.h"

zusiIndicator::zusiIndicator(QObject *parent) : QObject(parent){
}

quint8 zusiIndicator::checkPlzbType(QString value){
    if(value.contains("ETCS"))return plzbDevice;
    bool haseLzb = value.contains("LZB") || value.contains("EBICAB 2000");
    bool indusiDevice = value.contains("Indusi");
    if(haseLzb){
        plzbDevice = 7;
    }
    else if(indusiDevice){
        plzbDevice = 1;
    }
    else{
        plzbDevice = 5;
    }
    return plzbDevice;
    /*  Possible values from SEP Spec:
     *  1. I60
     *  2. I60 mit ER2
     *  3. I60 mit ER24 und PZB90
     *  4. I60R
     *  5. I60R mit PZB90
     *  6. I80
     *  7. I80 mit PZB90
     *  8. PZ80
     *  9. PZ80R
     *  10. PZ80R mit PZB90
     */
}

void zusiIndicator::setLmHauptschalter(uint8_t value){
    //qDebug() << "HS   " + QString::number(value);
    lmHauptschalter = value;
}
void zusiIndicator::setStatusStromabnehmer(uint8_t value){
    //qDebug() << "Stromabnehmer " + QString::number(value);
    stromabn1Oben =    value & 0x01;
    stromabn2Oben =   (value & 0x02) >> 1;
    stromabn3Oben =   (value & 0x04) >> 2;
    stromabn4Oben =   (value & 0x08) >> 3;
    stromabn1Hebend = (value & 0x10) >> 4;
    stromabn2Hebend = (value & 0x20) >> 5;
    stromabn3Hebend = (value & 0x40) >> 6;
    stromabn4Hebend = (value & 0x80) >> 7;
}
void zusiIndicator::setLmSchleudern(uint8_t value){lmSchleudern = value;}
void zusiIndicator::setLmGleiten(uint8_t value){lmGleiten = value;}
void zusiIndicator::setLmUhrzeitDigital(uint8_t value){(void)value;}
void zusiIndicator::setAfbSoll(uint16_t value){
    afbSoll = value;
    emit newAfbSoll(afbSoll, afbAn);
}
void zusiIndicator::setAfbAn(bool value){
    afbAn = value;
    emit newAfbSoll(afbSoll, afbAn);
}
void zusiIndicator::setFzgVMax(uint16_t value){
    emit newFzgVmaxTacho(value + 20);
}
void zusiIndicator::clearData(){
    stromabn2Oben = 0; stromabn3Oben = 0; stromabn4Oben = 0; stromabn1Hebend = 0;
    stromabn2Hebend = 0;stromabn3Hebend = 0;stromabn4Hebend = 0;
    SifaStoerschalter = 0; SifaLuftabsperrhahn = 0;
    afbSoll = 0; vIst = 0;
    setAfbSoll(0);
    setAfbAn(0);
}

void zusiIndicator::setDefaults(){
    stromabn2Oben = 0; stromabn3Oben = 0; stromabn4Oben = 0; stromabn1Hebend = 0;
    stromabn2Hebend = 0;stromabn3Hebend = 0;stromabn4Hebend = 0;
    stromabn1Oben = 0;
    lmHauptschalter = 0; lmGetriebe = 0; lmSchleudern = 0; lmGleiten = 0;
    lmUhrzeitDigital = 0; StwgHauptschalter = 0; lmSifa = 0; SifaHupe = 0;
    SifaStoerschalter = 0; SifaLuftabsperrhahn = 0;
    afbSoll = 0; vIst = 0;
    afbAn = false;
}
/*void zusiIndicator::makeLzbAnalogDatagram(){
    QVector<quint8> lzbValuesToDecoder(7,0);
//#                    S V       V       E
//#                    h Z       P       n
//telegramLzbLm  = "\x01\xA0\x00\x6A\x00\xFF\x01"
    lzbValuesToDecoder[6] = LzbZustand > 0;
}*/

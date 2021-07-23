#include "alphanumericinput.h"
#include <QDebug>
#include <QIntValidator>

alphaNumericInput::alphaNumericInput()
{
    timerforkey = new QTimer(this);
    timerforkey->setInterval(1000);
    connect(timerforkey,SIGNAL(timeout()),this,SLOT(setTimeout()));
}
QString alphaNumericInput::getText(){
    if(textHidden)return "●●●●●●●●●●●●";
    return enteredText + cursor;
}
void alphaNumericInput::setTimeout(){
    timerforkey->stop();
    counter = 0;
    lastKeyCode = "";
    timeout = true;
}

void alphaNumericInput::deleteLastChar(){
    enteredText.chop(1);
    textHidden = false;
}

void alphaNumericInput::clearText(){
    enteredText = "";
}

void alphaNumericInput::hideText(bool hide){
    textHidden = hide;
}

void alphaNumericInput::togleCursor(){
    if((cursor == "_") || !timeout){
        cursor = "";
    }
    else{
        cursor = "_";
    }
}

void alphaNumericInput::adItem(QString keyCode){
    QIntValidator v(0, 9, this);
    int pos = 0;
    bool valid = v.validate(keyCode, pos);
    if(!valid) return;
    if(enteredText.length() > 15) return;
    textHidden = false;
    counter = counter + 1;
    QString item = "";
    if (keyCode != lastKeyCode){ timeout = true;}
    if (keyCode == "0") { // Taste 0
        timerforkey->start();
        if (keyCode != lastKeyCode) counter = 1;
        if (counter == 1) item = "0";
        if (counter == 2){item = " "; counter = 0;}
        lastKeyCode = keyCode;
        timeout = true;
    }
    if (keyCode == "1") { // Taste 1
        timerforkey->start();
        counter = 0;
        item = "1";
        lastKeyCode = keyCode;
        timeout = true;
    }
    if (keyCode == "2") { // Taste 2
        timerforkey->start();
        if (keyCode != lastKeyCode){ counter = 1;}
        if (counter == 1) item = "2";
        if (counter == 2) item = "a";
        if (counter == 3) item = "b";
        if (counter == 4) item = "c";
        if (counter == 5) item = "ä";
        if (counter == 6) item = "A";
        if (counter == 7) item = "B";
        if (counter == 8) item = "C";
        if (counter == 9){item = "Ä";counter = 0;}
        lastKeyCode = keyCode;
    }
    if (keyCode == "3") { // Taste 3
        timerforkey->start();
        if (keyCode != lastKeyCode){ counter = 1;}
        if (counter == 1) item = "3";
        if (counter == 2) item = "d";
        if (counter == 3) item = "e";
        if (counter == 4) item = "f";
        if (counter == 5) item = "D";
        if (counter == 6) item = "E";
        if (counter == 7){item = "F"; counter = 0;}
        lastKeyCode = keyCode;
    }
    if (keyCode == "4") { // Taste 4
        timerforkey->start();
        if (keyCode != lastKeyCode) counter = 1;
        if (counter == 1) item = "4";
        if (counter == 2) item = "g";
        if (counter == 3) item = "h";
        if (counter == 4) item = "i";
        if (counter == 5) item = "G";
        if (counter == 6) item = "H";
        if (counter == 7){item = "I"; counter = 0;}
        lastKeyCode = keyCode;
    }
    if (keyCode == "5") { // Taste 5
        timerforkey->start();
        if (keyCode != lastKeyCode) counter = 1;
        if (counter == 1) item = "5";
        if (counter == 2) item = "j";
        if (counter == 3) item = "k";
        if (counter == 4) item = "l";
        if (counter == 5) item = "J";
        if (counter == 6) item = "K";
        if (counter == 7){item = "L"; counter = 0;}
        lastKeyCode = keyCode;
    }
    if (keyCode == "6") { // Taste 6
        timerforkey->start();
        if (keyCode != lastKeyCode) counter = 1;
        if (counter == 1) item = "6";
        if (counter == 2) item = "m";
        if (counter == 3) item = "n";
        if (counter == 4) item = "o";
        if (counter == 5) item = "ö";
        if (counter == 6) item = "M";
        if (counter == 7) item = "N";
        if (counter == 8) item = "O";
        if (counter == 9){item = "Ö"; counter = 0;}
        lastKeyCode = keyCode;
    }
    if (keyCode == "7") { // Taste 7
        timerforkey->start();
        if (keyCode != lastKeyCode) counter = 1;
        if (counter == 1) item = "7";
        if (counter == 2) item = "p";
        if (counter == 3) item = "q";
        if (counter == 4) item = "r";
        if (counter == 5) item = "s";
        if (counter == 6) item = "P";
        if (counter == 7) item = "Q";
        if (counter == 8) item = "R";
        if (counter == 9){item = "S";counter = 0;}
        lastKeyCode = keyCode;
    }
    if (keyCode == "8") { // Taste 8
        timerforkey->start();
        if (keyCode != lastKeyCode) counter = 1;
        if (counter == 1) item = "8";
        if (counter == 2) item = "t";
        if (counter == 3) item = "u";
        if (counter == 4) item = "v";
        if (counter == 5) item = "ü";
        if (counter == 6) item = "T";
        if (counter == 7) item = "U";
        if (counter == 8) item = "V";
        if (counter == 9){item = "Ü"; counter = 0;}
        lastKeyCode = keyCode;
    }
    if (keyCode == "9") { // Taste 9
        timerforkey->start();
        if (keyCode != lastKeyCode) counter = 1;
        if (counter == 1) item = "9";
        if (counter == 2) item = "w";
        if (counter == 3) item = "x";
        if (counter == 4) item = "y";
        if (counter == 5) item = "z";
        if (counter == 6) item = "W";
        if (counter == 7) item = "X";
        if (counter == 8) item = "Y";
        if (counter == 9){item = "Z"; counter = 0;}
        lastKeyCode = keyCode;
    }
    if(!timeout){
        enteredText.chop(1);
    }
    enteredText = enteredText + item;
    timeout = false;
}

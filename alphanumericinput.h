#ifndef ALPHANUMERICINPUT_H
#define ALPHANUMERICINPUT_H

#include <QObject>
#include <QTimer>

class alphaNumericInput : public QObject
{
    Q_OBJECT
public:
    alphaNumericInput();
private:
    QTimer *timerforkey;
    quint8 counter = 0;
    QString enteredText;
    QString lastKeyCode;
    QString cursor = "";
    bool timeout = true;
    bool textHidden = true;

private slots:
    void setTimeout();

public slots:
    void adItem(QString keyCode);
    void deleteLastChar();
    void clearText();
    void hideText(bool hide);
    void togleCursor();
    QString getText();
};

#endif // ALPHANUMERICINPUT_H

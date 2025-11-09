#include "mainwindow.h"
#include <QApplication>
/*
#include <QDebug>
#include <QMessageLogContext>
#include <QCoreApplication>

void myMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if (msg.contains("QAbstractSocket::connectToHost()")) {
        qDebug() << "Debug-Breakpoint erreicht";
        volatile int dummy = 1;
        while (dummy); // Hier im Debugger: dummy auf 0 setzen → geht weiter
    }

    // Standardausgabe beibehalten
    QByteArray localMsg = msg.toLocal8Bit();
    fprintf(stderr, "%s (%s:%u, %s)\n", localMsg.constData(),
            context.file, context.line, context.function);
}
*/
int main(int argc, char *argv[])
{
  //  qInstallMessageHandler(myMessageHandler);  // <--- hier aktivieren
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}





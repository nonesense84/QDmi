#ifndef ETCSTEXTWIDGET_H
#define ETCSTEXTWIDGET_H

#include <QWidget>
#include <QList>
#include <QPainter>
#include "era.h"
#include <QApplication>

class EtcsTextWidget : public QWidget {
    Q_OBJECT
public:
    explicit EtcsTextWidget(QWidget *parent = nullptr);
    void generateTestMessages();
public slots:
    void scrollUp();
    void scrollDown();
    void updateBlinking();
    void setEtcsTextMessages(const QList<EtcsTextMessage>& messages);

signals:
    void enableScrollUp(bool enabled);
    void enableScrollDown(bool enabled);
    void sendAckEtcsTextMessage(quint32 ID);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QList<EtcsTextMessage> messageList;
    int scrollIndex = 0;
    int borderThickness = 1;
    bool blinking = false;
    void paintFrame(QPainter *framePainter, QColor bright, QColor dark,  int bt);
    int calculateTextHeight() const;
};

#endif // ETCSTEXTWIDGET_H

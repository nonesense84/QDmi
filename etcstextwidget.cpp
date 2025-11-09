#include "etcstextwidget.h"
#include <QPaintEvent>
#include <QFontMetrics>
#include <QMouseEvent>

EtcsTextWidget::EtcsTextWidget(QWidget *parent) : QWidget(parent), borderThickness(2) {
    // We generate some test messages for initial display
    //generateTestMessages();
}

void EtcsTextWidget::generateTestMessages() {
    // We clear the message list and add some sample messages
    messageList.clear();
    messageList.append({1, QDateTime::currentDateTime(), 0, "INFO", "Short test message", "", 0});
    messageList.append({2, QDateTime::currentDateTime(), 0, "WARN", "A slightly longer message that should wrap across multiple lines to test word wrapping.", "", 0});
    messageList.append({3, QDateTime::currentDateTime(), 0, "ERROR", "This is a very long test message that will definitely wrap over multiple lines to check vertical alignment.", "", 0});
    messageList.append({4, QDateTime::currentDateTime(), 0, "ALERT", "ACK message", "", 1});
    update();
}

void EtcsTextWidget::setEtcsTextMessages(const QList<EtcsTextMessage>& messages) {
    // We update the message list and reset scrolling
    messageList = messages;
    scrollIndex = 0;
    update();
}

void EtcsTextWidget::scrollUp() {
    // We scroll up if we are not already at the top
    if (scrollIndex > 0) {
        scrollIndex--;
        update();
        emit enableScrollDown(true);
    }
    if (scrollIndex == 0) {
        emit enableScrollUp(false);
    }
}

void EtcsTextWidget::scrollDown() {
    // We scroll down if there are more messages below
    if (scrollIndex < messageList.size() - 1) {
        scrollIndex++;
        update();
        emit enableScrollUp(true);
    }
    if (scrollIndex >= messageList.size() - 1) {
        emit enableScrollDown(false);
    }
}
int EtcsTextWidget::calculateTextHeight() const {
    // We calculate the proportional text height based on widget height
    return static_cast<int>(width()/13.09);
}

void EtcsTextWidget::paintEvent(QPaintEvent *event) {
    int borderThickness = qApp->property("borderThickness").toInt();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    // We fill the background with the predefined dark blue color
    painter.fillRect(rect(), era::darkBlue);

    int lineHeight = calculateTextHeight();
    int timeFontSize = static_cast<int>(lineHeight * 0.6); // We make the time font smaller
    int textFontSize = static_cast<int>(lineHeight * 0.8); // We make the message text slightly bigger

    QFont timeFont("FreeSans");
    timeFont.setPixelSize(timeFontSize);

    QFont textFont("FreeSans");
    textFont.setPixelSize(textFontSize);

    painter.setPen(era::white);

    QList<EtcsTextMessage> displayMessages;
    bool ackFound = false;

    for (int i = 0; i < messageList.size(); ++i) {
        const EtcsTextMessage& msg = messageList.at(i);
        if (msg.ackNeeded) {
            displayMessages.append(msg);
            ackFound = true;
            break;
        }
    }

    if (ackFound) {
        emit enableScrollUp(false);
        emit enableScrollDown(false);
    } else {
        displayMessages = messageList;
        emit enableScrollUp(scrollIndex > 0);
        emit enableScrollDown(scrollIndex < messageList.size() - 1);
    }
    int y = 0;

    for (int i = scrollIndex; i < displayMessages.size() && y < height(); ++i) {
        const EtcsTextMessage& msg = displayMessages[i];
        QString timeString = msg.timeStamp.toString("hh:mm");

        int timeWidth = static_cast<int>(width() * 0.15);
        int textWidth = static_cast<int>(width() - timeWidth - 10); // We allocate more space for text
        int xOffset = timeWidth + 5;

        painter.setFont(textFont);
        QFontMetrics textMetrics(textFont);
        QRect textBoundingRect = textMetrics.boundingRect(0, 0, textWidth, 0, Qt::TextWordWrap, msg.msgText);
        int textHeight = textBoundingRect.height();

        // We adjust the time position to align it better with the first text line
        int timeY = y + (textMetrics.ascent() / 3);
        QRect timeRect(3, timeY, timeWidth, timeFontSize);
        QRect textRect(xOffset, y, textWidth, textHeight);

        // We draw the time and text messages
        painter.setFont(timeFont);
        painter.drawText(timeRect, Qt::AlignCenter, timeString);

        painter.setFont(textFont);
        painter.drawText(textRect, Qt::TextWordWrap | Qt::AlignVCenter, msg.msgText);

        y += textHeight;
    }
    if (blinking && !displayMessages.isEmpty() && displayMessages.first().ackNeeded) {// We draw the blinking border if required
        paintFrame(&painter, era::yellow, era::veryDarkYellow, borderThickness * 2);
    }
    else{ // Otherwise the regular frame.
        paintFrame(&painter, era::shadow, era::black, borderThickness);
    }
    (void)event; // To stop compiler warnings be because of unused stuff.
}


void EtcsTextWidget::updateBlinking() {
    // We toggle the blinking state and refresh the display
    blinking = !blinking;
    update();
}

void EtcsTextWidget::mouseReleaseEvent(QMouseEvent *event) {
    // We emit a click signal when the widget is clicked
    if(!messageList.isEmpty())
        emit sendAckEtcsTextMessage(messageList.at(scrollIndex).msgID);
    (void)event;
}
void EtcsTextWidget::paintFrame(QPainter *framePainter, QColor bright, QColor dark,  int bt){
    // We draw the 3D sunken border
    int w = width();
    int h = height();
    QPoint brightBorder[6] = {
        QPoint(0, h),                           // Bottom left corner
        QPoint(w, h),                           // Bottom right corner
        QPoint(w, 0),                           // Top right corner
        QPoint(w - bt, bt),                     // Shifted top right corner
        QPoint(w - bt, h - bt),                 //Shifted bottom right corner
        QPoint(bt, h - bt)                      // Shifted bottom left corner
    };
    framePainter->setPen(Qt::NoPen);            // Set no outlin
    framePainter->setBrush(bright);             // Apply the shadow color
    framePainter->drawPolygon(brightBorder, 6); // Draw the original polygon
    QPoint brightBorderMirrored[6];             // Create a mirrored version of the polygon
    for (int i = 0; i < 6; i++) {               // (diagonal reflection)
        brightBorderMirrored[i] = QPoint(w - brightBorder[i].x(), h - brightBorder[i].y());
    }
    framePainter->setBrush(dark);               // Set the brush to black for the mirrored polygon
    framePainter->drawPolygon(brightBorderMirrored, 6);   // Draw the mirrored polygon
}

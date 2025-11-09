QT += core gui svg network multimedia widgets

TARGET = QDmi
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

# Für maximale Kompatibilität mit alten Plattformen
CONFIG += c++11
# CONFIG += static  # Falls du statisch linken willst

SOURCES += \
    alphanumericinput.cpp \
    etcstextwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    gauge.cpp \
    dmilabel.cpp \
    lzb.cpp \
    manometer.cpp \
    sep.cpp \
    mtd.cpp \
    targetdistance.cpp \
    timetoindication.cpp \
    zusi3etcs.cpp \
    zusi3tcp.cpp \
    zusiindicator.cpp \
    power.cpp \
    zusipower.cpp \
    zusitraindata.cpp

HEADERS += \
    alphanumericinput.h \
    etcstextwidget.h \
    lzb.h \
    mainwindow.h \
    gauge.h \
    era.h \
    dmilabel.h \
    manometer.h \
    sep.h \
    mtd.h \
    targetdistance.h \
    timetoindication.h \
    zusi3etcs.h \
    zusi3tcp.h \
    zusiindicator.h \
    power.h \
    zusipower.h \
    zusitraindata.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    fonts.qrc \
    icons.qrc \
    sounds.qrc

# Install rule für Desktop-Zielsysteme
unix: target.path = /opt/$${TARGET}/bin
win32: target.path = $$[QT_INSTALL_PREFIX]/bin
!isEmpty(target.path): INSTALLS += target

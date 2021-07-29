#-------------------------------------------------
#
# Project created by QtCreator 2019-01-07T18:29:26
#
#-------------------------------------------------

QT       += core gui svg network multimedia
#ifdef Q_OS_ANDROID
#QT       += androidextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QDmi
TEMPLATE = app
ANDROID_VERSION_CODE = 15
ANDROID_VERSION_NAME = "1.3.0T4"
ANDROID_TARGET_SDK_VERSION = 29
APP_NAME = QDmi

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11
#CONFIG += static

SOURCES += \
    alphanumericinput.cpp \
    main.cpp \
    mainwindow.cpp \
    gauge.cpp \
    dmilabel.cpp \
    lzb.cpp \
    manometer.cpp \
    sep.cpp \
    mtd.cpp \
    zusi3tcp.cpp \
    zusiindicator.cpp \
    power.cpp \
    zusipower.cpp \
    zusitraindata.cpp

HEADERS += \
    alphanumericinput.h \
        mainwindow.h \
    gauge.h \
    era.h \
    dmilabel.h \
    lzb.h \
    manometer.h \
    sep.h \
    mtd.h \
    zusi3tcp.h \
    zusiindicator.h \
    db.h \
    power.h \
    zusipower.h \
    zusitraindata.h

FORMS += \
        mainwindow.ui

CONFIG += mobility
MOBILITY = 


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    fonts.qrc \
    icons.qrc \
    sunds.qrc

ANDROID_ABIS = armeabi-v7a

DISTFILES += \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android


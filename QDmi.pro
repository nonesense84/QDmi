QT += core gui svg network multimedia widgets

TARGET = QDmi
TEMPLATE = app

#ANDROID_VERSION_CODE = *?* #base-arm64
ANDROID_VERSION_CODE = 38 #armeabi

ANDROID_VERSION_NAME = "1.4.1"
ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++17
#CONFIG += debug
QMAKE_CXXFLAGS += -Wno-switch

SOURCES += \
    alphanumericinput.cpp \
    etcstextwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    gauge.cpp \
    dmilabel.cpp \
    lzb.cpp \
    manometer.cpp \
    planningarea.cpp \
    sep.cpp \
    mtd.cpp \
    targetdistance.cpp \
    timetoindication.cpp \
    zusi3etcs.cpp \
    zusi3tcp.cpp \
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
    planningarea.h \
    sep.h \
    mtd.h \
    targetdistance.h \
    timetoindication.h \
    zusi3etcs.h \
    zusi3tcp.h \
    power.h \
    zusipower.h \
    zusitraindata.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    fonts.qrc \
    icons.qrc \
    sounds.qrc

DISTFILES += \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/build.gradle \
    android/gradle.properties \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew \
    android/gradlew.bat \
    android/gradlew.bat \
    android/res/values/libs.xml \
    android/res/values/libs.xml \
    android/res/xml/qtprovider_paths.xml

# Install rule for non-Android
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

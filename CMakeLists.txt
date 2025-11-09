cmake_minimum_required(VERSION 3.16)

project(QDmi LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Android-spezifisches: Multi-ABI
if(ANDROID)
    set(CMAKE_ANDROID_ARCH_ABI "arm64-v8a;armeabi-v7a;x86;x86_64")
endif()

# Füge hier die gewünschten Qt-Module ein
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    Svg
    Network
    Multimedia
)

# Quellen, Header, UI-Dateien und Ressourcen
set(SOURCES
    main.cpp
    mainwindow.cpp
    alphanumericinput.cpp
    etcstextwidget.cpp
    gauge.cpp
    dmilabel.cpp
    lzb.cpp
    manometer.cpp
    sep.cpp
    mtd.cpp
    targetdistance.cpp
    timetoindication.cpp
    zusi3etcs.cpp
    zusi3tcp.cpp
    zusiindicator.cpp
    power.cpp
    zusipower.cpp
    zusitraindata.cpp
)

set(HEADERS
    mainwindow.h
    alphanumericinput.h
    etcstextwidget.h
    lzb.h
    gauge.h
    era.h
    dmilabel.h
    manometer.h
    sep.h
    mtd.h
    targetdistance.h
    timetoindication.h
    zusi3etcs.h
    zusi3tcp.h
    zusiindicator.h
    power.h
    zusipower.h
    zusitraindata.h
)

set(UIS
    mainwindow.ui
)

set(RESOURCES
    fonts.qrc
    icons.qrc
    sunds.qrc
)

# Erstelle die ausführbare Datei
qt_add_executable(QDmi
    ${SOURCES}
    ${HEADERS}
    ${UIS}
    ${RESOURCES}
)

# Verknüpfe mit den benötigten Qt-Bibliotheken
target_link_libraries(QDmi PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Svg
    Qt6::Network
    Qt6::Multimedia
)

# Android-spezifische Einstellungen
if(ANDROID)
    set_property(TARGET QDmi APPEND PROPERTY
        QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_SOURCE_DIR}/android
    )
endif()

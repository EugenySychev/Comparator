QT += quick widgets

CONFIG += c++14

HEADERS += \
    include/compareengine.h

SOURCES += \
    src/compareengine.cpp \
    src/main.cpp

DISTFILES += \
    qml/main.qml \
    qml/sidepannel.qml \
    qml/tabpanel.qml

RESOURCES += \
    qml/qml.qrc



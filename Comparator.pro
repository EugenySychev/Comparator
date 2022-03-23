QT += quick widgets

CONFIG += c++14

INCLUDEPATH += include

HEADERS += \
    include/compareengine.h \
    include/folderitem.h \
    include/foldermodel.h \
    include/utils.h

SOURCES += \
    src/compareengine.cpp \
    src/foldermodel.cpp \
    src/main.cpp

DISTFILES += \
    qml/SidePannel.qml \
    qml/main.qml \
    qml/TabPanel.qml

RESOURCES += \
    qml/qml.qrc



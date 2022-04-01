QT += quick widgets

CONFIG += c++14

INCLUDEPATH += include

HEADERS += \
    include/fileengineworker.h \
    include/compareengine.h \
    include/fileengine.h \
    include/folderitem.h \
    include/foldermodel.h \
    include/utils.h

SOURCES += \
    src/fileengineworker.cpp \
    src/compareengine.cpp \
    src/fileengine.cpp \
    src/foldermodel.cpp \
    src/main.cpp

DISTFILES += \
    qml/SidePannel.qml \
    qml/main.qml \

RESOURCES += \
    qml/qml.qrc



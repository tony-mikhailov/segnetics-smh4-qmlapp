QT += quick
CONFIG += c++11
CODECFORSRC = UTF-8
DEFINES += QT_DEPRECATED_WARNINGS
QML_IMPORT_PATH += "/home/build/Qt/5.9.4/armhf/qml"

linux-g++ {
    QML_IMPORT_PATH += "/home/build/Qt/5.9.4/armhf/qml"
    DEFINES += DESKTOP
    DEFINES += LINUX
}

unix:!macx: {

    !linux-g++ {
        DEFINES += SMH4
    }

    linux-g++ {
        LIBS += -L$$_PRO_FILE_PWD_/../lib/x86_64 
    }
}


SOURCES += main.cpp

HEADERS +=

RESOURCES += qml.qrc

DISTFILES += \
    SMH4.qml \
    qpp.qml \
    Globals.qml

OTHER_FILES += \
    SMH4.qml \
    qpp.qml \
    Globals.qml


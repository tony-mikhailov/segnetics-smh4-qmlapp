QT += quick concurrent
CONFIG += c++11
CODECFORSRC     = UTF-8

DEFINES += QT_DEPRECATED_WARNINGS

linux-g++ {
    QML_IMPORT_PATH += "/home/build/Qt/5.9.4/armhf/qml"
    DEFINES += DESKTOP
    DEFINES += LINUX
}

QML_IMPORT_PATH += "/home/build/Qt/5.9.4/armhf/qml"

unix:!macx: {

    !linux-g++ {
        DEFINES += SMH4
    }

    linux-g++ {
        LIBS += -L$$_PRO_FILE_PWD_/../lib/x86_64 
    }
}


SOURCES += main.cpp \
    smh4c.cpp 

HEADERS += \
    smh4c.h \
    smh4.h 

RESOURCES += qml.qrc \
    qml.qrc

DISTFILES += \
    qpp.qml \
    Globals.qml

OTHER_FILES += \
    qpp.qml \
    Globals.qml


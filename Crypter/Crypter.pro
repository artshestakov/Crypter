TARGET = Crypter
CONFIG += console c++11
CONFIG -= qt
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS #???

CONFIG(debug, debug|release){
    CONFIGURATION = Debug
}else{
    CONFIGURATION = Release
}

unix{
    PLATFORM = Linux64
}

win32 {
    PLATFORM = Win32
}

DESTDIR = $$PWD/../Bin/$${CONFIGURATION}-$${PLATFORM}
MOC_DIR = $$PWD/../Build/$${CONFIGURATION}-$${PLATFORM}
OBJECTS_DIR = $$PWD/../Build/$${CONFIGURATION}-$${PLATFORM}
INCLUDEPATH += $$PWD/../SNImageCrypt
INCLUDEPATH += .

LIBS += -L$$DESTDIR -Wl,-rpath="$${DESTDIR}",-rpath-link="$${DESTDIR}" -lSNImageCrypt

SOURCES += main.cpp

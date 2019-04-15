TARGET = Crypter
CONFIG += console c++11
CONFIG -= qt
TEMPLATE = app

CONFIG(debug, debug|release){
    CONFIGURATION = Debug
    DEFINES += _DEBUG
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
OBJECTS_DIR = $$PWD/../Build/$${CONFIGURATION}-$${PLATFORM}
INCLUDEPATH += .

SOURCES += main.cpp

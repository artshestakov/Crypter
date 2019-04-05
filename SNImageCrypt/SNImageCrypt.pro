TARGET = SNImageCrypt
TEMPLATE = lib
CONFIG += c++11
CONFIG -= qt
DEFINES += SNIMAGECRYPT_LIBRARY
DEFINES += QT_DEPRECATED_WARNINGS

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
INCLUDEPATH += .

HEADERS += \
    SNImageCrypt.h \
    lodepng.h

SOURCES += \
    SNImageCrypt.cpp \
    lodepng.cpp

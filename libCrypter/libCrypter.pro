TARGET = libCrypter
TEMPLATE = lib
CONFIG += c++11
CONFIG -= qt

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
OBJECTS_DIR = $$PWD/../Build/$${CONFIGURATION}-$${PLATFORM}
INCLUDEPATH += .

SOURCES += \
    LCCrypter.cpp \
	LCGlobal.cpp \
	LCPixel.cpp \
	LCTracer.cpp \
    lodepng.cpp

HEADERS += \
    LCCrypter.h \
	LCGlobal.h \
	LCPixel.h \
	LCTracer.h \
    lodepng.h \
	StdAfx.h

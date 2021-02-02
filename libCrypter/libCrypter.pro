TARGET = Crypter
TEMPLATE = lib
QMAKE_CFLAGS += -std=c99
CONFIG -= app_bundle
CONFIG -= qt
DEFINES += LIBCRYPTER_LIB

CONFIG(debug, debug|release){
    CONFIGURATION = Debug
	DEFINES += DEBUG
}else{
    CONFIGURATION = Release
}

unix {
    PLATFORM = Linux64
}

DESTDIR = $$PWD/../Bin/$${CONFIGURATION}-$${PLATFORM}
OBJECTS_DIR = $$PWD/$${CONFIGURATION}-$${PLATFORM}
INCLUDEPATH += .

HEADERS += \
    StdAfx.h \
    libCrypter.h \
    libCrypter_global.h \
    lodepng.h

SOURCES += \
    StdAfx.c \
    libCrypter.c \
    libCrypter_global.c \
    lodepng.c

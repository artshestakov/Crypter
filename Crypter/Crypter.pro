TARGET = Crypter
TEMPLATE = app
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
INCLUDEPATH += $$PWD/../libCrypter
INCLUDEPATH += .

LIBS += -L$$DESTDIR -Wl,-rpath="'\$$ORIGIN'",-rpath-link="'\$$ORIGIN'" \
    -lCrypter

SOURCES += \
    main.c

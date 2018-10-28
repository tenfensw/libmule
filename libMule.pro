# ===========================================================
# | Microcontroller-universal Library (that is Extendable)  |
# |     Written by Tim K (timprogrammer@rambler.ru)         |
# |            Copyright (c) 2018 Tim K.                    |
# ===========================================================

MULEMAJORVERSION = 1
MULEMINORVERSION = 0
MULEUPDATEVERSION = 0
MULEBUILDID = nightly
MULEAPIVERSION = 26102018

DEFINES += MULE_VERSION_MAJOR=$$MULEMAJORVERSION
DEFINES += MULE_VERSION_MINOR=$$MULEMINORVERSION
DEFINES += MULE_VERSION_UPDATE=$$MULEUPDATEVERSION
DEFINES += MULE_VERSION_API=$$MULEAPIVERSION
!equals($$MULEBUILDID, final) {
    DEFINES += MULE_VERSION_NIGHTLY
} else {
    DEFINES += MULE_VERSION_FINAL
}

QT       -= core gui # get rid of Qt dependencies that are not needed at all
TARGET = Mule
TEMPLATE = lib
CONFIG += staticlib
INCLUDEPATH += $$PWD/src

message( "libMule - Microcontroller-universal Library (that is Extendable)" )
message( "Release $$MULEMAJORVERSION . $$MULEMINORVERSION . $$MULEUPDATEVERSION ($$MULEBUILDID)" )
message( "Copyright (c) Tim K (timprogrammer@rambler.ru) 2018. Licensed under GNU LGPLv2.1" )
isEmpty($$MULETARGET) {
    MULETARGET = dummy
}
message( "Building for platform $$MULETARGET " )

equals(MULETARGET, dummy) {
    DEFINES += MULE_PLATFORM_DUMMY
    DEFINES += MULE_FEATURES_FILEIO
    DEFINES += MULE_FEATURES_SENSORS
    DEFINES += MULE_FEATURES_EASYMOTORS
    DEFINES += MULE_OTHER_HWPINTYPE=int
    DEFINES += MULE_OTHER_STRINGTYPE=std::string
    DEFINES += MULE_FEATURES_SUPPORTSCOUT
    DEFINES += MULE_FEATURES_DEBUG
    HEADERS += src/platformsupport/dummy/muledummyplatform.h
    SOURCES += src/platformsupport/dummy/muledummyplatform.cpp
}

SOURCES += \
        src/core/muleapplication.cpp \
    src/core/muledevice.cpp \
    src/platformsupport/common/mulecurrentplatform.cpp

HEADERS += \
        src/core/muleapplication.h \
        src/platformsupport/common/mulecommonplatform.h \
    src/core/muledevice.h \
    src/core/muleglobalfunctions.h \
    src/platformsupport/common/mulecurrentplatform.h

unix {
    target.path = /opt/timkoisoft/mule
    INSTALLS += target
}

#*****************************************************************************
# Ifstool qmake file
# Authors : Pratheek Mallikarjuna
#           Wesley Snyder
#*****************************************************************************

QT  += core
QT  += gui
QT  += printsupport
QT  += widgets

VERSION = "2.1"
DEFINES = "MacX64"

CONFIG  -= console
CONFIG  += app_bundle
CONFIG  += static

TEMPLATE    = app
TARGET      = $$PWD/bin/Ifstool
INCLUDEPATH += ../hdr

SOURCES += \
    main.cpp \
    ifstoolMainWindow.cpp \
    ifstool.cpp \
    subclassqapp.cpp \
    ifs2qt.cpp \
    ifstoolChildWindow.cpp \
    ifsOp.cpp\
    qcustomplot.cpp \
    plotMainWindow.cpp

FORMS    += \
    plotMainWindow.ui

HEADERS += \
    main.h\
    ../hdr/flip.h \
    ../hdr/ifs.h \
    ../hdr/ifsdocs.h \
    ../hdr/ifsequ.h \
    ../hdr/ifserr.h \
    ../hdr/ifsmatrix.h \
    ../hdr/ifstypes.h \
    ../hdr/nicedefs.h \
    ../hdr/resample.h \
    ../hdr/rfft.h \
    ../hdr/stddefs.h \
    ifstoolMainWindow.h \
    ifstool.h \
    generictypes.h \
    subclassqapp.h \
    ifs2qt.h \
    ifstoolChildWindow.h \
    ifsOp.h \
    qcustomplot.h \
    plotMainWindow.h

macx {
      QMAKE_MAC_SDK=macosx10.11
}

macx: LIBS += -L$$PWD/../lib/macX64/ -lifs

INCLUDEPATH += $$PWD/../lib/macX64/ifslib
DEPENDPATH += $$PWD/../lib/macX64/ifslib

macx: PRE_TARGETDEPS += $$PWD/../lib/macX64/libifs.a

RESOURCES += \
    images.qrc


QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
CONFIG += 3D_CONFIG

DEBUG_SUF =
DIR_SUF = release
CONFIG(debug, debug|release) {

   DEBUG_SUF =d
   DIR_SUF = debug
}

3D_CONFIG {
     OSG_ROOT = thirdParty/OSG
     EARTH_ROOT = thirdParty/OSGEARTH
     INCLUDEPATH += $${OSG_ROOT}/include $${EARTH_ROOT}/include
     DEPENDPATH += $${OSG_ROOT}/include $${EARTH_ROOT}/include
     LIBS += -L$${OSG_ROOT}/lib -L$${EARTH_ROOT}/lib
}


3D_CONFIG {
DEFINES += OSG_API

LIBS += \
#OSG
    -losg$${DEBUG_SUF} \
    -losgWidget$${DEBUG_SUF} \
    -lOpenThreads$${DEBUG_SUF} \
    -losgViewer$${DEBUG_SUF} \
    -losgManipulator$${DEBUG_SUF} \
    -losgUtil$${DEBUG_SUF} \
    -losgText$${DEBUG_SUF} \
    -losgDB$${DEBUG_SUF} \
    -losgGA$${DEBUG_SUF} \
    -losgSim$${DEBUG_SUF} \
    -losgParticle$${DEBUG_SUF} \
    -losgFX$${DEBUG_SUF} \
    -losgPresentation$${DEBUG_SUF} \
    -losgShadow$${DEBUG_SUF} \
    -losgUI$${DEBUG_SUF} \
    -losgTerrain$${DEBUG_SUF} \
#    -losgQt$${DEBUG_SUF} \
#OSGEARTH
    -losgEarth$${DEBUG_SUF} \
    -losgEarthAnnotation$${DEBUG_SUF} \
    -losgEarthFeatures$${DEBUG_SUF} \
    -losgEarthSplat$${DEBUG_SUF} \
    -losgEarthSymbology$${DEBUG_SUF} \
    -losgEarthUtil$${DEBUG_SUF}
}


DESTDIR = $${DIR_SUF}/bin
MOC_DIR = $${DIR_SUF}/moc
OBJECTS_DIR = $${DIR_SUF}/obj

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/osgQtWdgt.cpp \
    src/osgScene.cpp \
    src/osgView.cpp \
    src/pickObjectHandler.cpp \
    src/queryCrdHandler.cpp

HEADERS += \
    src/mainwindow.h \
    src/osgQtWdgt.h \
    src/osgScene.h \
    src/osgView.h \
    src/pickObjectHandler.h \
    src/queryCrdHandler.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

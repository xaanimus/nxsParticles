TEMPLATE = app

QT += qml quick
CONFIG += c++11
TARGET = nxsparticles

#Sources
SOURCES += \
    main.cpp \
    mainview.cpp \
    texturegenerator.cpp \
    Particles/particlecomputer.cpp \
    Particles/particlesystem.cpp \
    Particles/emitter.cpp \
    Scene/particlescenenode.cpp \
    Scene/scenemanager.cpp \
    Scene/mainrenderer.cpp \
    Scene/camera.cpp \
    Scene/scenenode.cpp


HEADERS += \
    mainview.h \
    utilities.h \
    texturegenerator.h \
    Particles/particlesystem.h \
    Particles/particlecomputer.h \
    Particles/emitter.h \
    Scene/mainrenderer.h \
    Scene/camera.h \
    Scene/scenenode.h \
    Scene/particlescenenode.h \
    Scene/scenemanager.h

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

INCLUDEPATH += /usr/local/Cellar/glm/0.9.7.1/include

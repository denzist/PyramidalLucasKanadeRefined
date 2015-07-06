TEMPLATE = app

QT += qml quick widgets

SOURCES += main.cpp \
    image_processing.tpp \
    matrix.tpp \
    image_filtering.tpp \
    pyramid.tpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    image_processing.h \
    matrix.h \
    image_filtering.h \
    pyramid.h

DISTFILES += \
    ../image.png \
    image2.png

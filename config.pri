# Auto-generated by IDE. All changes by user will be lost!
# Created at 2/6/13 12:21 PM

BASEDIR = $$_PRO_FILE_PWD_

INCLUDEPATH +=  \
    $$BASEDIR/src

SOURCES +=  \
    $$BASEDIR/src/CustomSlider.cpp \
    $$BASEDIR/src/NuttySlider.cpp \
    $$BASEDIR/src/NuttySliderApp.cpp \
    $$BASEDIR/src/main.cpp

HEADERS +=  \
    $$BASEDIR/src/CustomSlider.h \
    $$BASEDIR/src/NuttySlider.h \
    $$BASEDIR/src/NuttySliderApp.hpp

CONFIG += precompile_header
PRECOMPILED_HEADER = $$BASEDIR/precompiled.h

lupdate_inclusion {
    SOURCES += \
        $$BASEDIR/../assets/*.qml
}

TRANSLATIONS = \
    $${TARGET}.ts


# -------------------------------------------------
# Project created by QtCreator 2010-11-01T14:50:23
# -------------------------------------------------
QT += core \
    gui \
    widgets
TARGET = QTailer
TEMPLATE = app
SOURCES += main.cpp \
    qtailermain.cpp \
    uctabcontaint.cpp \
    qtailthread.cpp \
    hightlight.cpp \
    selecthighlight.cpp \
    about.cpp \
    qtailconfig.cpp \
    ucgeneralconfig.cpp \
    ucfileconfig.cpp \
    uceditfilehistory.cpp \
    ucdefaulthighlight.cpp \
    csession.cpp \
    sessionchooser.cpp \
    sessionname.cpp
HEADERS += qtailermain.h \
    uctabcontaint.h \
    qtailthread.h \
    hightlight.h \
    selecthighlight.h \
    about.h \
    qtailconfig.h \
    ucgeneralconfig.h \
    ucfileconfig.h \
    constant.h \
    uceditfilehistory.h \
    ucdefaulthighlight.h \
    csession.h \
    sessionchooser.h \
    sessionname.h
FORMS += qtailermain.ui \
    uctabcontaint.ui \
    selecthighlight.ui \
    about.ui \
    qtailconfig.ui \
    ucgeneralconfig.ui \
    ucfileconfig.ui \
    uceditfilehistory.ui \
    ucdefaulthighlight.ui \
    sessionchooser.ui \
    sessionname.ui
RESOURCES += Images.qrc
static { # everything below takes effect with CONFIG += static
    CONFIG += static
    CONFIG += staticlib # this is needed if you create a static library, not a static executable
    DEFINES += STATIC
    message("~~~ static build ~~~") # this is for information, that the static build is done
    mac: TARGET = $$join(TARGET,,,_static) #this adds an _static in the end, so you can seperate static build from non static build
    win32: TARGET = $$join(TARGET,,,s) #this adds an s in the end, so you can seperate static build from non static build
}

OTHER_FILES += \
    tools/Linux/cp-ressource.sh

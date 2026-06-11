QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ColoredButton.cpp \
    PathUtils.cpp \
    SCButton.cpp \
    TButton.cpp \
    level.cpp \
    main.cpp \
    meguru.cpp \
    widget.cpp

HEADERS += \
    ColoredButton.h \
    PathUtils.h \
    SCButton.h \
    TButton.h \
    config.h \
    level.h \
    meguru.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    草稿纸

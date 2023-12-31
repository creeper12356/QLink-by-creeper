QT       += core gui
QT	 += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets testlib

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# depreTcated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    backendprocessor.cpp \
    checklinktest.cpp \
    main.cpp \
    objects/box.cpp \
    objects/boxmap.cpp \
    objects/entity.cpp \
    objects/linkroute.cpp \
    objects/map.cpp \
    objects/plainbox.cpp \
    objects/propbox.cpp \
    objects/role.cpp \
    record.cpp \
    tools/betterbutton.cpp \
    tools/clock.cpp \
    tools/indexbar.cpp \
    tools/keylineedit.cpp \
    tools/scoreboard.cpp \
    widgets/basicsetdialog.cpp \
    widgets/gamemain.cpp \
    widgets/menupage.cpp \
    widgets/recordbrowser.cpp \
    widgets/recorditem.cpp \
    widgets/settings.cpp \
    widgets/welcomepage.cpp

HEADERS += \
    backendprocessor.h \
    checklinktest.h \
    config.h \
    objects/box.h \
    objects/boxmap.h \
    objects/entity.h \
    objects/linkroute.h \
    objects/map.h \
    objects/plainbox.h \
    objects/propbox.h \
    objects/role.h \
    record.h \
    tools/betterbutton.h \
    tools/clock.h \
    tools/indexbar.h \
    tools/keylineedit.h \
    tools/scoreboard.h \
    widgets/basicsetdialog.h \
    widgets/gamemain.h \
    widgets/menupage.h \
    widgets/recordbrowser.h \
    widgets/recorditem.h \
    widgets/settings.h \
    widgets/welcomepage.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ../res.qrc

FORMS += \
    widgets/basicsetdialog.ui \
    widgets/menupage.ui \
    widgets/recordbrowser.ui \
    widgets/settings.ui \
    widgets/welcomepage.ui \
    widgets/widget.ui

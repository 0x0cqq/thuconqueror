QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    src/main.cpp \
    src/windows/mainwindow.cpp \
    src/windows/startwindow.cpp \
    src/logic/unit.cpp \
    src/logic/block.cpp \
    src/logic/field.cpp \
    src/game/game.cpp \
    src/graph/graphunit.cpp \
    src/graph/graphblock.cpp \
    src/graph/graphfield.cpp \
    src/graph/unitdialog.cpp \
    src/graph/menudialog.cpp \
    src/graph/choosedialog.cpp \
    src/graph/bloodbar.cpp \
    src/game/graphview.cpp \
    src/game/enemyai.cpp \
    src/myapplication.cpp \
    src/basic/status.cpp


HEADERS += \
    src/myapplication.h \
    src/windows/mainwindow.h \
    src/windows/startwindow.h \
    src/graph/graphblock.h \
    src/game/graphview.h \
    src/graph/graphfield.h \
    src/graph/graphunit.h \
    src/graph/unitdialog.h \
    src/graph/menudialog.h \
    src/graph/choosedialog.h \
    src/graph/bloodbar.h \
    src/logic/unit.h \
    src/logic/block.h \ 
    src/logic/field.h \
    src/game/game.h \
    src/game/enemyai.h \
    src/basic/status.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

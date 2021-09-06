QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/startwindow.cpp \
    src/logic/unit.cpp \
    src/logic/block.cpp \
    src/logic/field.cpp \
    src/game.cpp \
    src/graph/graphunit.cpp \
    src/graph/graphblock.cpp \
    src/graph/graphfield.cpp \
    src/graph/unitdialog.cpp \
    src/graph/menudialog.cpp \
    src/graph/choosedialog.cpp \
    src/graphview.cpp \
    src/myapplication.cpp \
    src/basic/status.cpp


HEADERS += \
    src/myapplication.h \
    src/mainwindow.h \
    src/startwindow.h \
    src/graph/graphblock.h \
    src/graphview.h \
    src/graph/graphfield.h \
    src/graph/graphunit.h \
    src/graph/unitdialog.h \
    src/graph/menudialog.h \
    src/graph/choosedialog.h \
    src/logic/unit.h \
    src/logic/block.h \ 
    src/logic/field.h \
    src/game.h \
    src/basic/status.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

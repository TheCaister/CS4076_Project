QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Character.cpp \
    Command.cpp \
    CommandWords.cpp \
    Parser.cpp \
    Room.cpp \
    Stack.cpp \
    WordleEngine.cpp \
    ZorkUL.cpp \
    dialogues.cpp \
    item.cpp \
    mainwindow.cpp \
    old_zork.cpp

HEADERS += \
    Character.h \
    Command.h \
    CommandWords.h \
    Errors.h \
    Parser.h \
    Room.h \
    Stack.h \
    WordleEngine.h \
    ZorkUL.h \
    constants.h \
    dialogues.h \
    item.h \
    mainwindow.h \
    old_zork.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    cpp_criteria.txt \
    to-do.txt \
    wordle_words.txt

RESOURCES += \
    resources.qrc

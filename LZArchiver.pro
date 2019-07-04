CONFIG -= qt

INCL = include
SRC = src

INCLUDEPATH += $${INCL}

SOURCES += $${SRC}/main.cpp

HEADERS += $${INCL}/SLaunchOptions.h

HEADERS += $${INCL}/CArchiver.h
SOURCES += $${SRC}/CArchiver.cpp

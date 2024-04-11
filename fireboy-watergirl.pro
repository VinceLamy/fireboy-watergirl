TEMPLATE     = vcapp
TARGET       = fireboy-watergirl
CONFIG      += warn_on qt debug windows console
HEADERS     += ../fireboy-watergirl/src/map.h ../fireboy-watergirl/src/game.h ../fireboy-watergirl/src/mainmenu.h ../fireboy-watergirl/src/menubutton.h ../fireboy-watergirl/src/levelselection.h ../fireboy-watergirl/src/tutorialscreen.h ../fireboy-watergirl/src/character.h ../fireboy-watergirl/src/codegiver.h
SOURCES     += ../fireboy-watergirl/src/map.cpp ../fireboy-watergirl/src/game.cpp ../fireboy-watergirl/src/main.cpp ../fireboy-watergirl/src/mainmenu.cpp ../fireboy-watergirl/src/menubutton.cpp ../fireboy-watergirl/src/levelselection.cpp ../fireboy-watergirl/src/tutorialscreen.cpp ../fireboy-watergirl/src/character.cpp ../fireboy-watergirl/src/codegiver.cpp
INCLUDEPATH += ../fireboy-watergirl/src
QT          += widgets

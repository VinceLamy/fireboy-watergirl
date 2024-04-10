TEMPLATE     = vcapp
TARGET       = fireboy-watergirl
CONFIG      += warn_on qt debug windows console
HEADERS     += ../fireboy-watergirl/src/map.h ../fireboy-watergirl/src/game.h ../fireboy-watergirl/src/mainmenu.h ../fireboy-watergirl/src/menubutton.h ../fireboy-watergirl/src/levelselection.h	
SOURCES     += ../fireboy-watergirl/src/map.cpp ../fireboy-watergirl/src/game.cpp ../fireboy-watergirl/src/main.cpp ../fireboy-watergirl/src/mainmenu.cpp ../fireboy-watergirl/src/menubutton.cpp ../fireboy-watergirl/src/levelselection.cpp
INCLUDEPATH += ../fireboy-watergirl/src
QT          += widgets

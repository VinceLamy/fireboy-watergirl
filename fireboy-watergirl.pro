TEMPLATE     = vcapp
TARGET       = fireboy-watergirl
CONFIG      += warn_on qt debug windows console
HEADERS     += ../fireboy-watergirl/src/map.h ../fireboy-watergirl/src/game.h		
SOURCES     += ../fireboy-watergirl/src/map.cpp ../fireboy-watergirl/src/game.cpp ../fireboy-watergirl/src/main.cpp	
INCLUDEPATH += ../fireboy-watergirl/src
QT          += widgets

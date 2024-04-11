TEMPLATE     = vcapp
TARGET       = fireboy-watergirl
CONFIG      += warn_on qt debug windows console
VPATH		+= ./src
HEADERS     += button.h character.h codegiver.h codelock.h controller.h coordinate.h enum.h exit.h game.h gameovermenu.h gate.h keyboardhandler.h levelselection.h lever.h mainmenu.h map.h menubutton.h pool.h tile.h tutorialscreen.h wall.h ../Communication/communication.h ../Communication/serial/SerialPort.hpp
SOURCES     += button.cpp character.cpp codegiver.cpp codelock.cpp controller.cpp exit.cpp game.cpp gameovermenu.cpp gate.cpp keyboardhandler.cpp levelselection.cpp lever.cpp main.cpp mainmenu.cpp map.cpp menubutton.cpp pool.cpp tile.cpp tutorialscreen.cpp wall.cpp ../Communication/communication.cpp ../Communication/serial/SerialPort.cpp
INCLUDEPATH += ./src
QT          += widgets

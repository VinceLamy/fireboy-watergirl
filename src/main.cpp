#include <QApplication>
#include <iostream>

#include "game.h"

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	std::string port = "COM3";
	Game game(port.c_str()); //Utiliser pour manette
	
	return app.exec();
}
#include <QApplication>
#include <iostream>

#include "game.h"

int main()
{
	std::string port = "COM3";

	Game game(port.c_str()); //Utiliser pour manette
	
	return 0;
}
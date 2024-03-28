#include <iostream>

#include "game.h"
//#include //"../Communication/communication.h"

#define BAUD 256000
#define MSG_MAX_SIZE 512

#define VALEUR_TIMER 99

int main()
{
	std::string port = "COM3";

	Game game(port.c_str()); //Utiliser pour manette
	
	return 0;
}
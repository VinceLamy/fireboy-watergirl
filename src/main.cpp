#include <iostream>

#include "game.h"
//#include //"../Communication/communication.h"

#define BAUD 256000
#define MSG_MAX_SIZE 512

#define VALEUR_TIMER 99

int main()
{
	std::string port = "COM4";

	//	SerialPort* arduino = new SerialPort(port.c_str(), BAUD);
	Game game(true, port.c_str());
	game.Play();

	return 0;
}
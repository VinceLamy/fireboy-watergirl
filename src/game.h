#ifndef GAME_H
#define GAME_H

#define VALEUR_TIMER 60;

#include <chrono>
#include "../Communication/communication.h"

#include "map.h"



class Game
{
	Map _map;
	bool _gameOver;
	bool _levelFinished;
	bool _isJumping;
	bool _wasButton;
	chrono::time_point<chrono::system_clock> _start;
	chrono::time_point<chrono::system_clock> _end;
	int _jumpHeight;

	Communication* comm;
	input_data data;

	int etat_joueur = 0;
	bool parse_status = false;
	int compteur_depart = VALEUR_TIMER;
	int dt = 0;


public:
	Game();
	Game(const char* port);
	~Game();

	void init();

	void GetInput();
	void MovePlayers();
	void CheckPosition();
	void CheckGates();
	void CheckButtons();
	void CheckExits();
	void Interact();
	void SendResponse();

	void Play();

	void Show();

};

#endif CARACTER_H

#ifndef GAME_H
#define GAME_H

#define DISPLAY_OFF -1

#include <chrono>
#include "../Communication/communication.h"

#include "map.h"

struct input_data
{
	bool moveUp;
	bool moveRight;
	bool moveDown;
	bool moveLeft;

	bool switchChars;
	bool interact;
	bool jump;
	bool menu;

	bool accelUp;

	int random;

};

class Game
{
	Map _map;
	int _currentLevel = 1;
	bool _gameOver;
	bool _levelFinished;
	bool _isJumping;
	bool _updated;
	bool _codegiven;


	chrono::time_point<chrono::system_clock> _start;
	int _jumpHeight;
	int _code;

	Communication* comm;
	input_data data;

	bool parse_status = false;

	bool _manette;

public:
	Game(const char* port);
	~Game();

	void NewLevel();
	void MainMenu();
	int AskMainMenuInput();
	void ChooseLevel();
	void Menu();
	int AskMenuInput();

	void GetInput();
	void MovePlayers();
	void CheckPosition();
	void CheckGates();
	void CheckButtons();
	void CheckExits();
	void CheckPools();
	void Interact();
	void SendResponse();

	void Play();
};

#endif CARACTER_H

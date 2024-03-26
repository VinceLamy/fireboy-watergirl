#ifndef GAME_H
#define GAME_H

#include <chrono>

#include "map.h"

class Game
{
	Map _map;
	int _currentLevel = 1;
	bool _gameOver;
	bool _levelFinished;
	bool _isJumping;
	bool _wasButton;
	chrono::time_point<chrono::system_clock> _start;
	chrono::time_point<chrono::system_clock> _end;
	int _jumpHeight;
public:
	Game();
	~Game();

	void init();
	void NewLevel();
	void MainMenu();
	int AskMainMenuInput();
	void ChooseLevel();
	void Menu();
	int AskMenuInput();

	void GetInput();
	void CheckPosition();
	void CheckGates();
	void CheckButtons();
	void CheckExits();
	void Interact();

	void Play();

	void Show();

};

#endif CARACTER_H

#ifndef GAME_H
#define GAME_H

#define DISPLAY_OFF -1

#include <chrono>
#include "../Communication/communication.h"

#include "map.h"
#include "mainmenu.h"
#include "levelselection.h"
#include "tutorialscreen.h"
#include <QMainWindow>

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

class Game : public QObject
{
	Q_OBJECT

	Map* _map;
	int _currentLevel = 1;
	bool _gameOver;
	bool _levelFinished;
	bool _isJumping;
	bool _updated;
	bool _codegiven;


	std::chrono::time_point<std::chrono::system_clock> _start;
	int _jumpHeight;
	int _code;

	Communication* comm;
	input_data data;

	bool parse_status = false;

	bool _manette;

	QMainWindow* _mainWindow;
	MainMenu* _mainMenu;
	LevelSelection* _levelSelection;
	TutorialScreen* _tutorialScreen;
	QGraphicsView view;
	QTimer timer;

public slots:
	void LoadLevel(int level);
	void ShowTutorialScreen();
	void ShowMainMenu();
	void GameOverScreen();
	void Play();

public:
	Game(const char* port, QObject* parent = nullptr);
	~Game();

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
	
};

#endif CARACTER_H

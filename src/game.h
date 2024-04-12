#ifndef GAME_H
#define GAME_H

#define DISPLAY_OFF -1

#include <chrono>
#include "../Communication/communication.h"

#include "map.h"
#include "mainmenu.h"
#include "gameovermenu.h"
#include "ingamemenu.h"
#include "betweenlevelmenu.h"
#include "endgamemenu.h"
#include "codeinputmenu.h"
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
	bool _levelFinished;
	bool _updated;
	bool _codegiven;

	int _code;

	Communication* comm;
	input_data data;

	bool parse_status = false;

	bool _manette;

	QMainWindow* _mainWindow;
	MainMenu* _mainMenu;
	GameOverMenu* _gameOverMenu;
	BetweenLevelMenu* _betweenLevelMenu;
	EndGameMenu* _endGameMenu;
	CodeInputMenu* _codeInputMenu;
	LevelSelection* _levelSelection;
	TutorialScreen* _tutorialScreen;
	QGraphicsView view;
	QTimer timer;
	QTimer controllerTimer;
	InGameMenu* _inGameMenu;

	QString _numberEntered;

public slots:
	void LoadLevel(int level);
	void ShowTutorialScreen();
	void ShowMainMenu();
	void ShowGameOverMenu();
	void ShowEndGameMenu();
	void VerifyCode(QString number);
	void GameOverScreen();
	void BetweenLevelScreen();
	void Play();
	void NextLevel();
	void ControllerLoop();
	void SendDigitsToController(const QString& s);
	void ShowInGameMenu();
	void ResumeGame();
	void RestartGame();

public:
	Game(const char* port, QObject* parent = nullptr);
	~Game();

	void ChooseLevel();
	void Menu();

	void GetInput();
	void SendResponse();
	void CreateInputEvent();
};

#endif CARACTER_H

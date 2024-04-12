#include "game.h"
#include <iostream>
#include "conio.h"
#include "pool.h"
#include "Windows.h"
#include <stdlib.h>
#include <string>
#include <QWidget>

Game::Game(const char* port, QObject* parent) 
	: QObject(parent)
{
	comm = new Communication(port, true);

	_manette = comm->IsConnected();
	std::cout << _manette << std::endl;

	_mainWindow = new QMainWindow();
	_mainMenu = new MainMenu();
	_inGameMenu = new InGameMenu();
	_levelSelection = new LevelSelection();
	_tutorialScreen = new TutorialScreen();

	view.setFocusPolicy(Qt::StrongFocus);

	connect(_mainMenu, &MainMenu::levelSelected, this, &Game::LoadLevel);
	connect(_mainMenu, &MainMenu::levelSelection, this, &Game::ChooseLevel);
	connect(_mainMenu, &MainMenu::ShowTutorialScreen, this, &Game::ShowTutorialScreen);
	connect(_levelSelection, &LevelSelection::levelSelected, this, &Game::LoadLevel);
	connect(_levelSelection, &LevelSelection::returnToMainMenu, this, &Game::ShowMainMenu);
	connect(_tutorialScreen, &TutorialScreen::BackToMainMenu, this, &Game::ShowMainMenu);
	connect(_tutorialScreen, &TutorialScreen::StartTutorial, this, &Game::LoadLevel);
	connect(_inGameMenu, &InGameMenu::resumeGame, this, &Game::ResumeGame);
	connect(_inGameMenu, &InGameMenu::quitToMainMenu, this, &Game::ShowMainMenu);
	connect(_inGameMenu, &InGameMenu::restartGame, this, &Game::RestartGame);
	
	_mainWindow->resize(1280, 720);
	_mainWindow->setStyleSheet("QMainWindow {" "background-image: url(./sprite/menu/fractal-1722991_1920.jpg);" "}");
	_mainWindow->setCentralWidget(_mainMenu);
	_mainWindow->show();

	if (_manette)
	{
		connect(&controllerTimer, &QTimer::timeout, this, &Game::ControllerLoop);
	}
}

void Game::ShowInGameMenu()
{
	timer.stop();
	if (_manette)
		controllerTimer.stop();
	view.close();
	_inGameMenu = new InGameMenu();
	connect(_inGameMenu, &InGameMenu::resumeGame, this, &Game::ResumeGame);
	connect(_inGameMenu, &InGameMenu::quitToMainMenu, this, &Game::ShowMainMenu);
	connect(_inGameMenu, &InGameMenu::restartGame, this, &Game::RestartGame);
	_mainWindow->setStyleSheet("background-color: black;");
	_mainWindow->setCentralWidget(_inGameMenu);
	_mainWindow->show();
}

Game::~Game()
{
	delete comm;
}

void Game::LoadLevel(int level)
{
	_currentLevel = level;
	_codegiven = false;
	if (_manette)
		comm->OpenPort();
	
	int count = 0;

	while ((data.random > 10000 || data.random < 1000) && count < 10)
	{
		count++;
		Sleep(1000 / 15);
		GetInput();
	}
	if (count < 10)
	{
		code = data.random;
	}
	else
	{
		srand(time(NULL));
		code = rand() % (10000 - 999 + 1) + 999;
	}

	switch (_currentLevel)
	{
	case 0:
		_map = new Map("./map/0.txt", &view);
		break;
	case 1:
		_map = new Map("./map/1.txt", &view);
		break;
	case 2:
		_map = new Map("./map/2.txt", &view);
		break;
	case 3:
		_map = new Map(code, "./map/3.txt", &view);
		break;
	case 4:
		_map = new Map(code, "./map/4.txt", &view);
		break;
	case 5:
		_map =  new Map(code, "./map/5.txt", &view);
	case 6:
		system("CLS");
		std::cout << "FELICITATION!!! MERCI D'AVOIR JOUE!!!" << std::endl;
		Sleep(2000);
		_currentLevel = 1;
		ShowEndGameMenu();
		break;
	}

	connect(_map, &Map::GameOver, this, &Game::GameOverScreen);
	connect(_map, &Map::SendingDigits, this, &Game::SendDigitsToController);
	connect(_map, &Map::OpenInGameMenu, this, &Game::ShowInGameMenu);
	connect(_map, &Map::LevelFinished, this, &Game::ShowEndGameMenu);
	
	Play();
}

void Game::ShowTutorialScreen()
{
	_tutorialScreen = new TutorialScreen();
	connect(_tutorialScreen, &TutorialScreen::BackToMainMenu, this, &Game::ShowMainMenu);
	connect(_tutorialScreen, &TutorialScreen::StartTutorial, this, &Game::LoadLevel);
	_mainWindow->setStyleSheet("background-color: black;");
	_mainWindow->setCentralWidget(_tutorialScreen);
}

void Game::ResumeGame()
{
	_mainWindow->close();
	if (_manette)
		controllerTimer.start(1000 / 15);
	timer.start(1000 / 60);
	view.show();
}

void Game::RestartGame()
{
	LoadLevel(_currentLevel); 
}


void Game::ShowMainMenu()
{
	_mainMenu = new MainMenu();
	connect(_mainMenu, &MainMenu::levelSelected, this, &Game::LoadLevel);
	connect(_mainMenu, &MainMenu::levelSelection, this, &Game::ChooseLevel);
	connect(_mainMenu, &MainMenu::ShowTutorialScreen, this, &Game::ShowTutorialScreen);
	_mainWindow->setStyleSheet("QMainWindow {" "background-image: url(./sprite/menu/fractal-1722991_1920.jpg);" "}");
	_mainWindow->setCentralWidget(_mainMenu);
}

void Game::ShowGameOverMenu()
{
	_codeInputMenu = new CodeInputMenu();
	connect(_codeInputMenu, &CodeInputMenu::VerifyCode, this, &Game::VerifyCode);
	_mainWindow->setStyleSheet("QMainWindow {" "background-image: url(./sprite/menu/fractal-1722991_1920.jpg);" "}");
	_mainWindow->setCentralWidget(_codeInputMenu);
}

void Game::VerifyCode(QString numberEntered) {
	std::cout << numberEntered.toStdString();
}

void Game::ShowEndGameMenu() 
{
	if (_manette)
	{
		controllerTimer.stop();
		comm->ClosePort();
	}
	view.close();
	_endGameMenu = new EndGameMenu();
	connect(_endGameMenu, &EndGameMenu::BackToMainMenu, this, &Game::ShowMainMenu);
	_mainWindow->setStyleSheet("QMainWindow {" "background-image: url(./sprite/menu/fractal-1722991_1920.jpg);" "}");
	_mainWindow->setCentralWidget(_endGameMenu);
	_mainWindow->show();
}
void Game::GameOverScreen()
{
	timer.stop();
	if (_manette)
	{
		controllerTimer.stop();
		//comm->ClosePort();
	}
	view.close();
	delete _map;
	_mainWindow->show();
}

void Game::BetweenLevelScreen()
{
	timer.stop();
	if (_manette)
	{
		controllerTimer.stop();
		comm->ClosePort();
	}
	view.close();
	delete _map;
	_betweenLevelMenu = new BetweenLevelMenu(_currentLevel);
	connect(_betweenLevelMenu, &BetweenLevelMenu::LoadLevel, this, &Game::LoadLevel);
	connect(_betweenLevelMenu, &BetweenLevelMenu::BackToMainMenu, this, &Game::ShowMainMenu);
	_mainWindow->setStyleSheet("QMainWindow {" "background-image: url(./sprite/menu/fractal-1722991_1920.jpg);" "}");
	_mainWindow->setCentralWidget(_betweenLevelMenu);
	_mainWindow->show();
}


void Game::ChooseLevel()
{
	_levelSelection = new LevelSelection();
	connect(_levelSelection, &LevelSelection::levelSelected, this, &Game::LoadLevel);
	connect(_levelSelection, &LevelSelection::returnToMainMenu, this, &Game::ShowMainMenu);
	_mainWindow->setStyleSheet("QMainWindow {" "background-image: url(./sprite/menu/fractal-1722991_1920.jpg);" "}");
	_mainWindow->setCentralWidget(_levelSelection);
}

void Game::Menu()
{
	if (_manette)
		comm->OpenPort();
}

void Game::GetInput()
{
	data.jump = false;
	data.interact = false;
	data.switchChars = false;
	data.menu = false;

	data.moveRight = false;
	data.moveLeft = false;

	parse_status = comm->GetInputData();

	if (parse_status)
	{
		data.jump = comm->rcv_msg["boutons"]["3"] == 1;
		data.interact = comm->rcv_msg["boutons"]["2"] == 1 || std::stof(std::string(comm->rcv_msg["accel"]["z"])) > 0.3;
		data.switchChars = comm->rcv_msg["boutons"]["1"] == 1;
		data.menu = comm->rcv_msg["boutons"]["4"] == 1;

		data.moveRight = std::stof(std::string(comm->rcv_msg["joystick"]["x"])) < -0.5;
		data.moveLeft = std::stof(std::string(comm->rcv_msg["joystick"]["x"])) > 0.5;

		data.random = comm->rcv_msg["random"];
	}

	if (data.jump || data.interact || data.moveRight || data.moveLeft)
		_updated = true;
}

void Game::SendResponse()
{

	if (_map->GetActiveCharacter()->getElement() == WATER)
	{
		comm->send_msg["joueur"] = 2;
	}

	else
	{
		comm->send_msg["joueur"] = 1;
	}
	

	if (_codegiven) {
		comm->send_msg["seg"] = _code;
	}

	else {
		comm->send_msg["seg"] = DISPLAY_OFF;
	}


	comm->send_msg["lcd"] = std::to_string(_currentLevel);

	Sleep(10);

	comm->SendToPort(comm->send_msg);

}

void Game::NextLevel()
{
	timer.stop();
	if (_manette)
	{
		controllerTimer.stop();
		//comm->ClosePort();
	}
	view.close();

	if(_map)
		delete _map;

	if (_currentLevel == 0)
	{
		ShowMainMenu();
		_mainWindow->show();
		return;
	}

	_currentLevel++;
}

void Game::ControllerLoop()
{
	GetInput();
	CreateInputEvent();
	SendResponse();
}

void Game::CreateInputEvent()
{
	QKeyEvent* moveLeft = new QKeyEvent(QEvent::KeyPress, Qt::Key_A, Qt::NoModifier);
	QKeyEvent* moveRight = new QKeyEvent(QEvent::KeyPress, Qt::Key_D, Qt::NoModifier);
	QKeyEvent* Jump = new QKeyEvent(QEvent::KeyPress, Qt::Key_W, Qt::NoModifier);
	QKeyEvent* Switch = new QKeyEvent(QEvent::KeyPress, Qt::Key_Q, Qt::NoModifier);
	QKeyEvent* Interact = new QKeyEvent(QEvent::KeyPress, Qt::Key_E, Qt::NoModifier);
	QKeyEvent* Menu = new QKeyEvent(QEvent::KeyPress, Qt::Key_M, Qt::NoModifier);
	QKeyEvent* stopMoveLeft = new QKeyEvent(QEvent::KeyRelease, Qt::Key_A, Qt::NoModifier);
	QKeyEvent* stopMoveRight = new QKeyEvent(QEvent::KeyRelease, Qt::Key_D, Qt::NoModifier);

	if (data.jump)
		QApplication::postEvent(_map, Jump);
	if (data.interact)
	{
		QApplication::postEvent(_map, Interact);
		if (_manette)
			Sleep(20);
	}
	if (data.switchChars)
	{
		QApplication::postEvent(_map, Switch);
		if (_manette)
			Sleep(20);
	}
	if (data.menu)
		QApplication::postEvent(_map, Menu);
	if (data.moveRight)
		QApplication::postEvent(_map, moveRight);
	if (data.moveLeft)
		QApplication::postEvent(_map, moveLeft);
	if (!data.moveRight && !data.moveLeft)
		QApplication::postEvent(_map, stopMoveRight);
}

void Game::SendDigitsToController(const QString& s)
{
	int code = s.toInt();

	_code = code;
	_codegiven = true;
}

void Game::Play()
{
	if (_manette)
	{
		controllerTimer.start(1000 / 15);
	}
	_map->ReadMap();
	view.setRenderHint(QPainter::Antialiasing);
	view.setScene(_map);
	view.resize(1920, 1080);;
	view.show();
	_mainWindow->close();

	QObject::connect(&timer, &QTimer::timeout, _map, &Map::UpdateScene);
	timer.start(1000 / 60);
}


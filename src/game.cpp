#include "game.h"

#include <iostream>

#include "conio.h"
#include "pool.h"
#include "Windows.h"
#include <stdlib.h>
#include <string>

Game::Game(const char* port, QObject* parent) : QObject(parent)
{
	comm = new Communication(port, true);

	_manette = comm->IsConnected();
	std::cout << _manette << std::endl;

	_mainWindow = new QMainWindow();
	_mainMenu = new MainMenu();
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


	_mainWindow->resize(1280, 720);
	_mainWindow->setStyleSheet("QMainWindow {" "background-image: url(./sprite/menu/fractal-1722991_1920.jpg);" "}");
	_mainWindow->setCentralWidget(_mainMenu);
	_mainWindow->show();
}

Game::~Game()
{
	delete comm;
}

void Game::LoadLevel(int level)
{
	_currentLevel = level;

	_gameOver = _isJumping = _levelFinished = _codegiven = false;
	_jumpHeight = 0;

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
		_map = new Map("./map/3.txt", &view);
		break;
	case 4:
		_map = new Map("./map/4.txt", &view);
		break;
	case 5:
		_map =  new Map("./map/5.txt", &view);
		break;
	case 6:
		system("CLS");
		std::cout << "FELICITATION!!! MERCI D'AVOIR JOUE!!!" << std::endl;
		Sleep(2000);
		_currentLevel = 1;
		//MainMenu();
		break;
	}
	connect(_map, &Map::GameOver, this, &Game::GameOverScreen);
	connect(_map, &Map::LevelFinished, this, &Game::NextLevel);
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

void Game::ShowMainMenu()
{
	_mainMenu = new MainMenu();
	connect(_mainMenu, &MainMenu::levelSelected, this, &Game::LoadLevel);
	connect(_mainMenu, &MainMenu::levelSelection, this, &Game::ChooseLevel);
	connect(_mainMenu, &MainMenu::ShowTutorialScreen, this, &Game::ShowTutorialScreen);
	_mainWindow->setStyleSheet("QMainWindow {" "background-image: url(./sprite/menu/fractal-1722991_1920.jpg);" "}");
	_mainWindow->setCentralWidget(_mainMenu);
}

void Game::GameOverScreen()
{
	timer.stop();
	view.close();
	_map->StopTimer();
	delete _map;
	ShowMainMenu();
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
	if (_manette)
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
		}
	}
	else if (!_manette)
	{
		data.jump = GetAsyncKeyState('W') & 0x8000;
		data.interact = GetAsyncKeyState('E') & 0x8000;
		data.switchChars = GetAsyncKeyState('Q') & 0x8000;
		data.menu = GetAsyncKeyState('M') & 0x8000;

		data.moveRight = GetAsyncKeyState('D') & 0x8000;
		data.moveLeft = GetAsyncKeyState('A') & 0x8000;

		if (data.interact || data.switchChars)
			Sleep(50);
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
	view.close();
	_map->StopTimer();
	delete _map;
	if (_currentLevel == 0)
	{
		ShowMainMenu();
		_mainWindow->show();
		return;
	}
	_currentLevel++;
	LoadLevel(_currentLevel);
}

void Game::Play()
{
	//system("CLS");
	_map->ReadMap();
	view.setRenderHint(QPainter::Antialiasing);
	view.setScene(_map);
	view.resize(1920, 1080);;
	view.show();
	_mainWindow->close();

	QObject::connect(&timer, &QTimer::timeout, _map, &Map::UpdateScene);
	timer.start(1000 / 60);
}

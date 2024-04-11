#include "game.h"
#include <iostream>
#include "conio.h"
#include "pool.h"
#include "Windows.h"
#include <stdlib.h>
#include <string>

//using namespace std;


Game::Game(const char* port, QObject* parent) 
	: QObject(parent)
{
	comm = new Communication(port, true);

	_manette = comm->IsConnected();
	std::cout <<  _manette << std::endl;

	_mainWindow = new QMainWindow();
	_mainMenu = new MainMenu();
	_inGameMenu = new InGameMenu();
	_levelSelection = new LevelSelection();

	connect(_mainMenu, &MainMenu::levelSelected, this, &Game::LoadLevel);
	connect(_mainMenu, &MainMenu::levelSelection, this, &Game::ChooseLevel);
	connect(_levelSelection, &LevelSelection::levelSelected, this, &Game::LoadLevel);
	connect(_levelSelection, &LevelSelection::returnToMainMenu, this, &Game::ShowMainMenu);
	connect(_inGameMenu, &InGameMenu::resumeGame, this, &Game::ResumeGame);
	connect(_inGameMenu, &InGameMenu::quitToMainMenu, this, &Game::ShowMainMenu);
	connect(_inGameMenu, &InGameMenu::restartGame, this, &Game::RestartGame);
	
	_mainWindow->resize(1280, 720);
	_mainWindow->setStyleSheet("QMainWindow {" "background-image: url(./sprite/menu/fractal-1722991_1920.jpg);" "}");
	_mainWindow->setCentralWidget(_mainMenu);
	_mainWindow->show();

	//_mainWindow->setStyleSheet("QMainWindow {" "background-image: url(./sprite/menu/fractal-1722991_1920.jpg);" "}");
	//_mainWindow->setCentralWidget(_inGameMenu);

	//_map = Map("./map/testCode.txt");
}

Game::~Game()
{
	delete comm;
}

void Game::LoadLevel(int level)
{
	//Sert probablement à rien, puisque que tu redéfinis le niveau tout de suite après
	_currentLevel = level;
	_map.Clear();

	_gameOver = _isJumping = _levelFinished = _codegiven = false;
	_jumpHeight = 0;

	switch (_currentLevel)
	{
	case 0:
		_map = Map("./map/0.txt");
		break;
	case 1:
		_map = Map("./map/1.txt");
		break;
	case 2:
		_map = Map("./map/2.txt");
		break;
	case 3:
		_map = Map("./map/3.txt");
		break;
	case 4:
		_map = Map("./map/4.txt");
		break;
	case 5:
		_map = Map("./map/5.txt");
		break;
	case 6:
		system("CLS");
		std::cout << "FELICITATION!!! MERCI D'AVOIR JOUE!!!" << std::endl;
		Sleep(2000);
		_currentLevel = 1;
		//MainMenu();
		break;
	}
	Play();
}

void Game::ResumeGame()
{
	_inGameMenu->hide();
}

void Game::RestartGame()
{
	LoadLevel(_currentLevel); 
}


void Game::ShowMainMenu()
{
//	int userInput = AskMainMenuInput();
//
//	switch (userInput)
//	{
//	case 1:
//		_currentLevel = 0;
//		NewLevel();
//		Play();
//	case 2:
//		NewLevel();
//		Play();
//		break;
//	case 3:
//		ChooseLevel();
//		break;
//	case 4:
//		system("CLS");
//		std::cout << "A la prochaine" << std::endl;
//		Sleep(1000);
//		system("CLS");
//		exit(1);
//		break;
//	default:
//		break;
//	}

	_mainMenu = new MainMenu();
	connect(_mainMenu, &MainMenu::levelSelected, this, &Game::LoadLevel);
	connect(_mainMenu, &MainMenu::levelSelection, this, &Game::ChooseLevel);
	_mainWindow->setCentralWidget(_mainMenu);
}


void Game::ChooseLevel()
{
	//int userInput;

	//system("CLS");
	//std::cout << "SELECTION DE NIVEAU" << std::endl;
	//std::cout << "\nENTREZ UN ENTIER DE 1 A 5" << std::endl;
	//std::cin.clear();
	//fflush(stdin);

	//do
	//{
	//	std::cin >> userInput;

	//} while (userInput < 1 || userInput > 5);
	//	
	//_currentLevel = userInput;
	//NewLevel();
	//Play();

	_levelSelection = new LevelSelection();
	connect(_levelSelection, &LevelSelection::levelSelected, this, &Game::LoadLevel);
	connect(_levelSelection, &LevelSelection::returnToMainMenu, this, &Game::ShowMainMenu);
	_mainWindow->setCentralWidget(_levelSelection);
}


void Game::ShowInGameMenu()
{
	//int userInput = AskMenuInput();
	//
	//switch (userInput)
	//{
	//case 1:
	//	_map.ShowMap();
	//	break;
	//case 2:
	//	//NewLevel();
	//	Play();
	//	break;
	//case 3:
	//	_currentLevel = 1;
	//	_map.Clear();
	//	//MainMenu();
	//	break;
	//default:
	//	break;
	//}
	//
	//if (_manette)
	//	comm->OpenPort();

		_inGameMenu = new InGameMenu();
		connect(_inGameMenu, &InGameMenu::resumeGame, this, &Game::ResumeGame);
		connect(_inGameMenu, &InGameMenu::quitToMainMenu, this, &Game::ShowMainMenu);
		_mainWindow->setCentralWidget(_inGameMenu);
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

	if (_map.GetActiveCharacter()->getElement() == WATER)
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

void Game::MovePlayers()
{
	std::vector<std::vector<Tile*>> &grid = *_map.GetGrid();
	Coordinate ActivePlayerPos = _map.GetActiveCharacter()->GetPosition();

	std::chrono::duration<double> elapsed_time = std::chrono::system_clock::now() - _start;
	Coordinate ActivePlayerOldPos;
	
	if (_isJumping && elapsed_time < std::chrono::milliseconds{ 750 } && _jumpHeight < 3)
	{
		if (grid[ActivePlayerPos.y - 1][ActivePlayerPos.x]->GetType() == TILE)
		{
			ActivePlayerOldPos = _map.GetActiveCharacter()->GetPosition();
			_map.GetActiveCharacter()->SetPosition(ActivePlayerPos.x, ActivePlayerPos.y - 1);
			ActivePlayerPos = _map.GetActiveCharacter()->GetPosition();
			_map.Swap(ActivePlayerOldPos, ActivePlayerPos);
			_jumpHeight++;
		}
	}

	if (data.jump)
	{ 
		if (grid[ActivePlayerPos.y + 1][ActivePlayerPos.x]->GetType() == CHARACTER)
		{
			return;

		}
		if (_isJumping == false)
		{
				_start = std::chrono::system_clock::now();
				_isJumping = true;
			
			if (grid[ActivePlayerPos.y - 2][ActivePlayerPos.x]->GetType() == GATE)
				{
				Gate* thisGate = static_cast<Gate*>(grid[ActivePlayerPos.y - 2][ActivePlayerPos.x]);
				if (thisGate->GetState() == OPEN)
				{
					ActivePlayerOldPos = _map.GetActiveCharacter()->GetPosition();
					_map.GetActiveCharacter()->SetPosition(ActivePlayerPos.x, ActivePlayerPos.y - 3);
					ActivePlayerPos = _map.GetActiveCharacter()->GetPosition();
					_map.Swap(ActivePlayerOldPos, ActivePlayerPos);
					_jumpHeight += 3;
				}
				else if (thisGate->GetState() == CLOSED)
				{
					ActivePlayerOldPos = _map.GetActiveCharacter()->GetPosition();
					_map.GetActiveCharacter()->SetPosition(ActivePlayerPos.x, ActivePlayerPos.y - 1);
					ActivePlayerPos = _map.GetActiveCharacter()->GetPosition();
					_map.Swap(ActivePlayerOldPos, ActivePlayerPos);
					_jumpHeight++;
				}
			}
			else if (grid[ActivePlayerPos.y - 1][ActivePlayerPos.x]->GetType() == TILE)
			{
				ActivePlayerOldPos = _map.GetActiveCharacter()->GetPosition();
				_map.GetActiveCharacter()->SetPosition(ActivePlayerPos.x, ActivePlayerPos.y - 1);
				ActivePlayerPos = _map.GetActiveCharacter()->GetPosition();
				_map.Swap(ActivePlayerOldPos, ActivePlayerPos);
				_jumpHeight++;
			}
		}
		
	}

	if (data.moveLeft)
	{
		if (grid[ActivePlayerPos.y][ActivePlayerPos.x - 1]->GetType() == GATE)
		{
			Gate* thisGate = static_cast<Gate*>(grid[ActivePlayerPos.y][ActivePlayerPos.x - 1]);

			if (thisGate->GetState() == CLOSED)
				return;
			if (thisGate->GetState() == OPEN)
			{
				ActivePlayerOldPos = _map.GetActiveCharacter()->GetPosition();
				_map.GetActiveCharacter()->SetPosition(ActivePlayerPos.x - 2, ActivePlayerPos.y);
				ActivePlayerPos = _map.GetActiveCharacter()->GetPosition();
				_map.Swap(ActivePlayerOldPos, ActivePlayerPos);
			}
		}

		else if (grid[ActivePlayerPos.y][ActivePlayerPos.x - 1]->GetType() == TILE)
		{
			ActivePlayerOldPos = _map.GetActiveCharacter()->GetPosition();
			_map.GetActiveCharacter()->SetPosition(ActivePlayerPos.x - 1, ActivePlayerPos.y);
			ActivePlayerPos = _map.GetActiveCharacter()->GetPosition();
			_map.Swap(ActivePlayerOldPos, ActivePlayerPos);
		}
	}
	if (data.moveRight)
	{
		if (grid[ActivePlayerPos.y][ActivePlayerPos.x + 1]->GetType() == GATE)
		{
			Gate* thisGate = static_cast<Gate*>(grid[ActivePlayerPos.y][ActivePlayerPos.x + 1]);

			if (thisGate->GetState() == CLOSED)
				return;
			if (thisGate->GetState() == OPEN)
			{
				ActivePlayerOldPos = _map.GetActiveCharacter()->GetPosition();
				_map.GetActiveCharacter()->SetPosition(ActivePlayerPos.x + 2, ActivePlayerPos.y);
				ActivePlayerPos = _map.GetActiveCharacter()->GetPosition();
				_map.Swap(ActivePlayerOldPos, ActivePlayerPos);
			}
		}

		else if (grid[ActivePlayerPos.y][ActivePlayerPos.x + 1]->GetType() == TILE)
		{
			ActivePlayerOldPos = _map.GetActiveCharacter()->GetPosition();
			_map.GetActiveCharacter()->SetPosition(ActivePlayerPos.x + 1, ActivePlayerPos.y);
			ActivePlayerPos = _map.GetActiveCharacter()->GetPosition();
			_map.Swap(ActivePlayerOldPos, ActivePlayerPos);
		}
	}

	if (data.switchChars && !_isJumping)
	{
		_map.SwitchCharacter();
	}

	if (data.interact)
	{
		Interact();
	}

	if (data.menu)
	{
		ShowInGameMenu();
	}
}

void Game::Play()
{
	system("CLS");
	_map.ReadMap();
	_map.ShowMap();

	if(_manette)
		comm->OpenPort();

	do
	{
		GetInput();
		MovePlayers();
		CheckPosition();
		CheckButtons();
		CheckExits();
		CheckPools();

		if(_manette)
			SendResponse();

		if (_updated)
		{
			system("CLS");
			_map.ShowMap();
			_updated = false;
		}

		if (_manette)
			Sleep(10);

		else
			Sleep(50);

	} while (!_gameOver && !_levelFinished);

	system("CLS");
	if (_gameOver)
  {
		std::cout << "Gameover\n";
		Sleep(2000);
		//NewLevel();
		_gameOver = false;
		Play();
	}
	else if (_currentLevel == 0 && _levelFinished == true)
	{
		_map.Clear();
		_currentLevel = 1;
		//MainMenu();
	}
	else if (_levelFinished)
	{
		_currentLevel++;
		//NewLevel();
		_levelFinished = false;
		Play();
	}
		
}

void Game::CheckPosition()
{
	std::vector<std::vector<Tile*>>& grid = *_map.GetGrid();
	Coordinate ActivePlayerPos = _map.GetActiveCharacter()->GetPosition();
	std::chrono::duration<double> elapsed_time = std::chrono::system_clock::now() - _start;
	Coordinate ActivePlayerOldPos;

	if (grid[ActivePlayerPos.y + 1][ActivePlayerPos.x]->GetType() == TILE)
	{
		if (_isJumping && elapsed_time > std::chrono::milliseconds{ 750 })
		{
			ActivePlayerOldPos = _map.GetActiveCharacter()->GetPosition();
			_map.GetActiveCharacter()->SetPosition(ActivePlayerPos.x, ActivePlayerPos.y + 1);
			ActivePlayerPos = _map.GetActiveCharacter()->GetPosition();
			_map.Swap(ActivePlayerOldPos, ActivePlayerPos);
			_jumpHeight--; 

			if (grid[ActivePlayerPos.y + 1][ActivePlayerPos.x]->GetType() == WALL)
			{
				_isJumping = false;
				_jumpHeight = 0;
			}
		}
		else if (!_isJumping)
		{
			ActivePlayerOldPos = _map.GetActiveCharacter()->GetPosition();
			_map.GetActiveCharacter()->SetPosition(ActivePlayerPos.x, ActivePlayerPos.y + 1);
			ActivePlayerPos = _map.GetActiveCharacter()->GetPosition();
			_map.Swap(ActivePlayerOldPos, ActivePlayerPos);
			_jumpHeight--;
		}
		
		_updated = true;
	}
	else if (grid[ActivePlayerPos.y + 1][ActivePlayerPos.x]->GetType() == GATE)
	{
		Gate* thisGate = static_cast<Gate*>(grid[ActivePlayerPos.y + 1][ActivePlayerPos.x]);
		if (thisGate->GetState() == OPEN)
		{
			if (_isJumping && elapsed_time > std::chrono::milliseconds{ 750 })
			{
				ActivePlayerOldPos = _map.GetActiveCharacter()->GetPosition();
				_map.GetActiveCharacter()->SetPosition(ActivePlayerPos.x, ActivePlayerPos.y + 2);
				ActivePlayerPos = _map.GetActiveCharacter()->GetPosition();
				_map.Swap(ActivePlayerOldPos, ActivePlayerPos);
				_jumpHeight--;

				if (grid[ActivePlayerPos.y + 1][ActivePlayerPos.x]->GetType() == WALL)
				{
					_isJumping = false;
					_jumpHeight = 0;
				}
			}
			else if (!_isJumping)
			{
				ActivePlayerOldPos = _map.GetActiveCharacter()->GetPosition();
				_map.GetActiveCharacter()->SetPosition(ActivePlayerPos.x, ActivePlayerPos.y + 2);
				ActivePlayerPos = _map.GetActiveCharacter()->GetPosition();
				_map.Swap(ActivePlayerOldPos, ActivePlayerPos);
			}
		}
	}
	else
	{
		_isJumping = false;
		_jumpHeight = 0;
	}
}

void Game::CheckGates()
{
	for (int i = 0; i < _map.GetGates().size(); i++)
	{
		_map.GetGates()[i]->CheckControllers();
	}
}

void Game::CheckButtons()
{
	std::vector<std::vector<Tile*>> &grid = *_map.GetGrid();
	Coordinate coord;
	for (int i = 0; i < _map.GetButton().size(); i++)
	{
		coord = _map.GetButton()[i]->GetPosition();

		if (grid[coord.y - 1][coord.x]->GetType() == CHARACTER)
		{
			_map.GetButton()[i]->SetState(OPEN);
			CheckGates();
		}
		else
		{
			_map.GetButton()[i]->SetState(CLOSED);
			CheckGates();
		}
	}
}

void Game::CheckPools(){
	// Personnage actif
	int x = _map.GetActiveCharacter()->GetPosition().x;
	int y = _map.GetActiveCharacter()->GetPosition().y;

	// Arrête la fonction si le personnage n'est pas au-dessus d'une pool
	if (_map.GetPoolAt(x, y + 1) == nullptr) {
		return;
	}

	// Termine la partie si l'élément n'est pas le même que celui du personnage
	if (_map.GetPoolAt(x, y + 1)->GetElement() != _map.GetActiveCharacter()->getElement()) {
		_gameOver = true;
	}

}

void Game::CheckExits()
{
	std::vector<std::vector<Tile*>> &grid = *_map.GetGrid();
	Coordinate coord;
	for (int i = 0; i < _map.GetExit().size(); i++)
	{
		coord = _map.GetExit()[i]->GetPosition();

		if (grid[coord.y - 1][coord.x]->GetType() == CHARACTER)
		{
			_map.GetExit()[i]->SetState(OPEN);
		}
		else
		{
			_map.GetExit()[i]->SetState(CLOSED);
		}
	}

	for (int y = 0; y < _map.GetExit().size(); y++)
	{
		if (_map.GetExit()[y]->GetState() == CLOSED)
		{
			_levelFinished = false;
			return;
		}
		else
		{
			_levelFinished = true;
		}
	}
}

void Game::Interact()
{
	std::vector<std::vector<Tile*>> &grid = *_map.GetGrid();
	Coordinate ActivePlayerPos = _map.GetActiveCharacter()->GetPosition();


	if (grid[ActivePlayerPos.y + 1][ActivePlayerPos.x]->GetType() == LEVER)
	{
		Lever* thisLever = static_cast<Lever*>(grid[ActivePlayerPos.y + 1][ActivePlayerPos.x]);
		if (thisLever->GetState() == CLOSED)
			thisLever->SetState(OPEN);
		else
		{
			thisLever->SetState(CLOSED);
		}
		CheckGates();
	}
	else if (grid[ActivePlayerPos.y + 1][ActivePlayerPos.x]->GetType() == CODELOCK)
	{
		CodeLock* thisCodeLock = static_cast<CodeLock*>(grid[ActivePlayerPos.y + 1][ActivePlayerPos.x]);
		if (thisCodeLock->GetState() == OPEN)
			return;
		else
		{
			thisCodeLock->VerifyCode();
		}
		CheckGates();
		if(_manette)
			comm->OpenPort();
	}
	else if (grid[ActivePlayerPos.y + 1][ActivePlayerPos.x]->GetType() == CODEGIVER)
	{
		CodeGiver* thisCodeGiver = static_cast<CodeGiver*>(grid[ActivePlayerPos.y + 1][ActivePlayerPos.x]);
		int code = stoi(thisCodeGiver->ShowCode());

		_code = code;
		_codegiven = true;

		if (!_manette)
		{
			std::cout << code << std::endl;
			Sleep(2000);
		}
	}
}
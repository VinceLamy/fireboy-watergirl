#include "game.h"

#include <iostream>

#include "conio.h"
#include "pool.h"
#include "Windows.h"
#include <stdlib.h>
#include <string>

using namespace std;


Game::Game(const char* port)
{
	_gameOver = _isJumping = _wasButton = _levelFinished = false;
	_jumpHeight = 0;

	comm = new Communication(port, true);

	_manette = comm->IsConnected();
	cout <<  _manette << endl;
	MainMenu();
	
	//_map = Map("./map/testCode.txt");
}

Game::~Game()
{
	delete comm;
}

void Game::NewLevel()
{
	_map.Clear();

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
	}
}

void Game::MainMenu()
{
	int userInput = AskMainMenuInput();

	switch (userInput)
	{
	case 1:
		_currentLevel = 0;
		NewLevel();
		Play();
	case 2:
		NewLevel();
		Play();
		break;
	case 3:
		ChooseLevel();
		break;
	case 4:
		exit(1);
		break;
	default:
		break;
	}
}

void Game::ChooseLevel()
{
	int userInput;

	system("CLS");
	cout << "SELECTION DE NIVEAU" << endl;
	cout << "\nENTREZ UN ENTIER DE 1 A 5" << endl;
	cin.clear();
	fflush(stdin);

	do
	{
		cin >> userInput;

	} while (userInput < 1 || userInput > 5);
		
	_currentLevel = userInput;
	NewLevel();
	Play();
}

int Game::AskMainMenuInput()
{
	int userInput = 0;

	system("CLS");
	cout << "MENU PRINCIPAL" << endl;
	cout << "\n1-TUTORIEL" << endl;
	cout << "2-NOUVELLE PARTIE" << endl;
	cout << "3-CHOISIR NIVEAU" << endl;
	cout << "4-QUITTER\n" << endl;
	cin.clear();
	fflush(stdin);

	do
	{
		cin >> userInput;
	} while (userInput < 1 || userInput > 4);

	return userInput;
}

void Game::Menu()
{
	int userInput = AskMenuInput();

	switch (userInput)
	{
	case 1:
		_map.ShowMap();
		break;
	case 2:
		NewLevel();
		Play();
		break;
	case 3:
		_currentLevel = 1;
		_map.Clear();
		MainMenu();
		break;
	default:
		break;
	}

	if (_manette)
		comm->OpenPort();
}


int Game::AskMenuInput()
{
	int userInput;

	system("CLS");

	cout << "MENU" << endl;
	cout << "\n1-CONTINUER" << endl;
	cout << "2-RECOMMENCER" << endl;
	cout << "3-QUITTER\n" << endl;

	cin.clear();
	fflush(stdin);

	do
	{
		cin >> userInput;
	} while (userInput < 1 || userInput > 3);

	return userInput;
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

			//std::cout << "jump" << std::endl;
			data.jump = comm->rcv_msg["boutons"]["3"] == 1;
			data.interact = comm->rcv_msg["boutons"]["2"] == 1 || std::stof(std::string(comm->rcv_msg["accel"]["z"])) > 0.3;
			data.switchChars = comm->rcv_msg["boutons"]["1"] == 1;
			data.menu = comm->rcv_msg["boutons"]["4"] == 1;

			size_t len;
			data.moveRight = std::stof(std::string(comm->rcv_msg["joystick"]["x"])) < -0.5;
			data.moveLeft = std::stof(std::string(comm->rcv_msg["joystick"]["x"])) > 0.5;
		}
	}
	else if (!_manette)
	{
		data.jump = false;
		data.interact = false;
		data.switchChars = false;
		data.menu = false;

		data.moveRight = false;
		data.moveLeft = false;

		data.jump = GetAsyncKeyState('W') & 0x8000;
		data.interact = GetAsyncKeyState('E') & 0x8000;
		data.switchChars = GetAsyncKeyState('Q') & 0x8000;
		data.menu = GetAsyncKeyState('M') & 0x8000;

		size_t len;
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
	if (comm->rcv_msg["boutons"]["1"] == 1)
	{
		if (etat_joueur == 0)
		{
			comm->send_msg["joueur"] = 2;
			etat_joueur = 1;
		}
		else if (etat_joueur == 1)
		{
			comm->send_msg["joueur"] = 1;
			etat_joueur = 0;
		}
	}

	int deltaT = 0;

	//if (parse_status)
	//	deltaT = comm->rcv_msg["dt"].template get<int>();
	//else
	//{
	//	deltaT = 50;
	//}

	//dt += deltaT;

	//if (dt >= 1000)
	//{
	//	--compteur_depart;
	//	dt = 0;
	//}


	//if (compteur_depart <= 0)
	//	compteur_depart = VALEUR_TIMER;


	//comm->send_msg["seg"] = compteur_depart;

	comm->send_msg["lcd"] = to_string(_currentLevel);

	Sleep(10);

	comm->SendToPort(comm->send_msg);

}

void Game::MovePlayers()
{
	vector<vector<Tile*>> &grid = *_map.GetGrid();
	Coordinate ActivePlayerPos = _map.GetActiveCharacter()->GetPosition();
	chrono::duration<double> elapsed_time = chrono::system_clock::now() - _start;
	Coordinate ActivePlayerOldPos;
	
	if (_isJumping && elapsed_time < chrono::milliseconds{ 750 } && _jumpHeight < 3)
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
		if (_isJumping == false)
		{
			_start = chrono::system_clock::now();
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

		if (grid[ActivePlayerPos.y + 1][ActivePlayerPos.x - 1]->GetType() == POOL)
		{
			Pool* pool = _map.GetPoolAt(ActivePlayerPos.x - 1, ActivePlayerPos.y + 1);
			if (pool->GetElement() != _map.GetActiveCharacter()->getElement())
				_gameOver = true;
			else
			{
				ActivePlayerOldPos = _map.GetActiveCharacter()->GetPosition();
				_map.GetActiveCharacter()->SetPosition(ActivePlayerPos.x - 1, ActivePlayerPos.y);
				ActivePlayerPos = _map.GetActiveCharacter()->GetPosition();
				_map.Swap(ActivePlayerOldPos, ActivePlayerPos);
			}
		}

		if (grid[ActivePlayerPos.y][ActivePlayerPos.x - 1]->GetType() == TILE)
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

		if (grid[ActivePlayerPos.y + 1][ActivePlayerPos.x + 1]->GetType() == POOL)
		{
			Pool* pool = _map.GetPoolAt(ActivePlayerPos.x + 1, ActivePlayerPos.y + 1);
			if (pool->GetElement() != _map.GetActiveCharacter()->getElement())
				_gameOver = true;
			else
			{
				ActivePlayerOldPos = _map.GetActiveCharacter()->GetPosition();
				_map.GetActiveCharacter()->SetPosition(ActivePlayerPos.x + 1, ActivePlayerPos.y);
				ActivePlayerPos = _map.GetActiveCharacter()->GetPosition();
				_map.Swap(ActivePlayerOldPos, ActivePlayerPos);
			}
		}
		if (grid[ActivePlayerPos.y][ActivePlayerPos.x + 1]->GetType() == TILE)
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
		Menu();
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
		cout << "Gameover\n";
		Sleep(2000);
		NewLevel();
		_gameOver = false;
		Play();
	}
	else if (_currentLevel == 0 && _levelFinished == true)
	{
		_map.Clear();
		_currentLevel = 1;
		MainMenu();
	}
	else if (_levelFinished)
	{
		_currentLevel++;
		NewLevel();
		_levelFinished = false;
		Play();
	}
		
}

void Game::CheckPosition()
{
	vector<vector<Tile*>>& grid = *_map.GetGrid();
	Coordinate ActivePlayerPos = _map.GetActiveCharacter()->GetPosition();
	chrono::duration<double> elapsed_time = chrono::system_clock::now() - _start;
	Coordinate ActivePlayerOldPos;

	if (grid[ActivePlayerPos.y + 1][ActivePlayerPos.x]->GetType() == TILE)
	{
		if (_isJumping && elapsed_time > chrono::milliseconds{ 750 })
		{
			ActivePlayerOldPos = _map.GetActiveCharacter()->GetPosition();
			_map.GetActiveCharacter()->SetPosition(ActivePlayerPos.x, ActivePlayerPos.y + 1);
			ActivePlayerPos = _map.GetActiveCharacter()->GetPosition();
			_map.Swap(ActivePlayerOldPos, ActivePlayerPos);
			_jumpHeight--; 

			if (_jumpHeight == 0 || grid[ActivePlayerPos.y + 1][ActivePlayerPos.x]->GetType() == WALL)
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
			if (_isJumping && elapsed_time > chrono::milliseconds{ 750 })
			{
				ActivePlayerOldPos = _map.GetActiveCharacter()->GetPosition();
				_map.GetActiveCharacter()->SetPosition(ActivePlayerPos.x, ActivePlayerPos.y + 2);
				ActivePlayerPos = _map.GetActiveCharacter()->GetPosition();
				_map.Swap(ActivePlayerOldPos, ActivePlayerPos);
				_jumpHeight--;

				if (_jumpHeight == 0 || grid[ActivePlayerPos.y + 1][ActivePlayerPos.x]->GetType() == WALL)
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
	vector<vector<Tile*>> &grid = *_map.GetGrid();
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
	vector<vector<Tile*>> &grid = *_map.GetGrid();
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
	vector<vector<Tile*>> &grid = *_map.GetGrid();
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
		if (!_manette)
		{
			cout << code << endl;
			Sleep(2000);
		}
		if(_manette)
			comm->send_msg["seg"] = code;
		
	}
}
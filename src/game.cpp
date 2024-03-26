#include "game.h"

#include <iostream>

#include "conio.h"
#include "pool.h"
#include "Windows.h"
#include <stdlib.h>
#include <string>

using namespace std;

Game::Game()
{
	MainMenu();
	//_map = Map("./map/testCode.txt");

	_gameOver = _isJumping = _wasButton = _levelFinished = false;
	_jumpHeight = 0;
}

Game::~Game()
{
	
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

	do
	{
		system("CLS");
		cout << "SELECTION DE NIVEAU" << endl;
		cout << "\nENTREZ UN ENTIER DE 1 A 5" << endl;
		cin >> userInput;

	} while (userInput < 1 || userInput > 5);
		
	_currentLevel = userInput;
	NewLevel();
	Play();
}

int Game::AskMainMenuInput()
{
	int userInput = 0;
	do
	{
		system("CLS");
		cout << "MENU PRINCIPAL" << endl;
		cout << "\n1-TUTORIEL" << endl;
		cout << "2-NOUVELLE PARTIE" << endl;
		cout << "3-CHOISIR NIVEAU" << endl;
		cout << "4-QUITTER\n" << endl;
		cin.clear();
		fflush(stdin);
		cin >> userInput;
	} while (userInput < 1 || userInput > 3);

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
}

int Game::AskMenuInput()
{
	int userInput = 0;
	do
	{
		system("CLS");
		cout << "MENU" << endl;
		cout << "\n1-CONTINUER" << endl;
		cout << "2-RECOMMENCER" << endl;
		cout << "3-QUITTER\n" << endl;
		cin.clear();
		fflush(stdin);
		cin >> userInput;
	} while (userInput < 1 || userInput > 3);

	return userInput;
}

void Game::GetInput()
{
	vector<vector<Tile*>> grid = _map.GetGrid();
	Coordinate ActivePlayerPos = _map.GetActiveCharacter()->GetPosition();
	chrono::duration<double> elapsed_time = chrono::system_clock::now() - _start;

	if(_isJumping && elapsed_time < chrono::milliseconds{750} && _jumpHeight < 3)
	{
		if (grid[ActivePlayerPos.y - 1][ActivePlayerPos.x]->GetType() == TILE)
		{
			_map.GetActiveCharacter()->SetPosition(ActivePlayerPos.x, ActivePlayerPos.y - 1);
			swap(grid[ActivePlayerPos.y - 1][ActivePlayerPos.x], grid[ActivePlayerPos.y][ActivePlayerPos.x]);
			_jumpHeight++;
		}
	}

	if(GetKeyState('W') & 0x8000)
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
					_map.GetActiveCharacter()->SetPosition(ActivePlayerPos.x, ActivePlayerPos.y - 3);
					swap(grid[ActivePlayerPos.y - 3][ActivePlayerPos.x], grid[ActivePlayerPos.y][ActivePlayerPos.x]);
					_jumpHeight += 3;
				}
				else if (thisGate->GetState() == CLOSED)
				{
					_map.GetActiveCharacter()->SetPosition(ActivePlayerPos.x, ActivePlayerPos.y - 1);
					swap(grid[ActivePlayerPos.y - 1][ActivePlayerPos.x], grid[ActivePlayerPos.y][ActivePlayerPos.x]);
					_jumpHeight++;
				}
			}
			else if (grid[ActivePlayerPos.y - 1][ActivePlayerPos.x]->GetType() == TILE)
			{
				_map.GetActiveCharacter()->SetPosition(ActivePlayerPos.x, ActivePlayerPos.y - 1);
				swap(grid[ActivePlayerPos.y - 1][ActivePlayerPos.x], grid[ActivePlayerPos.y][ActivePlayerPos.x]);
				_jumpHeight++;
			}
		}
	}
	if (GetKeyState('A') & 0x8000)
	{
		if (grid[ActivePlayerPos.y][ActivePlayerPos.x - 1]->GetType() == GATE)
		{
			Gate* thisGate = static_cast<Gate*>(grid[ActivePlayerPos.y][ActivePlayerPos.x - 1]);
			
			if (thisGate->GetState() == CLOSED)
				return;
			if (thisGate->GetState() == OPEN)
			{
				_map.GetActiveCharacter()->SetPosition(ActivePlayerPos.x - 2, ActivePlayerPos.y);
				swap(grid[ActivePlayerPos.y][ActivePlayerPos.x - 2], grid[ActivePlayerPos.y][ActivePlayerPos.x]);
			}
		}

		if (grid[ActivePlayerPos.y + 1][ActivePlayerPos.x - 1]->GetType() == POOL)
		{
			Pool* pool = _map.GetPoolAt(ActivePlayerPos.x - 1, ActivePlayerPos.y + 1);
			if (pool->GetElement() != _map.GetActiveCharacter()->getElement())
				_gameOver = true;
			else
			{
				_map.GetActiveCharacter()->SetPosition(ActivePlayerPos.x - 1, ActivePlayerPos.y);
				swap(grid[ActivePlayerPos.y][ActivePlayerPos.x - 1], grid[ActivePlayerPos.y][ActivePlayerPos.x]);
			}
		}
		if (grid[ActivePlayerPos.y][ActivePlayerPos.x - 1]->GetType() == TILE)
		{
			_map.GetActiveCharacter()->SetPosition(ActivePlayerPos.x - 1, ActivePlayerPos.y);
			swap(grid[ActivePlayerPos.y][ActivePlayerPos.x - 1], grid[ActivePlayerPos.y][ActivePlayerPos.x]);
		}
	}
	if (GetKeyState('D') & 0x8000)
	{
		if (grid[ActivePlayerPos.y][ActivePlayerPos.x + 1]->GetType() == GATE)
		{
			Gate* thisGate = static_cast<Gate*>(grid[ActivePlayerPos.y][ActivePlayerPos.x + 1]);

				if (thisGate->GetState() == CLOSED)
					return;
				if (thisGate->GetState() == OPEN)
				{
					_map.GetActiveCharacter()->SetPosition(ActivePlayerPos.x + 2, ActivePlayerPos.y);
					swap(grid[ActivePlayerPos.y][ActivePlayerPos.x + 2], grid[ActivePlayerPos.y][ActivePlayerPos.x]);
				}
		}

		if (grid[ActivePlayerPos.y + 1][ActivePlayerPos.x + 1]->GetType() == POOL)
		{
			Pool* pool = _map.GetPoolAt(ActivePlayerPos.x + 1, ActivePlayerPos.y + 1);
			if (pool->GetElement() != _map.GetActiveCharacter()->getElement())
				_gameOver = true;
			else
			{
				_map.GetActiveCharacter()->SetPosition(ActivePlayerPos.x + 1, ActivePlayerPos.y);
				swap(grid[ActivePlayerPos.y][ActivePlayerPos.x + 1], grid[ActivePlayerPos.y][ActivePlayerPos.x]);
			}
		}
		if (grid[ActivePlayerPos.y][ActivePlayerPos.x + 1]->GetType() == TILE)
		{
			_map.GetActiveCharacter()->SetPosition(ActivePlayerPos.x + 1, ActivePlayerPos.y);
			swap(grid[ActivePlayerPos.y][ActivePlayerPos.x + 1], grid[ActivePlayerPos.y][ActivePlayerPos.x]);
		}

		
	}

	if (GetKeyState('Q') & 0x8000)
	{
		_map.SwitchCharacter();
	}

	if (GetKeyState('E') & 0x8000)
	{
		Interact();
	}

	if (GetKeyState('M') & 0x8000)
	{
		Menu();
	}
	_map.SetGrid(grid);
}


void Game::Play()
{
	_map.ReadMap();
	_map.ShowMap();

	do
	{
		GetInput();
		CheckPosition();
		CheckButtons();
		CheckExits();
		system("CLS");
		_map.ShowMap();
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
	vector<vector<Tile*>> grid = _map.GetGrid();
	Coordinate ActivePlayerPos = _map.GetActiveCharacter()->GetPosition();
	chrono::duration<double> elapsed_time = chrono::system_clock::now() - _start;

	if (grid[ActivePlayerPos.y + 1][ActivePlayerPos.x]->GetType() == TILE)
	{
		if (_isJumping && elapsed_time > chrono::milliseconds{ 750 })
		{
			_map.GetActiveCharacter()->SetPosition(ActivePlayerPos.x, ActivePlayerPos.y + 1);
			swap(grid[ActivePlayerPos.y + 1][ActivePlayerPos.x], grid[ActivePlayerPos.y][ActivePlayerPos.x]);
			_jumpHeight--;

			if (_jumpHeight == 0 || grid[ActivePlayerPos.y + 1][ActivePlayerPos.x]->GetType() == TILE)
			{
				_isJumping = false;
				_jumpHeight = 0;
			}
		}
		else if (!_isJumping)
		{
			_map.GetActiveCharacter()->SetPosition(ActivePlayerPos.x, ActivePlayerPos.y + 1);
			swap(grid[ActivePlayerPos.y + 1][ActivePlayerPos.x], grid[ActivePlayerPos.y][ActivePlayerPos.x]);
		}
	}
	else if (grid[ActivePlayerPos.y + 1][ActivePlayerPos.x]->GetType() == GATE)
	{
		Gate* thisGate = static_cast<Gate*>(grid[ActivePlayerPos.y + 1][ActivePlayerPos.x]);
		if (thisGate->GetState() == OPEN)
		{
			if (_isJumping && elapsed_time > chrono::milliseconds{ 750 })
			{
				_map.GetActiveCharacter()->SetPosition(ActivePlayerPos.x, ActivePlayerPos.y + 2);
				swap(grid[ActivePlayerPos.y + 2][ActivePlayerPos.x], grid[ActivePlayerPos.y][ActivePlayerPos.x]);
				_jumpHeight--;

				if (_jumpHeight == 0 || grid[ActivePlayerPos.y + 1][ActivePlayerPos.x]->GetType() != TILE)
				{
					_isJumping = false;
					_jumpHeight = 0;
				}
			}
			else if (!_isJumping)
			{
				_map.GetActiveCharacter()->SetPosition(ActivePlayerPos.x, ActivePlayerPos.y + 2);
				swap(grid[ActivePlayerPos.y + 2][ActivePlayerPos.x], grid[ActivePlayerPos.y][ActivePlayerPos.x]);
			}
		}

	}
	else
	{
		_isJumping = false;
		_jumpHeight = 0;
	}

	_map.SetGrid(grid);
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
	vector<vector<Tile*>> grid = _map.GetGrid();
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

void Game::CheckExits()
{
	vector<vector<Tile*>> grid = _map.GetGrid();
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
	vector<vector<Tile*>> grid = _map.GetGrid();
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
	}
	else if (grid[ActivePlayerPos.y + 1][ActivePlayerPos.x]->GetType() == CODEGIVER)
	{
		CodeGiver* thisCodeGiver = static_cast<CodeGiver*>(grid[ActivePlayerPos.y + 1][ActivePlayerPos.x]);
		thisCodeGiver->ShowCode();
	}
}
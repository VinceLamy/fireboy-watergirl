#include "game.h"

#include <iostream>

#include "conio.h"
#include "pool.h"
#include "Windows.h"

using namespace std;

Game::Game()
{
	_map = Map("./map/0.txt");

	_gameOver = _isJumping = _wasButton = _levelFinished = false;
	_jumpHeight = 0;
}

Game::~Game()
{
	
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

			if (grid[ActivePlayerPos.y][ActivePlayerPos.x - 1]->GetType() == TILE)
			{
				_map.GetActiveCharacter()->SetPosition(ActivePlayerPos.x, ActivePlayerPos.y - 1);
				swap(grid[ActivePlayerPos.y - 1][ActivePlayerPos.x], grid[ActivePlayerPos.y][ActivePlayerPos.x]);
				_jumpHeight++;
			}
		}
	}
	if (GetKeyState('A') & 0x8000)
	{
		if (grid[ActivePlayerPos.y][ActivePlayerPos.x - 1]->GetType() == WALL)
			return;

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
		if (grid[ActivePlayerPos.y][ActivePlayerPos.x + 1]->GetType() == WALL)
			return;

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
		CheckGates();
		CheckExits();
		system("CLS");
		_map.ShowMap();
		Sleep(125);
	} while (!_gameOver && !_levelFinished);

	system("CLS");
	if(_gameOver)
		cout << "Gameover\n";
	if (_levelFinished)
		cout << "Good Job!!!\n";
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

			if (_jumpHeight == 0 || grid[ActivePlayerPos.y + 1][ActivePlayerPos.x]->GetType() != TILE)
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
		}
		else
		{
			_map.GetButton()[i]->SetState(CLOSED);
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
	}
}
#include "game.h"

#include <iostream>

#include "tile.h"
#include "enum.h"
#include "pool.h"
#include "Windows.h"

using namespace std;

Game::Game()
{
	_map = Map("./src/0.txt");

	_gameOver = _isJumping = _wasButton = false;
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

	if (_isJumping && elapsed_time < chrono::milliseconds{ 750 } && _jumpHeight < 3)
	{
		if (grid[ActivePlayerPos.y - 1][ActivePlayerPos.x]->GetType() == TILE)
		{
			_map.GetActiveCharacter()->SetPosition(ActivePlayerPos.x, ActivePlayerPos.y - 1);
			swap(grid[ActivePlayerPos.y - 1][ActivePlayerPos.x], grid[ActivePlayerPos.y][ActivePlayerPos.x]);
			_jumpHeight++;
		}
	}

	if (GetKeyState('W') & 0x8000)
	{
		if (_isJumping == false)
		{
			_start = chrono::system_clock::now();
			_isJumping = true;

			if (grid[ActivePlayerPos.y - 1][ActivePlayerPos.x]->GetType() == TILE)
			{
				_map.GetActiveCharacter()->SetPosition(ActivePlayerPos.x, ActivePlayerPos.y - 1);
				swap(grid[ActivePlayerPos.y - 1][ActivePlayerPos.x], grid[ActivePlayerPos.y][ActivePlayerPos.x]);
				_jumpHeight++;
			}
		}
	}
	if (GetKeyState('A') & 0x8000)
	{
		if (grid[ActivePlayerPos.y][ActivePlayerPos.x - 1]->GetType() == WALL || grid[ActivePlayerPos.y][ActivePlayerPos.x - 1]->GetType() == GATE)
		{
			return;
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
		else
		{
			_map.GetActiveCharacter()->SetPosition(ActivePlayerPos.x - 1, ActivePlayerPos.y);
			swap(grid[ActivePlayerPos.y][ActivePlayerPos.x - 1], grid[ActivePlayerPos.y][ActivePlayerPos.x]);
		}
	}
	if (GetKeyState('D') & 0x8000)
	{
		if (grid[ActivePlayerPos.y][ActivePlayerPos.x + 1]->GetType() == WALL || grid[ActivePlayerPos.y][ActivePlayerPos.x + 1]->GetType() == GATE)
			return;

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
		else
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

	if (grid[ActivePlayerPos.y + 1][ActivePlayerPos.x]->GetType() == BUTTON)
	{
		_map.OpenGateAt(ActivePlayerPos.x, ActivePlayerPos.y + 1);
		_wasButton = true;
	}

	if (_wasButton && grid[ActivePlayerPos.y + 1][ActivePlayerPos.x]->GetType() != BUTTON)
	{
		if (grid[ActivePlayerPos.y + 1][ActivePlayerPos.x]->GetType() == BUTTON)
		{
			_map.CloseGateAt(ActivePlayerPos.x, ActivePlayerPos.y + 1);
			_wasButton = true;
		}
		else if (grid[ActivePlayerPos.y - 1][ActivePlayerPos.x]->GetType() == BUTTON)
		{
			_map.CloseGateAt(ActivePlayerPos.x, ActivePlayerPos.y - 1);
			_wasButton = true;
		}
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
		system("CLS");
		_map.ShowMap();
		Sleep(125);
	} while (!_gameOver);

	system("CLS");
	cout << "Gameover\n";
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

void Game::Interact()
{
	vector<vector<Tile*>> grid = _map.GetGrid();
	Coordinate ActivePlayerPos = _map.GetActiveCharacter()->GetPosition();

	if (grid[ActivePlayerPos.y + 1][ActivePlayerPos.x]->GetType() == LEVER)
	{
		_map.OpenGateAt(ActivePlayerPos.x, ActivePlayerPos.y + 1);
	}
}
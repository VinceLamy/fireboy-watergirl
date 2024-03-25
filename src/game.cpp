#include "game.h"

#include <iostream>

#include "conio.h"
#include "pool.h"
#include "Windows.h"

#include "enum.h"

using namespace std;

Game::Game()
{
	_map = Map("./map/0.txt");

	_gameOver = _isJumping = _levelFinished = false;
	_jumpHeight = 0;
}

void Game::GetInput()
{
	vector<vector<Tile*>> grid = _map.GetGrid();
	Coordinate ActivePlayerPos = _map.GetActiveCharacter()->GetPosition();
	chrono::duration<double> elapsed_time = chrono::system_clock::now() - _start;

	if(_isJumping && elapsed_time < chrono::milliseconds{750} && _jumpHeight < 3)
	{
		// S'assure que la tuile au dessus de personnage est vide pour lui permette continuer son ascension
		if (grid[ActivePlayerPos.y - 1][ActivePlayerPos.x]->GetType() == TILE)
		{
			_map.GetActiveCharacter()->SetPosition(ActivePlayerPos.x, ActivePlayerPos.y - 1);
			swap(grid[ActivePlayerPos.y - 1][ActivePlayerPos.x], grid[ActivePlayerPos.y][ActivePlayerPos.x]);

			// Permet de cr�er un effet d'animation en incr�mentant de 1 la hauteur � chaque rafraichissement
			_jumpHeight++;
		}
	}

	if(GetAsyncKeyState('W') & 0x8000)
	{
		if (_isJumping == false)
		{
			_start = chrono::system_clock::now();
			_isJumping = true;

			if (grid[ActivePlayerPos.y-1][ActivePlayerPos.x]->GetType() == TILE)
			{
				_map.GetActiveCharacter()->SetPosition(ActivePlayerPos.x, ActivePlayerPos.y - 1);
				swap(grid[ActivePlayerPos.y - 1][ActivePlayerPos.x], grid[ActivePlayerPos.y][ActivePlayerPos.x]);
				_jumpHeight++;
			}
		}
	}

	if (GetKeyState('A') & 0x8000)
	{
		// Assigne le type de la position � gauche du personnage actif
		Type type = grid[ActivePlayerPos.y][ActivePlayerPos.x - 1]->GetType();

		if (type == WALL){
			return;
		}

		// Si une gate � gauche
		if (type == GATE){			
			Gate* thisGate = static_cast<Gate*>(grid[ActivePlayerPos.y][ActivePlayerPos.x - 1]);
			
			if (thisGate->GetState() == CLOSED)
				return;
		}

		// D�placement vers la gauche
		_map.GetActiveCharacter()->SetPosition(ActivePlayerPos.x - 1, ActivePlayerPos.y);
		swap(grid[ActivePlayerPos.y][ActivePlayerPos.x - 1], grid[ActivePlayerPos.y][ActivePlayerPos.x]);
	}

	if (GetKeyState('D') & 0x8000)
	{
		// Assigne le type de la position � droite du personnage actif
		Type type = grid[ActivePlayerPos.y][ActivePlayerPos.x + 1]->GetType();

		// Si une gate � droite
		if (type == GATE){
			Gate* thisGate = static_cast<Gate*>(grid[ActivePlayerPos.y][ActivePlayerPos.x + 1]);

			// Si gate ferm�e, rien ne se produit
			if (thisGate->GetState() == CLOSED){
					return;
			}
		}

		if (type == WALL){
		return;
		}
	
		// D�placement vers la droite
		_map.GetActiveCharacter()->SetPosition(ActivePlayerPos.x + 1, ActivePlayerPos.y);
		swap(grid[ActivePlayerPos.y][ActivePlayerPos.x + 1], grid[ActivePlayerPos.y][ActivePlayerPos.x]);
	}

	if (GetAsyncKeyState('Q') & 0x8000)
	{
		// Impossible de changer de personnage si celui-ci se retrouve dans les airs
		if (_isJumping) {
			return;
		}

		_map.SwitchCharacter();
	}

	if (GetAsyncKeyState('E') & 0x8000)
	{
		Interact();
	}

	// Devrait-il �tre � la fin de tout. Peut-�tre que le jeu plante � cause qu'on travaille sur 2 versions diff�rentes de la grille. Prob pas le probl�me
	_map.SetGrid(grid);
}


void Game::Play()
{
	_map.ReadMap();

	do
	{
		GetInput();
		CheckPosition();
		CheckButtons();
		CheckGates();
		CheckExits();
		system("CLS");
		_map.ShowMap();
		CheckPools();
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
	// Permet de faire redescendre les personnages en hauteurs

	vector<vector<Tile*>> grid = _map.GetGrid();
	Coordinate ActivePlayerPos = _map.GetActiveCharacter()->GetPosition();
	chrono::duration<double> elapsed_time = chrono::system_clock::now() - _start;

	if (grid[ActivePlayerPos.y + 1][ActivePlayerPos.x]->GetType() == TILE)
	{
		if (_isJumping && elapsed_time > chrono::milliseconds{ 750 })
		{
			// Id�alement ne devrait pas se limiter au active player, car on peut changer de personnage en m�me temps qu'on est dans les airs
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
void Game::CheckPools() {
	// Acc�der � la position du fireboy
	int x = _map.GetFireBoy()->GetPosition().x;
	int y = _map.GetFireBoy()->GetPosition().y;

	// Condition qui est vraie si Fireboy est au dessus d'une pool
	if (_map.GetPoolAt(x, y+1) != nullptr){

		// Termine la partie si l'�l�ment n'est pas le m�me que celui du personnage
		if (_map.GetPoolAt(x, y + 1)->GetElement() != FIRE) {
			_gameOver = true;
		}
	}

	// Acc�der � la position du fireboy
	x = _map.GetWaterGirl()->GetPosition().x;
	y = _map.GetWaterGirl()->GetPosition().y;

	// Condition qui est vraie si Fireboy est au dessus d'une pool
	if (_map.GetPoolAt(x, y + 1) != nullptr){

		// Termine la partie si l'�l�ment n'est pas le m�me que celui du personnage
		if (_map.GetPoolAt(x, y + 1)->GetElement() != WATER){
			_gameOver = true;
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
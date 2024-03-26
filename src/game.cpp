#include "game.h"

#include <iostream>

#include "conio.h"
#include "pool.h"
#include "Windows.h"

#include "enum.h"

using namespace std;

Game::Game()
{
	_map = Map("./map/testCode.txt");

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
			// Fait physiquement monter le personnage dans les airs d'une position
			_map.GetActiveCharacter()->SetPosition(ActivePlayerPos.x, ActivePlayerPos.y - 1);
			swap(grid[ActivePlayerPos.y - 1][ActivePlayerPos.x], grid[ActivePlayerPos.y][ActivePlayerPos.x]);

			// Incrémente le compteur de la hauteur
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
		_map.SetGrid(grid);
	}

	if (GetKeyState('A') & 0x8000)
	{
		// Assigne le type de la position à gauche du personnage actif
		Type type = grid[ActivePlayerPos.y][ActivePlayerPos.x - 1]->GetType();

		// Si un mur à gauche, pas de déplacement
		if (type == WALL){
			return;
		}

		if (type == GATE){			
			Gate* thisGate = static_cast<Gate*>(grid[ActivePlayerPos.y][ActivePlayerPos.x - 1]);
			
			if (thisGate->GetState() == CLOSED)
				return;
		}

		// Déplacement vers la gauche
		_map.GetActiveCharacter()->SetPosition(ActivePlayerPos.x - 1, ActivePlayerPos.y);
		swap(grid[ActivePlayerPos.y][ActivePlayerPos.x - 1], grid[ActivePlayerPos.y][ActivePlayerPos.x]);
		_map.SetGrid(grid);
	}

	if (GetKeyState('D') & 0x8000)
	{
		// Assigne le type de la position à droite du personnage actif
		Type type = grid[ActivePlayerPos.y][ActivePlayerPos.x + 1]->GetType();

		// Si une gate fermée à droite, pas de déplacemen
		if (type == GATE){
			Gate* thisGate = static_cast<Gate*>(grid[ActivePlayerPos.y][ActivePlayerPos.x + 1]);

			if (thisGate->GetState() == CLOSED){
					return;
			}
		}

		// Si un mur à droite, pas de déplacemen
		if (type == WALL){
		return;
		}
	
		// Déplacement vers la droite
		_map.GetActiveCharacter()->SetPosition(ActivePlayerPos.x + 1, ActivePlayerPos.y);
		swap(grid[ActivePlayerPos.y][ActivePlayerPos.x + 1], grid[ActivePlayerPos.y][ActivePlayerPos.x]);
		_map.SetGrid(grid);
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

	// Regarde si la tuile en bas du personnage actif est vide
	if (grid[ActivePlayerPos.y + 1][ActivePlayerPos.x]->GetType() == TILE)
	{
		//Si le personnage est en saut et que le délai de saut est dépassé. Le delay de saut devrait etre un define dans le header file.
		if (_isJumping && elapsed_time > chrono::milliseconds{ 750 })
		{
			// Redescendre le compteur de hauteur et le personnage
			_map.GetActiveCharacter()->SetPosition(ActivePlayerPos.x, ActivePlayerPos.y + 1);
			swap(grid[ActivePlayerPos.y + 1][ActivePlayerPos.x], grid[ActivePlayerPos.y][ActivePlayerPos.x]);
			_jumpHeight--;

			// Vérifie que le personnage n'est pas en train de sauter ou que le personnage touche au sol
			if (_jumpHeight == 0 || grid[ActivePlayerPos.y + 1][ActivePlayerPos.x]->GetType() != TILE)
			{
				_isJumping = false;
				_jumpHeight = 0;
			}
		}

		// Pas sûr que ce soit la meilleure formulation des conditions dans cette fonction, mais bon...
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
	// Actualise l'état des Gate en fonction de l'état des controllers
	for (int i = 0; i < _map.GetGates().size(); i++)
	{
		_map.GetGates()[i]->CheckControllers();
	}
}

void Game::CheckButtons()
{
	// Actualise l'état des boutons en vérifiant si un personnage se trouve sur un bouton

	vector<vector<Tile*>> grid = _map.GetGrid();

	for (int i = 0; i < _map.GetButton().size(); i++)
	{
		Coordinate coord = _map.GetButton()[i]->GetPosition();

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
	// Possiblement à remodifier. Je pourrais checker seulement le personnage principal

	// Accéder à la position du fireboy
	int x = _map.GetFireBoy()->GetPosition().x;
	int y = _map.GetFireBoy()->GetPosition().y;

	// Condition qui est vraie si Fireboy est au dessus d'une pool
	if (_map.GetPoolAt(x, y+1) != nullptr){

		// Termine la partie si l'élément n'est pas le même que celui du personnage
		if (_map.GetPoolAt(x, y + 1)->GetElement() != FIRE) {
			_gameOver = true;
		}
	}

	// Accéder à la position du Watergirl
	x = _map.GetWaterGirl()->GetPosition().x;
	y = _map.GetWaterGirl()->GetPosition().y;

	// Condition qui est vraie si Watergirl est au dessus d'une pool
	if (_map.GetPoolAt(x, y + 1) != nullptr){

		// Termine la partie si l'élément n'est pas le même que celui du personnage
		if (_map.GetPoolAt(x, y + 1)->GetElement() != WATER){
			_gameOver = true;
		}
	}
}

void Game::CheckExits()
{
	vector<vector<Tile*>> grid = _map.GetGrid();
	Coordinate coord;

	// Aurait pu faire un for loop de 2, car size() est toujours 2 


	// Permet d'actualiser l'état de chacune des exits
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

	// Termine la partie si les deux exits sont OPEN
	for (int y = 0; y < _map.GetExit().size(); y++)
	{
		if (_map.GetExit()[y]->GetState() == CLOSED)
		{
			return;
		}
	}

	_levelFinished = true;
}

void Game::Interact()
{
	// Permet de modifier l'état d'un levier
	vector<vector<Tile*>> grid = _map.GetGrid();
	Coordinate ActivePlayerPos = _map.GetActiveCharacter()->GetPosition();


	// Vérifie que le personnage actif se trouve au-dessus d'un levier
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
	else if (grid[ActivePlayerPos.y + 1][ActivePlayerPos.x]->GetType() == CODELOCK)
	{
		CodeLock* thisCodeLock = static_cast<CodeLock*>(grid[ActivePlayerPos.y + 1][ActivePlayerPos.x]);
		if (thisCodeLock->GetState() == OPEN)
			return;
		else
		{
			thisCodeLock->VerifyCode();
		}
	}
	else if (grid[ActivePlayerPos.y + 1][ActivePlayerPos.x]->GetType() == CODEGIVER)
	{
		CodeGiver* thisCodeGiver = static_cast<CodeGiver*>(grid[ActivePlayerPos.y + 1][ActivePlayerPos.x]);
		thisCodeGiver->ShowCode();
	}
}
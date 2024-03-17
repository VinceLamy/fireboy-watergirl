#include "map.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include "button.h"
#include "character.h"
#include "controller.h"
#include "exit.h"
#include "gate.h"
#include "lever.h"
#include "pool.h"
#include "wall.h"
//#include "platform.h"


using namespace std;

Map::Map(const char* nomNiveau)
{
	if (!nomNiveau)
	{
		cout << "ERROR : Vous devez fournir un nom de fichier!\n";
		return;
	}

	_fileName = nomNiveau;
}

Map::~Map()
{
	Clear();
}

vector<vector<Tile*>> Map::GetGrid()
{
	return _grid;
}

void Map::ReadMap()
{
	ifstream niveau;
	niveau.open(_fileName, ios_base::in);
	if (niveau.is_open())
	{
		string ligne;
		getline(niveau, ligne);
		int x = stoi(ligne);
		getline(niveau, ligne);
		int y = stoi(ligne);
		_grid.resize(y, vector<Tile*>(x));

		//Remplit la map de Tile
		for (int a = 0; a < _grid.size(); a++)
		{
			for (int b = 0; b < _grid[a].size(); b++)
			{
				AddTile(b, a);
			}
		}

		//Ajoute les contours en Wall
		for (int i = 0; i < x; i++)
		{
			AddWall(i, 0);
			AddWall(i, y - 1);
		}
		for (int i = 0; i < y; i++)
		{
			AddWall(0, i);
			AddWall(x - 1, i);
		}

		//Ajoute tout le reste
		while (niveau.good())
		{
			int lastPlatformX;
			int lastPlatformY;
			getline(niveau, ligne);
			string s;
			stringstream ss(ligne);
			vector<string> v;

			while (getline(ss, s, ' ')) {
				v.push_back(s);
			}

			switch (ligne.c_str()[0])
			{
			case 'S':
				for (int i = stoi(v[1]); i <= stoi(v[2]); i++)
				{
					AddWall(i, stoi(v[3]));
				}
				break;
			case 'M':
				for (int i = stoi(v[2]); i <= stoi(v[3]); i++)
				{
					AddWall(stoi(v[1]), i);
				}
			case '-':
				for (int i = stoi(v[1]); i <= stoi(v[2]); i++)
				{
					AddPool(i, stoi(v[3]), WATER);
				}
				break;
			case '+':
				for (int i = stoi(v[1]); i <= stoi(v[2]); i++)
				{
					AddPool(i, stoi(v[3]), FIRE);
				}
				break;
			case 'E':
				AddCharacter(stoi(v[1]), stoi(v[2]), WATER);
				break;
			case 'F':
				AddCharacter(stoi(v[1]), stoi(v[2]), FIRE);
				break;
			case 'P':
				AddExit(stoi(v[1]), stoi(v[2]));
				break;
			case '|':
				Orientation o;
				if (ligne.c_str()[2] == 'H')
				{
					o = HORIZONTAL;
				}
				else if (ligne.c_str()[2] == 'V')
				{
					o = VERTICAL;
				}
				AddGate(stoi(v[2]), stoi(v[3]), stoi(v[4]), o);
				break;
			case 'B':
				AddButton(stoi(v[1]), stoi(v[2]));
				break;
			case 'L':
				AddLever(stoi(v[1]), stoi(v[2]));
				break;
			case '%':
				Orientation o2;
				if (ligne.c_str()[2] == 'H')
				{
					o2 = HORIZONTAL;
				}
				else if (ligne.c_str()[2] == 'V')
				{
					o2 = VERTICAL;
				}
				//AddPlatform(stoi(v[2]), stoi(v[3]), stoi(v[4]), stoi(v[5]), stoi(v[6]), o2);
				break;
			}
		}
		niveau.close();
	}
 }

void Map::ShowMap()
{
	for (int a = 0; a < _grid.size(); a++)
	{
		for (int b = 0; b < _grid[a].size(); b++)
		{
			_grid[a][b]->Show();
		}
		cout << endl;
	}
}
//
//void Map::CheckPlatforms()
//{
//	for (int y = 0; y < _grid.size(); y++)
//	{
//		for (int x = 0; x < _grid[y].size(); x++)
//		{
//			if (_grid[y][x]->GetType() == PLATFORM)
//			{
//				Platform* thisPlatform = static_cast<Platform*>(_grid[y][x]);
//				thisPlatform->CheckControllers();
//
//				if (thisPlatform->GetMoveMe() == true)
//				{
//					MovePlatform(x, y);
//					thisPlatform->SetMoveMe(false);
//				}
//			}
//		}
//	}
//}

void Map::SetGrid(vector<vector<Tile*>> g)
{
	_grid = g;
}
//
//void Map::MovePlatform(int x, int y)
//{
//	Platform* movingPlatform = static_cast<Platform*>(_grid[y][x]);
//	vector<Tile*> slaves = movingPlatform->GetSlaves();
//	movingPlatform->MovePlatform();
//	int size = movingPlatform->GetSize();
//
//	Coordinate final = movingPlatform->GetFinal();
//	Coordinate initial = movingPlatform->GetInitial();
//
//	Coordinate slavePos;
//	if (movingPlatform->GetOrientation() == HORIZONTAL)
//	{
//		if (movingPlatform->GetState() == CLOSED)
//		{
//			movingPlatform->SetState(MOVING);
//			delete _grid[final.y][final.x];
//			_grid[final.y][final.x] = _grid[y][x];
//			_grid[initial.y][initial.x] = new Tile(initial.x, initial.y);
//			for (int i = 0; i < slaves.size(); i++)
//			{
//				slavePos = slaves[i]->GetPosition();
//				delete _grid[slavePos.y][slavePos.x];
//				_grid[slavePos.y][slavePos.x] = slaves[i];
//				_grid[initial.y][initial.x + i + 1] = new Tile(initial.x + i + 1, initial.y);
//			}
//			movingPlatform->SetState(OPEN);
//		}
//		else if (movingPlatform->GetState() == OPEN)
//		{
//			movingPlatform->SetState(MOVING);
//			delete _grid[initial.y][initial.x];
//			_grid[initial.y][initial.x] = _grid[y][x];
//			_grid[final.y][final.x] = new Tile(final.x, final.y);
//			for (int i = 0; i < slaves.size(); i++)
//			{
//				slavePos = slaves[i]->GetPosition();
//				delete _grid[slavePos.y][slavePos.x];
//				_grid[slavePos.y][slavePos.x] = slaves[i];
//				_grid[final.y][final.x + i + 1] = new Tile(final.x + i + 1, final.y);
//			}
//			movingPlatform->SetState(CLOSED);
//		}
//	}
//	else if (movingPlatform->GetOrientation() == VERTICAL)
//	{
//		if (movingPlatform->GetState() == CLOSED)
//		{
//			movingPlatform->SetState(MOVING);
//			delete _grid[final.y][final.x];
//			_grid[final.y][final.x] = _grid[y][x];
//			_grid[initial.y][initial.x] = new Tile(initial.x, initial.y);
//			for (int i = 0; i < slaves.size(); i++)
//			{
//				slavePos = slaves[i]->GetPosition();
//				delete _grid[slavePos.y][slavePos.x];
//				_grid[slavePos.y][slavePos.x] = slaves[i];
//				_grid[initial.y - i - 1][initial.x] = new Tile(initial.x, initial.y - i - 1);
//			}
//			movingPlatform->SetState(OPEN);
//		}
//		else if (movingPlatform->GetState() == OPEN)
//		{
//			movingPlatform->SetState(MOVING);
//			delete _grid[initial.y][initial.x];
//			_grid[initial.y][initial.x] = _grid[y][x];
//			_grid[final.y][final.x] = new Tile(final.x, final.y);
//			for (int i = 0; i < slaves.size(); i++)
//			{
//				slavePos = slaves[i]->GetPosition();
//				delete _grid[slavePos.y][slavePos.x];
//				_grid[slavePos.y][slavePos.x] = slaves[i];
//				_grid[final.y - i - 1][final.x] = new Tile(final.x, final.y - i - 1);
//			}
//			movingPlatform->SetState(CLOSED);
//		}
//	}
//}

void Map::AddTile(int x, int y)
{
	Tile* nTile = new Tile(x, y);
	_grid[y][x] = nTile;
}

void Map::AddCharacter(int x, int y, Element e)
{
	if (e == FIRE)
	{
		_fireBoy = new Character(e, x, y, true);
		delete _grid[y][x];
		_grid[y][x] = _fireBoy;
	}
	else if (e == WATER)
	{
		_waterGirl = new Character(e, x, y, false);
		delete _grid[y][x];
		_grid[y][x] = _waterGirl;
	}
}

void Map::AddExit(int x, int y)
{
	Tile* nExit = new Exit(x, y);
	delete _grid[y][x];
	_grid[y][x] = nExit;
}

void Map::AddPool(int x, int y, Element e)
{
	_pool.push_back(new Pool(x, y, e));
	delete _grid[y][x];
	_grid[y][x] = _pool.back();

}

void Map::AddWall(int x, int y)
{
	Tile* nWall = new Wall(x, y);
	delete _grid[y][x];
	_grid[y][x] = nWall;
}

void Map::AddGate(int x, int y, int size, Orientation o)
{
	Tile* nSlaveGate;
	vector<Gate*> slaveGateVector;
	Gate* nSlaveGate2;
	if (o == HORIZONTAL)
	{
		for (int i = 1; i < size; i++)
		{
			nSlaveGate = new Gate(x + i, y);
			nSlaveGate2 = static_cast<Gate*>(nSlaveGate);
			delete _grid[y][x + i];
			_grid[y][x + i] = nSlaveGate;
			slaveGateVector.push_back(nSlaveGate2);
		}
	}
	else if (o == VERTICAL)
	{
		for (int i = 1; i < size; i++)
		{
			nSlaveGate = new Gate(x, y - i);
			nSlaveGate2 = static_cast<Gate*>(nSlaveGate);
			delete _grid[y - i][x];
			_grid[y - i][x] = nSlaveGate;
			slaveGateVector.push_back(nSlaveGate2);
		}
	}
	_gate.push_back(new Gate(x, y, size, o, slaveGateVector, _lastControllers));
	delete _grid[y][x];
	_grid[y][x] = _gate.back();
	_lastControllers.clear();
}

void Map::AddLever(int x, int y)
{
	Tile* nLever = new Lever(x, y);
	delete _grid[y][x];
	_grid[y][x] = nLever;
	Controller* platformLever = static_cast<Controller*>(nLever);
	_lastControllers.push_back(platformLever);
}

void Map::AddButton(int x, int y)
{
	Tile* nButton = new Button(x, y);
	delete _grid[y][x];
	_grid[y][x] = nButton;
	Controller* platformButton = static_cast<Controller*>(nButton);
	_lastControllers.push_back(platformButton);

}
//
//void Map::AddPlatform(int x, int y, int xFinal, int yFinal, int size, Orientation o)
//{
//	Tile* nSlavePlatform;
//	vector<Tile*> slavePlatformVector;
//	if (o == HORIZONTAL)
//	{
//		for (int i = 1; i < size; i++)
//		{
//			nSlavePlatform = new Platform(x + i, y);
//			delete _grid[y][x + i];
//			_grid[y][x + i] = nSlavePlatform;
//			slavePlatformVector.push_back(nSlavePlatform);
//		}
//	}
//	else if (o == VERTICAL)
//	{
//		for (int i = 1; i < size; i++)
//		{
//			nSlavePlatform = new Platform(x, y - i);
//			delete _grid[y - i][x];
//			_grid[y - i][x] = nSlavePlatform;
//			slavePlatformVector.push_back(nSlavePlatform);
//		}
//	}
//	Tile* nPlatform = new Platform(x, y, xFinal, yFinal, size, o, slavePlatformVector, _lastControllers);
//	delete _grid[y][x];
//	_grid[y][x] = nPlatform;
//	_lastControllers.clear();
//}

void Map::Clear()
{
	for (int a = 0; a < _grid.size(); a++)
	{
		for (int b = 0; b < _grid[a].size(); b++)
		{
			delete _grid[a][b];
		}
	}
	_fileName = NULL;
	_grid.clear();
	_lastControllers.clear();
}

Character* Map::GetActiveCharacter()
{
	if (_fireBoy->getState())
		return _fireBoy;

	return _waterGirl;
}

void Map::SwitchCharacter()
{
	if (_fireBoy->getState())
	{
		_fireBoy->Deactivate();
		_waterGirl->Activate();
	}
	else
	{
		_fireBoy->Activate();
		_waterGirl->Deactivate();
	}
}

Pool* Map::GetPoolAt(int x, int y)
{
	Coordinate coord;
	coord.x = x;
	coord.y = y;

	for (int i = 0; i < _pool.size(); i++)
		if (_pool[i]->GetPosition().x == coord.x && _pool[i]->GetPosition().y == coord.y)
			return _pool[i];
}

void Map::OpenGateAt(int x, int y)
{
	for (int i = 0; i < _gate.capacity(); i++)
	{
		_gate[i]->OpenGate(x, y);
		_grid[y][x] = new Tile(x, y);
	}
}

void Map::CloseGateAt(int x, int y)
{
	for (int i = 0; i < _gate.capacity(); i++)
	{
		_gate[i]->CloseGate(x, y);
		_grid[y][x] = new Tile(x, y);
	}
}
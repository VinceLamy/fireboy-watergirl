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

vector<vector<Tile*>>* Map::GetGrid()
{
	return &_grid;
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
			case '~':
				for (int i = stoi(v[1]); i <= stoi(v[2]); i++)
				{
					AddPool(i, stoi(v[3]), GOO);
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
				break;
			case '$':
				AddCodeLock(stoi(v[1]), stoi(v[2]), _lastCodeGiver);
				break;
			case '@':
				AddCodeGiver(stoi(v[1]), stoi(v[2]));
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
	Exit* exit = static_cast<Exit*>(nExit);
	_exit.push_back(exit);
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
	Button* thisButton = static_cast<Button*>(nButton);
	_lastControllers.push_back(platformButton);
	_button.push_back(thisButton);

}

void Map::AddCodeLock(int x, int y, vector<CodeGiver*>)
{
	Tile* nCodeLock = new CodeLock(x, y, _lastCodeGiver);
	Controller* thisCodeLock = static_cast<Controller*>(nCodeLock);
	_lastControllers.push_back(thisCodeLock);
	delete _grid[y][x];
	_grid[y][x] = nCodeLock;
	_lastCodeGiver.clear();
}

void Map::AddCodeGiver(int x, int y)
{
	Tile* nCodeGiver = new CodeGiver(x, y);
	CodeGiver* thisCodeGiver = static_cast<CodeGiver*>(nCodeGiver);
	_lastCodeGiver.push_back(thisCodeGiver);
	delete _grid[y][x];
	_grid[y][x] = nCodeGiver;
}

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
	_lastCodeGiver.clear();
	_button.clear();
	_waterGirl = NULL;
	_fireBoy = NULL;
	_pool.clear();
	_gate.clear();
	_exit.clear();
}

vector<Gate*> Map::GetGates()
{
	return _gate;
}

vector<Button*> Map::GetButton()
{
	return _button;
}

vector<Exit*> Map::GetExit()
{
	return _exit;
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
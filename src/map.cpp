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

#define BLOCKWIDTH 25
#define BLOCKHEIGHT 50

using namespace std;

Map::Map(const char* nomNiveau, QObject* parent) : QGraphicsScene(parent), _fileName(nomNiveau),
wallPixmap("./sprite/map/brick_brown_1.png"),
waterPixamp("./sprite/map/dngn_deep_water.png"),
lavaPixmap("./sprite/map/lava0.png"),
gooPixmap("./sprite/map/dngn_deep_water_murky.png"),
gatePixmap("./sprite/map/relief1.png"),
fireJpPixmap("./sprite/map/Fire_JP.png"),
waterAlexPixmap("./sprite/map/Water_Alex.png"),
codeGiverPixmap("./sprite/map/dngn_altar_beogh.png"),
exitPixmap("./sprite/map/dngn_enter_cocytus.png"),
leverPixmap("./sprite/map/Lever.png"),
buttonPixmap("./sprite/map/openButton.png"),
codeLockPixmap("./sprite/map/Old Padlock - GREY - 0000.png"),
backgroundPixmap("./sprite/map/background.png")
{
	wallPixmap = wallPixmap.scaled(QSize(BLOCKWIDTH, BLOCKHEIGHT));
	waterPixamp = waterPixamp.scaled(QSize(BLOCKWIDTH, BLOCKHEIGHT));
	lavaPixmap = lavaPixmap.scaled(QSize(BLOCKWIDTH, BLOCKHEIGHT));
	gooPixmap = gooPixmap.scaled(QSize(BLOCKWIDTH, BLOCKHEIGHT));
	fireJpPixmap = fireJpPixmap.scaled(QSize(BLOCKWIDTH, BLOCKHEIGHT));
	waterAlexPixmap = waterAlexPixmap.scaled(QSize(BLOCKWIDTH, BLOCKHEIGHT));
	codeGiverPixmap = codeGiverPixmap.scaled(QSize(2*BLOCKWIDTH, BLOCKHEIGHT + BLOCKHEIGHT/2));
	exitPixmap = exitPixmap.scaled(QSize(2*BLOCKWIDTH, BLOCKHEIGHT + BLOCKHEIGHT/2));
	leverPixmap = leverPixmap.scaled(QSize(2*BLOCKWIDTH, BLOCKHEIGHT + BLOCKHEIGHT/2));
	buttonPixmap = buttonPixmap.scaled(QSize(2*BLOCKWIDTH, BLOCKHEIGHT));
	codeLockPixmap = codeLockPixmap.scaled(QSize(2*BLOCKWIDTH, BLOCKHEIGHT + BLOCKHEIGHT/2));
	backgroundPixmap = backgroundPixmap.scaled(QSize(2560, 1440));
	setBackgroundBrush(backgroundPixmap);

	_fileName = nomNiveau;

	timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &Map::UpdateScene);
	timer->start(1000 / 60);
}

Map::~Map()
{
	Clear();
}

std::vector<vector<Tile*>>* Map::GetGrid()
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
		_fireBoy = new Character(fireJpPixmap, e, x*BLOCKWIDTH, y*BLOCKHEIGHT, true);
		addItem(_fireBoy);
		/*delete _grid[y][x];
		_grid[y][x] = _fireBoy;*/
		connect(_fireBoy, &Character::GameOver, this, &Map::SendGameOverToGame);
	}
	else if (e == WATER)
	{
		_waterGirl = new Character(waterAlexPixmap, e, x*BLOCKWIDTH, y*BLOCKHEIGHT, false);
		addItem(_waterGirl);
		/*delete _grid[y][x];
		_grid[y][x] = _waterGirl;*/
		connect(_waterGirl, &Character::GameOver, this, &Map::SendGameOverToGame);
	}
}

void Map::AddExit(int x, int y)
{
	Exit* nExit = new Exit(exitPixmap, x*BLOCKWIDTH, y*BLOCKHEIGHT - BLOCKHEIGHT - BLOCKHEIGHT/2);
	/*delete _grid[y][x];
	_grid[y][x] = nExit;
	Exit* exit = static_cast<Exit*>(nExit);*/
	_exit.push_back(nExit);
	addItem(nExit);
}

void Map::AddPool(int x, int y, Element e)
{
	if (e == WATER)
	{
		Pool* nPool = new Pool(waterPixamp, x*BLOCKWIDTH, y*BLOCKHEIGHT, e);
		addItem(nPool);
	}
	else if (e == FIRE)
	{
		Pool* nPool = new Pool(lavaPixmap, x * BLOCKWIDTH, y * BLOCKHEIGHT, e);
		addItem(nPool);
	}
	else if (e == GOO)
	{
		Pool* nPool = new Pool(gooPixmap, x * BLOCKWIDTH, y * BLOCKHEIGHT, e);
		addItem(nPool);
	}

	/*_pool.push_back(new Pool(x, y, e));
	delete _grid[y][x];
	_grid[y][x] = _pool.back();*/
}

void Map::AddWall(int x, int y)
{
	Wall* nWall = new Wall(wallPixmap, x*BLOCKWIDTH, y*BLOCKHEIGHT);
	addItem(nWall);
	/*delete _grid[y][x];
	_grid[y][x] = nWall;*/
}

void Map::AddGate(int x, int y, int size, Orientation o)
{
	/*Tile* nSlaveGate;*/
	/*vector<Gate*> slaveGateVector;*/
	/*Gate* nSlaveGate2;*/
	Gate* nGate;
	if (o == HORIZONTAL)
	{
		/*for (int i = 1; i < size; i++)
		{
			nSlaveGate = new Gate(x + i, y);
			nSlaveGate2 = static_cast<Gate*>(nSlaveGate);
			delete _grid[y][x + i];
			_grid[y][x + i] = nSlaveGate;
			slaveGateVector.push_back(nSlaveGate2);
		}*/
		gatePixmap = gatePixmap.transformed(QTransform().rotate(90));
		gatePixmap = gatePixmap.scaled(QSize(BLOCKWIDTH*size, BLOCKWIDTH));
		nGate = new Gate(gatePixmap, x * BLOCKWIDTH, y * BLOCKHEIGHT + BLOCKHEIGHT/4, _lastControllers);
		addItem(nGate);
	}
	else if (o == VERTICAL)
	{
		/*for (int i = 1; i < size; i++)
		{
			nSlaveGate = new Gate(x, y - i);
			nSlaveGate2 = static_cast<Gate*>(nSlaveGate);
			delete _grid[y - i][x];
			_grid[y - i][x] = nSlaveGate;
			slaveGateVector.push_back(nSlaveGate2);
		}*/
		gatePixmap = gatePixmap.scaled(QSize(BLOCKWIDTH, BLOCKHEIGHT*size));
		nGate = new Gate(gatePixmap, x * BLOCKWIDTH, (y - size + 1)* BLOCKHEIGHT, _lastControllers);
		addItem(nGate);
	}
	_gate.push_back(nGate);
	/*delete _grid[y][x];
	_grid[y][x] = _gate.back();*/
	_lastControllers.clear();
}

void Map::AddLever(int x, int y)
{
	Lever* nLever = new Lever(leverPixmap, x*BLOCKWIDTH, y*BLOCKHEIGHT - BLOCKHEIGHT - BLOCKHEIGHT/2);
	/*delete _grid[y][x];
	_grid[y][x] = nLever;*/
	Controller* connectLever = static_cast<Controller*>(nLever);
	_lastControllers.push_back(connectLever);
	addItem(nLever);
}

void Map::AddButton(int x, int y)
{
	Button* nButton = new Button(buttonPixmap, x*BLOCKWIDTH, y*BLOCKHEIGHT - BLOCKHEIGHT);
	/*delete _grid[y][x];
	_grid[y][x] = nButton;*/
	Controller* connectButton = static_cast<Controller*>(nButton);
	_lastControllers.push_back(connectButton);
	_button.push_back(nButton);
	addItem(nButton);

}

void Map::AddCodeLock(int x, int y, vector<CodeGiver*>)
{
	CodeLock* nCodeLock = new CodeLock(codeLockPixmap, x*BLOCKWIDTH, y*BLOCKHEIGHT - BLOCKHEIGHT - BLOCKHEIGHT / 2, _lastCodeGiver);
	Controller* connectCodeLock = static_cast<Controller*>(nCodeLock);
	_lastControllers.push_back(connectCodeLock);
	/*delete _grid[y][x];
	_grid[y][x] = nCodeLock;*/
	_lastCodeGiver.clear();
	addItem(nCodeLock);
}

void Map::AddCodeGiver(int x, int y)
{
	CodeGiver* nCodeGiver = new CodeGiver(codeGiverPixmap, x * BLOCKWIDTH, y * BLOCKHEIGHT - BLOCKHEIGHT - BLOCKHEIGHT / 2);
	/*CodeGiver* thisCodeGiver = static_cast<CodeGiver*>(nCodeGiver);*/
	_lastCodeGiver.push_back(nCodeGiver);
	addItem(nCodeGiver);
	/*delete _grid[y][x];
	_grid[y][x] = nCodeGiver;*/
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

	/*for (int i = 0; i < _pool.size(); i++)
		if (_pool[i]->GetPosition().x == coord.x && _pool[i]->GetPosition().y == coord.y)
			return _pool[i];*/

	return nullptr;
}

void Map::Swap(Coordinate pos1, Coordinate pos2)
{
	swap(_grid[pos1.y][pos1.x], _grid[pos2.y][pos2.x]);
}

void Map::SendGameOverToGame()
{
	emit GameOver();
}

void Map::UpdateScene()
{
	if (_fireBoy != nullptr && _waterGirl != nullptr)
	{
		if (_fireBoy->getState())
		{
			_fireBoy->advance(0);
		}
		else
		{
			_waterGirl->advance(0);

		}
	}
}



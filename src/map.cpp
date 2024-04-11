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

#define BLOCKWIDTH 27
#define BLOCKHEIGHT 52

using namespace std;

Map::Map(const char* nomNiveau, QObject* parent) : QGraphicsScene(parent), _fileName(nomNiveau),
wallPixmap("./sprite/map/brick_brown_1.png"),
waterPixamp("./sprite/map/dngn_deep_water.png"),
lavaPixmap("./sprite/map/lava0.png"),
gooPixmap("./sprite/map/dngn_deep_water_murky.png"),
gatePixmap("./sprite/map/relief1.png"),
fireJpPixmap("./sprite/map/Fire_JP_V2.png"),
waterAlexPixmap("./sprite/map/Water_Alex_V2.png"),
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

}

Map::~Map()
{
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
		_fireBoy->setZValue(1);
		addItem(_fireBoy);
		connect(_fireBoy, &Character::GameOver, this, &Map::SendGameOverToGame);
		connect(_fireBoy, &Character::SwitchCharacter, this, &Map::SwitchCharacter);
		connect(_fireBoy, &Character::CheckGates, this, &Map::CheckGates);
	}
	else if (e == WATER)
	{
		_waterGirl = new Character(waterAlexPixmap, e, x*BLOCKWIDTH, y*BLOCKHEIGHT, false);
		addItem(_waterGirl);
		_waterGirl->setZValue(1);
		connect(_waterGirl, &Character::GameOver, this, &Map::SendGameOverToGame);
		connect(_waterGirl, &Character::SwitchCharacter, this, &Map::SwitchCharacter);
		connect(_waterGirl, &Character::CheckGates, this, &Map::CheckGates);
	}
}

void Map::AddExit(int x, int y)
{
	Exit* nExit = new Exit(exitPixmap, x*BLOCKWIDTH, y*BLOCKHEIGHT - BLOCKHEIGHT - BLOCKHEIGHT/2);
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
}

void Map::AddWall(int x, int y)
{
	Wall* nWall = new Wall(wallPixmap, x*BLOCKWIDTH, y*BLOCKHEIGHT);
	addItem(nWall);
}

void Map::AddGate(int x, int y, int size, Orientation o)
{
	Gate* nGate;
	if (o == HORIZONTAL)
	{
		QPixmap tempPixmap = gatePixmap.transformed(QTransform().rotate(90));
		tempPixmap = gatePixmap.scaled(QSize(BLOCKWIDTH*size, BLOCKWIDTH));
		nGate = new Gate(tempPixmap, size, o,  x * BLOCKWIDTH, y * BLOCKHEIGHT + BLOCKHEIGHT/4, _lastControllers);
		addItem(nGate);
	}
	else if (o == VERTICAL)
	{
		QPixmap tempPixmap = gatePixmap.scaled(QSize(BLOCKWIDTH, BLOCKHEIGHT*size));
		nGate = new Gate(tempPixmap, size, o, x * BLOCKWIDTH, (y - size + 1)* BLOCKHEIGHT, _lastControllers);
		addItem(nGate);
	}
	_gate.push_back(nGate);
	_lastControllers.clear();
}

void Map::AddLever(int x, int y)
{
	Lever* nLever = new Lever(leverPixmap, x*BLOCKWIDTH, y*BLOCKHEIGHT - BLOCKHEIGHT - BLOCKHEIGHT/2);
	Controller* connectLever = static_cast<Controller*>(nLever);
	_lastControllers.push_back(connectLever);
	addItem(nLever);
}

void Map::AddButton(int x, int y)
{
	Button* nButton = new Button(buttonPixmap, x*BLOCKWIDTH, y*BLOCKHEIGHT - BLOCKHEIGHT);
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
	_lastCodeGiver.clear();
	addItem(nCodeLock);
}

void Map::AddCodeGiver(int x, int y)
{
	CodeGiver* nCodeGiver = new CodeGiver(codeGiverPixmap, x * BLOCKWIDTH, y * BLOCKHEIGHT - BLOCKHEIGHT - BLOCKHEIGHT / 2);
	_lastCodeGiver.push_back(nCodeGiver);
	addItem(nCodeGiver);
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

void Map::CheckGates()
{
	
	for (int i = 0; i < _gate.size(); i++)
	{
			if (&_gate != nullptr)
				_gate[i]->CheckControllers();
	}

	if (&_gate != nullptr)
	{
		for (int i = 0; i < _gate.size(); i++)
		{
			if (_gate[i]->GetState() == CLOSED)
			{
				if (_gate[i]->GetOrientation() == HORIZONTAL)
				{

					QPixmap tempPixmap = gatePixmap.transformed(QTransform().rotate(90));
					tempPixmap = gatePixmap.scaled(QSize(BLOCKWIDTH * _gate[i]->GetSize(), BLOCKWIDTH));
					_gate[i]->setPixmap(tempPixmap);

				}
				else if (_gate[i]->GetOrientation() == VERTICAL)
				{
					QPixmap tempPixmap = gatePixmap.scaled(QSize(BLOCKWIDTH, BLOCKHEIGHT * _gate[i]->GetSize()));
					_gate[i]->setPixmap(tempPixmap);
				}
			}
			else if (_gate[i]->GetState() == OPEN)
			{
				_gate[i]->setPixmap(emptyPixmap);
			}
		}
	}
}

void Map::CheckButtons()
{
	for (int i = 0; i < _button.size(); i++)
	{
		if(&_button != nullptr)
			_button[i]->CheckOver();
	}
}

void Map::CheckExits()
{
	for (int i = 0; i < _exit.size(); i++)
	{
		if(&_exit != nullptr)
			_exit[i]->CheckIn();
	}

	if (&_exit != nullptr)
	{
		if (_exit[0]->GetState() == OPEN && _exit[1]->GetState() == OPEN)
		{
			levelFinished = true;
			emit LevelFinished();
		}
	}
}

//void Map::StopTimer()
//{
//	timer->stop();
//}

void Map::SendGameOverToGame()
{
	levelFinished = true;
	emit GameOver();
}

void Map::UpdateScene()
{	
	if(!levelFinished)
		CheckButtons();
	if (!levelFinished)
		CheckGates();
	if (!levelFinished)
	{
		if (_fireBoy != nullptr && _waterGirl != nullptr)
		{
			if (_fireBoy->getState())
			{
				_fireBoy->setFocus();
				_fireBoy->advance(0);
			}
			else
			{
				_waterGirl->setFocus();
				_waterGirl->advance(0);

			}
		}
	}
	if (!levelFinished)
		CheckExits();
}



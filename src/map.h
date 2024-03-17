#ifndef MAP_H
#define MAP_H

#include <vector>

#include "character.h"
#include "tile.h"
#include "controller.h"
#include "gate.h"
#include "pool.h"

using namespace std;

class Map
{
public:
	Map(const char* nomNiveau = NULL);
	~Map();

	void ReadMap();
	void ShowMap();

	vector<vector<Tile*>> GetGrid();
	//const char* GetFilename;

	void CheckPlatforms();

	void SetGrid(vector<vector<Tile*>> g);
	void MovePlatform(int x, int y);

	void AddTile(int x, int y);
	void AddCharacter(int x, int y, Element e);
	void AddExit(int x, int y);
	void AddPool(int x, int y, Element e);
	void AddWall(int x, int y);
	void AddGate(int x, int y, int size, Orientation o);
	void AddLever(int x, int y);
	void AddButton(int x, int y);
	//void AddPlatform(int x, int y, int xFinal, int yFinal, int size, Orientation o);

	void Clear();

	Character* GetActiveCharacter();
	void SwitchCharacter();

	Pool* GetPoolAt(int x, int y);
	void OpenGateAt(int x, int y);
	void CloseGateAt(int x, int y);

private:
	const char* _fileName;
	vector<vector<Tile*>> _grid;
	vector<Controller*> _lastControllers;
	Character* _waterGirl;
	Character* _fireBoy;
	vector<Pool*> _pool;
	vector<Gate*> _gate;
};

#endif MAP_H


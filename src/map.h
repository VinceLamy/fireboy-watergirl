#ifndef MAP_H
#define MAP_H

#include <vector>

#include "character.h"
#include "tile.h"
#include "controller.h"
#include "gate.h"
#include "pool.h"
#include "exit.h"

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
	
	void AddTile(int x, int y);
	void AddCharacter(int x, int y, Element e);
	void AddExit(int x, int y);
	void AddPool(int x, int y, Element e);
	void AddWall(int x, int y);
	void AddGate(int x, int y, int size, Orientation o);
	void AddLever(int x, int y);
	void AddButton(int x, int y);

	void Clear();

	vector<Gate*> GetGates();
	vector<Button*> GetButton();
	vector<Exit*> GetExit();
	Character* GetActiveCharacter();
	void SwitchCharacter();

	Pool* GetPoolAt(int x, int y);

private:
	const char* _fileName;
	vector<vector<Tile*>> _grid;
	vector<Controller*> _lastControllers;
	vector<Button*> _button;
	Character* _waterGirl;
	Character* _fireBoy;
	vector<Pool*> _pool;
	vector<Gate*> _gate;
	vector<Exit*> _exit;
};

#endif MAP_H
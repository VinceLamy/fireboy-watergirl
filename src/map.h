 #ifndef MAP_H
#define MAP_H

#include <vector>

#include "character.h"
#include "tile.h"
#include "controller.h"
#include "gate.h"
#include "pool.h"
#include "exit.h"
#include "codelock.h"
#include "codegiver.h"

//using namespace std;

class Map
{
public:
	Map(const char* nomNiveau = NULL);
	~Map();

	void ReadMap();
	void ShowMap();

	std::vector<std::vector<Tile*>>* GetGrid();
	
	void AddTile(int x, int y);
	void AddCharacter(int x, int y, Element e);
	void AddExit(int x, int y);
	void AddPool(int x, int y, Element e);
	void AddWall(int x, int y);
	void AddGate(int x, int y, int size, Orientation o);
	void AddLever(int x, int y);
	void AddButton(int x, int y);
	void AddCodeLock(int x, int y, std::vector<CodeGiver*>);
	void AddCodeGiver(int x, int y);

	void Clear();

	std::vector<Gate*> GetGates();
	std::vector<Button*> GetButton();
	std::vector<Exit*> GetExit();
	Character* GetActiveCharacter();
	void SwitchCharacter();

	Pool* GetPoolAt(int x, int y);

	void Swap(Coordinate pos1, Coordinate pos2);

private:
	const char* _fileName;
	std::vector<std::vector<Tile*>> _grid;
	std::vector<Controller*> _lastControllers;
	std::vector<CodeGiver*> _lastCodeGiver;
	std::vector<Button*> _button;
	Character* _waterGirl;
	Character* _fireBoy;
	std::vector<Pool*> _pool;
	std::vector<Gate*> _gate;
	std::vector<Exit*> _exit;

};

#endif MAP_H
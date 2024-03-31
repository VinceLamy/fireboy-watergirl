#ifndef GATE_H
#define GATE_H

#include "tile.h"
#include "button.h"
#include "lever.h"
#include <vector>

using namespace std;

class Gate : public Tile
{
public:
	Gate(int x, int y);
	Gate(int x, int y, int size, Orientation o, vector<Gate*> slaveGates, vector<Controller*> controllers);

	State GetState();
	void SetState(State s);

	void CheckControllers();

	void Show();

private:
	int _size;
	Orientation _orientation;
	vector<Gate*> _slaveGates;
	vector<Controller*> _controllers;
	State _state = CLOSED;
};

#endif GATE_H
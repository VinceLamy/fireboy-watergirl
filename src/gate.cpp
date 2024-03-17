#include "gate.h"
#include <iostream>

using namespace	std;

Gate::Gate(int x, int y)
{
	SetPosition(x, y);
	SetType(GATE);
}

Gate::Gate(int x, int y, int size, Orientation o, vector<Gate*> slaveGates, vector<Controller*> controllers)
{
	SetPosition(x, y);
	_size = size;
	_orientation = o;
	_slaveGates = slaveGates;
	_controllers = controllers;
	SetType(GATE);
}

Gate::~Gate()
{
}

void Gate::SetState(State s)
{
	_state = s;
}

void Gate::OpenGate(int x, int y)
{
	Coordinate coord;

	coord.x = x;
	coord.y = y;

	for (int i = 0; i < _controllers.size(); i++)
	{
		if (_controllers[i]->GetPosition().x == coord.x && _controllers[i]->GetPosition().y == coord.y)
		{
			if (_controllers[i]->GetState() == CLOSED)
			{
				_state = OPEN;
				_controllers[i]->SetState(OPEN);

				for (int y = 0; y < _slaveGates.size(); y++)
					_slaveGates[y]->SetState(OPEN);
			}
		}
	}
}

void Gate::CloseGate(int x, int y)
{
	Coordinate coord;

	coord.x = x;
	coord.y = y;

	for (int i = 0; i < _controllers.size(); i++)
	{
		if (_controllers[i]->GetPosition().x == coord.x && _controllers[i]->GetPosition().y == coord.y)
		{
			if (_controllers[i]->GetState() == OPEN)
			{
				_state = CLOSED;
				_controllers[i]->SetState(CLOSED);

				for (int y = 0; y < _slaveGates.size(); y++)
					delete _slaveGates[y];
			}
		}
	}
}


void Gate::Show()
{
	if (_state == CLOSED)
		cout << '|';

	if (_state == OPEN)
		cout << ' ';
}

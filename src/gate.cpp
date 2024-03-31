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

State Gate::GetState()
{
	return _state;
}

void Gate::SetState(State s)
{
	_state = s;
}

void Gate::CheckControllers()
{
	for (int i = 0; i < _controllers.size(); i++)
	{
		if (_state == CLOSED)
		{
			if (_controllers[i]->GetState() == OPEN)
			{
				_state = OPEN;

				for (int y = 0; y < _slaveGates.size(); y++)
				{
					_slaveGates[y]->SetState(OPEN);
				}
			}

		}
		if (_state == OPEN)
		{
			if (_controllers[i]->GetState() == CLOSED)
			{
				_state = CLOSED;
				for (int y = 0; y < _slaveGates.size(); y++)
				{
					_slaveGates[y]->SetState(CLOSED);
				}
			}
			else
			{
				_state = OPEN;
				for (int y = 0; y < _slaveGates.size(); y++)
				{
					_slaveGates[y]->SetState(OPEN);
				}
				break;
			}
		}
	}
}


void Gate::Show()
{
	if (_state == CLOSED)
		cout << '|';

	else if (_state == OPEN)
		cout << ' ';
}
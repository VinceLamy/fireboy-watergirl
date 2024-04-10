#include "gate.h"
#include <iostream>

//using namespace	std;

//Gate::Gate(int x, int y)
//{
//	SetPosition(x, y);
//	SetType(GATE);
//}

Gate::Gate(const QPixmap& pixmap, int size, Orientation o, qreal x, qreal y, std::vector<Controller*> controllers) : QGraphicsPixmapItem(pixmap)
{
	/*SetPosition(x, y);*/
	setPos(x, y);
	_size = size;
	_orientation = o;
	/*_slaveGates = slaveGates;*/
	_controllers = controllers;
	/*SetType(GATE);*/
}

State Gate::GetState()
{
	return _state;
}

void Gate::SetState(State s)
{
	_state = s;
}

int Gate::GetSize()
{
	return _size;
}

Orientation Gate::GetOrientation()
{
	return _orientation;
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
				return;

				/*for (int y = 0; y < _slaveGates.size(); y++)
				{
					_slaveGates[y]->SetState(OPEN);
				}*/
			}

		}
		if (_state == OPEN)
		{
			if (_controllers[i]->GetState() == CLOSED)
			{
				_state = CLOSED;
				/*for (int y = 0; y < _slaveGates.size(); y++)
				{
					_slaveGates[y]->SetState(CLOSED);
				}*/
			}
			else
			{
				_state = OPEN;
				/*for (int y = 0; y < _slaveGates.size(); y++)
				{
					_slaveGates[y]->SetState(OPEN);
				}*/
				return;
			}
		}
	}
}


void Gate::Show()
{
	if (_state == CLOSED)
		std::cout << '|';

	else if (_state == OPEN)
		std::cout << ' ';
}
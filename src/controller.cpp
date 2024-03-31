#include "controller.h"

void Controller::SetState(State s)
{
	_state = s;
}

State Controller::GetState()
{
	return _state;
}

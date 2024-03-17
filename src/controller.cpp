#include "controller.h"

Controller::Controller()
{
}

Controller::~Controller()
{
}

void Controller::SetState(State s)
{
	_state = s;
}

State Controller::GetState()
{
	return _state;
}

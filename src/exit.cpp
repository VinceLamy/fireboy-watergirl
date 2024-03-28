#include "exit.h"
#include <iostream>

using namespace	std;

Exit::Exit(int x, int y)
{
	SetPosition(x, y);
	SetType(EXIT);
}

Exit::~Exit()
{
}

void Exit::SetState(State s)
{
	_state = s;
}

State Exit::GetState()
{
	return _state;
}

void Exit::Show()
{
	cout << MAGENTA << 'P' << RESET;
}

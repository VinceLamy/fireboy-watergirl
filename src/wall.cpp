#include "wall.h"
#include <iostream>

//using namespace	std;

Wall::Wall(int x, int y)
{
	SetPosition(x, y);
	SetType(WALL);
}

void Wall::Show()
{
	std::cout << '#';
}

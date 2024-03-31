#include "tile.h"
#include <iostream>

using namespace std;

Tile::Tile()
{
}

Tile::Tile(int x, int y)
{
	_position.x = x;
	_position.y = y;
	_type = TILE;
}

Tile::~Tile()
{

}

Coordinate Tile::GetPosition()
{
	return _position;
}

Type Tile::GetType()
{
	return _type;
}


void Tile::SetPosition(float x, float y)
{
	_position.x = x;
	_position.y = y;
}

void Tile::SetType(Type t)
{
	_type = t;
}

void Tile::Show()
{
	cout << ' ';
}

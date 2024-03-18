#ifndef ENUM_H
#define ENUM_H

enum Type
{
	TILE,
	BUTTON,
	CHARACTER,
	EXIT,
	GATE,
	LEVER,
	PLATFORM,
	POOL,
	WALL,
	CODELOCK,
	CODEGIVER
};

enum Orientation
{
	HORIZONTAL,
	VERTICAL
};

enum State
{
	CLOSED,
	MOVING,
	OPEN
};

enum Element
{
	FIRE,
	WATER,
	GOO
};

#endif ENUM_H


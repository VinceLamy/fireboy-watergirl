#ifndef POOL_H
#define POOL_H

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define CYAN    "\033[36m"
#define GREEN   "\033[32m"

#include "tile.h"

class Pool : public Tile
{
public:
	Pool(int x, int y, Element e);
	~Pool();

	void SetElememt(Element e);
	Element GetElement();

	void Show();

private:
	Element _element;
};

#endif POOL_H
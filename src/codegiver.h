#ifndef CODEGIVER_H
#define CODEGIVER_H

#include "tile.h"
#include <iostream>

using namespace std;

class CodeGiver : public Tile
{
public:
	CodeGiver(int x, int y);
	~CodeGiver();
	string ShowCode();
	void SetDigits(string digits);
	void Show();
	void Clear();

private:
	string _mydigits;
};

#endif CODEGIVER_H

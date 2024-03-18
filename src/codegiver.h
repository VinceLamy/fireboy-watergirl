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
	void ShowCode();
	void SetDigits(string digits);
	void Show();

private:
	string _mydigits;
};

#endif CODEGIVER_H

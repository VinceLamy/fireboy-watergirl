#ifndef CODEGIVER_H
#define CODEGIVER_H

#define RESET	"\033[0m"
#define BRIGHTYELLOW	"\033[93m"

#include "tile.h"
#include <iostream>

using namespace std;

class CodeGiver : public Tile
{
public:
	CodeGiver(int x, int y);
	string ShowCode();
	void SetDigits(string digits);
	void Show();
	void Clear();

private:
	string _mydigits;
};

#endif CODEGIVER_H

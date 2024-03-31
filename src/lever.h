#ifndef LEVER_H
#define LEVER_H

#define RESET	"\033[0m"
#define YELLOW	"\033[33m"

#include "controller.h"

class Lever : public Controller
{
public:
	Lever(int x, int y);
	void Show();
};

#endif LEVER_H
#ifndef LEVER_H
#define LEVER_H

#include "controller.h"

class Lever : public Controller
{
public:
	Lever(int x, int y);
	Lever();
	void Show();
};

#endif LEVER_H
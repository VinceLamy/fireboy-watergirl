#ifndef BUTTON_H
#define BUTTON_H

#define RESET	"\033[0m"
#define YELLOW	"\033[33m"

#include "controller.h"

class Button : public Controller
{
public:
	Button(int x, int y);
	Button();
	void Show();
};

#endif BUTTON_H

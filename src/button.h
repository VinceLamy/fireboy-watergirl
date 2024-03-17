#ifndef BUTTON_H
#define BUTTON_H

#include "controller.h"

class Button : public Controller
{
public:
	Button(int x, int y);
	Button();
	void Show();
};

#endif BUTTON_H

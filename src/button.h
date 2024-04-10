#ifndef BUTTON_H
#define BUTTON_H

#define RESET	"\033[0m"
#define YELLOW	"\033[33m"

#include "controller.h"
#include "character.h"

class Button : public Controller
{
public:
	Button(QPixmap& pixmap, qreal x, qreal y);
	void Show();
	void CheckOver();
};

#endif BUTTON_H

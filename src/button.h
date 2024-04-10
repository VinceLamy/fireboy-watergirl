#ifndef BUTTON_H
#define BUTTON_H

#define RESET	"\033[0m"
#define YELLOW	"\033[33m"

#include "controller.h"

class Button : public Controller
{
public:
	Button(QPixmap& pixmap, qreal x, qreal y);
	void Show();
};

#endif BUTTON_H

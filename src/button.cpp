#include "button.h"
#include <iostream>

//using namespace	std;

Button::Button(QPixmap& pixmap, qreal x, qreal y) : Controller(pixmap, x, y)
{
	/*SetPosition(x, y);
	SetType(BUTTON);*/
}

void Button::Show()
{
	std::cout << YELLOW << 'B' << RESET;
}

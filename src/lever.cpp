#include "lever.h"
#include <iostream>

//using namespace	std;

Lever::Lever(QPixmap& pixmap, qreal x, qreal y) : Controller(pixmap, x, y)
{
	/*SetPosition(x, y);
	SetType(LEVER);*/
}

//void Lever::Show()
//{
//	if (GetState() == CLOSED)
//		std::cout << YELLOW << 'L' << RESET;
//	if (GetState() == OPEN)
//		std::cout << YELLOW << '<' << RESET;
//}
#include "exit.h"
#include <iostream>

//using namespace	std;

Exit::Exit(QPixmap& pixmap, qreal x, qreal y) : QGraphicsPixmapItem(pixmap)
{
	setPos(x, y);
	/*SetPosition(x, y);
	SetType(EXIT);*/
}

void Exit::SetState(State s)
{
	_state = s;
}

State Exit::GetState()
{
	return _state;
}

//void Exit::Show()
//{
//	std::cout << MAGENTA << 'P' << RESET;
//}

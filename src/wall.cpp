#include "wall.h"
#include <iostream>

//using namespace	std;

Wall::Wall(const QPixmap& pixmap, qreal x, qreal y) : QGraphicsPixmapItem(pixmap)
{
	//SetPosition(x, y);
	//SetType(WALL);
	setPos(x, y);
}

//void Wall::Show()
//{
//	std::cout << '#';
//}

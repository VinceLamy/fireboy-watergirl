#ifndef WALL_H
#define WALL_H

#include "tile.h"
#include <QtWidgets>

class Wall : public QGraphicsPixmapItem
{
public:
	Wall(const QPixmap& pixmap, qreal x, qreal y);
	//void Show();
};

#endif WALL_H
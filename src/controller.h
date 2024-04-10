#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "tile.h"
#include <QtWidgets>

class Controller : public QGraphicsPixmapItem
{
public:
	Controller(QPixmap&, qreal x, qreal y);
	void SetState(State s);
	State GetState();

	/*virtual void Show() = 0;*/

private:
	State _state = CLOSED;
};

#endif CONTROLLER_H

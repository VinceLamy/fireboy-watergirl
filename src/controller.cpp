#include "controller.h"

Controller::Controller(QPixmap& pixmap, qreal x, qreal y) : QGraphicsPixmapItem(pixmap)
{
	setPos(x, y);
}

void Controller::SetState(State s)
{
	_state = s;
}

State Controller::GetState()
{
	return _state;
}

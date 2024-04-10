#ifndef EXIT_H
#define EXIT_H

#define RESET	"\033[0m"
#define MAGENTA	"\033[35m"

#include "tile.h"
#include "QtWidgets"

class Exit : public QGraphicsPixmapItem
{
public:
	Exit(QPixmap& pixmap, qreal x, qreal y);

	void SetState(State s);
	State GetState();

	//void Show();

private:
	State _state = CLOSED;
};

#endif EXIT_H
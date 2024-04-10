#ifndef GATE_H
#define GATE_H

#include "tile.h"
#include "button.h"
#include "lever.h"
#include <vector>
#include <QtWidgets>

//using namespace std;

class Gate : public QGraphicsPixmapItem
{
public:
	/*Gate(int x, int y);*/
	Gate(const QPixmap& pixmap, int size, Orientation o, qreal x, qreal y, std::vector<Controller*> controllers);

	State GetState();
	void SetState(State s);
	int GetSize();
	Orientation GetOrientation();
	void CheckControllers();

	void Show();

private:
	int _size;
	Orientation _orientation;
	/*std::vector<Gate*> _slaveGates;*/
	std::vector<Controller*> _controllers;
	State _state = CLOSED;
};

#endif GATE_H
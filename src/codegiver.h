#ifndef CODEGIVER_H
#define CODEGIVER_H

#define RESET	"\033[0m"
#define BRIGHTYELLOW	"\033[93m"

#include "tile.h"
#include <iostream>
#include <QtWidgets>

//using namespace std;

class CodeGiver : public QGraphicsPixmapItem
{
public:
	CodeGiver(QPixmap& pixmap, qreal x, qreal y);
	std::string ShowCode();
	void SetDigits(std::string digits);
	/*void Show();*/
	void Clear();

private:
	std::string _mydigits;
};

#endif CODEGIVER_H

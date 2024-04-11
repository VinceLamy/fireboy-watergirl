#ifndef CODEGIVER_H
#define CODEGIVER_H

#define RESET	"\033[0m"
#define BRIGHTYELLOW	"\033[93m"

#include "tile.h"
#include <iostream>
#include <QtWidgets>

//using namespace std;

class CodeGiver : public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT
public:
	CodeGiver(QPixmap& pixmap, qreal x, qreal y, QObject* parent = nullptr);
	void ShowCode();
	void SetDigits(std::string digits);
	/*void Show();*/
	void Clear();

signals:
	void SendingCode(const QString& s);

private:
	std::string _mydigits;
};

#endif CODEGIVER_H

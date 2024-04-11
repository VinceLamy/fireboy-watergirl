#include "codegiver.h"
#include "Windows.h"

//using namespace std;

CodeGiver::CodeGiver(QPixmap& pixmap, qreal x, qreal y, QObject* parent) : QObject(parent), QGraphicsPixmapItem(pixmap)
{
	setPos(x, y);
	/*SetPosition(x, y);
	SetType(CODEGIVER);*/
}

void CodeGiver::ShowCode()
{
	emit SendingCode(_mydigits);
}

void CodeGiver::SetDigits(std::string digits)
{
	_mydigits = digits;
}

//void CodeGiver::Show()
//{
//	std::cout << BRIGHTYELLOW << '@' << RESET;
//}

void CodeGiver::Clear()
{
	_mydigits = "";
}

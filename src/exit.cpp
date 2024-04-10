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

void Exit::CheckIn()
{
    bool characterIn = false;

    QList<QGraphicsItem*> collidingItemsList = collidingItems(Qt::IntersectsItemShape);
    for (QGraphicsItem* item : collidingItemsList)
    {
            if (dynamic_cast<Character*>(item) != nullptr)
            {
                characterIn = true;
                break;
            }
    }

    if (characterIn)
    {
        SetState(OPEN);
    }
    else
    {
        SetState(CLOSED);
    }
}

//void Exit::Show()
//{
//	std::cout << MAGENTA << 'P' << RESET;
//}

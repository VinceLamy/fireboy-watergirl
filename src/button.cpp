#include "button.h"
#include <iostream>

//using namespace	std;

Button::Button(QPixmap& pixmap, qreal x, qreal y) : Controller(pixmap, x, y)
{
	/*SetPosition(x, y);
	SetType(BUTTON);*/
}

void Button::Show()
{
	std::cout << YELLOW << 'B' << RESET;
}

void Button::CheckOver()
{
    bool characterOver = false;

	QList<QGraphicsItem*> collidingItemsList = collidingItems(Qt::IntersectsItemShape);
    for (QGraphicsItem* item : collidingItemsList)
    {
        QList<QGraphicsItem*> collidingItemsList = collidingItems(Qt::IntersectsItemShape);
        for (QGraphicsItem* item : collidingItemsList)
        {
            if (dynamic_cast<Character*>(item) != nullptr)
            {
                characterOver = true;
                break;
            }
        }
    }

    if (characterOver)
    {
        SetState(OPEN);
    }
    else
    {
        SetState(CLOSED);
    }
	
}

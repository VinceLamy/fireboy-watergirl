#include "character.h"
#include <iostream>
#include "wall.h"
#include "pool.h"
#include "gate.h"
#include "button.h"

//using namespace std;

Character::Character(const QPixmap& pixmap, Element e, qreal x, qreal y, bool state, QObject* parent) : QGraphicsPixmapItem(pixmap), QObject(parent)
{
	_element = e;
	setPos(x, y);
	/*SetPosition(x, y);*/
	/*SetType(CHARACTER);*/
	setState(state);
	setFlag(QGraphicsItem::ItemIsFocusable);
}

Character::Character(Character& character)
{
	_element = character._element;
	_state = character._state;
}

void Character::Activate()
{
	_state = true;
}

void Character::Deactivate()
{
	_state = false;
}

bool Character::getState()
{
	return _state;
}

Element Character::getElement()
{
	return _element;
}

void Character::setState(bool state)
{
	_state = state;
}

void Character::keyPressEvent(QKeyEvent* event)
{
	switch (event->key())
	{
	case Qt::Key_A:
		dx = -5;
		break;
	case Qt::Key_D:
		dx = 5;
		break;
	case Qt::Key_W:
		if (onGround)
		{
			dy = -13.5;
			onGround = false;
		}
		break;
	default:
		break;
	}
}

void Character::keyReleaseEvent(QKeyEvent* event)
{
	switch (event->key()) 
	{
	case Qt::Key_A:
	case Qt::Key_D:
		dx = 0;
		break;
	default:
		break;
	}
}

void Character::advance(int phase)
{
	if (!phase) 
	{
		dy += gravity;

		moveBy(dx, 0);
		HorizontalCollision();

		moveBy(0, dy);
		VerticalCollision();
	}
}

void Character::HorizontalCollision()
{
	QList<QGraphicsItem*> collidingItemsList = collidingItems(Qt::IntersectsItemShape);
	for (QGraphicsItem* item : collidingItemsList)
	{
		if (dynamic_cast<Wall*>(item) != nullptr && !dynamic_cast<Character*>(item))
		{
			QRectF charRect = item->boundingRect();
			QRectF wallRect = item->boundingRect();

			if (charRect.right() > wallRect.left() && dx > 0)
			{
				setPos(item->x() - charRect.width(), pos().y());
				dx = 0;
			}
			if (charRect.left() < wallRect.right() && dx < 0)
			{
				setPos(item->x() + charRect.width(), pos().y());
				dx = 0;
			}
		}
		if (dynamic_cast<Gate*>(item) != nullptr && !dynamic_cast<Character*>(item))
		{
			QRectF charRect = item->boundingRect();
			QRectF gateRect = item->boundingRect();

			if (charRect.right() > gateRect.left() && dx > 0)
			{
				setPos(item->x() - charRect.width(), pos().y());
				dx = 0;
			}
			if (charRect.left() < gateRect.right() && dx < 0)
			{
				setPos(item->x() + charRect.width(), pos().y());
				dx = 0;
			}
		}
	}
}

void Character::VerticalCollision()
{
	QList<QGraphicsItem*> collidingItemsList = collidingItems(Qt::IntersectsItemShape);
	for(QGraphicsItem * item : collidingItemsList)
	{
		if (dynamic_cast<Wall*>(item) != nullptr && !dynamic_cast<Character*>(item))
		{
			QRectF charRect = item->boundingRect();
			QRectF wallRect = item->boundingRect();

			if (charRect.bottom() > wallRect.top() && dy > 0)
			{
				setPos(x(), item->pos().y() - charRect.height());
				dy = 0;
				onGround = true;
			}
			if (charRect.top() < wallRect.bottom() && dy < 0)
			{
				setPos(x(), item->pos().y() + wallRect.height());
			}
		}
		if (dynamic_cast<Gate*>(item) != nullptr && !dynamic_cast<Character*>(item))
		{
			QRectF charRect = item->boundingRect();
			QRectF gateRect = item->boundingRect();

			if (charRect.bottom() > gateRect.top() && dy > 0)
			{
				setPos(x(), item->pos().y() - charRect.height());
				dy = 0;
				onGround = true;
			}
			if (charRect.top() < gateRect.bottom() && dy < 0)
			{
				setPos(x(), item->pos().y() + gateRect.height());
			}
		}
		if (dynamic_cast<Pool*>(item) != nullptr && !dynamic_cast<Character*>(item))
		{
			Pool* pool = dynamic_cast<Pool*>(item);
			Character* character = dynamic_cast<Character*>(item);
			QRectF charRect = item->boundingRect();
			QRectF poolRect = item->boundingRect();

			if (charRect.bottom() > poolRect.top() && dy > 0)
			{
				setPos(x(), item->pos().y() - charRect.height());
				dy = 0;
				onGround = true;
				if (_element != pool->GetElement())
				{
					emit GameOver();
				}
			}
			if (charRect.top() < poolRect.bottom() && dy < 0)
			{
				setPos(x(), item->pos().y() + poolRect.height());
			}
		}
		if (dynamic_cast<Button*>(item) != nullptr && !dynamic_cast<Character*>(item))
		{
			QRectF charRect = item->boundingRect();
			QRectF buttonRect = item->boundingRect();

			if (charRect.bottom() > buttonRect.top() && dy > 0)
			{
				setPos(x(), item->pos().y() - 8);
				dy = 0;
				onGround = true;
			}
		}
	}
}

//void Character::Show()
//{
//	if (_element == WATER)
//	{
//		
//		std::cout << CYAN << 'E' << RESET;
//	}
//	else if (_element == FIRE)
//	{
//		std::cout << RED << 'F' << RESET;
//	}
//}

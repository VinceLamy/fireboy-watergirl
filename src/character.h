#ifndef CARACTER_H
#define CARACTER_H

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define CYAN    "\033[36m"

#include "tile.h"
#include <QtWidgets>
#include <QGraphicsPixmapItem>
#include <QObject>

class Character : public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT
public:
	Character(const QPixmap& pixmap, Element e, qreal x, qreal y, bool = false, QObject* parent = nullptr);
	Character(Character&);

	void Activate();
	void Deactivate();

	bool getState();
	Element getElement();

	void setState(bool state);

	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;
	void advance(int phase) override;

	/*void Show();*/

signals:
	void GameOver();
	void SwitchCharacter();
	void CheckGates();

private:
	Element _element;
	bool _state;
	qreal dx = 0;
	qreal dy = 0;
	const qreal gravity = 0.4;
	bool onGround = false;

	void HorizontalCollision();
	void VerticalCollision();

	void Interact();

	QPixmap openLeverPixmap;
	QPixmap closedLeverPixmap;
};

#endif CARACTER_H

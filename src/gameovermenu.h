#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include "menubutton.h"
#include "enum.h"

#ifndef GAMEOVERMENU_H
#define GAMEOVERMENU_H

class GameOverMenu : public QWidget
{
	Q_OBJECT

public:
	GameOverMenu(int currentLevel, QWidget* parent = nullptr);
	~GameOverMenu();

signals:
	void levelSelected(int level);
	void BackToMainMenu();

private slots:
	void Back();
	void Next();

private:
	QVBoxLayout* _layout;
	QVBoxLayout* _littleLayout;
	QLabel* _title;
	std::vector<MenuButton*> _listButton;

	int _currentLevel;
	void InitMenu();
};
#endif GAMEOVERMENU_H
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include "menubutton.h"

#ifndef MAINMENU_H
#define MAINMENU_H

class MainMenu : public QWidget
{
	Q_OBJECT

public:
	MainMenu(QWidget* parent = nullptr);
	~MainMenu();

signals:
	void levelSelected(int level);
	void ShowTutorialScreen();
	void levelSelection();

private:
	QVBoxLayout* _layout;
	QVBoxLayout* _littleLayout;
	QLabel* _title;
	MenuButton* _newGame;
	MenuButton* _tutorial;
	MenuButton* _chooseLevel;
	MenuButton* _quit;
	int _currentLevel;
	void InitMainMenu();
	void NewGame();
	void Tutorial();
	void ChooseLevel();
	void Quit();
};
#endif MAINMENU_H
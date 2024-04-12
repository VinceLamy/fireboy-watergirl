#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include "menubutton.h"
#include "enum.h"

#ifndef BETWEENLEVELMENU_H
#define BETWEENLEVELMENU_H

class BetweenLevelMenu : public QWidget
{
	Q_OBJECT

public:
	BetweenLevelMenu(int currentLevel, QWidget* parent = nullptr);
	~BetweenLevelMenu();

signals:
	void LoadLevel();
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
#endif BETWEENLEVELMENU_H
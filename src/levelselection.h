#include "menubutton.h"
#include <QVBoxLayout>

#ifndef LEVELSELECTION_H
#define LEVELSELECTION_H

class LevelSelection : public QWidget
{
	Q_OBJECT

public:
	LevelSelection(QWidget* parent = nullptr);
	~LevelSelection();

signals:
	void levelSelected(int level);
	void returnToMainMenu();

private:
	QVBoxLayout* _layout;
	MenuButton* _level1;
	MenuButton* _level2;
	MenuButton* _level3;
	MenuButton* _level4;
	MenuButton* _level5;
	MenuButton* _back;
	int _currentLevel;
	void initUi();
	void Level1();
	void Level2();
	void Level3();
	void Level4();
	void Level5();
	void Back();
};
#endif LEVELSELECTION_H

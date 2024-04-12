#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include "menubutton.h"
#include "enum.h"

#ifndef ENDGAMEMENU_H
#define ENDGAMEMENU_H

class EndGameMenu : public QWidget
{
	Q_OBJECT

public:
	EndGameMenu(QWidget* parent = nullptr);
	~EndGameMenu();

signals:
	void BackToMainMenu();

private slots:
	void Back();

private:
	QVBoxLayout* _layout;
	QVBoxLayout* _littleLayout;
	QLabel* _title;
	std::vector<MenuButton*> _listButton;

	void InitMenu();
};
#endif ENDGAMEMENU_H
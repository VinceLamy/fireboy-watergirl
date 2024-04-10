#include "menubutton.h"
#include <QGridLayout>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>
#include <QHBoxLayout>

#ifndef TUTORIALSCREEN_H
#define TUTORIALSCREEN_H

class TutorialScreen : public QWidget
{
	Q_OBJECT
public:
	TutorialScreen(QWidget* parent = nullptr);
	~TutorialScreen();

signals:
	void StartTutorial(int level);
	void BackToMainMenu();

private slots:
	void Next();
	void Back();

private:
	QGridLayout* _layout;
	QHBoxLayout* _hLayout;
	QVBoxLayout* _vLayout;
	MenuButton* _back;
	MenuButton* _next;
	QLabel* _exit;
	QLabel* _lavaPool;
	QLabel* _waterPool;
	QLabel* _gooPool;
	QLabel* _gate;
	QLabel* _button;
	QLabel* _lever;
	QLabel* _codeLock;
	QLabel* _codeGiver;
	QLabel* _exitPic;
	QLabel* _lavaPoolPic;
	QLabel* _waterPoolPic;
	QLabel* _gooPoolPic;
	QLabel* _gatePic;
	QLabel* _buttonPic;
	QLabel* _leverPic;
	QLabel* _codeLockPic;
	QLabel* _codeGiverPic;
	void InitUi();

};
#endif TUTORIALSCREEN_H


#include "tutorialscreen.h"

#define ICONSIZE 50

TutorialScreen::TutorialScreen(QWidget* parent) : QWidget(parent)
{
	InitUi();
}

TutorialScreen::~TutorialScreen()
{
}

void TutorialScreen::Next()
{
	emit StartTutorial(0);
}

void TutorialScreen::Back()
{
	emit BackToMainMenu();
}

void TutorialScreen::InitUi()
{
	QPixmap waterPixmap("./sprite/map/dngn_deep_water.png");
	QPixmap lavaPixmap("./sprite/map/lava0.png");
	QPixmap gooPixmap("./sprite/map/dngn_deep_water_murky.png");
	QPixmap gatePixmap("./sprite/map/relief1.png");
	QPixmap codeGiverPixmap("./sprite/map/dngn_altar_beogh.png");
	QPixmap exitPixmap("./sprite/map/dngn_enter_cocytus.png");
	QPixmap leverPixmap("./sprite/map/Lever.png");
	QPixmap buttonPixmap("./sprite/map/openButton.png");
	QPixmap codeLockPixmap("./sprite/map/Old Padlock - GREY - 0000.png");

	waterPixmap = waterPixmap.scaled(QSize(ICONSIZE, ICONSIZE));
	lavaPixmap = lavaPixmap.scaled(QSize(ICONSIZE, ICONSIZE));
	gooPixmap = gooPixmap.scaled(QSize(ICONSIZE, ICONSIZE));
	gatePixmap = gatePixmap.scaled(QSize(ICONSIZE, ICONSIZE));
	codeGiverPixmap = codeGiverPixmap.scaled(QSize(ICONSIZE, ICONSIZE));
	exitPixmap = exitPixmap.scaled(QSize(ICONSIZE, ICONSIZE));
	leverPixmap = leverPixmap.scaled(QSize(ICONSIZE, ICONSIZE));
	buttonPixmap = buttonPixmap.scaled(QSize(ICONSIZE, ICONSIZE));
	codeLockPixmap = codeLockPixmap.scaled(QSize(ICONSIZE, ICONSIZE));

	_exitPic = new QLabel();
	_lavaPoolPic = new QLabel();
	_waterPoolPic = new QLabel();
	_gooPoolPic = new QLabel();
	_gatePic = new QLabel();
	_buttonPic = new QLabel();
	_leverPic = new QLabel();
	_codeLockPic = new QLabel();
	_codeGiverPic = new QLabel();

	_exitPic->setPixmap(exitPixmap);
	_lavaPoolPic->setPixmap(lavaPixmap);
	_waterPoolPic->setPixmap(waterPixmap);
	_gooPoolPic->setPixmap(gooPixmap);
	_gatePic->setPixmap(gatePixmap);
	_buttonPic->setPixmap(buttonPixmap);
	_leverPic->setPixmap(leverPixmap);
	_codeLockPic->setPixmap(codeLockPixmap);
	_codeGiverPic->setPixmap(codeGiverPixmap);

	_layout = new QGridLayout();
	_hLayout = new QHBoxLayout();
	_vLayout = new QVBoxLayout();
	_layout->setColumnStretch(0, 1);
	_layout->setColumnStretch(2, 1);
	_layout->setColumnStretch(4, 1);

	_vLayout->addStretch();
	_vLayout->addLayout(_layout);
	_vLayout->addStretch();
	_vLayout->addLayout(_hLayout);
	_vLayout->addStretch();

	QFont font("Arial", 18, QFont::Bold);

	_exit = new QLabel("The goal of the game is to get to these two exits with your two characters");
	_exit->setFont(font);
	_exit->setStyleSheet("Color : #FFFFFF");
	_layout->addWidget(_exit, 0, 3);
	_layout->addWidget(_exitPic, 0, 1);

	_lavaPool = new QLabel("Only the red character can pass on lava");
	_lavaPool->setFont(font);
	_lavaPool->setStyleSheet("Color : #FFFFFF");
	_layout->addWidget(_lavaPool, 1, 3);
	_layout->addWidget(_lavaPoolPic, 1, 1);

	_waterPool = new QLabel("Only the blue character can pass on water");
	_waterPool->setFont(font);
	_waterPool->setStyleSheet("Color : #FFFFFF");
	_layout->addWidget(_waterPool, 2, 3);
	_layout->addWidget(_waterPoolPic, 2, 1);

	_gooPool = new QLabel("No character can pass on goo");
	_gooPool->setFont(font);
	_gooPool->setStyleSheet("Color : #FFFFFF");
	_layout->addWidget(_gooPool, 3, 3);
	_layout->addWidget(_gooPoolPic, 3, 1);

	_gate = new QLabel("Gates will block your path to victory, but they can be opened");
	_gate->setFont(font);
	_gate->setStyleSheet("Color : #FFFFFF");
	_layout->addWidget(_gate, 4, 3);
	_layout->addWidget(_gatePic, 4, 1);

	_button = new QLabel("A button is activated when you step on it, when it's activated, it will open a gate");
	_button->setFont(font);
	_button->setStyleSheet("Color : #FFFFFF");
	_layout->addWidget(_button, 5, 3);
	_layout->addWidget(_buttonPic, 5, 1);

	_lever = new QLabel("A lever is activated when you interact with it, when it's activated, it will open a gate");
	_lever->setFont(font);
	_lever->setStyleSheet("Color : #FFFFFF");
	_layout->addWidget(_lever, 6, 3);
	_layout->addWidget(_leverPic, 6, 1);

	_codeLock = new QLabel("A lock is activated when you interact with it and give it the right 4 digit code,  when it's activated, it will open a gate");
	_codeLock->setFont(font);
	_codeLock->setStyleSheet("Color : #FFFFFF");
	_layout->addWidget(_codeLock, 7, 3);
	_layout->addWidget(_codeLockPic, 7, 1);

	_codeGiver = new QLabel("The lady on the altar will whisper a part of the 4 digits code, look on your controller!");
	_codeGiver->setFont(font);
	_codeGiver->setStyleSheet("Color : #FFFFFF");
	_layout->addWidget(_codeGiver, 8, 3);
	_layout->addWidget(_codeGiverPic, 8, 1);

	_back = new MenuButton("Back");
	_next = new MenuButton("Next");
	_hLayout->addStretch();
	_hLayout->addWidget(_back);
	_hLayout->addStretch();
	_hLayout->addWidget(_next);
	_hLayout->addStretch();

	setLayout(_vLayout);

	connect(_back, &QPushButton::released, this, &TutorialScreen::Back);
	connect(_next, &QPushButton::released, this, &TutorialScreen::Next);

}

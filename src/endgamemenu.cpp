#include "endgamemenu.h"

EndGameMenu::EndGameMenu(QWidget* parent) : QWidget(parent)
{
	InitMenu();
}

EndGameMenu::~EndGameMenu()
{

}

void EndGameMenu::InitMenu()
{
	QFont titleFont("Arial", 75, QFont::Bold);

	QPixmap firePixmap("./sprite/menu/Fire_JP_V2_joyeux.png");
	QPixmap waterPixmap("./sprite/menu/Water_Alex_V2_joyeux.png");

	_fire = new QLabel();
	_water = new QLabel();

	_fire->setPixmap(firePixmap);
	_water->setPixmap(waterPixmap);

	_layout = new QVBoxLayout();
	_littleLayout = new QVBoxLayout();
	_hLayout = new QHBoxLayout(this);
	_littleLayout->setAlignment(Qt::AlignCenter);

	_title = new QLabel("Congratulation!!!");
	_title->setAlignment(Qt::AlignHCenter);
	_title->setFont(titleFont);
	_title->setStyleSheet("color: #FFFFFF");
	_layout->addSpacing(75);
	_layout->addWidget(_title);
	_layout->addLayout(_littleLayout);
	_littleLayout->addStretch();

	_listButton.push_back(new MenuButton("Main Menu"));
	connect(_listButton.back(), &QPushButton::released, this, &EndGameMenu::Back);

	for (int i = 0; i < _listButton.size(); i++)
	{
		_littleLayout->addWidget(_listButton[i]);
		_littleLayout->addStretch();
	}

	_hLayout->addStretch();
	_hLayout->addWidget(_fire);
	_hLayout->addStretch();
	_hLayout->addLayout(_layout);
	_hLayout->addStretch();
	_hLayout->addWidget(_water);
	_hLayout->addStretch();

	this->setLayout(_hLayout);
}

void EndGameMenu::Back()
{
	emit BackToMainMenu();
}

#include "EndGameMenu.h"

#include "gameovermenu.h"

GameOverMenu::GameOverMenu(int currentLevel, QWidget* parent) : QWidget(parent)
{
	InitMenu();
	_currentLevel = currentLevel;
}

GameOverMenu::~GameOverMenu()
{
}

void GameOverMenu::InitMenu()
{
	QFont titleFont("Arial", 50, QFont::Bold);

	_layout = new QVBoxLayout(this);
	_littleLayout = new QVBoxLayout();
	_littleLayout->setAlignment(Qt::AlignCenter);

	_title = new QLabel("Game over");
	_title->setAlignment(Qt::AlignHCenter);
	_title->setFont(titleFont);
	_title->setStyleSheet("color: #FFFFFF");
	_layout->addWidget(_title);
	_layout->addLayout(_littleLayout);
	_littleLayout->addStretch();

	_listButton.push_back(new MenuButton("Retry"));
	connect(_listButton.back(), &QPushButton::released, this, &GameOverMenu::Next);

	_listButton.push_back(new MenuButton("Main menu"));
	connect(_listButton.back(), &QPushButton::released, this, &GameOverMenu::Back);

	for (int i = 0; i < _listButton.size(); i++)
	{
		_littleLayout->addWidget(_listButton[i]);
		_littleLayout->addStretch();
	}
	
	this->setLayout(_layout);
}

void GameOverMenu::Next()
{
	int selectedLevel = _currentLevel;
	emit levelSelected(selectedLevel);
}

void GameOverMenu::Back()
{
	emit BackToMainMenu();
}

#include "GameOverMenu.h"

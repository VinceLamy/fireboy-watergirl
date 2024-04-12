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
	QFont titleFont("Arial", 125, QFont::Bold);

	_layout = new QVBoxLayout(this);
	_hLayout = new QHBoxLayout();

	_title = new QLabel("Game Over");
	_title->setAlignment(Qt::AlignHCenter);
	_title->setFont(titleFont);
	_title->setStyleSheet("color: #9E1A1A");
	_layout->addStretch();
	_layout->addWidget(_title);
	_layout->addStretch();
	_layout->addLayout(_hLayout);
	_layout->addStretch();

	
	_listButton.push_back(new MenuButton("Retry"));
	connect(_listButton.back(), &QPushButton::released, this, &GameOverMenu::Next);

	_listButton.push_back(new MenuButton("Main Menu"));
	connect(_listButton.back(), &QPushButton::released, this, &GameOverMenu::Back);

	_hLayout->addStretch();

	for (int i = 0; i < _listButton.size(); i++)
	{
		_hLayout->addWidget(_listButton[i]);
		_hLayout->addStretch();
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

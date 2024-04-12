#include "betweenlevelmenu.h"

BetweenLevelMenu::BetweenLevelMenu(int currentLevel, QWidget* parent) : QWidget(parent)
{
	InitMenu();
	_currentLevel = currentLevel;
}

BetweenLevelMenu::~BetweenLevelMenu()
{

}

void BetweenLevelMenu::InitMenu()
{
	QFont titleFont("Arial", 50, QFont::Bold);

	_layout = new QVBoxLayout(this);
	_littleLayout = new QVBoxLayout();
	_littleLayout->setAlignment(Qt::AlignCenter);

	_title = new QLabel("Niveau termine");
	_title->setAlignment(Qt::AlignHCenter);
	_title->setFont(titleFont);
	_title->setStyleSheet("color: #FFFFFF");
	_layout->addWidget(_title);
	_layout->addLayout(_littleLayout);
	_littleLayout->addStretch();

	_listButton.push_back(new MenuButton("Continue"));
	connect(_listButton.back(), &QPushButton::released, this, &BetweenLevelMenu::Next);

	_listButton.push_back(new MenuButton("Main menu"));
	connect(_listButton.back(), &QPushButton::released, this, &BetweenLevelMenu::Back);

	for (int i = 0; i < _listButton.size(); i++)
	{
		_littleLayout->addWidget(_listButton[i]);
		_littleLayout->addStretch();
	}

	this->setLayout(_layout);
}

void BetweenLevelMenu::Next()
{
	int levelSelected = _currentLevel + 1;
	emit LoadLevel(levelSelected);
}

void BetweenLevelMenu::Back()
{
	emit BackToMainMenu();
}

#include "BetweenLevelMenu.h"

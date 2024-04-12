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
	QFont titleFont("Arial", 100, QFont::Bold);

	_layout = new QVBoxLayout(this);
	_hLayout = new QHBoxLayout();

	_title = new QLabel("Level Completed!");
	_title->setAlignment(Qt::AlignHCenter);
	_title->setFont(titleFont);
	_title->setStyleSheet("color: #FFFFFF");
	_layout->addStretch();
	_layout->addWidget(_title);
	_layout->addStretch();
	_layout->addLayout(_hLayout);
	_layout->addStretch();


	_listButton.push_back(new MenuButton("Next Level"));
	connect(_listButton.back(), &QPushButton::released, this, &BetweenLevelMenu::Next);

	_listButton.push_back(new MenuButton("Main Menu"));
	connect(_listButton.back(), &QPushButton::released, this, &BetweenLevelMenu::Back);

	_hLayout->addStretch();
	for (int i = 0; i < _listButton.size(); i++)
	{
		_hLayout->addWidget(_listButton[i]);
		_hLayout->addStretch();
	}

	this->setLayout(_layout);
}

void BetweenLevelMenu::Next()
{
	emit LoadLevel();
}

void BetweenLevelMenu::Back()
{
	emit BackToMainMenu();
}

#include "BetweenLevelMenu.h"

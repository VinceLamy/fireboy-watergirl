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
	QFont titleFont("Arial", 50, QFont::Bold);

	_layout = new QVBoxLayout(this);
	_littleLayout = new QVBoxLayout();
	_littleLayout->setAlignment(Qt::AlignCenter);

	_title = new QLabel("Jeu termine");
	_title->setAlignment(Qt::AlignHCenter);
	_title->setFont(titleFont);
	_title->setStyleSheet("color: #FFFFFF");
	_layout->addWidget(_title);
	_layout->addLayout(_littleLayout);
	_littleLayout->addStretch();

	_listButton.push_back(new MenuButton("Main menu"));
	connect(_listButton.back(), &QPushButton::released, this, &EndGameMenu::Back);

	for (int i = 0; i < _listButton.size(); i++)
	{
		_littleLayout->addWidget(_listButton[i]);
		_littleLayout->addStretch();
	}

	this->setLayout(_layout);
}

void EndGameMenu::Back()
{
	emit BackToMainMenu();
}

#include "EndGameMenu.h"

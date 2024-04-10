#include "mainmenu.h"

MainMenu::MainMenu(QWidget* parent) : QWidget(parent)
{
	InitMainMenu();
}

MainMenu::~MainMenu()
{
}

void MainMenu::InitMainMenu()
{
	QFont titleFont("Arial", 50, QFont::Bold);

	_layout = new QVBoxLayout(this);
	_littleLayout = new QVBoxLayout();
	_littleLayout->setAlignment(Qt::AlignCenter);

	_title = new QLabel("The Best Game");
	_title->setAlignment(Qt::AlignHCenter);
	_title->setFont(titleFont);
	_title->setStyleSheet("color: #FFFFFF");
	_layout->addSpacing(75);
	_layout->addWidget(_title);
	_layout->addLayout(_littleLayout);
	_littleLayout->addStretch();


	_newGame = new MenuButton("New Game");
	connect(_newGame, &QPushButton::released, this, &MainMenu::NewGame);
	_littleLayout->addWidget(_newGame);
	_littleLayout->addStretch();

	_tutorial = new MenuButton("Tutorial");
	connect(_tutorial, &QPushButton::released, this, &MainMenu::Tutorial);
	_littleLayout->addWidget(_tutorial);
	_littleLayout->addStretch();

	_chooseLevel = new MenuButton("Select Level");
	connect(_chooseLevel, &QPushButton::released, this, &MainMenu::ChooseLevel);
	_littleLayout->addWidget(_chooseLevel);
	_littleLayout->addStretch();

	_quit = new MenuButton("Quit");
	connect(_quit, &QPushButton::released, this, &MainMenu::Quit);
	_littleLayout->addWidget(_quit);
	_littleLayout->addStretch();

	this->setLayout(_layout);
}

void MainMenu::NewGame()
{
	int selectedLevel = 1;
	emit levelSelected(selectedLevel);
}

void MainMenu::Tutorial()
{;
	emit ShowTutorialScreen();
}

void MainMenu::ChooseLevel()
{
	emit levelSelection();
}

void MainMenu::Quit()
{
	QApplication::quit();
}



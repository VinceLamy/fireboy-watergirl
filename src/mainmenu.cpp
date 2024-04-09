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
	QFont titleFont("Arial", 24, QFont::Bold);

	_layout = new QVBoxLayout(this);
	_layout->setAlignment(Qt::AlignCenter);

	_title = new QLabel("The Best Game");
	_title->setAlignment(Qt::AlignHCenter);
	_title->setFont(titleFont);
	_title->setStyleSheet("color: #FFFFFF");
	_layout->addStretch(1);
	_layout->addWidget(_title);
	_layout->addStretch(2);

	_newGame = new MenuButton("New Game");
	connect(_newGame, &QPushButton::released, this, &MainMenu::NewGame);
	_layout->addWidget(_newGame);
	_layout->addStretch(1);

	_tutorial = new MenuButton("Tutorial");
	connect(_tutorial, &QPushButton::released, this, &MainMenu::Tutorial);
	_layout->addWidget(_tutorial);
	_layout->addStretch(1);

	_chooseLevel = new MenuButton("Select Level");
	connect(_chooseLevel, &QPushButton::released, this, &MainMenu::ChooseLevel);
	_layout->addWidget(_chooseLevel);
	_layout->addStretch(1);

	_quit = new MenuButton("Quit");
	connect(_quit, &QPushButton::released, this, &MainMenu::Quit);
	_layout->addWidget(_quit);
	_layout->addStretch(1);

	this->setLayout(_layout);
}

void MainMenu::NewGame()
{
	int selectedLevel = 1;
	emit levelSelected(selectedLevel);
}

void MainMenu::Tutorial()
{
	int selectedLevel = 0;
	emit levelSelected(selectedLevel);
}

void MainMenu::ChooseLevel()
{
	emit levelSelection();
}

void MainMenu::Quit()
{
	QApplication::quit();
}

int MainMenu::GetCurrentLevel()
{
	return _currentLevel;
}


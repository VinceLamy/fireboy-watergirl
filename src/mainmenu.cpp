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
	QPixmap fireJpPixmap("./sprite/map/Fire_JP_V2.png");
	QPixmap waterAlexPixmap("./sprite/map/Water_Alex_V2.png");
	_water = new QLabel();
	_fire = new QLabel();
	_water->setPixmap(waterAlexPixmap);
	_fire->setPixmap(fireJpPixmap);

	_layout = new QVBoxLayout();
	_littleLayout = new QVBoxLayout();
	_bigLayout = new QHBoxLayout(this);
	_littleLayout->setAlignment(Qt::AlignCenter);

	_title = new QLabel("Fire JP & Water Alex");
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
	
	_bigLayout->addStretch();
	_bigLayout->addWidget(_fire);
	_bigLayout->addStretch();
	_bigLayout->addLayout(_layout);
	_bigLayout->addStretch();
	_bigLayout->addWidget(_water);
	_bigLayout->addStretch();

	this->setLayout(_bigLayout);
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



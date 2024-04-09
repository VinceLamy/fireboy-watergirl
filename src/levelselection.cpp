#include "levelselection.h"

LevelSelection::LevelSelection(QWidget* parent) : QWidget(parent)
{
	initUi();
}

LevelSelection::~LevelSelection()
{
}

void LevelSelection::initUi()
{
	_layout = new QVBoxLayout(this);
	_layout->setAlignment(Qt::AlignHCenter);
	_layout->addStretch();

	_level1 = new MenuButton("Level 1");
	connect(_level1, &QPushButton::released, this, &LevelSelection::Level1);
	_layout->addWidget(_level1);
	_layout->addStretch();

	_level2 = new MenuButton("Level 2");
	connect(_level2, &QPushButton::released, this, &LevelSelection::Level2);
	_layout->addWidget(_level2);
	_layout->addStretch();

	_level3 = new MenuButton("Level 3");
	connect(_level3, &QPushButton::released, this, &LevelSelection::Level3);
	_layout->addWidget(_level3);
	_layout->addStretch();

	_level4 = new MenuButton("Level 4");
	connect(_level4, &QPushButton::released, this, &LevelSelection::Level4);
	_layout->addWidget(_level4);
	_layout->addStretch();

	_level5 = new MenuButton("Level 5");
	connect(_level5, &QPushButton::released, this, &LevelSelection::Level5);
	_layout->addWidget(_level5);
	_layout->addStretch();
}

void LevelSelection::Level1()
{
	int level = 1;
	emit levelSelected(level);
}

void LevelSelection::Level2()
{
	int level = 2;
	emit levelSelected(level);
}

void LevelSelection::Level3()
{
	int level = 3;
	emit levelSelected(level);
}

void LevelSelection::Level4()
{
	int level = 4;
	emit levelSelected(level);
}

void LevelSelection::Level5()
{
	int level = 5;
	emit levelSelected(level);
}

#include "InGameMenu.h"

InGameMenu::InGameMenu(QWidget* parent) : QWidget(parent)
{
    setupUI();
    connectSignalsSlots();
}

InGameMenu::~InGameMenu()
{
}

void InGameMenu::setupUI()
{
    layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);

    resumeButton = new MenuButton("Resume");
    quitToMainButton = new MenuButton("Main Menu");
    restartButton = new MenuButton("Restart");

    layout->addStretch();
    layout->addWidget(resumeButton);
    layout->addStretch();
    layout->addWidget(restartButton);
    layout->addStretch();
    layout->addWidget(quitToMainButton);
    layout->addStretch();
    setLayout(layout);
}

void InGameMenu::connectSignalsSlots()
{
    connect(resumeButton, &QPushButton::clicked, this, &InGameMenu::resumeGame);
    connect(quitToMainButton, &QPushButton::clicked, this, &InGameMenu::quitToMainMenu);
    connect(restartButton, &QPushButton::clicked, this, &InGameMenu::restartGame);
}


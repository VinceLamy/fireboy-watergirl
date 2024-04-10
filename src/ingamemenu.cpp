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

    resumeButton = new QPushButton("Resume", this);
    quitToMainButton = new QPushButton("Quit to Main Menu", this);
    // rajouter bouton manquant********

    layout->addStretch();
    layout->addWidget(resumeButton);
    layout->addWidget(quitToMainButton);
    setLayout(layout);
}

void InGameMenu::connectSignalsSlots()
{
    connect(resumeButton, &QPushButton::clicked, this, &InGameMenu::resumeGame);
    connect(quitToMainButton, &QPushButton::clicked, this, &InGameMenu::quitToMainMenu);
}

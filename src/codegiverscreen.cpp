#include "codegiverscreen.h"

CodeGiverScreen::CodeGiverScreen(QString s, QWidget* parent) : QWidget(parent)
{
	code = s;
	InitUI();
}

CodeGiverScreen::~CodeGiverScreen()
{
}

void CodeGiverScreen::Ok()
{
	emit SendOk();
}

void CodeGiverScreen::InitUI()
{
	QFont titleFont("Arial", 50, QFont::Bold);
	QFont codeFont("Arial", 150, QFont::Bold);

	title = new QLabel("This is a part of the Lock's code");
	title->setFont(titleFont);
	codeLabel = new QLabel(code);
	title->setStyleSheet("color: #FFFFFF");
	codeLabel->setFont(codeFont);
	ok = new MenuButton("Ok");
	codeLabel->setStyleSheet("color: #FFFFFF");

	top = new QVBoxLayout();
	mid = new QVBoxLayout();
	bot = new QVBoxLayout();
	top->setAlignment(Qt::AlignCenter);
	mid->setAlignment(Qt::AlignCenter);
	bot->setAlignment(Qt::AlignCenter);

	this->setLayout(top);
	top->addStretch();
	top->addWidget(title);
	top->addStretch();
	top->addLayout(mid);
	mid->addWidget(codeLabel);
	top->addStretch();
	top->addLayout(bot);
	top->addStretch();
	bot->addWidget(ok);

	connect(ok, &MenuButton::released, this, &CodeGiverScreen::Ok);
}

#include "codeinputmenu.h"

CodeInputMenu::CodeInputMenu(CodeLock* code, QWidget* parent) : QWidget(parent)
{
	InitMenu();
	_code = code;
}

CodeInputMenu::~CodeInputMenu()
{

}

void CodeInputMenu::InitMenu()
{
	QFont titleFont("Arial", 50, QFont::Bold);

	_layout = new QVBoxLayout(this);
	_littleLayout = new QVBoxLayout();
	_littleLayout->setAlignment(Qt::AlignCenter);

	_title = new QLabel("Enter your Four Digits Code");
	_layout->addSpacing(100);
	_title->setAlignment(Qt::AlignHCenter);
	_title->setFont(titleFont);
	_title->setStyleSheet("color: #FFFFFF");
	_layout->addWidget(_title);
	_layout->addLayout(_littleLayout);
	_littleLayout->addStretch(2);

	_number = new QLineEdit();
	_number->setStyleSheet("QLineEdit {" "border-image: url(./sprite/menu/red_button10.png);" "}");
	_number->setFixedSize(400, 50);
	_number->setAlignment(Qt::AlignCenter);

	_confirm = new MenuButton("Confirm");
	connect(_confirm, &QPushButton::released, this, &CodeInputMenu::Back);

	_littleLayout->addWidget(_number);
	_littleLayout->addStretch(1);
	_littleLayout->addWidget(_confirm);
	_littleLayout->addStretch(1);
	this->setLayout(_layout);
}

void CodeInputMenu::Back()
{
	emit VerifyCode(_number->text(), _code);
}

#include "CodeInputMenu.h"

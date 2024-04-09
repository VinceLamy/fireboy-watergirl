#include "menubutton.h"
#define BUTTON_H_SIZE 400
#define BUTTON_V_SIZE 50


MenuButton::MenuButton(QString s, QPushButton* parent) : QPushButton(parent)
{
	QFont buttonFont("Arial", 12, QFont::Bold);

	setText(s);
	setFont(buttonFont);
	setFixedSize(BUTTON_H_SIZE, BUTTON_V_SIZE);
	setStyleSheet("QPushButton {" "border-image: url(./sprite/menu/blue_button00.png);" "}" "QPushButton:hover {" "border-image: url(./sprite/menu/red_button11.png);" "}" "QPushButton:pressed {" "border-image: url(./sprite/menu/red_button10.png);" "}");
}

MenuButton::~MenuButton()
{
}

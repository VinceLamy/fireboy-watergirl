#include <QPushButton>
#include <QString>

#ifndef MENUBUTTON_H
#define MENUBUTTON_H

class MenuButton : public QPushButton
{
	Q_OBJECT

public:
	MenuButton(QString s, QPushButton* parent = nullptr);
	~MenuButton();
};
#endif MENUBUTTON_H


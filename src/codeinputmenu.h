#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include "menubutton.h"
#include "enum.h"

#ifndef CODEINPUTMENU_H
#define CODEINPUTMENU_H

class CodeInputMenu : public QWidget
{
	Q_OBJECT

public:
	CodeInputMenu(QWidget* parent = nullptr);
	~CodeInputMenu();

signals:
	void VerifyCode(QString code);

private slots:
	void Back();

private:
	QVBoxLayout* _layout;
	QVBoxLayout* _littleLayout;
	QLabel* _title;
	QLineEdit* _number;
	MenuButton* _confirm;

	void InitMenu();
};
#endif CODEINPUTMENU_H
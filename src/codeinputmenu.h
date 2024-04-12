#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include "menubutton.h"
#include "enum.h"
#include "codelock.h"

#ifndef CODEINPUTMENU_H
#define CODEINPUTMENU_H

class CodeInputMenu : public QWidget
{
	Q_OBJECT

public:
	CodeInputMenu(CodeLock* code, QWidget* parent = nullptr);
	~CodeInputMenu();

signals:
	void VerifyCode(QString code, CodeLock* CodeLock);

private slots:
	void Back();

private:
	QVBoxLayout* _layout;
	QVBoxLayout* _littleLayout;
	QLabel* _title;
	QLineEdit* _number;
	MenuButton* _confirm;
	CodeLock* _code;

	void InitMenu();
};
#endif CODEINPUTMENU_H
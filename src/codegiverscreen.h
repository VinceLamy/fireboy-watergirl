#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include "menubutton.h"

#ifndef CODEGIVERSCREEN_H
#define CODEGIVERSCREEN_H
class CodeGiverScreen : public QWidget
{
	Q_OBJECT
public:
	CodeGiverScreen(QString s, QWidget* parent = nullptr);
	~CodeGiverScreen();

public slots:
	void Ok();

signals:
	void SendOk();


private:
	QString code;
	QLabel* title;
	QLabel* codeLabel;
	QVBoxLayout* top;
	QVBoxLayout* mid;
	QVBoxLayout* bot;
	MenuButton* ok;
	void InitUI();
};
#endif CODEGIVERSCREEN_H

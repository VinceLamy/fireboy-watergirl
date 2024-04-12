#ifndef CODELOCK_H
#define CODELOCK_H

#include "controller.h"
#include "codegiver.h"
#include "./Communication/communication.h"
#include <iostream>
#include <vector>

#define RESET	"\033[0m"
#define BRIGHTYELLOW	"\033[93m"

//using namespace std;

struct CodeDigits {
	int first;
	int second;
	int third;
	int fourth;
};

class CodeLock : public Controller
{
public:
	CodeLock(int code, QPixmap& pixmap, qreal x, qreal y, std::vector<CodeGiver*> codeGiver);
	void GiveCode(CodeDigits code);
	void VerifyCode();
	void GenerateCode(int code);
	/*void Show();*/
	void Clear();

private:
	CodeDigits _myCode = { 0, 0, 0, 0 };
	std::vector<CodeGiver*> _codeGiver;
};

#endif CODELOCK_H

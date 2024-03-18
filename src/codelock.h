#ifndef CODELOCK_H
#define CODELOCK_H

#include "controller.h"
#include "codegiver.h"
#include <iostream>
#include <vector>

using namespace std;

struct CodeDigits {
	int first;
	int second;
	int third;
	int fourth;
};

class CodeLock : public Controller
{
public:
	CodeLock(int x, int y, vector<CodeGiver*> codeGiver);
	~CodeLock();
	void GiveCode(CodeDigits code);
	void VerifyCode();
	void GenerateCode();
	void Show();

private:
	CodeDigits _myCode;
	vector<CodeGiver*> _codeGiver;
	int _codeType;
};

#endif CODELOCK_H

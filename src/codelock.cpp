#include "codelock.h"
#include "Windows.h"
#include <time.h>
#include <iostream>
#include <string>

using namespace std;

CodeLock::CodeLock(int x, int y, vector<CodeGiver*> codeGiver)
{
	SetPosition(x, y);
	SetType(CODELOCK);
	GenerateCode();
	_codeGiver = codeGiver;
	GiveCode(_myCode);
}

CodeLock::~CodeLock()
{
}

void CodeLock::GiveCode(CodeDigits code)
{
	if (_codeGiver.size() == 1)
	{
		_codeGiver[0]->SetDigits(to_string(_myCode.first) + to_string(_myCode.second) + to_string(_myCode.third) + to_string(_myCode.fourth));
	}
	else if (_codeGiver.size() == 2)
	{
		_codeGiver[0]->SetDigits(to_string(_myCode.first) + to_string(_myCode.second));
		_codeGiver[1]->SetDigits(to_string(_myCode.third) + to_string(_myCode.fourth));
	}
	else if (_codeGiver.size() == 4)
	{
		_codeGiver[0]->SetDigits(to_string(_myCode.first));
		_codeGiver[1]->SetDigits(to_string(_myCode.second));
		_codeGiver[2]->SetDigits(to_string(_myCode.third));
		_codeGiver[3]->SetDigits(to_string(_myCode.fourth));
	}
}

void CodeLock::VerifyCode()
{
	string playerCode;
	cout << "Enter the 4 digits code" << endl;
	cin >> playerCode;


	string codeToCheck = to_string(_myCode.first) +
		to_string(_myCode.second) +
		to_string(_myCode.third) +
		to_string(_myCode.fourth);

	if (playerCode == codeToCheck)
	{
		cout << "\nCorrect" << endl;
		Sleep(2000);
		SetState(OPEN);
	}
	else
	{
		cout << "\nIncorrect" << endl;
		Sleep(2000);
	}
}

void CodeLock::GenerateCode()
{
	srand(time(NULL));
	_myCode.first = rand() % 10;
	_myCode.second = rand() % 10;
	_myCode.third = rand() % 10;
	_myCode.fourth = rand() % 10;
}

void CodeLock::Show()
{
	cout << '$';
}

#include "codelock.h"
#include "Windows.h"
#include <time.h>
#include <iostream>
#include <string>
#include <conio.h>
#include<limits>


CodeLock::CodeLock(QPixmap& pixmap, qreal x, qreal y, std::vector<CodeGiver*> codeGiver) : Controller(pixmap, x, y)
{
	GenerateCode();
	_codeGiver = codeGiver;
	GiveCode(_myCode);
}

void CodeLock::GiveCode(CodeDigits code)
{
	if (_codeGiver.size() == 1)
	{
		_codeGiver[0]->SetDigits(std::to_string(_myCode.first) +
			std::to_string(_myCode.second) +
			std::to_string(_myCode.third) +
			std::to_string(_myCode.fourth));
	}
	else if (_codeGiver.size() == 2)
	{
		_codeGiver[0]->SetDigits(std::to_string(_myCode.first) + std::to_string(_myCode.second));
		_codeGiver[1]->SetDigits(std::to_string(_myCode.third) + std::to_string(_myCode.fourth));
	}
	else if (_codeGiver.size() == 4)
	{
		_codeGiver[0]->SetDigits(std::to_string(_myCode.first));
		_codeGiver[1]->SetDigits(std::to_string(_myCode.second));
		_codeGiver[2]->SetDigits(std::to_string(_myCode.third));
		_codeGiver[3]->SetDigits(std::to_string(_myCode.fourth));
	}
}

void CodeLock::VerifyCode()
{
	std::string playerCode;
	std::cout << "Enter the 4 digits code" << std::endl;
	std::cin.clear();
	fflush(stdin);
	std::cin >> playerCode;


	std::string codeToCheck = std::to_string(_myCode.first) +
		std::to_string(_myCode.second) +
		std::to_string(_myCode.third) +
		std::to_string(_myCode.fourth);

	if (playerCode == codeToCheck)
	{
		std::cout << "\nCorrect" << std::endl;
		Sleep(2000);
		SetState(OPEN);
	}
	else
	{
		std::cout << "\nIncorrect" << std::endl;
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

void CodeLock::Clear()
{
	_myCode.first = 0;
	_myCode.second = 0;
	_myCode.third = 0;
	_myCode.fourth = 0;
}


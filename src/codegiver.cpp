#include "codegiver.h"
#include "Windows.h"

//using namespace std;

CodeGiver::CodeGiver(int x, int y)
{
	SetPosition(x, y);
	SetType(CODEGIVER);
}

std::string CodeGiver::ShowCode()
{
	return _mydigits;
}

void CodeGiver::SetDigits(std::string digits)
{
	_mydigits = digits;
}

void CodeGiver::Show()
{
	std::cout << BRIGHTYELLOW << '@' << RESET;
}

void CodeGiver::Clear()
{
	_mydigits = "";
}

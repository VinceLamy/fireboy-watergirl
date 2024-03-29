#include "codegiver.h"
#include "Windows.h"

using namespace std;

CodeGiver::CodeGiver(int x, int y)
{
	SetPosition(x, y);
	SetType(CODEGIVER);
}

CodeGiver::~CodeGiver()
{
	Clear();
}

string CodeGiver::ShowCode()
{
	return _mydigits;
}

void CodeGiver::SetDigits(string digits)
{
	_mydigits = digits;
}

void CodeGiver::Show()
{
	cout << BRIGHTYELLOW << '@' << RESET;
}

void CodeGiver::Clear()
{
	_mydigits = "";
}

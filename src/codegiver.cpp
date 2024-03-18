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
}

void CodeGiver::ShowCode()
{
	cout << _mydigits << endl;
	Sleep(2000);
}

void CodeGiver::SetDigits(string digits)
{
	_mydigits = digits;
}

void CodeGiver::Show()
{
	cout << '@';
}

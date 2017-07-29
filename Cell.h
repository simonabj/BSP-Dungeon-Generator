#pragma once
#include <string>

class Cell
{
private:
	int DivisionIdentifier = 0;
	char repChar = '#';
public:
	Cell();
	void SetIdentifier(int identifier);
	int GetIdentifier();
	char to_c();
	std::string to_s(int length);
};


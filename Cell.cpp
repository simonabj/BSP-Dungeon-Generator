#include "Cell.h"

Cell::Cell()
{
}

void Cell::SetIdentifier(int identifier) {
	this->DivisionIdentifier = identifier;
	this->repChar = '0' + identifier + ((identifier >= 94) ? 1 : 0) ;
};

int Cell::GetIdentifier() {
	return this->DivisionIdentifier;
}

char Cell::to_c() {
	return repChar;
}

std::string Cell::to_s(int length) {
	std::string temp = " ";
	for (int i = length-1; i >= 0; i--)
	{
		temp += 48+((this->DivisionIdentifier >> i) & 1);
	}
	temp += " ";
	return temp;
}
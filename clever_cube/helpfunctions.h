#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include <vector>

using std::string;
using std::vector;

int GetRandomNumber(int, int);
void createCustomEllipse(HDC, RECT);
vector <string> splitString(const string&, const char&);
void makeGate(int, RECT&, RECT, int*);

#endif

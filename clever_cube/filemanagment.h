#ifndef FILEMANAGEMENT_H
#define FILEMANAGEMENT_H

#include <string>
#include <windows.h>

using std::string;

void getLevelInfo(const string&, string&, double&, double&, double&, double&, double&, double&, double&);
void getSavedGame(int&, int&);
BOOL saveGame(HWND, int, int);
BOOL fileExists(LPCSTR);
void makeBarriers(RECT*, double[][100], int, int*, RECT);
void getLevelInfo(const string&, string &, double&, double&, double&, double&, double&, double&, double&);
void CleanFiles(int onlyProgress = 0);

#endif

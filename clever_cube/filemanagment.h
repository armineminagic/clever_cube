#ifndef FILEMANAGEMENT_H
#define FILEMANAGEMENT_H

#include <string>

void getSavedGame(int&, int&);
BOOL saveGame(HWND, int, int);
BOOL fileExists(LPCSTR);
void getInfo(const std::string&, std::string &, double&, double&, double&, double&, double&, double&, double&);
void getInfo(const std::string&, int&, int&);
void CleanFiles(int onlyProgress = 0);

#endif

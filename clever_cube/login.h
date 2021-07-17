#ifndef LOGIN_H
#define LOGIN_H

#include "sqlite3.h"
#include <vector>
using std::string;

void createLoginItems(HWND&, HWND[]);
void createLoginItems(HWND&, std::vector<HWND>&);
int callback(void *, int , char **, char **);
void createLoginSItems(HWND&, HWND*);
int Login(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, int &CONTROL_WINDOW, string &uname, string&password, sqlite3* db);

#endif

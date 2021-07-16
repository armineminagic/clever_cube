#ifndef LOGIN_H
#define LOGIN_H

#include "sqlite3.h"

using std::string;

void createLoginSItems(HWND&, HWND[]);
int callback(void *, int , char **, char **);
void createLoginSItems(HWND&, HWND*);
int Login(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, int &OPTION, string &uname, string&password, sqlite3* db);

#endif

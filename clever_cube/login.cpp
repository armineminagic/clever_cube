#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <string>
#include <commctrl.h>

#include "defaults.h"
#include "sqlite3.h"
#include "login.h"
#include "buttons.h"

using std::string;
using std::cout;
using std::endl;

int first_line=1;
int size_column,num_col=0;
int select_query = 0;
char* err = 0;
static HWND hwndlist;
int Login(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, int &OPTION, string &uname, string&password, sqlite3* db) {
    //size_t passwordLength, usernameLength = 0;

	static HDC hdc;

     switch (message)                  /* handle the messages */
    {
        case WM_CREATE:

            static HWND BUTTONS[10] = {};
            createLoginSItems(hwnd, BUTTONS);

			ReleaseDC(hwnd, hdc);
            break;
        case WM_COMMAND: { // here we can handle the buttons
		switch (LOWORD(wParam)){
				case LOGIN_BTN : {

					int status;
					//sqlite3_open("baza", &db);
					char username[64];
					char *err = 0;
					HWND hwndeditUname = GetDlgItem(hwnd, USERNAME_ENTRY);
					memset(&username, '\0', sizeof(username)/sizeof(username[0]));
					GetWindowText(hwndeditUname, username, sizeof(username));
					if(strlen(username) == 0){
						MessageBox(hwnd, TEXT("Please enter your username"), TEXT("Wrong input"), MB_OK);
						uname = username;
						break;
					}

					char password[64];
					HWND hwndeditPw = GetDlgItem(hwnd, PASSWORD_ENTRY);
					memset(&password, '\0', sizeof(password)/sizeof(password[0]));
					GetWindowText(hwndeditPw, password, sizeof(password));
					if(strlen(password) == 0){
						MessageBox(hwnd, TEXT("Please enter your password"), TEXT("Wrong input"), MB_OK);
						break;
					}

					auto sql = (string("SELECT id FROM user WHERE username = " ) + string(username) + string(" AND password = ") + string(password)).c_str();

					cout << sql << endl;
					OPTION = 1;
                    DestroyButtons(BUTTONS);
                    SendMessage(hwnd, WM_CREATE, wParam, lParam);
                    SetFocus(hwnd);
                    InvalidateRect(hwnd, NULL, false);
                    UpdateWindow(hwnd);
					status = sqlite3_exec(db, sql, callback, 0, &err);
					cout << status << endl;

				break;
			}
			}
		switch(wParam) {

			case PLAYGAME_BTN : {
                OPTION = 1;
                DestroyButtons(BUTTONS);
                SendMessage(hwnd, WM_CREATE, wParam, lParam);
                SetFocus(hwnd);
                InvalidateRect(hwnd, NULL, false);
                UpdateWindow(hwnd);
				break;
			}
		}

		return 0;
    }
        case WM_DESTROY:
            PostQuitMessage (0);
            break;
    }
    return 0;
}

void createLoginSItems(HWND &hwnd, HWND BUTTONS[])
{
    BUTTONS[0] = CreateWindow("Static","Player:",WS_VISIBLE|WS_CHILD|WS_BORDER,250,50,100,20,hwnd,NULL,NULL,NULL);

    BUTTONS[1] = CreateWindow("Edit","",WS_VISIBLE|WS_CHILD|WS_BORDER|ES_MULTILINE|ES_AUTOVSCROLL|ES_AUTOHSCROLL,310,50,200,20,hwnd,(HMENU)USERNAME_ENTRY,GetModuleHandle(NULL),NULL);

    BUTTONS[2] =CreateWindow("Static","Password:",WS_VISIBLE|WS_CHILD|WS_BORDER,250,100,100,20,hwnd,NULL,NULL,NULL);

    BUTTONS[3] =CreateWindow("Edit","",WS_VISIBLE|WS_CHILD|WS_BORDER|ES_MULTILINE|ES_AUTOVSCROLL|ES_AUTOHSCROLL,350,100,160,20,hwnd,(HMENU)PASSWORD_ENTRY,GetModuleHandle(NULL),NULL);

    BUTTONS[4] = CreateWindow("Button","Log in",WS_VISIBLE|WS_CHILD,150,304,100,50,hwnd,(HMENU)LOGIN_BTN,NULL,NULL);

    BUTTONS[5] = CreateWindow("Button","PlayGame",WS_VISIBLE|WS_CHILD,550,304,100,50,hwnd,(HMENU)PLAYGAME_BTN,NULL,NULL);
}

int callback(void *pArg, int argc, char **argv, char **imekolone)
{
    LVITEM lvi;
    if(first_line)
    {
        LVCOLUMN lvc;
        num_col = argc - 1;
        for(int i = 0; i<argc; ++i)
        {
            lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM ;
            lvc.fmt = LVCFMT_LEFT;
            lvc.cx = size_column / argc;
            lvc.pszText = imekolone[i];
            lvc.iSubItem = i;
            ListView_InsertColumn(hwndlist,i,&lvc);
        }
        first_line = 0;
    }
    memset(&lvi,0,sizeof(lvi));
    lvi.mask = LVIF_TEXT;
    lvi.pszText = argv[0];
    lvi.iSubItem = 0;
    ListView_InsertItem(hwndlist, &lvi);
    printf("|\t%s\t", argv[0]);
    for (int i = 1; i < argc; i++)
    {
        printf("|\t%s\t", argv[i]);
        lvi.pszText = argv[i];
        lvi.iSubItem = i;
        ListView_SetItem(hwndlist, &lvi);
    }
    printf("| \n");
    return 0;
}

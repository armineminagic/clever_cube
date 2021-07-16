#include <windows.h>
#include <commctrl.h>
#include "sqlite3.h"

#include "defaults.h"
#include "playgame.h"
#include "login.h"


LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
sqlite3* db;

LPINITCOMMONCONTROLSEX lpInitCtrls;
HBITMAP FrogImg, FrogImgMask;

int WINAPI WinMain (HINSTANCE hInstance,
                     HINSTANCE prevInstance,
                     LPSTR cmdLine,
                     int cmdShow)
{



	InitCommonControlsEx(lpInitCtrls);
    UNREFERENCED_PARAMETER( prevInstance );
    UNREFERENCED_PARAMETER( cmdLine );

    WNDCLASSEX wndClass = { 0 };
    wndClass.cbSize = sizeof( WNDCLASSEX ) ;
    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProc;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wndClass.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor (NULL, IDC_ARROW);
    wndClass.hbrBackground = CreateSolidBrush(RGB(57, 162, 219));
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = "Clever cube";

    if( !RegisterClassEx( &wndClass ) )
        return -1;

    RECT rc = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
    HWND hwnd = CreateWindowA(
        "Clever cube",
        GAME_TITLE,
        WS_CLIPCHILDREN|WS_OVERLAPPEDWINDOW&~WS_THICKFRAME&~WS_MAXIMIZEBOX,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rc.right - rc.left,
        rc.bottom - rc.top,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if( !hwnd )
        return -1;

    ShowWindow( hwnd, cmdShow );

    MSG msg = {0};

    while( msg.message != WM_QUIT )
    {
		Sleep(2);
        if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
    }

    return static_cast<int>( msg.wParam );
}

LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	static int OPTION = 0;
    string username, password;
	if(OPTION == 1) {
	    playGame(hwnd, message, wParam, lParam, OPTION, username);
	} else {
        Login(hwnd, message, wParam, lParam, OPTION, username, password, db);
	}
	return DefWindowProc (hwnd, message, wParam, lParam);
}



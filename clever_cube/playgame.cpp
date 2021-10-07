#include <windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <unistd.h>
#include <time.h>

#include "defaults.h"
#include "helpfunctions.h"
#include "filemanagment.h"
#include "movement.h"
#include "colision.h"
#include "buttons.h"
#include "barriers.h"

using std::ifstream;
using std::cout;
using std::endl;
using std::string;
using std::vector;

int playGame(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, int &CONTROL_WINDOW, const std::string& username)
{
    string uname = username;

    HDC hdc;
    static HDC hdcBuffer;

    static HBITMAP hBitmap;

    PAINTSTRUCT ps;

    static vector<HWND> BUTTONS;
    static HBRUSH color[50] = {};

    // player size
    static int PlayerWidth = 45;
    static int PlayerHeight = 45;

    static int GateInfo[2] = {};
    GateInfo[0] = 15;
    GateInfo[1] = GetRandomNumber(45,65);

    static int Level = 1;
    static int PreviousLevel = 1;

    static int Deaths = 0;
    static int PreviousDeaths = 0;

    static RECT client = {}, prevState = {};

    static RECT player = {};
    static RECT mainPlayerPos = {};
    static RECT gameBoxWnd = {};

    static RECT door = {};

    static RECT barriers[100] = {};
    static double barriersInfo[100][100] = {}; // barriers info
    static vector<RECT> bars;

    static int theEnd = -1;

    char cwd[MAX_PATH_LENGTH] = {};
    int ctrl = 0;
    static int move = 10; // step of the player

    static int offset[4] = {};
    offset[0] = 30; // left
    offset[1] = 35; // right
    offset[2] = 80; // top
    offset[3] = 35; // bottom

    static int width, height;

    switch (message)
    {
    case WM_NCCREATE :
    {
        cout << PreviousDeaths << " " << PreviousLevel << endl;
        getSavedGame(PreviousLevel, PreviousDeaths);
        cout << PreviousDeaths << " " << PreviousLevel << endl;
        Level = PreviousLevel;
        Deaths = PreviousDeaths;
        break;
    }
    case WM_CREATE:
    {
		hdc = GetDC(hwnd);
		hdcBuffer = CreateCompatibleDC(NULL);

        PlaySound("levelstart.wav", NULL, SND_FILENAME | SND_ASYNC);
        Deaths = 0;
        Level = 1;
        theEnd = 0;

        getcwd(cwd, MAX_PATH_LENGTH);

        GetClientRect (hwnd, &client);

		width = client.right;
        height = client.bottom;

		hBitmap = CreateCompatibleBitmap(hdc, width, height);

        BUTTONS.push_back(CreateWindow(TEXT("button"),  TEXT(BTN_END_LBL), WS_VISIBLE | WS_CHILD,
                                       client.right - offset[1] - 80, client.bottom - offset[3],
                                       80, 25, hwnd, (HMENU)END_BTN, NULL, NULL));

        BUTTONS.push_back(CreateWindow(TEXT("button"),  TEXT(BTN_RESTART_LABEL_LBL), WS_VISIBLE | WS_CHILD,
                                       client.right - offset[1] - 190, client.bottom - offset[3],
                                       100, 25, hwnd, (HMENU)RESTART_BTN, NULL, NULL));

        BUTTONS.push_back(CreateWindow(TEXT("button"),  TEXT(BTN_SAVE_LABEL_LBL), WS_VISIBLE | WS_CHILD,
                                       client.right - offset[1] - 300, client.bottom - offset[3],
                                       120, 25, hwnd, (HMENU)SAVEGAME_BTN, NULL, NULL));


		color[0] = CreateSolidBrush (RGB(172, 255, 173)); // Game window
        color[1] = CreateSolidBrush (RGB(84, 67, 107)); // player purple
        color[2] = CreateSolidBrush(RGB(80, 203, 147)); // door
        color[3] = CreateSolidBrush(RGB(18, 93, 152)); // barrier rectangle
        color[4] = CreateSolidBrush(RGB(255, 97, 109)); // barrier circle
        color[5] = CreateSolidBrush(RGB(198, 139, 89)); // wall


		SelectObject(hdcBuffer, hBitmap);

        gameBoxWnd.left = client.left + offset[0];
        gameBoxWnd.right = client.right - offset[1];
        gameBoxWnd.top = client.top + offset[2];
        gameBoxWnd.bottom = client.bottom - offset[3];

        mainPlayerPos.right = client.left + offset[0] + PlayerWidth;
        mainPlayerPos.left = client.left + offset[0];
        mainPlayerPos.top = (client.top + client.bottom) /2;
        mainPlayerPos.bottom = (client.top + client.bottom)/2 + PlayerHeight;

        player = mainPlayerPos;

        createDoor(offset[2], door, gameBoxWnd, GateInfo);
        createBarriers(barriers, barriersInfo, Level, offset, mainPlayerPos);

        SetTimer(hwnd,1,UPDATE_RATE, NULL);

        ReleaseDC(hwnd, hdc);

        return 0;
    }

    case WM_PAINT:
    {
        hdc = BeginPaint( hwnd, &ps );

        BitBlt(hdcBuffer,0,0,width,height, NULL, NULL, NULL, WHITENESS);

        if(!theEnd)
        {
            DrawText(hdcBuffer, GAME_TITLE, -1, &client, DT_CENTER);
            DrawText(hdcBuffer, EXPLAIN, -1, &client, DT_CENTER);

            char tmpText[100];
            sprintf_s(tmpText, LEVEL_TXT, uname.c_str(), Level);
            DrawText(hdcBuffer, TEXT(tmpText), -1, &client, DT_LEFT);

            sprintf_s(tmpText, DEATHS_TXT,Deaths);
            DrawText(hdcBuffer, TEXT(tmpText), -1, &client, DT_RIGHT);

            FillRect(hdcBuffer, &gameBoxWnd, color[0]);
            FrameRect(hdcBuffer, &gameBoxWnd, CreateSolidBrush(RGB(0,0,0)));

            FillRect (hdcBuffer, &player, color[1]);

            FillRect (hdcBuffer, &door, color[2]);

            for(long long unsigned int i = 0; i < sizeof(barriers)/sizeof(barriers[0]); i++)
            {
                if(barriersInfo[i][B_TYPE] == 1) // Rectangle
                    FillRect(hdcBuffer, &barriers[i], color[3]);
                else if(barriersInfo[i][B_TYPE] == 2)   // Ellipse
                {
                    SelectObject(hdcBuffer, color[4]);
                    createCustomEllipse(hdcBuffer, barriers[i]);
                }
                else if(barriersInfo[i][B_TYPE] == 3) // Wall
                {
                    if(barriersInfo[i][B_MOVE] == 1)
                    {
                        FillRect(hdcBuffer, &barriers[i], color[5]);
                    }
                    else if(barriersInfo[i][B_MOVE] == 2)
                    {
                        SelectObject(hdcBuffer, color[5]);
                        createCustomEllipse(hdcBuffer, barriers[i]);
                    }
                }
            }

            BitBlt(hdc, 0, 0, width, height, hdcBuffer, 0, 0, SRCCOPY);

            EndPaint(hwnd,&ps);

            ReleaseDC(hwnd, hdc);
            ReleaseDC(hwnd, hdcBuffer);

            for(long long unsigned int i = 0; i < sizeof(barriers)/sizeof(barriers[0]); i++)
            {
                if(barriersInfo[i][B_TYPE] != 3 && gameOver(hwnd, player, barriers[i], mainPlayerPos, CONTROL_WINDOW, Level, Deaths, BUTTONS, wParam, lParam))
                {
                    createBarriers(barriers, barriersInfo, Level, offset, mainPlayerPos);
                    Deaths++;
                    break;
                }
            }

        }
        else
        {
			char tmpText[100];
            sprintf_s(tmpText, FINISH_GAME, uname.c_str(), Level);
            DrawText(hdcBuffer, TEXT(tmpText), -1, &client, DT_CENTER);

            // Display the back buffer
            BitBlt(hdc, 0, 0, width, height, hdcBuffer, 0, 0, SRCCOPY);

            EndPaint(hwnd,&ps);
            ReleaseDC(hwnd, hdc);
            ReleaseDC(hwnd, hdcBuffer);
        }

        return 0;

    }
    case WM_KEYDOWN:
    {

        prevState = player;

        movePlayer(player, gameBoxWnd, move, ctrl);

        for(long long unsigned int i = 0; i < sizeof(barriers)/sizeof(barriers[0]); i++)
        {
            if(barriersInfo[i][B_TYPE] == 3)
            {
                checkWall(player, barriers[i], ctrl-1, prevState);
            }
        }

        if(ctrl)
        {
            InvalidateRect(hwnd,NULL,false);
            UpdateWindow(hwnd);
        }
        if(player.right > door.left && rectanglesOverlap(door,player))
        {
            KillTimer(hwnd,1);
            PlaySound("win.wav", NULL, SND_FILENAME | SND_ASYNC);

            MessageBox(hwnd, TEXT(WIN_TXT), TEXT(WIN_TXT_TITLE), MB_OK);

            char tmp[100];
            sprintf_s(tmp, "LEVELS/level%d.txt", Level+1);
            ifstream file;
            file.open(tmp, std::ifstream::out);
            if(file.good())
            {
                createDoor(offset[2], door, gameBoxWnd, GateInfo);

                SetTimer(hwnd,1,UPDATE_RATE, NULL);

                player = mainPlayerPos;
                Level++;

                createBarriers(barriers, barriersInfo, Level, offset, mainPlayerPos);
                theEnd = 0;
            }
            else
            {
                player.left = 0;
                player.bottom = 0;
                theEnd = 1;
            }

            InvalidateRect(hwnd,NULL,false);
            UpdateWindow(hwnd);
        }

        return 0;
    }
    case WM_TIMER :
    {
        auto size = sizeof(barriers)/sizeof(barriers[0]);
        for(long long unsigned int i = 0; i < size; i++)
        {
            if(barriersInfo[i][B_TYPE] == 3)
                continue;

            if(barriersInfo[i][B_MOVE] == 1) // rectangle
                moveUpAndDown(barriers[i],gameBoxWnd,(long)barriersInfo[i][B_STEP],i,barriers,barriersInfo);
            else if(barriersInfo[i][B_MOVE] == 2) // ellipse
                moveInCircle(barriers[i],gameBoxWnd,barriersInfo[i][B_STEP],barriersInfo[i][B_RADIUS],i,barriers, barriersInfo);
            else if(barriersInfo[i][B_MOVE] == 3) // wall
                moveBounceOnWalls(barriers[i],gameBoxWnd,(long)barriersInfo[i][B_STEP],i,barriers,barriersInfo);
            else if(barriersInfo[i][B_MOVE] == 4)
                followThePlayer(barriers[i],player, gameBoxWnd,(long)barriersInfo[i][B_STEP]);
            else if(barriersInfo[i][B_MOVE] == 5)
                moveLeftAndRight(barriers[i],gameBoxWnd,(long)barriersInfo[i][B_STEP],i,barriers,barriersInfo);
        }

        InvalidateRect(hwnd,NULL,false);
        UpdateWindow(hwnd);
    }

    case WM_COMMAND:
    {
        switch(wParam)
        {
        case END_BTN :
        {
            if(MessageBox(hwnd, TEXT("Save Game?"), TEXT("Ending game"), MB_YESNO) == IDYES)
            {
                CONTROL_WINDOW = 0;
                cout << "Saving process";
                theEnd = 0;
				Sleep(600);
				cout << ".";
				Sleep(600);
				cout << ".";
				Sleep(600);
				cout << "." << endl;
				saveGame(hwnd, Level, Deaths);
                DestroyButtons(BUTTONS);
                SendMessage(hwnd, WM_CREATE, wParam, lParam);
                SetFocus(hwnd);
                InvalidateRect(hwnd, NULL, TRUE);
                UpdateWindow(hwnd);
                return 1;
            }
            else
            {
                CONTROL_WINDOW = 0;
				Level = 1;
                Deaths = 0;
                DestroyButtons(BUTTONS);
                CleanFiles(1);
                SendMessage(hwnd, WM_CREATE, wParam, lParam);
                SetFocus(hwnd);
                InvalidateRect(hwnd, NULL, TRUE);
                UpdateWindow(hwnd);
                return 1;
            }
            break;
        }
        case RESTART_BTN :
        {
            KillTimer(hwnd,1);
            if(MessageBox(hwnd, TEXT(ARE_YOU_SURE_MSG), TEXT("Restart the Game"), MB_OKCANCEL) == IDOK)
            {
                Deaths = PreviousDeaths;
                player = mainPlayerPos;
                Level = PreviousLevel;
                theEnd = 0;
                createBarriers(barriers, barriersInfo, Level, offset, mainPlayerPos);
            }
            SetTimer(hwnd,1,UPDATE_RATE, NULL);
            break;
        }
        case SAVEGAME_BTN:
        {

            saveGame(hwnd, Level, Deaths);

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
        return 0;
    }
    return 0;
}


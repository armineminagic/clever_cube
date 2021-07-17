#include <windows.h>
#include <vector>

#include "defaults.h"
#include "buttons.h"
#include "filemanagment.h"


using std::vector;

// checks if the next move is outside of the container
bool outOfGameBox(RECT &player, RECT gameBoxWnd, int direction, int move)
{
    // left
    if(direction == 0 && (player.left - move < gameBoxWnd.left))
    {
        player.right = gameBoxWnd.left + (player.right - player.left);
        player.left = gameBoxWnd.left;
        return true;
    }
    // right
    if(direction == 1 && (player.right + move > gameBoxWnd.right))
    {
        player.left = gameBoxWnd.right - (player.right - player.left);
        player.right = gameBoxWnd.right;
        return true;
    }

    // up
    if(direction == 2 && (player.top - move < gameBoxWnd.top))
    {
        player.bottom = gameBoxWnd.top + (player.bottom - player.top);
        player.top = gameBoxWnd.top;
        return true;

    }

    // down
    if(direction == 3 && (player.bottom + move > gameBoxWnd.bottom))
    {
        player.top = gameBoxWnd.bottom - (player.bottom - player.top);
        player.bottom = gameBoxWnd.bottom;
        return true;
    }
    return false;

}


// Returns 1 if the rectangles overlap, 0 otherwise
int rectanglesOverlap(RECT a, RECT b)
{

    if (a.left >= b.right || a.right <= b.left ||
            a.top >= b.bottom || a.bottom <= b.top)
        return 0;
    return 1;
}


// makes sure that the player doesn't pass the wall
void checkWall(RECT &r, RECT barrier, int d, RECT p)
{

    if(rectanglesOverlap(r, barrier))
    {

        if((d == 3 || d == 2) && (p.right <= barrier.left))
            d = 1;

        if((d == 3 || d == 2) && (p.left >= barrier.right))
            d = 0;

        // right to left
        if(d == 0 && r.left < barrier.right)
        {
            r.right += barrier.right - r.left;
            r.left = barrier.right;
        }
        // left to right
        if(d == 1 && r.right > barrier.left)
        {
            r.left -= r.right - barrier.left;
            r.right = barrier.left;
        }
        // down to up
        if(d == 2 && r.top < barrier.bottom)
        {
            r.bottom += barrier.bottom - r.top;
            r.top = barrier.bottom;
        }
        // up to down
        if(d == 3 && r.bottom > barrier.top)
        {
            r.top -= r.bottom - barrier.top;
            r.bottom = barrier.top;
        }
    }

}


// game over
bool gameOver(HWND hwnd, RECT &player, RECT &barrier, RECT mainPlayerPos, int& CONTROL_WINDOW, int& Level, int& Deaths, vector<HWND>& BUTTONS, WPARAM wParam, LPARAM lParam)
{

    if(player.right > barrier.left && player.left < barrier.right && rectanglesOverlap(player, barrier))
    {
        PlaySound("crash.wav", NULL, SND_FILENAME | SND_ASYNC);
        KillTimer(hwnd,1);

        if(MessageBox(hwnd, TEXT(LOSE_TXT), TEXT(LOSE_TXT_TITLE), MB_YESNO) == IDYES)
        {

            SetTimer(hwnd,1,UPDATE_RATE, NULL);

            player = mainPlayerPos;

            while(rectanglesOverlap(mainPlayerPos, barrier))
            {
                barrier.left -= 4500;
                barrier.right -= 4500;

            }

            InvalidateRect(hwnd,NULL,false);
            UpdateWindow(hwnd);
            return true;
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
            return true;
        }
    }
    return false;
}

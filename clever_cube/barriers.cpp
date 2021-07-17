#include <windows.h>
#include <vector>
#include <direct.h>
#include <dirent.h>
#include <fstream>

#include "filemanagment.h"
#include "helpfunctions.h"
#include "defaults.h"
#include "colision.h"

using std::fill_n;
using std::ifstream;
using std::string;

// make the door
void createDoor(int offset, RECT &door, RECT gameBoxWnd, int GateInfo[])
{

    door.top = GetRandomNumber(offset,gameBoxWnd.bottom);
    door.left = gameBoxWnd.right - GateInfo[0];
    door.right = gameBoxWnd.right;
    if(door.top + GateInfo[1] > gameBoxWnd.bottom)
    {
        door.top -= door.top + GateInfo[1] - gameBoxWnd.bottom;
        door.bottom = gameBoxWnd.bottom;
    }
    else
    {
        door.bottom = door.top + GateInfo[1];
    }
}

// generate barriers
void createBarriers(RECT barriers[], double barriersInfo[][100], int Level, int offset[], RECT mainPlayerPos)
{
    TCHAR DEFAULT_DIR_PATH[MAX_PATH] = {0};
    RECT tmp = {};
    for(int i = 0; i < 100; i++)
    {
        barriers[i] = tmp;
        fill_n(barriersInfo[i], 5, 0);
    }

    SetCurrentDirectory(DEFAULT_DIR_PATH);

    if(!opendir("LEVELS"))
    {

        _mkdir("LEVELS");
        HANDLE hFile;

        DWORD wmWritten;

        char levelData[] = "Ellipse(150,250,30,30,1,25)\r\nRectangle(219,157,54,41,1,10)\r\nEllipse(371,148,95,97,1,10)\r\nRectangle(286,108,20,30,3,25)\r\nEllipse(550,148,95,97,1,15)\r\nRectangle(350,91,20,20,3,20)\r";

        hFile = CreateFile("LEVELS/level1.txt",GENERIC_READ|GENERIC_WRITE,
                           FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

        WriteFile(hFile,levelData,(DWORD)(sizeof(levelData)),&wmWritten,NULL);

        CloseHandle(hFile);

    }

    char filename[100];
    sprintf_s(filename, "LEVELS/level%d.txt", Level);

    ifstream hFile(filename);

    if(!hFile)
        return;

    string line;
    string name = "";
    double left, top, right, bottom, mv, radius, step;
    double extra = 0;

    int i = 0;
    while(getline(hFile, line))
    {
        getInfo(line, name, left, top, right, bottom, mv, radius, step);

        if(name == "Rectangle")
            barriersInfo[i][B_TYPE] = 1; // Rectangle
        else if (name == "Ellipse")
            barriersInfo[i][B_TYPE] = 2; // Ellipse
        else if (name == "Wall")
            barriersInfo[i][B_TYPE] = 3; // Wall
        else
            continue;

        barriers[i].left = (long)(offset[0] + left);
        barriers[i].right = (long)(barriers[i].left + right);
        barriers[i].top = (long)(offset[2] + top);
        barriers[i].bottom = (long)(barriers[i].top + bottom);

        if (radius != 0)
        {
            barriersInfo[i][B_MOVE] = mv;
            extra = barriersInfo[i][B_RADIUS] = radius;
        }
        else
        {
            barriersInfo[i][B_MOVE] = mv;
        }

        while(rectanglesOverlap(mainPlayerPos, barriers[i]))
        {
            barriers[i].left += (long)(45 + extra);
            barriers[i].right += (long)(45 + extra);
        }
        if(barriersInfo[i][B_TYPE] == 3)
        {
            barriersInfo[i][B_STEP] = 0;
            ++i;
            continue;
        }

        barriersInfo[i][B_STEP] = step;
        name = "";
        left = top = right = bottom = mv = radius = step = 0;
        i++;
    }
}

// makes a custom ellipse
void createCustomEllipse(HDC hdc, RECT barrier)
{
    Ellipse(hdc, barrier.left, barrier.top, barrier.right, barrier.bottom);
}

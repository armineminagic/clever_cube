#include <string>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <direct.h>
#include <dirent.h>

#include "filemanagment.h"
#include "defaults.h"
#include "helpfunctions.h"
#include "movement.h"
#include "colision.h"

using std::ifstream;
using std::cout;
using std::endl;
using std::string;
using std::fill_n;
using std::vector;

TCHAR DEFAULT_DIR_PATH[MAX_PATH] = {0};

// File management
void CleanFiles(int onlyProgress)
{
    SetCurrentDirectory(DEFAULT_DIR_PATH);

    if(onlyProgress)
    {
        DeleteFile("savegame.txt");
    }
}

// save the progress of the gameplay
BOOL saveGame(HWND hwnd, int Level, int Deaths)
{
    SetCurrentDirectory(DEFAULT_DIR_PATH);

    string pszText;
    int TextLength = 1;
    char tmp[100];

    sprintf_s(tmp, "%d|%d", Level, Deaths);

    pszText.append(tmp);

    char szFileName[100] = "savegame.txt";

    HANDLE hFile;
    hFile = CreateFile(szFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                       FILE_ATTRIBUTE_NORMAL, NULL);

    if(hFile != INVALID_HANDLE_VALUE)
    {

        TextLength = pszText.size();

        if(TextLength > 0)
        {
            DWORD dwWritten;
            if(!WriteFile(hFile, pszText.c_str(), TextLength, &dwWritten, NULL))
                MessageBox(hwnd,TEXT(MSG_ERROR),TEXT(MSG_ERROR),MB_OK);

        }

        CloseHandle(hFile);
        MessageBox(hwnd,TEXT(MSG_PROGRESS_SAVED),TEXT(MSG_SUCCESS),MB_OK);
        return TRUE;
    }
    else
    {
        MessageBox(hwnd,TEXT(MSG_ERROR_WRITE),TEXT(MSG_ERROR),MB_OK);
        return FALSE;
    }

    return FALSE;

}

// Get saved game
void getSavedGame(int &Level, int &Deaths)
{
    SetCurrentDirectory(DEFAULT_DIR_PATH);

    ifstream hFile("savegame.txt");

    if(!hFile)
    {
        cout << "savegame.txt does not exists" << endl;
        return;
    }

    vector <string> txt;
    string line;

    while(getline(hFile, line))
    {
        cout << line << endl;
        if(line.find_first_of("|") != string::npos)
        {
            getInfo(line, Level, Deaths);
            break;
        }

    }
}

BOOL fileExists(LPCTSTR szPath)
{
    DWORD dwAttrib = GetFileAttributes(szPath);

    return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
            !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

void getInfo(const string& line, string &name, double& left, double& top, double& right, double& bottom, double& mv, double& radius, double& step)
{
    auto currentIt = line.begin();
    string tmp;
    if (currentIt != line.end())
    {
        // Get name
        name = "";
        while(*currentIt != '(' && currentIt != line.end())
        {
            name += *currentIt;
            currentIt++;
        }

        currentIt++;
        // Get left object coordinate
        while(*currentIt != ',' && currentIt != line.end())
        {
            tmp += *currentIt;
            currentIt++;
        }
        left = std::stod(tmp);
        tmp = "";

        // Get top object coordinate
        currentIt++;
        while(*currentIt != ',' && currentIt != line.end())
        {
            tmp += *currentIt;
            currentIt++;
        }
        top = std::stod(tmp);
        tmp = "";

        // Get right object coordinate
        currentIt++;
        while(*currentIt != ',' && currentIt != line.end())
        {
            tmp += *currentIt;
            currentIt++;
        }
        right = std::stod(tmp);
        tmp = "";

        // Get bottom object coordinate
        currentIt++;
        while(*currentIt != ',' && currentIt != line.end())
        {
            tmp += *currentIt;
            currentIt++;
        }
        bottom = std::stod(tmp);
        tmp = "";

        // Check if it is wall, it's should be stationary with right, left, top, bottom coordinates without step
        if (name == "Wall")
        {
            currentIt++;
            while(*currentIt != ')' && currentIt != line.end())
            {
                tmp += *currentIt;
                currentIt++;
            }
            mv = std::stod(tmp);
            step = 0;
            radius = 0;
            tmp = "";
            return;
        }

        // Check if radius exists
        currentIt++;
        if (line.find('|') != std::string::npos)
        {
            // Get move type (randomized, stationary or one direction)
            while(*currentIt != '|')
            {
                tmp += *currentIt;
                currentIt++;
            }
            mv = std::stod(tmp);
            tmp = "";

            // Get radius
            currentIt++;
            while(*currentIt != ',')
            {
                tmp += *currentIt;
                currentIt++;
            }
            radius = std::stod(tmp);
            tmp = "";
        }
        else
        {
            radius = 0;
            // Get move type of object (randomized, stationary or one direction)
            while(*currentIt != ',' && currentIt != line.end())
            {
                tmp += *currentIt;
                currentIt++;
            }
            mv = std::stod(tmp);
            tmp = "";
        }

        // Get step of moving objects
        currentIt++;
        while(*currentIt != ')' && currentIt != line.end())
        {
            tmp += *currentIt;
            currentIt++;
        }
        step = std::stod(tmp);
        tmp = "";

    }
    return;
}

void getInfo(const string& line, int& level, int& nod)
{
    auto it  = line.begin();
    if (line.find('|') != std::string::npos)
    {
        string tmp = "";
        while(it != line.end())
        {
            if (*it != '|')
            {
                tmp += *it;
                ++it;
            }
            else if (*it == '|')
            {
                level = std::stoi(tmp);
                tmp = "";
                it++;
            }
            else if (it + 1 == line.end())
            {
                nod = std::stoi(tmp);
                break;
            }
        }
    }
}

#include <windows.h>
#include <algorithm>
#include <cmath>

#include "defaults.h"
#include "colision.h"

using namespace std;

// moves up and down the object
int moveUpAndDown(RECT &r, RECT limit, int step, int id, RECT barriers[], double barriersInfo[][100], int reset)
{
    static double info[100];

    if(reset)
    {
        fill_n(info, 100, 0);
        return 0;
    }
    // 0 means it's going down
    if(info[id] == 0)
    {
        if(r.bottom + step >= limit.bottom)
        {
            r.top += limit.bottom - r.bottom;
            r.bottom = limit.bottom;
            info[id] = 1;
        }
        else
        {
            r.top += step;
            r.bottom += step;
        }
    }
    else
    {
        if(r.top - step <= limit.top)
        {
            r.bottom -= r.top - limit.top;
            r.top = limit.top;
            info[id] = 0;
        }
        else
        {
            r.top -= step;
            r.bottom -= step;
        }
    }

    // check if it hit wall
    for(int i = 0; i < 100; i++)
        if(barriersInfo[i][B_TYPE] == 3 && rectanglesOverlap(r, barriers[i]))
        {
            if(!info[id])
            {
                r.top = barriers[i].top - (r.bottom - r.top);
                r.bottom = barriers[i].top;
            }
            else
            {
                r.bottom = barriers[i].bottom + (r.bottom - r.top);
                r.top = barriers[i].bottom;
            }

            info[id] = !info[id];
            break;
        }

    return 0;
}

// moves up and down the object
int moveLeftAndRight(RECT &r, RECT limit, int step, int id, RECT barriers[], double barriersInfo[][100], int reset)
{
    static double info[100];
    if(reset)
    {
        fill_n(info, 100, 0);
        return 0;
    }
    // 0 means he is going right
    if(info[id] == 0)
    {
        if(r.right + step >= limit.right)
        {
            r.left += limit.right - r.right;
            r.right = limit.right;
            info[id] = 1;
        }
        else
        {
            r.left += step;
            r.right += step;
        }
    }
    else
    {
        if(r.left - step <= limit.left)
        {
            r.right -= r.left - limit.left;
            r.left = limit.left;
            info[id] = 0;
        }
        else
        {
            r.left -= step;
            r.right -= step;
        }
    }
    // check if it collides with any wall
    for(int i = 0; i < 100; i++)
        if(barriersInfo[i][B_TYPE] == 3 && rectanglesOverlap(r, barriers[i]))
        {
            if(!info[id])
            {
                r.left = barriers[i].left - (r.right - r.left);
                r.right = barriers[i].left;
            }
            else
            {
                r.right = barriers[i].right + (r.right - r.left);
                r.left = barriers[i].right;
            }

            info[id] = !info[id];
            break;
        }
    return 0;
}

// moves the object in a circle
int moveInCircle(RECT &r, RECT limit, double step, double radius, int id, RECT barriers[], double barriersInfo[][100], int reset)
{
    static double info[100][10] = {};
    // we handle the reset
    if(reset == 1)
    {
        for(int i = 0; i < 100; i++)
            fill_n(info[i], 10, 0);
        return 0;
    }

    step = step/10;

    if(!info[id][0])
    {
        info[id][0] = r.left;
        info[id][1] = r.right;
        info[id][2] = r.top;
        info[id][3] = r.bottom;
        info[id][4] = 0; // angle
        info[id][5] = 1; // check if we change direction

    }

    info[id][4] = fmod(info[id][4],360);

    RECT prev = r;

    r.left = (long)(info[id][0] + radius * cos(info[id][4]));
    r.right = (long)(info[id][1] + radius * cos(info[id][4]));
    r.top = (long)(info[id][2] + radius * sin(info[id][4]));
    r.bottom = (long)(info[id][3] + radius * sin(info[id][4]));

    if(r.left < limit.left)
    {
        r.right += limit.left - r.left;
        r.left = limit.left;
        info[id][5] = !info[id][5];
    }

    if(r.top < limit.top)
    {
        r.bottom += limit.top - r.top;
        r.top = limit.top;
        info[id][5] = !info[id][5];
    }
    if(r.right > limit.right)
    {
        r.left -= r.right - limit.right;
        r.right = limit.right;
        info[id][5] = !info[id][5];
    }
    if(r.bottom > limit.bottom)
    {
        r.top -= r.bottom - limit.bottom;
        r.bottom = limit.bottom;
        info[id][5] = !info[id][5];
    }
    int h = r.bottom - r.top, w = r.right - r.left;
    // check if it collides with any wall
    for(int i = 0; i < 100; i++)
        if(barriersInfo[i][B_TYPE] == 3 && rectanglesOverlap(r, barriers[i]))
        {
            info[id][5] = !info[id][5];
            // RL
            if( prev.left >= barriers[i].right )
            {

                r.right = barriers[i].right + w;
                r.left = barriers[i].right;
            }
            // BT
            if( prev.top >= barriers[i].bottom )
            {

                r.bottom = barriers[i].bottom + h;
                r.top = barriers[i].bottom;
            }
            // LR
            if( prev.right <= barriers[i].left )
            {

                r.left = barriers[i].left - w;
                r.right = barriers[i].left;
            }
            // TB
            if( prev.bottom <= barriers[i].top )
            {

                r.top = barriers[i].top - h;
                r.bottom = barriers[i].top;
            }
            //break;
        }

    if(info[id][5])
        info[id][4] += step;
    else
        info[id][4] -= step;
    return 0;
}

// bouces the object on the walls of the container
int moveBounceOnWalls(RECT &r, RECT limit, int step, int id, RECT barriers[], double barriersInfo[][100], int reset)
{

    static int info[100][5];
    if(!info[id][0])
    {
        info[id][0] = 1; // x
        info[id][1] = 1; // y
        //info[id][2] = 0;
    }
    if(reset)
    {
        for(int i = 100; i >= 0; i--)
            fill_n(info[i], 5, 1);
        return 0;
    }

    RECT prev = r;

    r.left += step * info[id][0];
    r.top += step * info[id][1];
    r.right += step * info[id][0];
    r.bottom += step * info[id][1];

    int h = r.bottom - r.top, w = r.right - r.left;
    // we handle the container
    // we touch the top
    if(r.top < limit.top)
    {
        r.bottom = limit.top + h;
        r.top = limit.top;
        info[id][1] = 1;
    }
    // we touch the bottom
    if(r.bottom > limit.bottom)
    {
        r.top = limit.bottom - h;
        r.bottom = limit.bottom;
        info[id][1] = -1;

    }
    // we touch the left
    if(r.left < limit.left)
    {
        r.right = limit.left + w;
        r.left = limit.left;
        info[id][0] = 1;
    }
    // we touch the right
    if(r.right > limit.right)
    {
        r.left = limit.right - w;
        r.right = limit.right;
        info[id][0] = -1;
    }

    // check if it collides with any wall
    for(int i = 0; i < 100; i++)
        if(barriersInfo[i][B_TYPE] == 3 && rectanglesOverlap(r, barriers[i]))
        {
            // RL
            if( prev.left >= barriers[i].right )
            {
                info[id][0] = 1;
                r.right = barriers[i].right + w;
                r.left = barriers[i].right;
            }
            else
                // BT
                if( prev.top >= barriers[i].bottom )
                {
                    info[id][1] = 1;
                    r.bottom = barriers[i].bottom + h;
                    r.top = barriers[i].bottom;
                }
                else
                    // LR
                    if( prev.right <= barriers[i].left )
                    {
                        info[id][0] = -1;
                        r.left = barriers[i].left - w;
                        r.right = barriers[i].left;
                    }
                    else
                        // TB
                        if( prev.bottom <= barriers[i].top )
                        {
                            info[id][1] = -1;
                            r.top = barriers[i].top - h;
                            r.bottom = barriers[i].top;
                        }
            //break;
        }

    return 1;
}

// moves the object to follow the player
void followThePlayer(RECT &r, RECT destination, RECT limit, int step)
{
    double dx = destination.right - r.right;
    double dy = destination.bottom - r.bottom;

    double angle = atan2(dy,dx);

    r.left += (long)(step * cos(angle));
    r.right += (long)(step * cos(angle));
    r.top += (long)(step * sin(angle));
    r.bottom += (long)(step * sin(angle));
    // if he tryes to move outsite the container we stop it
    if(r.top < limit.top)
    {
        r.bottom += limit.top - r.top;
        r.top = limit.top;
    }

    if(r.left < limit.left)
    {
        r.right += limit.left - r.left;
        r.left = limit.left;
    }
    if(r.right > limit.right)
    {
        r.left -= r.right - limit.right;
        r.right = limit.right;
    }
    if(r.bottom > limit.bottom)
    {
        r.top -= r.bottom - limit.bottom;
        r.bottom = limit.bottom;
    }
}

// handeles the movement of the player
void movePlayer(RECT &player, RECT gameBoxWnd, int move, int &ctrl)
{
    if(GetAsyncKeyState(VK_LEFT) && !outOfGameBox(player,gameBoxWnd, 0, move))
    {

        player.left -= move;
        player.right -= move;
        ctrl = 1;
    }


    if(GetAsyncKeyState(VK_RIGHT) && !outOfGameBox(player,gameBoxWnd, 1, move))
    {
        player.left += move;
        player.right += move;
        ctrl = 2;
    }

    if(GetAsyncKeyState(VK_UP) && !outOfGameBox(player,gameBoxWnd, 2, move))
    {
        player.top -= move;
        player.bottom -= move;
        ctrl = 3;
    }


    if(GetAsyncKeyState(VK_DOWN) && !outOfGameBox(player,gameBoxWnd, 3, move))
    {
        player.top += move;
        player.bottom += move;
        ctrl = 4;
    }

}

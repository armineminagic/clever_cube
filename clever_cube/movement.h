#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <windows.h>

int moveUpAndDown(RECT &r, RECT limit, int step, int id, RECT barriers[], double barriersInfo[][100], int reset = 0);
int moveLeftAndRight(RECT&, RECT, int, int, RECT*, double[][100], int reset = 0);
int moveInCircle(RECT&, RECT, double, double, int, RECT*, double[][100], int reset = 0);
int moveBounceOnWalls(RECT&, RECT, int, int, RECT*, double[][100], int reset = 0);
void followThePlayer(RECT&, RECT, RECT, int);

void movePlayer(RECT&, RECT, int, int&);

#endif

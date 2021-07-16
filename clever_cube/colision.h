
#ifndef COLLISIONS_H
#define COLLISIONS_H

bool outOfGameBox(RECT&, RECT, int, int);
int rectanglesOverlap(RECT a, RECT b);
void checkWall(RECT&, RECT, int, RECT);
bool game_over(HWND, RECT&, RECT&, RECT);

#endif
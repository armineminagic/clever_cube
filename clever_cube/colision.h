#ifndef COLLISIONS_H
#define COLLISIONS_H
#include <vector>

bool outOfGameBox(RECT&, RECT, int, int);
int rectanglesOverlap(RECT a, RECT b);
void checkWall(RECT&, RECT, int, RECT);
bool gameOver(HWND, RECT&, RECT&, RECT,int&, int&, int&, std::vector<HWND>&, WPARAM, LPARAM);

#endif

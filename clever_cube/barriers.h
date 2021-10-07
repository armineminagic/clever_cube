#ifndef BARRIERS_H_INCLUDED
#define BARRIERS_H_INCLUDED

void createDoor(int, RECT&, RECT, int*);
void createBarriers(RECT*, double[][100], int, int*, RECT);
void createCustomEllipse(HDC, RECT);

#endif // BARRIERS_H_INCLUDED

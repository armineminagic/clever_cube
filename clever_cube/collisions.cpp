#include <windows.h>
#include "defaults.h"

// checks if the next move is outside of the container
bool outOfGameBox(RECT &r1, RECT gameBoxWnd, int direction, int move) {
	// left
	if(direction == 0 && (r1.left - move < gameBoxWnd.left)){
		r1.right = gameBoxWnd.left + (r1.right - r1.left);
		r1.left = gameBoxWnd.left;
		return true;
	}
	// right
	if(direction == 1 && (r1.right + move > gameBoxWnd.right)){
		r1.left = gameBoxWnd.right - (r1.right - r1.left);
		r1.right = gameBoxWnd.right;
		return true;
	}

	// up
	if(direction == 2 && (r1.top - move < gameBoxWnd.top)) {
		r1.bottom = gameBoxWnd.top + (r1.bottom - r1.top);
		r1.top = gameBoxWnd.top;
		return true;

	}

	// down
	if(direction == 3 && (r1.bottom + move > gameBoxWnd.bottom)) {
		r1.top = gameBoxWnd.bottom - (r1.bottom - r1.top);
		r1.bottom = gameBoxWnd.bottom;
		return true;
	}
	return false;

}


// Returns 1 if the rectangles overlap, 0 otherwise
int rectanglesOverlap(RECT a, RECT b) {

	if (a.left >= b.right || a.right <= b.left ||
    a.top >= b.bottom || a.bottom <= b.top)
		return 0;
    return 1;
}


// makes sure that the player doesn't pass the wall
void checkWall(RECT &r, RECT barrier, int d, RECT p) {

	if(rectanglesOverlap(r, barrier)){

		if((d == 3 || d == 2) && (p.right <= barrier.left))
			d = 1;

		if((d == 3 || d == 2) && (p.left >= barrier.right))
			d = 0;

		// right to left
		if(d == 0 && r.left < barrier.right) {
			r.right += barrier.right - r.left;
			r.left = barrier.right;
		}
		// left to right
		if(d == 1 && r.right > barrier.left) {
			r.left -= r.right - barrier.left;
			r.right = barrier.left;
		}
		// down to up
		if(d == 2 && r.top < barrier.bottom) {
			r.bottom += barrier.bottom - r.top;
			r.top = barrier.bottom;
		}
		// up to down
		if(d == 3 && r.bottom > barrier.top) {
			r.top -= r.bottom - barrier.top;
			r.bottom = barrier.top;
		}
	}

}


// game over
bool game_over(HWND hwnd, RECT &r1, RECT &barrier, RECT DefPos) {

	if(r1.right > barrier.left && r1.left < barrier.right && rectanglesOverlap(r1, barrier)){
		PlaySound("crash.wav", NULL, SND_FILENAME | SND_ASYNC);
		KillTimer(hwnd,1);

		MessageBox(hwnd, TEXT(LOSE_TXT), TEXT(LOSE_TXT_TITLE), MB_OK);

		SetTimer(hwnd,1,UPDATE_RATE, NULL);

		r1 = DefPos;

		while(rectanglesOverlap(DefPos, barrier)) {
			barrier.left -= 4500;
			barrier.right -= 4500;

		}

		InvalidateRect(hwnd,NULL,false);
		UpdateWindow(hwnd);
		return true;
	}
	return false;
}

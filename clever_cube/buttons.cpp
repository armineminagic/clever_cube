#include <windows.h>

// destroy the buttons on the current view
void DestroyButtons(HWND BTN[]){
	auto szBTN = sizeof(BTN)/sizeof(BTN[0]);
	for( int i = 0; i < szBTN; i++ ) {
		DestroyWindow(BTN[i]);
	}

}

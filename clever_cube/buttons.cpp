#include <windows.h>
#include <vector>

using std::vector;

// clean buttons on current window to prepare for next one
void DestroyButtons(vector <HWND>& buttons)
{
    for (auto it = buttons.begin(); it < buttons.end(); ++it)
    {
        DestroyWindow(*it);
    }
}

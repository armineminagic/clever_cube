#include <string>
#include <time.h>
#include <vector>
#include <windows.h>

using std::vector;
using std::string;

// generates a random numer between the limits
int GetRandomNumber(int nLow, int nHigh)
{
	 srand( (unsigned)time( NULL ) );
    return (rand() % (nHigh - nLow + 1)) + nLow;
}

// splits the string an array based on the second argument as a delimitator
vector <string> splitString(const string& str, const char& ch) {
    string next;
    vector<string> result;

    // For each character in the string
    for (string::const_iterator it = str.begin(); it != str.end(); it++) {
        // If we've hit the terminal character
        if (*it == ch) {
            // If we have some characters accumulated
            if (!next.empty()) {
                // Add them to the result vector
                result.push_back(next);
                next.clear();
            }
        } else {
            // Accumulate the next character into the sequence
            next += *it;
        }
    }
    if (!next.empty())
         result.push_back(next);
    return result;
}


// makes a custom ellipse
void createCustomEllipse(HDC hdc, RECT barrier) {
	Ellipse(hdc, barrier.left, barrier.top, barrier.right, barrier.bottom);
}

// make the gate where
void makeGate(int offset, RECT &Gate, RECT gameBoxWnd, int GateInfo[]) {

		Gate.top = GetRandomNumber(offset,gameBoxWnd.bottom);
		Gate.left = gameBoxWnd.right - GateInfo[0];
		Gate.right = gameBoxWnd.right;
		if(Gate.top + GateInfo[1] > gameBoxWnd.bottom) {
			Gate.top -= Gate.top + GateInfo[1] - gameBoxWnd.bottom;
			Gate.bottom = gameBoxWnd.bottom;
		} else {
			Gate.bottom = Gate.top + GateInfo[1];
		}

}

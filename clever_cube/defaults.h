#ifndef DEFAULTS_H
#define DEFAULTS_H

#define WINDOW_WIDTH 840
#define WINDOW_HEIGHT 500

#define UPDATE_RATE 50

#define MAX_PATH_LENGTH 256

#define GAME_TITLE         "Clever cube"
#define EXPLAIN            "\n Drive trough the door"

#define LEVEL_TXT          "\n\n\n    Username: %s Level: %d"
#define DEATHS_TXT         "\n\n\n Deaths: %d       "

#define WIN_TXT            "Level compleeted!\n\n Press ok to get to the next level..."
#define WIN_TXT_TITLE      "Congratulation"
#define LOSE_TXT           "Try again!"
#define LOSE_TXT_TITLE     "You lost"
#define FINISH_GAME        "CONGRATULATIONS!\n\n%s you are the winner!\n\n"

#define ARE_YOU_SURE_MSG   "Are you sure?"
#define CONFIRM_MSG        "Confirm"

#define MSG_SUCCESS        "Success"
#define MSG_PROGRESS_SAVED "The progress was saved!"
#define MSG_ERROR          "ERROR"
#define MSG_OVERWRITE      "File exists, you want to owerwrite?"
#define MSG_ERROR_WRITE    "ERROR WRITING FILE!"
#define DIR_NAME           "Levels"

// barriers informations
#define B_MOVE 0    // Moving type (0 stacionar, 3 randomized, 2 one directiron )
#define B_TYPE 1    // 1 Rectangle 2 Ellipse 3 Wall
#define B_STEP 2    // Moving steps
#define B_RADIUS 3  // Moving radius

// buttons
#define BTN_SAVE_LABEL_LBL    "Save"
#define BTN_RESTART_LABEL_LBL "Reset"
#define BTN_SAVE_PROGRESS_MSG "Save Game"
#define BTN_END_LBL           "End Game"


#define LOGIN_BTN    100
#define PLAYGAME_BTN 101
#define BACK_BTN     102
#define END_BTN      103
#define RESTART_BTN  104
#define SAVEGAME_BTN 105

// Text Boxes
#define USERNAME_ENTRY 106
#define PASSWORD_ENTRY 107


#define ID_BTN_CREATE 111
#define ID_BTN_WALL   112
#define ID_BTN_SAVE   113

#endif

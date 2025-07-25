//Modifications:
//05/11/21  modified singleplayer and multiplayer declarations. added
//          added resetInputs declaration - DGR
//04/26/21  modified multiplayer declaration - DGR
//04/25/21  initial version

#ifndef GAMESET_H
#define GAMESET_H

// header file for GameSet.c, the file that includes the menu tree functions

#include "Player.h"
#include "CheckWin.h"
#include "BoardStructure.h"

//prints menu and handles menu user choice
void printmenu();

//begins the setup necessary to begin network game
void networkSetup(PLAYER p1, PLAYER p2, int liveBoard[8][8], LIST *list);

//begins a multiplayer game
void localMultiplayer(PLAYER p1, PLAYER p2, int liveBoard[8][8], LIST *list);

//begins a singleplayer game
void singleplayer(PLAYER p1, PLAYER p2, int liveBoard[8][8], LIST *list);

//resets character array used for input
void resetInputs(char input[7]);

//initializes the board for beginning of game
void boardReset(int liveBoard[8][8]);

#endif /* STRUCTURES_H */

/* EOF */

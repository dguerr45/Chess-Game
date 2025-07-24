/**********************************************************/
/* Title: AI.h                                            */
/* Author: Daniel Guerra-Rojas                            */
/*                                                        */
/* Modifications:                                         */
/* 05/08/21  DGR  Initial version                         */
/**********************************************************/
#ifndef AI_H
#define AI_H

#include "Player.h"

//controls how AI moves in game
void randomAI(PLAYER p1, char piece[7], char position[7], int liveBoard[8][8]);

//finds all pieces on the board for the AI's corresponding color
//void findPieces(PLAYER p1, int pieceRow[16], int pieceCol[16], int liveBoard[8][8]);

#endif /* AI_H */

/* EOF */
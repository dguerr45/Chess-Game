/**********************************************************/
/* Title: BoardEdit.h                                     */
/* Author: Daniel Guerra-Rojas                            */
/*                                                        */
/**********************************************************/

#ifndef BOARD_EDIT_H
#define BOARD_EDIT_H

//takes in live gameplay board as input
//prints out board to console
void printBoard(int liveBoard[8][8]);

//function to handle user input to move pieces
//returns 1 if pawn moves or capture happens, 0 otherwise
int moves(int liveBoard[8][8], char input1[7], char input2[7], int *capture, int *promotion);

//prompts user what piece they would like to promote pawn too
int promotePawn();

#endif /* BOARD_EDIT_H */

/* EOF */
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

// Function to handle user input to move pieces
//
// Returns 1 if pawn moves or capture happens, 0 otherwise
int moves(int liveBoard[8][8], char input1[7], char input2[7], int *capture, int *promotion);

// Prompts user what piece they would like to promote pawn too
//
// Returns (number - 1) piece representation of selected piece
int promotePawn();

#endif /* BOARD_EDIT_H */

/* EOF */
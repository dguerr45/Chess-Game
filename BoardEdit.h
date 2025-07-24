//Modifications:
//05/11/21  modified moves declaration - DGR
//04/26/21  modified moves declaration - DGR
//04/25/21  initial version

// header file for BoardEdit.c, the file that edits the board and prints it
#ifndef BOARD_EDIT_H
#define BOARD_EDIT_H

//takes in live gameplay board as input
//prints out board to console
void printBoard(int liveBoard[8][8]);

//function to handle user input to move pieces
//returns 1 if piece is captured from move, 0 otherwise
int moves (int liveBoard[8][8], char input1[5], char input2[5], int *capture, int *promotion);

//prompts user what piece they would liek to promote pawn too
int promotePawn();

#endif /* BOARD_EDIT_H */

/* EOF */
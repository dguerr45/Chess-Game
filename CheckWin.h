/**********************************************************/
/* Title: CheckWin.h                                     */
/* Author: Daniel Guerra-Rojas                            */
/*                                                        */
/**********************************************************/

#ifndef CHECKWIN_H
#define CHECKWIN_H

#include <stdio.h>

//checks if either king is in check. Returns 1 if white king is in check
//returns 2 if black king is in check. Otherwise, returns 0
int check(int board[8][8]);

//function for lookahead
int lookahead(int board[8][8], char input1[7], char input2[7]);

//function to determine checkmate
int checkmate(int b[8][8],int colorincheck);

#endif /* CHECKWIN_H */

/* EOF */
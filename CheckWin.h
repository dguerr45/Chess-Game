// Temp file for check function (not implemented at all)
//author: Felipe, Daniel
#ifndef CHECKWIN_H
#define CHECKWIN_H

#include <stdio.h>

//function for check()
int check(int board[8][8]);

//function for lookahead
int lookahead(int board[8][8], char input1[7], char input2[7]);

//function to determine checkmate
int checkmate(int b[8][8],int colorincheck);

#endif /* CHECKWIN_H */

/* EOF */

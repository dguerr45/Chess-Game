/**********************************************************/
/* Title: TextLog.h										  */
/* Author: Daniel Guerra-Rojas							  */
/*														  */
/**********************************************************/

#ifndef TEXTLOG_H
#define TEXTLOG_H

#include "BoardStructure.h"

//outputs a text log of the game to directory that's it's in
//name of file is decided by user
void makeTextLog(LIST *list);

//returns letter of piece in Chess Notation
void letterRepresentation(int board[8][8], char piece[3], int captured, char printedText[3]);

#endif /* TEXTLOG_H */

/* EOF */
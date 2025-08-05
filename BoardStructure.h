/**********************************************************/
/* Title: BoardStructure.h                                */
/* Author: Daniel Guerra-Rojas                            */
/*                                                        */
/**********************************************************/

#ifndef STRUCTURES_H
#define STRUCTURES_H

#include "Player.h"

typedef struct List LIST;
typedef struct BoardEntry ENTRY;
typedef struct RecordedBoard BOARD;

struct List{
    int length;     // number of entries in list
    ENTRY *first;   // pointer to first entry in list
    ENTRY *last;    // pointer to last entry in list
};

struct BoardEntry{
    LIST *list;         // pointer to list header
    ENTRY *next;        // pointer to next entry in list
    ENTRY *previous;    // pointer to previous entry in list
    BOARD *board;       // pointer to board
};

struct RecordedBoard{
    int recordedBoard[8][8];    // int values represent chess pieces
    int currentMove;            // player number of who's turn it is
    char piece[3];              // original position of piece user chose
    char pieceMovement[3];      // position of where piece is moved to
    int captured;               // if opponents piece captured. default 0
    int check;                  // if move put opponent king in check. default 0
    int castle;                 // if player castled in game. default 0
    int promotion;              // if pawn was promoted in game. default 0
    int resign;                 // if player resigned game. default 0
    int draw;                   // if game ended in draw. default 0
};

//creates the list header
LIST *CreateList();

//creates an entry to be used in list
ENTRY *CreateEntry();

//creates a board to be used in list and links it to inputted entry
//current game board passed in and recorded along with who's turn it is
BOARD *CreateBoard(ENTRY *entry, int liveBoard[8][8], int playerNum);

//adds entry to end of list and links list and entry
void Append(LIST *list, ENTRY *entry);

//deletes a list (and any entries if they exist)
void DeleteList(LIST *list);

//deletes first entry in the list (and board associated with it)
void DeleteFirstEntry(LIST *list);

//deletes last entry in the list (and board associated with it)
void DeleteLastEntry(LIST *list);

#endif /* STRUCTURES_H */

/* EOF */
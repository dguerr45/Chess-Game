/**********************************************************/
/* Title: BoardStructure.c                                */
/* Author: Daniel Guerra-Rojas                            */
/*                                                        */
/**********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "BoardStructure.h"

//creates the list header
LIST *CreateList(){
    LIST *list;

    //allocates memory
    list = calloc(1, sizeof(LIST));

    //checks if memory was allocated correctly
    if(!list){
        perror("Out of memory! Aborting...");
        exit(10);
    }
    list->length = 0;
    list->first = NULL;
    list->last = NULL;
    return list;
}

//creates an entry to be used in list
ENTRY *CreateEntry(){
    //allocates memory
    ENTRY *entry = calloc(1, sizeof(ENTRY));

    //checks if memory was allocated correctly
    if(!entry){
        perror("Out of memory! Aborting...");
        exit(10);
    }
    entry->list = NULL;
    entry->next = NULL;
    entry->previous = NULL;
    entry->board = NULL;
    return entry;
}

//creates a board to be used in list and links it to inputted entry
//current game board passed in and recorded along with who's turn it is
BOARD *CreateBoard(ENTRY *entry, int liveBoard[8][8], int playerNum){
    BOARD *board;

    //allocates memory and initializes all values to 0
    board = calloc(1, sizeof(BOARD));

    //checks if memory was allocated correctly
    if(!board){
        perror("Out of memory! Aborting...");
        exit(10);
    }

    //checks if entry pointer is valid
    if(!entry){
        perror("Invalid pointer! Aborting...");
        exit(10);
    }
    entry->board = board;

    //copies live board over to recorded board
    for(int x = 0; x < 8; x++){
        for(int y = 0; y < 8; y++){
            board->recordedBoard[x][y] = liveBoard[x][y];
        }
    }
    board->currentMove = playerNum;
    board->captured = 0;
    board->check = 0;
    board->castle = 0;
    board->promotion = 0;
    board->resign = 0;
    board->draw = 0;
    return board;
}

//adds entry to end of list and links list and entry
void Append(LIST *list, ENTRY *entry){
    //checks if pointers are valid
    if(!list){
        perror("Invalid pointer! Aborting...");
        exit(10);
    }
    if(!entry){
        perror("Invalid pointer! Aborting...");
        exit(10);
    }

    if(list->length == 0){      //if list is empty
        entry->previous = NULL;
        entry->next = NULL;
        entry->list = list;
        list->first = entry;
        list->last = entry;
    } else {                    //if list is not empty
        entry->next = NULL;
        entry->previous = list->last;
        entry->list = list;
        list->last->next = entry;
        list->last = entry;
    }
    list->length += 1;

}

//deletes a list (and any entries if they exist)
void DeleteList(LIST *list){
    //checks if list is valid
    if(!list){
        perror("Invalid pointer! Aborting...");
        exit(10);
    }

    while(list->length){
        DeleteFirstEntry(list);
    }
    free(list);
    list = NULL;
}

//deletes first entry in the list (and board associated with it)
void DeleteFirstEntry(LIST *list){
    //checks if list is valid
    if(!list){
        perror("Invalid pointer! Aborting...");
        exit(10);
    }

    ENTRY *entry = list->first;

    //checks if entry is valid
    if(!entry){
        perror("Invalid pointer! Aborting...");
        exit(10);
    }

    //checks if board exists in entry
    if(!entry->board){
        perror("Invalid pointer! Aborting...");
        exit(10);
    }

    free(entry->board);

    if(list->length != 1){      //if list has more than 1 entry

        //second entry cuts ties with first entry
        entry->next->previous = NULL;

        //second entry nows becomes new first entry in list
        list->first = entry->next;
    } else {        //if list has only one entry
        list->first = NULL;
        list->last = NULL;
    }
    entry->next = NULL;
    entry->list = NULL;

    //gets rid of original first entry
    free(entry);
    list->length -= 1;
}

//deletes last entry in the list (and board associated with it)
void DeleteLastEntry(LIST *list){
    //checks if list is valid
    if(!list){
        perror("Invalid pointer! Aborting...");
        exit(10);
    }

    ENTRY *entry = list->last;

    //checks if entry is valid
    if(!entry){
        perror("Invalid pointer! Aborting...");
        exit(10);
    }

    //checks if board exists in entry
    if(!entry->board){
        perror("Invalid pointer! Aborting...");
        exit(10);
    }

    free(entry->board);

    if(list->length != 1){      //if last has more than 1 entry

        //second to last entry cuts ties with last entry
        entry->previous->next = NULL;

        //second to last entry nows becomes new last entry
        list->last = entry->previous;
    } else {        //if list has only one entry
        list->first = NULL;
        list->last = NULL;
    }
    entry->previous = NULL;
    entry->list = NULL;

    //gets rid of original last entry
    free(entry);
    list->length -= 1;
}

/* EOF */
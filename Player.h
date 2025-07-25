/**********************************************************/
/* Title: Player.h                                        */
/* Author: Daniel Guerra-Rojas                            */
/*                                                        */
/* Modifications:                                         */
/* 04/29/21  DGR  Modified PLAYER struct                  */
/* 04/20/21  DGR  Initial version                         */
/**********************************************************/
#ifndef PLAYER_H
#define PLAYER_H

typedef struct{
    int playerNum;          //the player's number determining who goes first (1 or 2)
    char playerColor[2];    //the player's color (w or b)
}PLAYER;


#endif /* PLAYER_H */

/* EOF */
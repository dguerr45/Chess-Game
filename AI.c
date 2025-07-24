/**********************************************************/
/* Title: AI.c                                            */
/* Author: Daniel Guerra-Rojas                            */
/*                                                        */
/* Modifications:                                         */
/* 05/08/21  DGR  Initial version                         */
/**********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "PieceMovement.h"
#include "AI.h"

//controls how AI moves in game
void randomAI(PLAYER p1, char piece[7], char position[7], int liveBoard[8][8]){
    srand(time(NULL));
    int pieceRow, pieceCol, positionRow, positionCol;

    //stays in while loop until random piece and random move are both valid choices
    while(1){

        //if player 1 is white, then AI can only choose black pieces
        if(p1.playerNum == 1){

            //selects random coordinate on board for piece
            pieceRow = rand() % 8;
            pieceCol = rand() % 8;

            //if space selected is white or empty, then skips to next loop and starts again
            if(liveBoard[pieceRow][pieceCol] >= 0){
                continue;
            }

            //selects random coordinate to move selected piece to
            positionRow = rand() % 8;
            positionCol = rand() % 8;

            //coordinates have to be converted to char array for PieceMovement() function
            piece[1] = 56 - pieceRow;
            piece[0] = pieceCol + 97;
            position[1] = 56 - positionRow;
            position[0] = positionCol + 97;

            //checks if space moving to is non-black space and if movement is valid for piece
            if(liveBoard[positionRow][positionCol] >= 0 && PieceMovement(piece, position, liveBoard)){
                break;
            }
        } else {    //if player 1 is black, then AI can only choose white pieces

            //selects random coordinate on board for piece
            pieceRow = rand() % 8;
            pieceCol = rand() % 8;

            //if space selected is black or empty, then skips to next loop and starts again
            if(liveBoard[pieceRow][pieceCol] <= 0){
                continue;
            }

            //selects random coordinate to move selected piece to
            positionRow = rand() % 8;
            positionCol = rand() % 8;

            //coordinates have to be converted to char array for PieceMovement() function
            piece[1] = 56 - pieceRow;
            piece[0] = pieceCol + 97;
            position[1] = 56 - positionRow;
            position[0] = positionCol + 97;

            //checks if space moving to is non-white space and if movement is valid for piece
            if(liveBoard[positionRow][positionCol] <= 0 && PieceMovement(piece, position, liveBoard)){
                break;
            }
        }
    }
}

/* EOF */
//Modification:
//05/11/21  modified moves function - DGR
//05/10/21  modified moves function - DGR
//04/29/21  modified moves function - DGR
//04/26/21  added moves function - DGR
//04/25/21  initial version

//.c file for BoardEdit, the file that edits and prints out the board

#include <stdio.h>
#include <stdlib.h>
#include "Player.h"
#include "BoardEdit.h"
#include "BoardStructure.h"
#include "PieceMovement.h"

//takes in live gameplay board as input
//prints out board to console
void printBoard(int liveBoard[8][8]){
    int row, col, sideNum;
    sideNum = 8;

    //prints top row of board
    printf("\n   a    b    c    d    e    f    g    h\n");

    //for loop that prints out board
    for(row = 0; row < 8; row++){
		printf(" +----+----+----+----+----+----+----+----+\n");
		printf("%d|", sideNum);

        for(col = 0; col < 8; col++){
			if(liveBoard[row][col] < 0){        //if negative, prints "b" for black chess pieces
				printf(" b");
			} else if(liveBoard[row][col] > 0){
				printf(" w");                   //if positive, prints "w" for white chess piece
			} else{
				printf("  ");                  //otherwise, blank for empty space
			}

			if(abs( liveBoard[row][col] ) == 1){    //prints P for pawn
				printf("P |");
			} else if(abs( liveBoard[row][col] ) == 2){     //prints R for rook
				printf("R |");
			} else if(abs( liveBoard[row][col] ) == 3){     //prints N for knight
				printf("N |");
			} else if(abs( liveBoard[row][col] ) == 4){     //prints B for bishop
				printf("B |");
			} else if(abs( liveBoard[row][col] ) == 5){     //prints Q for queen
				printf("Q |");
			} else if(abs( liveBoard[row][col] ) == 6){     //prints K for king
				printf("K |");
			} else {                                        //otherwise, blank for empty space
				printf("  |");
			}
        }
		printf("%d\n", sideNum);
		sideNum--;
    }
	printf(" +----+----+----+----+----+----+----+----+\n");

    //prints bottom row of board
    printf("   a    b    c    d    e    f    g    h\n");
}

//function to handle user input to move pieces
//returns 1 if pawn moves or capture happens, 0 otherwise
int moves(int liveBoard[8][8], char input1[7], char input2[7], int *capture, int *promotion){
    int row1, col1, row2, col2;
    int fiftyMoveReset = 0;

    // turns char inputs into cordinates for board
    // old coordinates (first input by user
    col1 = input1[0] - 97;
    row1 = abs( input1[1] - 56 );
    
    // new coordinates (second input by user)
    col2 = input2[0] - 97;
    row2 = abs( input2[1] - 56 );

    // updates board according to move
    if(liveBoard[row2][col2] != 0) // if space is not blank,
    {
        liveBoard[row2][col2] = liveBoard[row1][col1]; // set new space piece equal to the old space piece
        fiftyMoveReset = 1;    // becomes 1 if a capture happens
        *capture = 1;    //becomes 1 if capture happens
    } 
    else if (is_space_blank(liveBoard[row2][col2]) == 1) // if space is blank, 
    {
        if (abs(liveBoard[row1][col1]) == 1) // if piece moved is black/white pawn
        {   			
            fiftyMoveReset = 1;
        }
        *capture = 0;    //becomes 0 if capture didn't happen

        if (get_en_passant() == 1) // and en passant has been performed,
        {
            if (white_or_black(liveBoard[row1][col1]) == 1) // if the piece is white,
            {
                liveBoard[row2][col2] = liveBoard[row1][col1]; // still update the space it moves two normally,
                liveBoard[row2 + 1][col2] = 0; // but set the space below it to be empty
            }
            else // if the piece is black,
            {
                liveBoard[row2][col2] = liveBoard[row1][col1]; // still update the space it moves to normally,
                liveBoard[row2 - 1][col2] = 0; // but set the space above it to be empty
            }
        } else if( abs(liveBoard[row1][col1]) == 1){    //if piece moved is black/white pawn
            if( white_or_black(liveBoard[row1][col1]) == 1){    //if white pawn
                if(row2 == 0){
                    liveBoard[row2][col2] = promotePawn() + 1;
                    *promotion = liveBoard[row2][col2];
                } else {
                    liveBoard[row2][col2] = liveBoard[row1][col1];
                    *promotion = 0;

                }
            } else {    //if black pawn
                if(row2 == 7){
                    liveBoard[row2][col2] = promotePawn() + 1;
                    *promotion = liveBoard[row2][col2];
                } else {
                    liveBoard[row2][col2] = liveBoard[row1][col1];
                    *promotion = 0;

                }
            }
        }
        else // if en passant is not performed,
            {
                liveBoard[row2][col2] = liveBoard[row1][col1]; // just update the space normally (no capture since space is blank)
        }
    }
    
    liveBoard[row1][col1] = 0; // make old space blank
    
    return fiftyMoveReset;
}

//prompts user what piece they would liek to promote pawn too
int promotePawn(){
    int newPieceValue;

    while(1){
        printf("\nWhat would you like to promote the pawn to?");
        printf("\n\n1: Rook");
        printf("\n2: Knight");
        printf("\n3: Bishop");
        printf("\n4: Queen");
        printf("\n\nEnter choice (single number only): ");
        scanf(" %d", &newPieceValue);
        while(getchar() != '\n');    //clears input buffer

        if( newPieceValue >= 1 && newPieceValue <=4){
            break;
        } else {
            printf("\n\x1b[31mInvalid Input. Please try again.\x1b[0m\n");
        }
    }
    return newPieceValue;
}

/* EOF */

/**********************************************************/
/* Title: TextLog.c										  */
/* Author: Daniel Guerra-Rojas							  */
/*														  */
/* Modifications:										  */
/* 05/11/21  DGR  Initial version						  */
/**********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TextLog.h"
#include "BoardStructure.h"

//outputs a text log of the game to directory that's it's in
//name of file is decided by user
void makeTextLog(LIST *list){
    int x = 1, counter = 1;
    char printedText[4] = {};
    char tempName[60] = {};
    FILE *fileName;

    //prompts user what they want to name the new file
    printf("\nWhat would you like to name the text log: ");
    scanf(" %s", tempName);
    while(getchar() != '\n');
    strcat(tempName, ".txt");

    //begins writing process to file under user inputted name
    fileName = fopen(tempName, "w");

    DeleteFirstEntry(list);

    fprintf(fileName, "    White\tBlack\n");
    while(list->length){

        //if last entry in list, then breaks from loop
        if(list->length == 1){
            if( (list->first->board->resign != 0) || (list->first->board->draw == 1) ){
                break;
            }
        }
        if(x % 2 == 1){    //prints the first half of the lin
            fprintf(fileName, "%d.  ", counter);

            if(list->first->board->castle != 0){    //if a castle happened in the turn
                if(list->first->board->castle == 1){    //queenside castle
                    if(list->first->board->check == 1){    //appends '+' if move put opponent in check
                        fprintf(fileName, "0-0+ \t");
                    } else {
                        fprintf(fileName, "0-0  \t");
                    }
                } else {    //kingside castle
                    if(list->first->board->check == 1){    //appends '+' if move put opponent in check
                        fprintf(fileName, "0-0-0+\t");
                    } else {
                        fprintf(fileName, "0-0-0\t");
                    }
                }
                
            } else if(list->first->board->promotion != 0){
                letterRepresentation(list->first->board->recordedBoard, list->first->board->pieceMovement, list->first->board->captured, printedText);

                if(list->first->board->check == 1){
                    fprintf(fileName, "%s%s+ \t", list->first->board->pieceMovement, printedText);
                } else {
                    fprintf(fileName, "%s%s  \t", list->first->board->pieceMovement, printedText);
                }
            } else {    //if normal move happened in the turn
                letterRepresentation(list->first->board->recordedBoard, list->first->board->pieceMovement, list->first->board->captured, printedText);

                //determines number of characters in output to adjust space between white and black moves
                switch( (int)strlen(printedText) ){
                    case 0:
                        if(list->first->board->check == 1){    //appends '+' if move put opponent in check
                            fprintf(fileName, "%s%s+  \t", printedText, list->first->board->pieceMovement);
                        } else{
                            fprintf(fileName, "%s%s   \t", printedText, list->first->board->pieceMovement);
                        }
                        break;
                    case 2:
                        if(list->first->board->check == 1){    //appends '+' if move put opponent in check
                            fprintf(fileName, "%s%s+  \t", printedText, list->first->board->pieceMovement);
                        } else{
                            fprintf(fileName, "%s%s   \t", printedText, list->first->board->pieceMovement);
                        }
                        break;
                    case 3:
                        if(list->first->board->check == 1){    //appends '+' if move put opponent in check
                            fprintf(fileName, "%s%s+ \t", printedText, list->first->board->pieceMovement);
                        } else{
                            fprintf(fileName, "%s%s  \t", printedText, list->first->board->pieceMovement);
                        }
                        break;
                    default:
                        if(list->first->board->check == 1){    //appends '+' if move put opponent in check
                            fprintf(fileName, "%s%s+\t", printedText, list->first->board->pieceMovement);
                        } else{
                            fprintf(fileName, "%s%s \t", printedText, list->first->board->pieceMovement);
                        }
                        break;
                }
            }
        }
        if(x % 2 == 0){    //prints the second half of the line
            if(list->first->board->castle != 0){    //if a castle happened in the turn
                if(list->first->board->castle == 1){    //queenside castle
                    fprintf(fileName, "0-0\t");
                } else {    //kingside castle
                    fprintf(fileName, "0-0-0\t");
                }
                if(list->first->board->check == 1){    //appends '+' if move put opponent in check
                    strcat(printedText, "+");
                }
            } else if(list->first->board->promotion != 0){
                letterRepresentation(list->first->board->recordedBoard, list->first->board->pieceMovement, list->first->board->captured, printedText);

                if(list->first->board->check == 1){
                    fprintf(fileName, "%s%s+ \t", list->first->board->pieceMovement, printedText);
                } else {
                    fprintf(fileName, "%s%s  \t", list->first->board->pieceMovement, printedText);
                }
            } else {    //if normal move happened in the turn
                letterRepresentation(list->first->board->recordedBoard, list->first->board->pieceMovement, list->first->board->captured, printedText);
                fprintf(fileName, "%s%s", printedText, list->first->board->pieceMovement);
                if(list->first->board->check == 1){    //appends '+' if move put opponent in check
                    strcat(printedText, "+");
                }
                
            }
            fprintf(fileName, "\n");
            counter++;
        }
        DeleteFirstEntry(list);
        x++;
    }
    if(list->first->board->draw == 1){    //if game ended in draw
        fprintf(fileName, "1/2 - 1/2\n");
    } else{
        if(list->first->board->resign != 0){
            if(list->first->board->resign == 1){
                fprintf(fileName, "0 - 1\n");
            } else {
                fprintf(fileName, "1 - 0\n");
            }
        } else {
            if(list->first->board->currentMove == 1){
                fprintf(fileName, "1 - 0\n");
            } else {
                fprintf(fileName, "0 - 1\n");
            }
        }
    }
    fclose(fileName);
    printf("%s has been created\n", tempName);
}

//returns letter of piece in Chess Notation
//adds a 'x' to signify if a capture had happened in the move
void letterRepresentation(int board[8][8], char piece[3], int captured, char printedText[3]){
    int row, col;

    col = piece[0] - 97;
    row = abs( piece[1] - 56 );

    //determines what piece was moved
    switch( abs(board[row][col]) ){
        case 1:
            if(captured == 1){
                printedText[0] = piece[0];
            } else {
                printedText[0] = '\0';
            }
            
            break;
        case 2:
            printedText[0] = 'R';
            break;
        case 3:
            printedText[0] = 'N';
            break;
        case 4:
            printedText[0] = 'B';
            break;
        case 5:
            printedText[0] = 'Q';
            break;
        case 6:
            printedText[0] = 'K';
            break;
    }

    //determines if a capture happened
    if(captured){
        printedText[1] = 'x';
    } else {
        printedText[1] = '\0';
    }
}

/* EOF */
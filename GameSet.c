//Modifications:
//05/11/21  modified printmenu, implemented BoardStructure into singleplayer
//          and multiplayer, and added resetInputs function - DGR
//05/10/21  modified singleplayer and multiplayer functions - DGR
//05/08/21  added singleplayer function - DGR
//05/03/21  changed EXIT input to RESIGN - DGR
//04/29/21  modified multiplayer and printmenu function - DGR
//04/26/21  modified multipayer and added resetBoard - DGR
//04/25/21  initial version

// .c file for GameSet, the file that has functions that handle the menu tree

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "GameSet.h"
#include "BoardEdit.h"
#include "Player.h"
#include "PieceMovement.h"
#include "CheckWin.h"
#include "AI.h"
#include "BoardStructure.h"
#include "TextLog.h"

//prints menu and handles menu user choice
void printmenu()
{
    //initializes variables
    int done = 0, i;
    PLAYER p1;
    PLAYER p2;
    int liveBoard[8][8] = {};
    char input[3] = {};
    LIST *list;

    do
    {
        printf("\n\x1b[92mWelcome to MAD for Chess!\x1b[0m\n");
        printf("-------------------------\n");
        printf("1: Network Multiplayer -- Human v. Human\n");
        printf("2: Local Multiplayer -- Human v. Human\n");
        printf("3: Single Player -- Human v. AI\n");
        printf("4: Exit\n");
        printf("\nPlease choose one of the following: ");
        
        //prompts user for menu input
        fgets(input, sizeof(input), stdin);

        //clears input buffer if input larger than 1
        if(input[1] != '\n' && input[1] != 0){
            while(getchar() != '\n');
        }
        switch(input[0] + 8 - 56)
        {
            case 1:
                list = CreateList();    //creates list pointer and assigned to location of allocated memory
                networkSetup(p1, p2, liveBoard, list);
                //makeTextLog(list);
                DeleteList(list);
                break;
            case 2:
                list = CreateList();    //creates list pointer and assigned to location of allocated memory
                localMultiplayer(p1, p2, liveBoard, list);
                makeTextLog(list);
                DeleteList(list);
                break;
            case 3:
                list = CreateList();    //creates list pointer and assigned to location of allocated memory
                singleplayer(p1, p2, liveBoard, list);
                makeTextLog(list);
                DeleteList(list);
                break;
            case 4:
                printf("\n\x1b[92mThank you for playing! Goodbye :)\x1b[0m\n\n");
                done = 1;		
                break;
            default:
                printf("\n\x1b[31mIncorrect input. Please try again.\x1b[0m\n");
                break;
        }
        //resets input variable to all NULL characters
        for(i = 0; i < 3; i++){
            input[i] = 0;
        }
    } while (done != 1);
}

//begins a multiplayer game
void localMultiplayer(PLAYER p1, PLAYER p2, int liveBoard[8][8], LIST *list)
{
    int castle = 0;
    char input1[7] = {};
    char input2[7] = {};
    int fiftyMoveCounter = 0;
    int cancel = 0;
    int resign = 0;
    int capture = 0;
    int promotion = 0;
    int direction = 0;
    char king[3] = {};
    BOARD *board;
    ENTRY *entry;
    boardReset(liveBoard);

    //creates ENTRY and BOARD, links both to list, and records starting board
    entry = CreateEntry();
    board = CreateBoard(entry, liveBoard, 0);    //0 because game not in play
    Append(list, entry);

    printf("\nMultiplayer Game Started!\n");

    //prompts user if they want to be player 1 or not (white or not)
    while(1){
        printf("\nPlayer 1, would you like to be White or Black?\n");
        printf("(input: w or b): ");

        //prompts user for input
        fgets(p1.playerColor, sizeof(p1.playerColor), stdin);

        //clears input buffer if input larger than 1
        if(p1.playerColor[1] != '\n' && p1.playerColor[1] != 0){
            while(getchar() != '\n');
        }

        //makes input lowercase
        p1.playerColor[0] = tolower(p1.playerColor[0]);

        if(p1.playerColor[0] == 'w' && p1.playerColor[1] == '\n'){
            p1.playerNum = 1;
            p2.playerColor[0] = 'b';
            p2.playerNum = 2;
            break;
        } else if(p1.playerColor[0] == 'b' && p1.playerColor[1] == '\n'){
            p1.playerNum = 2;
            p2.playerColor[0] = 'w';
            p2.playerNum = 1;
            break;
        } else {
            printf("\n\x1b[31mIncorrect Input. Please try again.\x1b[0m\n");
        }
    }
    printBoard(liveBoard);
    printf("\nInput follows standard 'a1' or 'c4'\nType '\x1b[92mRESIGN\x1b[0m' when you wish to stop the game\n");
    printf("\nType '\x1b[92mCASTLE\x1b[0m' during your turn if you would like to castle (NOTE: to castle, your king and the rook you are using cannot have moved from their start positions).");

    //switches between players until game ends
    while(1){

        //if cancel is 1, then player 2 wanted to pick a new piece
        if( !cancel ){

            //prompts user to select a piece
            //do while loop makes sure player chooses valid piece and if input is valid
            while(1){
                if(p1.playerNum == 1){
                    printf("\nPlayer 1 (White), select a piece you want to move: ");
                } else {
                    printf("\nPlayer 2 (White), select a piece you want to move: ");
                }
                //prompts users for input to select piece
                fgets(input1, sizeof(input1), stdin);
                
                //if input is larger than 6, then input buffer is cleared
                if(input1[5] != '\n' && input1[5] != 0){
                    while(getchar() != '\n');
                }
                if(strcmp(input1, "RESIGN") == 0){    //checks for "RESIGN" input
                    resign = p1.playerNum;
                    break;
                } else if(strcmp(input1, "CASTLE") == 0){    // checks for "CASTLE" input
                    printf("\nWhich direction do you want to castle? (NUMBER INPUTS ONLY -- left: 1, right: 2): ");
                    scanf(" %d", &direction);		

                    printf("\nWhat is the current location of your king? (input follows standard -- ex. 'e1'): ");
                    scanf(" %2s", king);
                    while(getchar() != '\n');
                    king[0] = tolower(king[0]);    //makes input lowercase
                    
                    if (castling(king, liveBoard, direction) == 2)
                    {
                        printf("\nYou cannot castle. There are pieces in the way.\n");
                    }
                    else if (castling(king, liveBoard, direction) == 0)
                    {
                        printf("\nIncorrect input.\n");
                    }
                    else
                    {
                        castling_move(king, liveBoard, direction);
                    }
                    king[0] = king[1] = 0;
                    castle = 1;
                    break;
                } 
                input1[0] = tolower(input1[0]);    //makes input lowercase

                if(input1[1] >= 49 && input1[1] <= 56 && input1[0] >= 97 && input1[0] <= 104 && input1[2] == '\n'){   //checks if input is valid
                    if(liveBoard[abs( input1[1] - 56 )][ (input1[0] - 97) ] > 0){   //checks if piece picked is white
                        break;
                    } else {
                        printf("\n\x1b[31mIncorrect Input. Please try again.\x1b[0m\n");
                        printBoard(liveBoard);
                        input1[0] = input1[1] = input1[5] = 0;
                    }
                } else {
                    printf("\n\x1b[31mIncorrect Input. Please try again.\x1b[0m\n");
                    printBoard(liveBoard);
                    input1[0] = input1[1] = input1[5] = 0;
                }
            }

        //if user typed "RESIGN", then game ends
        if(resign){
            break;
        }

        //prompts user where he would like to move the piece
        //do while loop makes sure input is valid
        while(1){
            if (castle == 1) // checks if castle was performed
	        {
		        break;
	        }
            if(p1.playerNum == 1){
                printf("\nPlayer 1 (White), select a space to move selected piece to: ");
            } else {
                printf("\nPlayer 2 (White), select a space to move selected piece to: ");
            }
            //prompts users for input to select piece
            fgets(input2, sizeof(input2), stdin);
            
            //if input is larger than 6, then input buffer is cleared
            if(input2[5] != '\n' && input2[5] != 0){
                while(getchar() != '\n');
            }
            if(strcmp(input2, "RESIGN") == 0){	//checks for "RESIGN" input
                resign = p1.playerNum;
                break;
            } else if(strcmp(input2, "CANCEL") == 0){    //if "CANCEL", then player 1 wants to pick a new piece
                cancel = 1;
                resetInputs(input1, input2);
                break;
            }
                input2[0] = tolower(input2[0]);    //makes input lowercase
                
                if(input2[1] >= 49 && input2[1] <= 56 && input2[0] >= 97 && input2[0] <= 104 && input2[2] == '\n'){   //checks if input is valid

                    //checks if place moving to is non-white piece
                    if(liveBoard[abs( input2[1] - 56 )][ (input2[0] - 97) ] <= 0 && PieceMovement(input1, input2, liveBoard) ){
                        break;
                    } else {
                        printf("\n\x1b[31mIncorrect Input. Please try again.\x1b[0m\n");
                        printBoard(liveBoard);
                        input2[0] = input2[1] = input2[5] = 0;
                    }
                } else {
                    printf("\n\x1b[31mIncorrect Input. Please try again.\x1b[0m\n");
                    printBoard(liveBoard);
                    input2[0] = input2[1] = input2[5] = 0;
                }
            }

            //if user typed "RESIGN", then game ends
            if(resign){
                break;
            } else if(cancel){
                cancel = 0;
                continue;
            }
            if(moves(liveBoard, input1, input2, &capture, &promotion) == 0){
                fiftyMoveCounter++;
                if(fiftyMoveCounter == 50){    //if fifty moves are reached without capture or pawn movement, draw
                    break;
                }
            } else {
                fiftyMoveCounter = 0;
            }

            //records data to be used in linked list
            entry = CreateEntry();
            board = CreateBoard(entry, liveBoard, p1.playerNum);
            Append(list, entry);
            strncpy(board->piece, input1, 2);    //records what piece was moved
            strncpy(board->pieceMovement, input2, 2);    //records where piece was moved to
            board->captured = capture;    //records if a piece was captured
            board->promotion = promotion;    //records if promotion happened
            board->resign = 0;    //0 because no one has resigned

            resetInputs(input1, input2);
            printBoard(liveBoard);
            
            if(castle == 1){    //records if castle happened in game
                castle = 0;
                board->castle = direction;
                direction = 0;
            }
            if( check(liveBoard) == 2 ){
                printf("\n\x1b[96mBlack is in check\x1b[0m");
                board->check = 1;
            }
        }
        cancel = 0;    //resets "cancel" so player 1 doesn't get skipped anymore

        //prompts user to select a piece
        //do while loop makes sure player chooses valid piece and if input is valid
        while(1){
            if(p2.playerNum == 1){
                printf("\nPlayer 1 (Black), select a piece you want to move: ");
            } else {
                printf("\nPlayer 2 (Black), select a piece you want to move: ");
            }
            //prompts users for input to select piece
            fgets(input1, sizeof(input1), stdin);
            
            //if input is larger than 5, then input buffer is cleared
            if(input1[5] != '\n' && input1[5] != 0){
                while(getchar() != '\n');
            }
            if(strcmp(input1, "RESIGN") == 0){	//checks for "RESIGN" input
                resign = p2.playerNum;
                break;
            } else if(strcmp(input1, "CASTLE") == 0){    // checks for "CASTLE" input
                printf("\nWhich direction do you want to castle? (NUMBER INPUTS ONLY -- left: 1, right: 2): ");
		        scanf(" %d", &direction);		

		        printf("\nWhat is the current location of your king? (input follows standard -- ex. 'e1'): ");
		        scanf(" %2s", king);
                while(getchar() != '\n');
                king[0] = tolower(king[0]);    //makes input lowercase
                
                printf("castling: %d", castling(king, liveBoard, direction));

                if (castling(king, liveBoard, direction) == 2)
                {
                    printf("\nYou cannot castle. There are pieces in the way.\n");
                }
                else if (castling(king, liveBoard, direction) == 0)
                {
                    printf("\nIncorrect input.\n");
                }
                else
                {
                    castling_move(king, liveBoard, direction);
                }
                king[0] = king[1] = 0;
                castle = 1;
                break;
            }
            input1[0] = tolower(input1[0]);    //makes input lowercase
            
            if(input1[1] >= 49 && input1[1] <= 56 && input1[0] >= 97 && input1[0] <= 104 && input1[2] == '\n'){   //checks if input is valid
                if(liveBoard[abs( input1[1] - 56 )][ (input1[0] - 97) ] < 0){   //checks if piece picked is black
                    break;
                } else {
                    printf("\n\x1b[31mIncorrect Input. Please try again.\x1b[0m\n");
                    printBoard(liveBoard);
                    input1[0] = input1[1] = input1[5] = 0;
                }
            } else {
                printf("\n\x1b[31mIncorrect Input. Please try again.\x1b[0m\n");
                printBoard(liveBoard);
                input1[0] = input1[1] = input1[5] = 0;
            }
        }

        //if user typed "RESIGN", then game ends
        if(resign){
            break;
        }

        //prompts user where he would like to move the piece
        //do while loop makes sure input is valid
        while(1){
            if (castle == 1) // checks if castle was performed
	        {
		        break;
	        }
            if(p2.playerNum == 1){
                printf("\nPlayer 1 (Black), select a space to move selected piece to: ");
            } else {
                printf("\nPlayer 2 (Black), select a space to move selected piece to: ");
            }
            //prompts users for input to select piece
            fgets(input2, sizeof(input2), stdin);
            
            //if input is larger than 5, then input buffer is cleared
            if(input2[5] != '\n' && input2[5] != 0){
                while(getchar() != '\n');
            }
            if(strcmp(input2, "RESIGN") == 0){	//checks for "RESIGN" input
                resign = p2.playerNum;
                break;
            } else if(strcmp(input2, "CANCEL") == 0){    //if "CANCEL", then player 2 wants to pick a new piece
                cancel = 1;
                resetInputs(input1, input2);
                break;
            }
            input2[0] = tolower(input2[0]);    //makes input lowercase
            
            if(input2[1] >= 49 && input2[1] <= 56 && input2[0] >= 97 && input2[0] <= 104 && input2[2] == '\n'){   //checks if input is valid

                //checks if place moving to is non-black piece
                if(liveBoard[abs( input2[1] - 56 )][ (input2[0] - 97) ] >= 0 && PieceMovement(input1, input2, liveBoard)){
                    break;
                } else {
                    printf("\n\x1b[31mIncorrect Input. Please try again.\x1b[0m\n");
                    printBoard(liveBoard);
                    input2[0] = input2[1] = input2[5] = 0;
                }
            } else {
                printf("\n\x1b[31mIncorrect Input. Please try again.\x1b[0m\n");
                printBoard(liveBoard);
                input2[0] = input2[1] = input2[5] = 0;
            }
        }

        //if user typed "RESIGN", then game ends
        if(resign){
            break;
        } else if(cancel){
            continue;
        }
        if(moves(liveBoard, input1, input2, &capture, &promotion) == 0){
            fiftyMoveCounter++;
            if(fiftyMoveCounter == 50){    //if fifty moves are reached without capture or pawn movement, game is draw
                break;
            }
        } else {
            fiftyMoveCounter = 0;
        }

        //records data to be used in linked list
        entry = CreateEntry();
        board = CreateBoard(entry, liveBoard, p2.playerNum);
        Append(list, entry);
        strncpy(board->piece, input1, 2);    //records what piece was moved
        strncpy(board->pieceMovement, input2, 2);    //records where piece was moved to
        board->captured = capture;    //records if piece was captured
        board->promotion = promotion;    //records if promotion happened
        board->resign = 0;    //0 because no one has resigned

        resetInputs(input1, input2);
        printBoard(liveBoard);
	
        if(castle == 1){    //records if castle happened in game
            castle = 0;
            board->castle = direction;
            direction = 0;
        }
	    if( check(liveBoard) == 1 ){
            printf("\n\x1b[96mWhite is in check\x1b[0m");
            board->check = 1;
        }
    }
    if(resign != 0){    //if one of the players resigned
        entry = CreateEntry();
        board = CreateBoard(entry, liveBoard, 0);    //0 because no player moved
        Append(list, entry);
        board->resign = resign;    //records which player resigned
    } else if(fiftyMoveCounter == 50){    //if the game ended in draw
        entry = CreateEntry();
        board = CreateBoard(entry, liveBoard, 0);    //0 because no player moved
        Append(list, entry);
        board->draw = 1;    //records if game ended in draw
    }
}

//begins a singleplayer game
void singleplayer(PLAYER p1, PLAYER p2, int liveBoard[8][8], LIST *list){
    int resign = 0;
    int fiftyMoveCounter = 0;
    int capture = 0;
    int cancel = 0;
    char input1[7] = {};
    char input2[7] = {};
    int direction = 0;
    int castle = 0;
    int promotion = 0;
	char king[3] = {};
    ENTRY *entry;
    BOARD *board;

    boardReset(liveBoard);

    //creates ENTRY and BOARD, links both to list, and records starting board
    entry = CreateEntry();
    board = CreateBoard(entry, liveBoard, 0);    //0 because game not in play
    Append(list, entry);

    printf("\nSingleplayer Game Started!\n");

    //prompts user if they want to move first or not (white or not)
    while(1){
        printf("\nPlayer 1, would you like to be White or Black?\n");
        printf("(input: w or b): ");

        //prompts user for input
        fgets(p1.playerColor, sizeof(p1.playerColor), stdin);

        //clears input buffer if input larger than 1
        if(p1.playerColor[1] != '\n' && p1.playerColor[1] != 0){
            while(getchar() != '\n');
        }
        p1.playerColor[0] = tolower(p1.playerColor[0]);    //makes input lowercase

        if(p1.playerColor[0] == 'w' && p1.playerColor[1] != 0){
            p1.playerNum = 1;
            p2.playerNum = 2;
            p2.playerColor[0] = 'b';
            break;
        } else if(p1.playerColor[0] == 'b' && p1.playerColor[1] != 0){
            p1.playerNum = 3;    //temp value so that AI can go first
            p2.playerNum = 1;
            p2.playerColor[0] = 'w';
            break;
        } else {
            printf("\n\x1b[31mIncorrect Input. Please try again.\x1b[0m\n");
        }
    }
    printBoard(liveBoard);
    printf("\nInput follows standard 'a1' or 'c4'\nType '\x1b[92mRESIGN\x1b[0m' when you wish to stop the game\n");

    //switches between player and AI until game ends
    while(1){

        //prompts player to select a piece
        //do while loop makes sure player chooses valid piece and if input is valid
        while(1){
            if(p1.playerNum == 1){    //if player chose to be white
                printf("\nPlayer (White), select a piece you want to move: ");
            } else if(p1.playerNum == 2){    //if player chose to be black
                printf("\nPlayer (Black), select a piece you want to move: ");
            } else {
                break;    //leaves while loop so AI can move first
            }
            //prompts users for input to select piece
            fgets(input1, sizeof(input1), stdin);
            
            //if input is larger than 6, then input buffer is cleared
            if(input1[5] != '\n' && input1[5] != 0){
                while(getchar() != '\n');
            }
            if(strcmp(input1, "RESIGN") == 0){	//checks for "RESIGN" input
                resign = p1.playerNum;
                break;
            } else if(strcmp(input1, "CASTLE") == 0){    // checks for "CASTLE" input
                printf("\nWhich direction do you want to castle? (NUMBER INPUTS ONLY -- left: 1, right: 2): ");
		        scanf(" %d", &direction);		

                printf("\nWhat is the current location of your king? (input follows standard -- ex. 'e1'): ");
                scanf(" %2s", king);
                while(getchar() != '\n');
                king[0] = tolower(king[0]);    //makes input lowercase

                if (castling(king, liveBoard, direction) == 2)
                {
                    printf("\nYou cannot castle. There are pieces in the way.");
                }
                else if (castling(king, liveBoard, direction) == 0)
                {
                    printf("\nIncorrect input.");
                }
                else
                {
                    castling_move(king, liveBoard, direction);
                }
                king[0] = king[1] = 0;
                castle = 1;
                break;
            }
            input1[0] = tolower(input1[0]);    //makes input lowercase
            
            if(input1[1] >= 49 && input1[1] <= 56 && input1[0] >= 97 && input1[0] <= 104 && input1[2] == '\n'){    //checks if input is valid
                if(p1.playerNum == 1){    //if player chose to be white pieces
                    if(liveBoard[abs( input1[1] - 56 )][ (input1[0] - 97) ] > 0){   //checks if piece picked is white
                        break;
                    } else {
                        printf("\n\x1b[31mIncorrect Input. Please try again.\x1b[0m\n");
                        printBoard(liveBoard);
                        input1[0] = input1[1] = input1[5] = 0;
                    }
                } else if(p1.playerNum == 2){    //if player chose to be black pieces
                    if(liveBoard[abs( input1[1] - 56 )][ (input1[0] - 97) ] < 0){   //checks if piece picked is black
                        break;
                    } else {
                        printf("\n\x1b[31mIncorrect Input. Please try again.\x1b[0m\n");
                        printBoard(liveBoard);
                        input1[0] = input1[1] = input1[5] = 0;
                    }
                }
            } else {
                printf("\n\x1b[31mIncorrect Input. Please try again.\x1b[0m\n");
                printBoard(liveBoard);
                input1[0] = input1[1] = input1[5] = 0;
            }
        }

        //if user typed "RESIGN", then game ends
        if(resign){
            break;
        }

        //prompts user where he would like to move the piece
        //do while loop makes sure input is valid
        while(1){
            if (castle == 1) // checks if castle was performed
	        {
		        break;
	        }
            if(p1.playerNum == 1){
                printf("\nPlayer (White), select a space to move selected piece to: ");
            } else if(p1.playerNum == 2){
                printf("\nPlayer (Black), select a space to move selected piece to: ");
            } else {
                break;
            }
            //prompts users for input to select piece
            fgets(input2, sizeof(input2), stdin);
            
            //if input is larger than 6, then input buffer is cleared
            if(input2[5] != '\n' && input2[5] != 0){
                while(getchar() != '\n');
            }
            if(strcmp(input2, "RESIGN") == 0){	//checks for "RESIGN" input
                resign = p1.playerNum;
                break;
            } else if(strcmp(input2, "CANCEL") == 0){
                cancel = 1;
                resetInputs(input1, input2);
                break;
            }
            input2[0] = tolower(input2[0]);    //makes input lowercase
            
            if(input2[1] >= 49 && input2[1] <= 56 && input2[0] >= 97 && input2[0] <= 104 && input2[2] == '\n'){    //checks if input is valid
                if(p1.playerNum == 1){    //if player chose to be white pieces

                    //checks if place moving to is non-white piece
                    if(liveBoard[abs( input2[1] - 56 )][ (input2[0] - 97) ] <= 0 && PieceMovement(input1, input2, liveBoard) ){
                        break;
                    } else {
                        printf("\n\x1b[31mIncorrect Input. Please try again.\x1b[0m\n");
                        printBoard(liveBoard);
                        input2[0] = input2[1] = input2[5] = 0;
                    }
                } else if(p1.playerNum == 2){    //if player chose to be black pieces

                    //checks if place moving to is non-black piece
                    if(liveBoard[abs( input2[1] - 56 )][ (input2[0] - 97) ] >= 0 && PieceMovement(input1, input2, liveBoard)){
                        break;
                    } else {
                        printf("\n\x1b[31mIncorrect Input. Please try again.\x1b[0m\n");
                        printBoard(liveBoard);
                        input2[0] = input2[1] = input2[5] = 0;
                    }
                }
            } else {
                printf("\n\x1b[31mIncorrect Input. Please try again.\x1b[0m\n");
                printBoard(liveBoard);
                input2[0] = input2[1] = input2[5] = 0;
            }
        }
        if( (p1.playerNum == 1) || (p1.playerNum == 2) ){    //this is so AI can move first if p1 playerNum is 3
            //if user typed "RESIGN", then game ends
            if(resign){
                break;
            } else if(cancel){
                cancel = 0;
                continue;
            }
            if(moves(liveBoard, input1, input2, &capture, &promotion) == 0){
                fiftyMoveCounter++;
                if(fiftyMoveCounter == 50){    //if fifty moves are reached without capture or pawn movemnet, game is draw
                    break;
                }
            } else {
                fiftyMoveCounter = 0;
            }

            //records data to be used in linked list
            entry = CreateEntry();
            board = CreateBoard(entry, liveBoard, p1.playerNum);
            Append(list, entry);
            strncpy(board->piece, input1, 2);    //records what piece was moved
            strncpy(board->pieceMovement, input2, 2);    //records where piece was moved to
            board->captured = capture;    //records if piece was captured
            board->promotion = promotion;    //records if promotion happened
            board->resign = 0;    //0 because no one has resigned

            resetInputs(input1, input2);
            printBoard(liveBoard);

            if(castle == 1){    //records if castle happened in game
                castle = 0;
                board->castle = direction;
                direction = 0;
            }
            if(p1.playerNum == 1){
                if( check(liveBoard) == 2 ){
                    printf("\n\x1b[96mBlack is in check\x1b[0m");
                    board->check = 1;
                } 
            } else {
                if( check(liveBoard) == 1 ){
                    printf("\n\x1b[96mWhite is in check\x1b[0m");
                    board->check = 1;
                }
            }
        } else {
            p1.playerNum = 2;
        }
        

        randomAI(p1, input1, input2, liveBoard);    //calls function for AI to make move
        if(moves(liveBoard, input1, input2, &capture, &promotion) == 0){
            fiftyMoveCounter++;
            if(fiftyMoveCounter == 50){    //if fifty moves are reached without capture or pawn movemnet, game is draw
                break;
            }
        } else {
            fiftyMoveCounter = 0;
        }
        printf("\nAI, picked piece at: %c%c\n", input1[0], input1[1]);
        printf("AI, moved piece to: %c%c\n", input2[0], input2[1]);

        //records data to be used in linked list
        entry = CreateEntry();
        board = CreateBoard(entry, liveBoard, p2.playerNum);
        Append(list, entry);
        strncpy(board->piece, input1, 2);    //records what piece was moved
        strncpy(board->pieceMovement, input2, 2);    //records where piece was moved to
        board->captured = capture;    //records if piece was captured
        board->promotion = promotion;    //records if promotion happened
        board->resign = 0;    //0 because no one has resigned

        resetInputs(input1, input2);
        printBoard(liveBoard);

        if(p1.playerNum == 1){
            if( check(liveBoard) == 1 ){
	            printf("\n\x1b[96mWhite is in check\x1b[0m");
                board->check = 1;
            }
        } else{
            if( check(liveBoard) == 2 ){
                printf("\n\x1b[96mBlack is in check\x1b[0m");
                board->check = 1;
            }
        }
    }
    if(resign != 0){
        entry = CreateEntry();
        board = CreateBoard(entry, liveBoard, 0);    //0 because no player moves
        Append(list, entry);
        board->resign = resign;    //records if player resigned
    } else if(fiftyMoveCounter == 50){    //if the game ended in draw
        entry = CreateEntry();
        board = CreateBoard(entry, liveBoard, 0);    //0 because no player moved
        Append(list, entry);
        board->draw = 1;    //records if game ended in draw
    }
}

//resets character arrays used for input
void resetInputs(char input1[7], char input2[7]){
    input1[0] = input1[1] = input1[2] = input1[3] = input1[4] = input1[5] = 0;
    input2[0] = input2[1] = input2[2] = input2[3] = input2[4] = input2[5] = 0;
}

//initializes the board for beginning of game
void boardReset(int liveBoard[8][8]){
    int row, col;

    //sets 7th row and 2nd row to pawns
    for(row = 0; row < 8; row++){
        for(col = 0; col < 8; col++){

            if(row == 0){	//checks if top row of board
                if( (col == 0 || col == 7) ){		//sets rooks onto board
                    liveBoard[row][col] = -2;
                } else if( (col == 1 || col == 6) ){	//sets knights onto board
                    liveBoard[row][col] = -3;
                } else if( (col == 2 || col == 5) ){	//sets bishops onto board
                    liveBoard[row][col] = -4;
                } else if(col == 3){					//sets queen onto board
                    liveBoard[row][col] = -5;
                } else if(col == 4){					//sets king onto board
                    liveBoard[row][col] = -6;
                }
            } else if(row == 7){	//checks if bottom row of board
                if( (col == 0 || col == 7) ){		//sets rooks onto board
                    liveBoard[row][col] = 2;
                } else if( (col == 1 || col == 6) ){	//sets knights onto board
                    liveBoard[row][col] = 3;
                } else if( (col == 2 || col == 5) ){	//sets bishops onto board
                    liveBoard[row][col] = 4;
                } else if(col == 3){					//sets queen onto board
                    liveBoard[row][col] = 5;
                } else if(col == 4){					//sets king onto board
                    liveBoard[row][col] = 6;
                }
            } else if(row >= 2 && row <= 5){
                liveBoard[row][col] = 0;
            }

            //sets all pawns onto board
            if(row == 1){		//for black
                liveBoard[row][col] = -1;
            }
            if(row == 6){		//for white
                liveBoard[row][col] = 1;
            }
        }
    }
}

/* EOF */

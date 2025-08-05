/**********************************************************/
/* Title: ClientChess.c                                   */
/* Author: Daniel Guerra-Rojas                            */
/*                                                        */
/**********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include "ClientChess.h"
#include "GameSet.h"
#include "BoardStructure.h"
#include "BoardEdit.h"
#include "PieceMovement.h"

/* #define DEBUG */	/* be verbose */

// print error diagnostics and abort
void FatalError(const char *ErrorMsg){
    printf("\n./ChessGame: %s\n", ErrorMsg);
    printf("./ChessGame: Exiting!\n");
    exit(20);
} /* end of FatalError */

//begins the setup necessary to begin network game
void networkSetup(PLAYER p1, PLAYER p2, int liveBoard[8][8], LIST *list){
    //intilialized variables below here
    int menuChoice, done = 1, host = 0;
    int length, n, SocketFD, PortNo;
    struct sockaddr_in ServerAddress;
    struct hostent *Server;
    char SendBuf[256];
    char RecvBuf[256];
    char user[20] = "";

    //prompts user for the name of the server (zuma, crytsalcove, bondi, or laguna)
    while(done){
        printf("\nWhat's the name of the server you are trying to connect to?\n");
        printf("(examples: zuma, crystalcove, bondi, or laguna)");
        printf("(type \"BACK\" to go back to Main Menu): ");

        fgets(SendBuf, sizeof(SendBuf), stdin);
        length = strlen(SendBuf);
        if(SendBuf[length-1] == '\n'){
            SendBuf[length-1] = 0;
        }

        Server = gethostbyname(SendBuf);
        if(strcmp(SendBuf, "BACK") == 0){
            return;
        } else if(Server == NULL){
            printf("\n\x1b[31mIncorrect server name. Please try again.\x1b[0m\n");
        } else {
            break;
        }
    }

    //prompts user for the port number to connect to (12600-12699 are the ones we allow)
    while(done){
        printf("\nWhat is the Port Number that the server is using?\n");
        printf("(servers we allow are 12600-12699): ");

        scanf(" %d", &PortNo);
        if(PortNo < 12600 || PortNo > 12699){
            printf("\n\x1b[31mInvalid Port Number. Should be between 12600-12699.\x1b[0m\n");
        } else {
            break;
        }
    }

    //sets up some server stuff for client side
    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_port = htons(PortNo);
    ServerAddress.sin_addr = *(struct in_addr*)Server->h_addr_list[0];

    //host game or join game prompt to user
    while(done){
        printf("\nWhich of the following would you like to do?\n");
        printf("-------------------------------------------\n");
        printf("1. Host a Network Multiplayer Game\n");
        printf("2. Join a Network Multiplayer Game\n");
        printf("3. Go back to Main Menu\n");
        printf("\n Please choose one of the above: ");

        scanf(" %d", &menuChoice);
        while(getchar() != '\n');
        switch(menuChoice){
            case 1:
                host = 1;
                done = 0;
                break;
            case 2:
                done = 0;
                break;
            case 3:
                return;
            default:
                printf("\n\x1b[31mIncorrect input. Please try again.\x1b[0m\n");
        }
    }

    done = 1;
    //begins the process to logging in/registering
    while(done){
        OpenConnection(&SocketFD, &PortNo, &ServerAddress, sizeof(ServerAddress));

        printf("\nWhich of the following do you want to do?\n");
        printf("-----------------------------------------\n");
        printf("1. Register (FOR NEW ACCOUNTS ONLY)\n");
        printf("2. Log-in (FOR EXISTING ACCOUNTS ONLY)\n");
        printf("3. Go back to Main Menu\n");
        printf("\nPlease choose one of the above: ");

        scanf(" %d", &menuChoice);
        while(getchar() != '\n');

        switch(menuChoice){
            case 1:
                strncpy(SendBuf, "regis:", sizeof(SendBuf)-1);
                if( Register(SendBuf, user) ) continue;
                n = write(SocketFD, SendBuf, strlen(SendBuf));
                if(n < 0){
                    FatalError("writing to socket failed");
                }
                break;
            case 2:
                strncpy(SendBuf, "login", sizeof(SendBuf)-1);
                if(host == 1){
                    strncat(SendBuf, "1:", sizeof(SendBuf)-1-strlen(SendBuf));
                } else {
                    strncat(SendBuf, "2:", sizeof(SendBuf)-1-strlen(SendBuf));
                }
                if( LogIn(SendBuf, user) ) continue;
                n = write(SocketFD, SendBuf, strlen(SendBuf));
                if(n < 0){
                    FatalError("writing to socket failed");
                }
                break;
            case 3:
                return;
            default:
                printf("\n\x1b[31mIncorrect input. Please try again.\x1b[0m\n");
        }
        
        //checks if login/registering process was successful
        if( menuChoice == 1 || menuChoice == 2){
            sleep(1);
            n = read(SocketFD, RecvBuf, sizeof(RecvBuf)-1);
            if(n < 0){
                FatalError("reading from socket failed");
            }
            RecvBuf[n] = 0;

            //add stuff for checking if process was successful
            if(strcmp(RecvBuf, "good") == 0){
                if(menuChoice == 1){
                    printf("\n\x1b[33mRegistration successful.\x1b[0m\n");
                } else {
                    printf("\n\x1b[33mLog-in successful.\x1b[0m\n");
                    break;
                }
            } else if(strcmp(RecvBuf, "bad") == 0){
                if(menuChoice == 1){
                    printf("\n\x1b[31mINVALID INPUT.\x1b[0m The username you just ");
                    printf("entered is already\n registered. Please try again.\n");
                } else {
                    printf("\n\x1b[31mINVALID INPUT.\x1b[0m The inputted username/password ");
                    printf("does not exist.\nPlease try again.\n");
                }
            } else if(0 == strcmp(RecvBuf, "full") ){
                printf("\nThe session you are trying to join is full.\n");
            } else if(0 == strcmp(RecvBuf, "host") ){
                printf("\nThere is already a host in the session you are trying to join.\n");
            }
        }
        CloseConnection(&SocketFD);
    }
    NetworkMultiplayer(p1, p2, liveBoard, list, &SocketFD, &PortNo, &ServerAddress,
      sizeof(ServerAddress), user, host);
}

//begins the game between clients
void NetworkMultiplayer(PLAYER p1, PLAYER p2, int liveBoard[8][8], LIST *list,
  int *SocketFD, int *PortNo, struct sockaddr_in *ServerAddress, int ServerSize,
  char user[20], int host){

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
    int n;
    char SendBuf[256] = "";
    char RecvBuf[256] = "";

    boardReset(liveBoard);

    //creates ENTRY and BOARD, links both to list, and records starting board
    entry = CreateEntry();
    board = CreateBoard(entry, liveBoard, 0);    //0 because game not in play
    Append(list, entry);

    printf("\nNetwork Game Started!\n");

    //prompts user if they want to move first or not (white or not)
    while(1){
        OpenConnection(SocketFD, PortNo, ServerAddress, ServerSize);

        if(host == 1){
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
        } else {
            printf("\n\x1b[36mWaiting for Host to choose color.\n\x1b[0m");

            //has to wait for server to send something
            strncpy(SendBuf, "PING", 255);
            n = write(*SocketFD, SendBuf, strlen(SendBuf));
            if(n < 0){
                FatalError("writing to socket failed");
            }
            n = read(*SocketFD, RecvBuf, sizeof(RecvBuf)-1);
            if(n < 0){
                FatalError("reading from socket failed");
            }
            RecvBuf[n] = 0;

            if(0 == strcmp(RecvBuf, "black")){
                p1.playerNum = 3;
                p2.playerNum = 1;
                break;
            } else if(0 == strcmp(RecvBuf, "white")) {
                p1.playerNum = 1;
                p2.playerNum = 2;
                break;
            }
            CloseConnection(SocketFD);
            sleep(3);
        }
    }

    //tells server what color host chose
    if(host == 1){
        strncpy(SendBuf, "color:", 255);
        if(p1.playerNum == 1){
            strncat(SendBuf, "white", 255-strlen(SendBuf));
        } else {
            strncat(SendBuf, "black", 255-strlen(SendBuf));
        }
        n = write(*SocketFD, SendBuf, strlen(SendBuf));
        if(n < 0){
            FatalError("writing to socket failed");
        }
        n = read(*SocketFD, RecvBuf, sizeof(RecvBuf)-1);
        if(n < 0){
            FatalError("reading from sockete failed");
        }
        CloseConnection(SocketFD);
    }

    printBoard(liveBoard);
    printf("\nInput follows standard 'a1' or 'c4'\nType '\x1b[92mRESIGN\x1b[0m' when you wish to stop the game\n");
    printf("\nTo send a message to opponent, just put the message in quotation marks like so:\n");
    printf("\"Hello\"\n");

    //switches between client1 and client2 until game ends
    while(1){

        //prompts player to select a piece
        //while loop makes sure player chooses valid piece and if input is valid
        while(1){
            //at beginning of turn, checks with server if message is available for client
            OpenConnection(SocketFD, PortNo, ServerAddress, ServerSize);
            if(host == 1){
                strncpy(SendBuf, "message1", sizeof(SendBuf)-1);
            } else {
                strncpy(SendBuf, "message2", sizeof(SendBuf)-1);
            }
            SendBuf[sizeof(SendBuf)-1] = 0;
            n = write(*SocketFD, SendBuf, strlen(SendBuf));
            if (n < 0){
                FatalError("writing to socket failed");
            }
            n = read(*SocketFD, RecvBuf, sizeof(RecvBuf)-1);
            if(n < 0){
                FatalError("reading from socket failed");
            }
            RecvBuf[n] = 0;
            if(RecvBuf[0] == '\"' && RecvBuf[strlen(RecvBuf)-1] == '\"'){
                printf("\n\x1b[32mMessage from opponent:\n\x1b[0m");
                printf("%s\n", RecvBuf);
            }
            CloseConnection(SocketFD);

            if(p1.playerNum == 1){    //if player chose to be white
                printf("\nPlayer (White), select a piece you want to move: ");
            } else if(p1.playerNum == 2){    //if player chose to be black
                printf("\nPlayer (Black), select a piece you want to move: ");
            } else {
                break;    //leaves while loop so AI can move first
            }
            //prompts users for input to select piece
            fgets(SendBuf, sizeof(SendBuf)-1, stdin);
            
            //clears the input buffer
            if(strlen(SendBuf) == 255 && SendBuf[255] != '\n'){
                while(getchar() != '\n');
            }

            if(strcmp(SendBuf, "RESIGN\n") == 0){	//checks for "RESIGN" input
                resign = p1.playerNum;
                break;

            } else if(strcmp(SendBuf, "CASTLE\n") == 0){    // checks for "CASTLE" input
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

            } else if(SendBuf[0] == '\"' && SendBuf[strlen(SendBuf)-2] == '\"'){
                if(strlen(SendBuf) < 255){
                    SendBuf[strlen(SendBuf)-1] = 0;
                }

                OpenConnection(SocketFD, PortNo, ServerAddress, ServerSize);
                if(host == 1){
                    SendBuf[strlen(SendBuf)-1] = '1';
                } else {
                    SendBuf[strlen(SendBuf)-1] = '2';
                }
                n = write(*SocketFD, SendBuf, strlen(SendBuf) );
                if(n < 0){
                    FatalError("writing to socket failed");
                }
                n = read(*SocketFD, RecvBuf, sizeof(RecvBuf)-1);
                if(n < 0){
                    FatalError("reading from socket failed");
                }
                CloseConnection(SocketFD);
                continue;
            }
            if(3 == strlen(SendBuf)){
                input1[0] = tolower(SendBuf[0]);    //makes input lowercase
                input1[1] = SendBuf[1];
                input1[2] = SendBuf[2];
                
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
        }

        //if user typed "RESIGN", then game ends
        if(resign){
            OpenConnection(SocketFD, PortNo, ServerAddress, ServerSize);
            if(host == 1){
                strncpy(SendBuf, "RESIGN1", sizeof(SendBuf)-1);
            } else {
                strncpy(SendBuf, "RESIGN2", sizeof(SendBuf)-1);
            }
            SendBuf[sizeof(SendBuf)-1] = 0;
            n = write(*SocketFD, SendBuf, strlen(SendBuf));
            if(n < 0){
                FatalError("writing to socket failed");
            }
            n = read(*SocketFD, RecvBuf, sizeof(RecvBuf)-1);
            if(n < 0){
                FatalError("reading from socket failed");
            }
            CloseConnection(SocketFD);
            break;
        }

        //prompts user where he would like to move the piece
        //do while loop makes sure input is valid
        while(1){
            if (castle == 1) // checks if castle was performed
	        {
		        break;
	        }

            //at beginning of turn, checks with server if message is available for client
            OpenConnection(SocketFD, PortNo, ServerAddress, ServerSize);
            if(host == 1){
                strncpy(SendBuf, "message1", sizeof(SendBuf)-1);
            } else {
                strncpy(SendBuf, "message2", sizeof(SendBuf)-1);
            }
            SendBuf[sizeof(SendBuf)-1] = 0;
            n = write(*SocketFD, SendBuf, strlen(SendBuf));
            if (n < 0){
                FatalError("writing to socket failed");
            }
            n = read(*SocketFD, RecvBuf, sizeof(RecvBuf)-1);
            if(n < 0){
                FatalError("reading from socket failed");
            }
            RecvBuf[n] = 0;
            if(RecvBuf[0] == '\"' && RecvBuf[strlen(RecvBuf)-1] == '\"'){
                printf("\n\x1b[32mMessage from opponent:\n\x1b[0m");
                printf("%s\n", RecvBuf);
            }
            CloseConnection(SocketFD);

            if(p1.playerNum == 1){
                printf("\nPlayer (White), select a space to move selected piece to: ");
            } else if(p1.playerNum == 2){
                printf("\nPlayer (Black), select a space to move selected piece to: ");
            } else {
                break;
            }
            //prompts users for input to select piece
            fgets(SendBuf, sizeof(SendBuf), stdin);
            
            //clears the input buffer
            if(strlen(SendBuf) == 255){
                while(getchar() != '\n');
            }

            if(strcmp(SendBuf, "RESIGN\n") == 0){	//checks for "RESIGN" input
                resign = p1.playerNum;
                break;
            } else if(strcmp(SendBuf, "CANCEL\n") == 0){
                cancel = 1;
                resetInputs(input1);
                resetInputs(input2);
                break;
            } else if(SendBuf[0] == '\"' && SendBuf[strlen(SendBuf)-2] == '\"'){
                if(strlen(SendBuf) < 255){
                    SendBuf[strlen(SendBuf)-1] = 0;
                }

                OpenConnection(SocketFD, PortNo, ServerAddress, ServerSize);
                if(host == 1){
                    SendBuf[strlen(SendBuf)-1] = '1';
                } else {
                    SendBuf[strlen(SendBuf)-1] = '2';
                }
                n = write(*SocketFD, SendBuf, strlen(SendBuf) );
                if(n < 0){
                    FatalError("writing to socket failed");
                }
                n = read(*SocketFD, RecvBuf, sizeof(RecvBuf)-1);
                if(n < 0){
                    FatalError("reading from socket failed");
                }
                CloseConnection(SocketFD);
                continue;
            }
            if(3 == strlen(SendBuf)){
                input2[0] = tolower(SendBuf[0]);    //makes input lowercase
                input2[1] = SendBuf[1];
                input2[2] = SendBuf[2];
                
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
        }
        if( (p1.playerNum == 1) || (p1.playerNum == 2) ){    //this is so AI can move first if p1 playerNum is 3
            //if user typed "RESIGN", then game ends
            if(resign){
                OpenConnection(SocketFD, PortNo, ServerAddress, ServerSize);
                if(host == 1){
                    strncpy(SendBuf, "RESIGN1", sizeof(SendBuf)-1);
                } else {
                    strncpy(SendBuf, "RESIGN2", sizeof(SendBuf)-1);
                }
                SendBuf[sizeof(SendBuf)-1] = 0;
                n = write(*SocketFD, SendBuf, strlen(SendBuf));
                if(n < 0){
                    FatalError("writing to socket failed");
                }
                n = read(*SocketFD, RecvBuf, sizeof(RecvBuf)-1);
                if(n < 0){
                    FatalError("reading from socket failed");
                }
                CloseConnection(SocketFD);
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

            SendBuf[0] = input1[0];
            SendBuf[1] = input1[1];
            SendBuf[2] = input2[0];
            SendBuf[3] = input2[1];
            if(host == 1){
                SendBuf[4] = '1';
            } else {
                SendBuf[4] = '2';
            }
            SendBuf[5] = 0;

            OpenConnection(SocketFD, PortNo, ServerAddress, ServerSize);
            n = write(*SocketFD, SendBuf, strlen(SendBuf) );
            if(n < 0){
                FatalError("writing to socket failed");
            }
            n = read(*SocketFD, RecvBuf, sizeof(RecvBuf)-1);
            if(n < 0){
                FatalError("reading from socket failed");
            }
            CloseConnection(SocketFD);

            resetInputs(input1);
            resetInputs(input2);
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
        
        //this is where client has to wait for the opponent to move
        while(1){
            printf("\n\x1b[36mWaiting for opponent feedback.\x1b[0m\n");
            sleep(3);
            if(host == 1){
                strncpy(SendBuf, "WAITING1", sizeof(SendBuf)-1);
            } else {
                strncpy(SendBuf, "WAITING2", sizeof(SendBuf)-1);
            }
            SendBuf[sizeof(SendBuf)-1] = 0;

            OpenConnection(SocketFD, PortNo, ServerAddress, ServerSize);
            n = write(*SocketFD, SendBuf, strlen(SendBuf));
            if(n < 0){
                FatalError("writing to socket failed");
            }
            n = read(*SocketFD, RecvBuf, sizeof(RecvBuf)-1);
            if(n < 0){
                FatalError("reading from socket failed");
            }
            RecvBuf[n] = 0;
            CloseConnection(SocketFD);

            if(0 == strcmp(RecvBuf, "RESIGN") ){
                printf("\n\x1b[33mOpponent Resigned.\x1b[0m\n");
                resign = p2.playerNum;
                break;
            } else if(RecvBuf[0] == '\"' && RecvBuf[strlen(RecvBuf)-1] == '\"'){
                printf("\n\x1b[32mMessage from opponent:\n\x1b[0m");
                printf("%s\n", RecvBuf);

                printf("\nWould you like to respond?\n");
                printf("1. Yes\n");
                printf("2. No\n");

                printf("\nPlease choose one of the above: ");
                scanf(" %d", &n);
                while(getchar() != '\n');
                if(n == 1){
                    while(1){
                        printf("Input your message in quotation marks.\n");
                        printf("(example: \"Hello\"): ");
                        fgets(SendBuf, sizeof(SendBuf), stdin);
                        if(strlen(SendBuf) < 255){
                            SendBuf[strlen(SendBuf)-1] = 0;
                        }
                        SendBuf[sizeof(SendBuf)-1] = 0;

                        if(SendBuf[0] == '\"' && SendBuf[strlen(SendBuf)-1] == '\"'){
                            if(host == 1){
                                SendBuf[strlen(SendBuf)-1] = '1';
                            } else {
                                SendBuf[strlen(SendBuf)-1] = '2';
                            }

                            OpenConnection(SocketFD, PortNo, ServerAddress, ServerSize);
                            n = write(*SocketFD, SendBuf, strlen(SendBuf));
                            if(n < 0){
                                FatalError("writing to socket failed");
                            }
                            n = read(*SocketFD, RecvBuf, sizeof(RecvBuf)-1);
                            if(n < 0){
                                FatalError("reading from socket failed");
                            }
                            CloseConnection(SocketFD);

                            break;
                        } else {
                            printf("\nMessage was not sent. Try again.\n");
                        }
                    }
                }
            } else if(strlen(RecvBuf) == 5){
                strncpy(input1, RecvBuf, 2);
                strncpy(input2, &RecvBuf[2], 2);
                break;
            }
        }
        if(resign){
            break;
        }

        if(moves(liveBoard, input1, input2, &capture, &promotion) == 0){
            fiftyMoveCounter++;
            if(fiftyMoveCounter == 50){    //if fifty moves are reached without capture or pawn movemnet, game is draw
                break;
            }
        } else {
            fiftyMoveCounter = 0;
        }
        printf("\nOpponent, picked piece at: %c%c\n", input1[0], input1[1]);
        printf("Opponent, moved piece to: %c%c\n", input2[0], input2[1]);

        //records data to be used in linked list
        entry = CreateEntry();
        board = CreateBoard(entry, liveBoard, p2.playerNum);
        Append(list, entry);
        strncpy(board->piece, input1, 2);    //records what piece was moved
        strncpy(board->pieceMovement, input2, 2);    //records where piece was moved to
        board->captured = capture;    //records if piece was captured
        board->promotion = promotion;    //records if promotion happened
        board->resign = 0;    //0 because no one has resigned

        resetInputs(input1);
        resetInputs(input2);
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

    //this is taken care of after games ends
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

//begins process of dealing with clients request
void OpenConnection(int *SocketFD, int *PortNo, struct sockaddr_in *ServerAddress, int ServerSize){
    *SocketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (SocketFD < 0){
        FatalError("socket creation failed");
    }
    if (connect(*SocketFD, (struct sockaddr*)ServerAddress, ServerSize) < 0){
        FatalError("connecting to server failed");
    }
}

//ends process of dealing with clients request
void CloseConnection(int *SocketFD){
    close(*SocketFD);
}

//handles process for user registering
int Register(char SendBuf[256], char user[20]){
    int length;

    printf("\n\x1b[33mRegistration has been chosen.\x1b[0m\n");
    printf("\nYou will now be prompted to input a username\n");
    printf("and password.\n");

    printf("\nUsername and password must be between 6-15\n");
    printf("characters long and have no spaces.\n");

    printf("\nNOTE: The username entered must be unique and\n");
    printf("not already in the system, otherwise\n");
    printf("an error message will be displayed and\n");
    printf("a new username will have to be inputted.\n");
    printf("You can type \"BACK\" at any time to go\n");
    printf("back to the previous screen.\n");
    printf("\n-----------------------\n");

    //prompts user to input UNIQUE username for account
    while(1){
        printf("Please enter a UNIQUE username: ");

        fgets(user, 20, stdin);
        if(strlen(user) == 19){
            while(getchar() != '\n');
        }
        length = strlen(user);
        if(user[length-1] == '\n'){
            user[--length] = 0;
        }
        if(6 <= length && length <= 15 && strchr(user, ' ') == 0 && strchr(user, 9) == 0){
            strncat(SendBuf, user, 255-strlen(SendBuf));
            break;
        } else if(strcmp(user, "BACK") == 0){
            return 1;
        } else {
            printf("\n\x1b[31mInvalid input! Input did not meet requirements.\x1b[0m\n");
        }
    }
    //appends a space to separate username and password
    strncat(SendBuf, " ", 255-strlen(SendBuf));

    //prompts user to input password for account
    while(1){
        printf("\nPassword: ");

        fgets(user, 20, stdin);
        length = strlen(user);
        if(user[length-1] == '\n'){
            user[--length] = 0;
        }
        if(6 <= length && length <= 15 && strchr(user, ' ') == 0 && strchr(user, 9) == 0){
            strncat(SendBuf, user, 255-strlen(SendBuf));
            break;
        } else if(strcmp(user, "BACK") == 0){
            return 1;
        } else {
            printf("\n\x1b[31mInvalid input! Input did not meet requirements.\x1b[0m\n");
        }
    }

    return 0;
}

//handles process for user logging in
int LogIn(char SendBuf[256], char user[20]){
    int length;

    printf("\n\x1b[33mLog-in has been chosen.\x1b[0m\n");
    printf("\nYou will now be prompted to input a username\n");
    printf("and password.\n");

    printf("\nUsername and password must be between 6-15\n");
    printf("characters long and have no spaces.\n");

    printf("\nNOTE: If the username/password does not exist,\n");
    printf("an error message will display and you\n");
    printf("will be prompted to input your username/\n");
    printf("password again. You can type \"BACK\" at any\n");
    printf("time to go back to the previous screen.\n");
    printf("\n-----------------------");

    //prompts user to input username for account
    while(1){
        printf("\nUsername: ");

        fgets(user, 20, stdin);
        length = strlen(user);
        if(user[length-1] == '\n'){
            user[--length] = 0;
        }
        if(6 <= length && length <= 15 && strchr(user, ' ') == 0 && strchr(user, 9) == 0){
            strncat(SendBuf, user, 255-strlen(SendBuf));
            break;
        } else if(strcmp(user, "BACK") == 0){
            return 1;
        } else {
            printf("\n\x1b[31mInvalid input! Input did not meet requirements.\x1b[0m\n");
        }
    }
    //appends a space to separate username and password
    strncat(SendBuf, " ", 255-strlen(SendBuf));

    //prompts user to input password for account
    while(1){
        printf("\nPassword: ");

        fgets(user, 20, stdin);
        length = strlen(user);
        if(user[length-1] == '\n'){
            user[--length] = 0;
        }
        if(6 <= length && length <= 15 && strchr(user, ' ') == 0 && strchr(user, 9) == 0){
            strncat(SendBuf, user, 255-strlen(SendBuf));
            break;
        } else if(strcmp(user, "BACK") == 0){
            return 1;
        } else {
            printf("\n\x1b[31mInvalid input! Input did not meet requirements.\x1b[0m\n");
        }
    }
    printf("\nVerifying account.\n");
    return 0;
}

/* EOF */
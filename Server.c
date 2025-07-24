/**********************************************************/
/* Title: Server.c                                        */
/* Author: Daniel Guerra-Rojas, Rainer Doemer             */
/*                                                        */
/* Modifications:                                         */
/* 05/29/21  DGR  Initial version                         */
/**********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <sys/select.h>
#include <arpa/inet.h>

/* #define DEBUG */	/* be verbose */

/*** type definitions ****************************************************/

typedef void (*ClientHandler)(int DataSocketFD);
typedef void (*TimeoutHandler)(void);

/*** global variables ****************************************************/

const char *Program	= NULL;

int Shutdown = 0;    /* keep running until Shutdown == 1 */

static char player1[16] = "";    //keeps username stored from user as ID

static char player2[16] = "";    //keeps username stored from user as ID

/*** global functions ****************************************************/

// print error diagnostics and abort
void FatalError(const char *ErrorMsg){
    fputs(Program, stderr);
    fputs(": ", stderr);
    perror(ErrorMsg);
    fputs(Program, stderr);
    fputs(": Exiting!\n", stderr);
    exit(20);
} /* end of FatalError */

/* create a socket on this server */
int MakeServerSocket(uint16_t PortNo){
    int ServSocketFD;
    struct sockaddr_in ServSocketName;

    /* create the socket */
    ServSocketFD = socket(PF_INET, SOCK_STREAM, 0);
    if (ServSocketFD < 0){
        FatalError("service socket creation failed");
    }

    /* bind the socket to this server */
    ServSocketName.sin_family = AF_INET;
    ServSocketName.sin_port = htons(PortNo);
    ServSocketName.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(ServSocketFD, (struct sockaddr*)&ServSocketName,
	  sizeof(ServSocketName)) < 0){
        FatalError("binding the server to a socket failed");
    }

    /* start listening to this socket */
    if (listen(ServSocketFD, 5) < 0){    /* max 5 clients in backlog */
        FatalError("listening on socket failed");
    }
    return ServSocketFD;
} /* end of MakeServerSocket */

//checks if new account already exists in database
//if so, returns 1 and doesn't allow new account to be created
//else, reuturns 0 and creates account with inputted details
int RegisterCheck(char inputtedUsername[20], char inputtedPassword[20]){
    FILE *fp;
    char fileLine[40] = "";
    char fileUsername[20] = "";
    char filePassword[20] = "";

    //attempts to open file for reading and appending
    fp = fopen("accounts.txt", "a+");

    //reads through lines of txt file checking if username already exists
    while(1){
        //if end of file is reached, while loop breaks
        if(NULL == fgets(fileLine, sizeof(fileLine), fp) ){
            break;
        }
        sscanf(fileLine, "%s %s", fileUsername, filePassword);

        //checks if username user entered matches a username from database
        if(0 == strcmp(fileUsername, inputtedUsername) ){
            return 1;
        }
    }

    //if username doesn't match, then username and password is added to database
    fprintf(fp, "%s %s\n", inputtedUsername, inputtedPassword);

    fclose(fp);
    return 0;
}

//checks if account exists in database
//returns 0 if it does, otherwise returns 1
int LoginCheck(char inputtedUsername[20], char inputtedPassword[20]){
    FILE *fp;
    char fileLine[40] = "";
    char fileUsername[20] = "";
    char filePassword[20] = "";

    //attempts to open file for reading and appending
    fp = fopen("accounts.txt", "a+");

    //reads through lines of txt file checking if username matches inputted username
    while(1){
        //if end of file is reached, while loop breaks
        if(NULL == fgets(fileLine, sizeof(fileLine), fp) ){
            break;
        }
        sscanf(fileLine, "%s %s", fileUsername, filePassword);

        //checks if username user entered matches a username from database
        if(0 == strcmp(fileUsername, inputtedUsername) ){

            //checks if password user entered matches password for account
            if(0 == strcmp(filePassword, inputtedPassword) ){
                return 0;
            } else {
                return 1;
            }
        }
    }


    fclose(fp);
    return 1;
}

/* process a time request by a client */
void ProcessRequest(int DataSocketFD){
    //static variables are used to store data between function calls
    int  l, n;
    static int transfer1 = 0, transfer2 = 0, resign = 0;
    char input[20] = "";
    static char color[20] = "";
    char username[20] = "";
    char password[20] = "";
    char RecvBuf[256];	/* message buffer for receiving a message */
    char SendBuf[256];	/* message buffer for sending a response */
    static char message1[256] = "";    //message stored to send to Host
    static char message2[256] = "";    //message stored to send to Joinee

    n = read(DataSocketFD, RecvBuf, sizeof(RecvBuf)-1);
    if (n < 0){
        FatalError("reading from data socket failed");
    }
    RecvBuf[n] = 0;
    strncpy(input, RecvBuf, 5);
    #ifdef DEBUG
        printf("%s: Received message: %s\n", Program, RecvBuf);
    #endif
    if( 0 == strcmp(input, "login") ){
            l = strchr(RecvBuf, ' ') - strchr(RecvBuf, ':') - 1;
            if(RecvBuf[5] == '1'){
                strncpy(player1, strchr(RecvBuf, ':')+1, l);

                //checks if account entered is in database for login
                strncpy(username, strchr(RecvBuf, ':')+1, l);
                strncpy(password, strchr(RecvBuf, ' ')+1, sizeof(password));
                if( LoginCheck(username, password) ){
                    strncpy(SendBuf, "bad", sizeof(SendBuf)-1);
                    SendBuf[sizeof(SendBuf)-1] = 0;
                } else {
                    strncpy(SendBuf, "good", sizeof(SendBuf)-1);
                    SendBuf[sizeof(SendBuf)-1] = 0;
                }

            } else if(RecvBuf[5] == '2'){
                strncpy(player2, strchr(RecvBuf, ':')+1, l);
                
                //checks if account entered is in database for login
                strncpy(username, strchr(RecvBuf, ':')+1, l);
                strncpy(password, strchr(RecvBuf, ' ')+1, sizeof(password));
                if( LoginCheck(username, password) ){
                    strncpy(SendBuf, "bad", sizeof(SendBuf)-1);
                    SendBuf[sizeof(SendBuf)-1] = 0;
                } else {
                    strncpy(SendBuf, "good", sizeof(SendBuf)-1);
                    SendBuf[sizeof(SendBuf)-1] = 0;
                }
                
            } else if(RecvBuf[5] == '1'){
                strncpy(SendBuf, "host", sizeof(SendBuf)-1);
            }

    } else if(0 == strcmp(input, "regis") ){
        //checks if accounts exists in database. creates account if it doesn't
        l = strchr(RecvBuf, ' ') - strchr(RecvBuf, ':') - 1;
        strncpy(username, strchr(RecvBuf, ':')+1, l);
        strncpy(password, strchr(RecvBuf, ' ')+1, sizeof(password));
        if( RegisterCheck(username, password) ){
            strncpy(SendBuf, "bad", sizeof(SendBuf)-1);
            SendBuf[sizeof(SendBuf)-1] = 0;
        } else {
            strncpy(SendBuf, "good", sizeof(SendBuf)-1);
            SendBuf[sizeof(SendBuf)-1] = 0;
        }

    } else if(0 == strcmp(input, "color")){
        transfer2 = 1;
        if(0 == strcmp( (strchr(RecvBuf, ':')+1), "white" ) ){
            strncpy(color, "black", sizeof(color)-1);
        } else {
            strncpy(color, "white", sizeof(color)-1);
        }
        strncpy(SendBuf, "color acquired", sizeof(SendBuf)-1);
        SendBuf[sizeof(SendBuf)-1] = 0;
        
    } else if(0 == strcmp(RecvBuf, "PING") ){
        //if server needs to send something to user
        if(transfer2 == 1){
            if(0 == strcmp(color, "white")){
                strncpy(SendBuf, "white", sizeof(SendBuf)-1);
            } else {
                strncpy(SendBuf, "black", sizeof(SendBuf)-1);
            }
            SendBuf[sizeof(SendBuf)-1] = 0;
            transfer2 = 0;
        } else {
            strncpy(SendBuf, "not ready", sizeof(SendBuf)-1);
            SendBuf[sizeof(SendBuf)-1] = 0;
        }

    } else if(5 == strlen(RecvBuf) ){
        if(RecvBuf[4] == '1'){
            transfer2 = 1;
            strncpy(message2, RecvBuf, sizeof(message2)-1);

        } else if(RecvBuf[4] == '2'){
            transfer1 = 1;
            strncpy(message1, RecvBuf, sizeof(message1)-1);

        }
        strncpy(SendBuf, "done", sizeof(SendBuf)-1);
        SendBuf[sizeof(SendBuf)-1] = 0;

    } else if(0 == strcmp(RecvBuf, "message1") ){
        if(transfer1 == 1){
            strncpy(SendBuf, message1, sizeof(SendBuf)-1);
            SendBuf[sizeof(SendBuf)-1] = 0;
            transfer1 = 0;

        } else {
            strncpy(SendBuf, "no", sizeof(SendBuf)-1);
            SendBuf[sizeof(SendBuf)-1] = 0;
        }

    } else if(0 == strcmp(RecvBuf, "message2") ){
        if(transfer2 == 1){
            strncpy(SendBuf, "yes", sizeof(SendBuf)-1);
            SendBuf[sizeof(SendBuf)-1] = 0;
            n = write(DataSocketFD, SendBuf, strlen(SendBuf));
            if(n < 0){
                FatalError("writing to the socket failed");
            }
            n = read(DataSocketFD, RecvBuf, sizeof(RecvBuf)-1);
            if(n < 0){
                FatalError("reading from socket failed");
            }
            strncpy(SendBuf, message1, sizeof(SendBuf)-1);
            SendBuf[sizeof(SendBuf)-1] = 0;
            transfer2 = 0;

        } else {
            strncpy(SendBuf, "no", sizeof(SendBuf)-1);
        }

    } else if(RecvBuf[0] == '\"' && RecvBuf[strlen(RecvBuf)-1] == '1'){
        //stores message and knows to send it to joinee
        strncpy(message2, RecvBuf, sizeof(message2)-1);
        message2[strlen(message2)-1] = '\"';
        transfer2 = 1;

        strncpy(SendBuf, "done", sizeof(SendBuf)-1);
        SendBuf[sizeof(SendBuf)-1] = 0;

    } else if(RecvBuf[0] == '\"' && RecvBuf[strlen(RecvBuf)-1] == '2'){
        //stores message and knows to send it to host
        strncpy(message1, RecvBuf, sizeof(message1)-1);
        message1[strlen(message1)-1] = '\"';
        transfer1 = 1;

        strncpy(SendBuf, "done", sizeof(SendBuf)-1);
        SendBuf[sizeof(SendBuf)-1] = 0;

    } else if(0 == strcmp(RecvBuf, "WAITING1") ){
        if(resign == 1){
            //lets Host know that Joinee resigned
            strncpy(SendBuf, "RESIGN", sizeof(SendBuf)-1);
            SendBuf[sizeof(SendBuf)-1] = 0;
            resign = 0;

        } else if(transfer1 == 1){
            //sends message from joinee to host
            strncpy(SendBuf, message1, sizeof(SendBuf)-1);
            SendBuf[sizeof(SendBuf)-1] = 0;
            transfer1 = 0;

        } else {
            strncpy(SendBuf, "not ready", sizeof(SendBuf)-1);
            SendBuf[sizeof(SendBuf)-1] = 0;
            n = write(DataSocketFD, SendBuf, strlen(SendBuf));
            if(n < 0){
                FatalError("writing to socket failed");
            }
        }
        
    } else if(0 == strcmp(RecvBuf, "WAITING2") ){
        if(resign == 1){
            //lets Joinee know that Host resigned
            strncpy(SendBuf, "RESIGN", sizeof(SendBuf)-1);
            SendBuf[sizeof(SendBuf)-1] = 0;
            resign = 0;

        } else if(transfer2 == 1){
            //sends message from host to joinee
            strncpy(SendBuf, message2, sizeof(SendBuf)-1);
            SendBuf[sizeof(SendBuf)-1] = 0;
            transfer2 = 0;

        } else {
            strncpy(SendBuf, "not ready", sizeof(SendBuf)-1);
            SendBuf[sizeof(SendBuf)-1] = 0;
            n = write(DataSocketFD, SendBuf, strlen(SendBuf));
            if(n < 0){
                FatalError("writing to socket failed");
            }
        }

    } else if (0 == strcmp(RecvBuf, "RESIGN1") ){    //if a client resigns, server has to let other client know
        resign = 1;
        transfer2 = 1;

        strncpy(SendBuf, "done", sizeof(SendBuf)-1);
        SendBuf[sizeof(SendBuf)-1] = 0;

    } else if(0 == strcmp(RecvBuf, "RESIGN2") ){
        resign = 1;
        transfer1 = 1;

        strncpy(SendBuf, "done", sizeof(SendBuf)-1);
        SendBuf[sizeof(SendBuf)-1] = 0;

    } else if(0 == strcmp(RecvBuf, "SHUTDOWN") ){
        Shutdown = 1;
        strncpy(SendBuf, "OK SHUTDOWN", sizeof(SendBuf)-1);
        SendBuf[sizeof(SendBuf)-1] = 0;
        
    } else {
        strncpy(SendBuf, "ERROR unknown command ", sizeof(SendBuf)-1);
        SendBuf[sizeof(SendBuf)-1] = 0;
        strncat(SendBuf, RecvBuf, sizeof(SendBuf)-1-strlen(SendBuf));
    }
    
    l = strlen(SendBuf);
    #ifdef DEBUG
        printf("%s: Sending response: %s.\n", Program, SendBuf);
    #endif
    n = write(DataSocketFD, SendBuf, l);
    if (n < 0){
        FatalError("writing to data socket failed");
    }
} /* end of ProcessRequest */

/* simple server main loop */
void ServerMainLoop(
  int ServSocketFD,		/* server socket to wait on */
  ClientHandler HandleClient,	/* client handler to call */
  int Timeout){			/* timeout in micro seconds */

    int DataSocketFD;	/* socket for a new client */
    socklen_t ClientLen;
    struct sockaddr_in
	ClientAddress;	/* client address we connect with */
    fd_set ActiveFDs;	/* socket file descriptors to select from */
    fd_set ReadFDs;	/* socket file descriptors ready to read from */
    struct timeval TimeVal;
    int res, i;

    FD_ZERO(&ActiveFDs);		/* set of active sockets */
    FD_SET(ServSocketFD, &ActiveFDs);	/* server socket is active */
    while(!Shutdown){
        ReadFDs = ActiveFDs;
        TimeVal.tv_sec  = Timeout / 1000000;	/* seconds */
        TimeVal.tv_usec = Timeout % 1000000;	/* microseconds */

        /* block until input arrives on active sockets or until timeout */
        res = select(FD_SETSIZE, &ReadFDs, NULL, NULL, &TimeVal);
        if (res < 0){
            FatalError("wait for input or timeout (select) failed");
        } else {		/* some FDs have data ready to read */
            for(i=0; i<FD_SETSIZE; i++){
                if (FD_ISSET(i, &ReadFDs)){
                    if (i == ServSocketFD){    /* connection request on server socket */
                        #ifdef DEBUG
                            printf("%s: Accepting new client %d...\n", Program, i);
                        #endif
                        ClientLen = sizeof(ClientAddress);
                        DataSocketFD = accept(ServSocketFD, 
                          (struct sockaddr*)&ClientAddress, &ClientLen);
                        if (DataSocketFD < 0){
                            FatalError("data socket creation (accept) failed");
                        }
                        #ifdef DEBUG
                            printf("%s: Client %d connected from %s:%hu.\n", Program, i, 
                              inet_ntoa(ClientAddress.sin_addr), ntohs(ClientAddress.sin_port));
                        #endif
                        FD_SET(DataSocketFD, &ActiveFDs);
                    } else {    /* active communication with a client */
                        #ifdef DEBUG
                            printf("%s: Dealing with client %d...\n", Program, i);
                        #endif
                        HandleClient(i);
                        #ifdef DEBUG
                            printf("%s: Closing client %d connection.\n", Program, i);
                        #endif
                        close(i);
                        FD_CLR(i, &ActiveFDs);
                    }
                }
    	    }
        }
    }
} /* end of ServerMainLoop */

/*** main function *******************************************************/
int main(int argc, char *argv[])
{
    int ServSocketFD;	/* socket file descriptor for service */
    int PortNo;		/* port number */

    Program = argv[0];	/* publish program name (for diagnostics) */
    #ifdef DEBUG
        printf("%s: Starting...\n", Program);
    #endif
    if (argc < 2){
        fprintf(stderr, "Usage: %s port\n", Program);
        exit(10);
    }
    PortNo = atoi(argv[1]);	/* get the port number */
    if (PortNo <= 2000){
        fprintf(stderr, "%s: invalid port number %d, should be >2000\n",
		  Program, PortNo);
        exit(10);
    }
    #ifdef DEBUG
        printf("%s: Creating the server socket...\n", Program);
    #endif
    ServSocketFD = MakeServerSocket(PortNo);
    printf("%s: Providing current time at port %d...\n", Program, PortNo);
    ServerMainLoop(ServSocketFD, ProcessRequest, 250000);
    printf("\n%s: Shutting down.\n", Program);
    close(ServSocketFD);
    return 0;
}

/* EOF */

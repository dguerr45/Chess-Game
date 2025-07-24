/**********************************************************/
/* Title: ClientChess.h                                   */
/* Author: Daniel Guerra-Rojas                            */
/*                                                        */
/* Modifications:                                         */
/* 05/30/21  DGR  Initial version                         */
/**********************************************************/
#ifndef CLIENTCHESS_H
#define CLIENTCHESS_H

#include <netinet/in.h>
#include "BoardStructure.h"

// print error diagnostics and abort
void FatalError(const char *ErrorMsg);

//begins the game between clients
void NetworkMultiplayer(PLAYER p1, PLAYER p2, int liveBoard[8][8], LIST *list,
  int *SocketFD, int *PortNo, struct sockaddr_in *ServerAddress, int ServerSize,
  char user[20], int host);

//starts process of dealing with clients request
void OpenConnection(int *SocketFD, int *PortNo, struct sockaddr_in *ServerAddress, int ServerSize);

//ends process of dealing with clients request
void CloseConnection(int *SocketFD);

//handles process for user registering
int Register(char SendBuf[256], char user[20]);

//handles process for user logging in
int LogIn(char SendBuf[256], char user[20]);

#endif /* CLIENTCHESS_H */

/* EOF */
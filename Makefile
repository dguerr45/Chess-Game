######################################################################
#
# 05/11/21 DG				   : added BoardStructure.o 
# 05/07/21 Felipe Damian       : added CheckWin files
# 04/26/21 MB, DG, AK, FD      : updated Makefile for alpha
# 04/25/21 Arhant Katare       : updated for EECS22 Lab Alpha Release
#
######################################################################

DESIGN = ChessGame testChessGame Server

#libs
LIBS   = libAI.a

CC     = gcc
DEBUG  = -DDEBUG -g
CFLAGS = -ansi -std=c99 -Wall -c
LFLAGS = -ansi -std=c99 -Wall
AR     = ar rc
RANLIB = ranlib


all: ChessGame testChessGame Server


########### generate object files ###########

#target to generate GameSet.o
GameSet.o: GameSet.h GameSet.c BoardEdit.h Player.h PieceMovement.h CheckWin.h BoardStructure.h TextLog.h AI.h
	$(CC) $(CFLAGS) GameSet.c -o GameSet.o

#target to generate Main.o
Game.o: GameSet.h Main.c BoardEdit.h
	$(CC) $(CFLAGS) Main.c -o Main.o

#target to generate BoardEdit.o
BoardEdit.o: Player.h BoardEdit.h BoardEdit.c BoardStructure.h PieceMovement.h
	$(CC) $(CFLAGS) BoardEdit.c -o BoardEdit.o

#target to generate PieceMovement.o
PieceMovement.o: PieceMovement.h PieceMovement.c CheckWin.h
	$(CC) $(CFLAGS) PieceMovement.c -o PieceMovement.o

#target to generate CheckWin.o
CheckWin.o: CheckWin.h CheckWin.c BoardEdit.h PieceMovement.h
	$(CC) $(CFLAGS) CheckWin.c -o CheckWin.o

#target to generate AI.o
AI.o: AI.h AI.c PieceMovement.h
	$(CC) $(CFLAGS) AI.c -o AI.o

#target to generate BoardStructure.o
BoardStructure.o: BoardStructure.h BoardStructure.c
	$(CC) $(CFLAGS) BoardStructure.c -o BoardStructure.o

#target to generate TextLog.o
TextLog.o: TextLog.h TextLog.c BoardStructure.h
	$(CC) $(CFLAGS) TextLog.c -o TextLog.o

#target to generate clientChess.o
ClientChess.o: GameSet.h ClientChess.c
	$(CC) $(CFLAGS) ClientChess.c -o ClientChess.o

#target to generate testGameSet.o
testGameSet.o: GameSet.h GameSet.c BoardEdit.h Player.h PieceMovement.h CheckWin.h BoardStructure.h TextLog.h AI.h
	$(CC) $(CFLAGS) $(DEBUG) GameSet.c -o testGameSet.o

#target to generate testGame.o
testGame.o: GameSet.h Main.c BoardEdit.h
	$(CC) $(CFLAGS) $(DEBUG) Main.c -o testGame.o

#target to generate testBoardEdit.o
testBoardEdit.o: Player.h BoardEdit.h BoardEdit.c BoardStructure.h PieceMovement.h
	$(CC) $(CFLAGS) $(DEBUG) BoardEdit.c -o testBoardEdit.o

#target to generate testPieceMovement.o
testPieceMovement.o: PieceMovement.h PieceMovement.c CheckWin.h
	$(CC) $(CFLAGS) $(DEBUG) PieceMovement.c -o testPieceMovement.o

#target to generate testCheckWin.o
testCheckWin.o: CheckWin.h CheckWin.c BoardEdit.h PieceMovement.h
	$(CC) $(CFLAGS) $(DEBUG) CheckWin.c -o testCheckWin.o

#target to generate testAI.o
testAI.o: AI.h AI.c PieceMovement.h
	$(CC) $(CFLAGS) $(DEBUG) AI.c -o testAI.o

#target to generate BoardStructure.o
testBoardStructure.o: BoardStructure.h BoardStructure.c
	$(CC) $(CFLAGS) $(DEBUG) BoardStructure.c -o testBoardStructure.o

#target to generate testTextLog.o
testTextLog.o: TextLog.h TextLog.c BoardStructure.h
	$(CC) $(CFLAGS) $(DEBUG) TextLog.c -o testTextLog.o

#target to generate testClientChess.o
testClientChess.o: GameSet.h ClientChess.c
	$(CC) $(CFLAGS) $(DEBUG) ClientChess.c -o testClientChess.o

########### generate executables ###########

#target to generate Game
ChessGame: Main.o BoardEdit.o GameSet.o PieceMovement.o CheckWin.o AI.o BoardStructure.o TextLog.o ClientChess.o
	$(CC) $(LFLAGS) GameSet.o Main.o BoardEdit.o PieceMovement.o CheckWin.o AI.o BoardStructure.o TextLog.o ClientChess.o -o ChessGame

testChessGame: testGame.o testBoardEdit.o testGameSet.o testPieceMovement.o testCheckWin.o testAI.o testBoardStructure.o testTextLog.o testClientChess.o
	$(CC) $(LFLAGS) $(DEBUG) testGame.o testBoardEdit.o testGameSet.o testPieceMovement.o testCheckWin.o testAI.o testBoardStructure.o testTextLog.o testClientChess.o -o testChessGame

#target to generate Server
Server: Server.c
	$(CC) $(LFLAGS) $(DEBUG) Server.c -o Server

clean:
	rm -f $(DESIGN)
	rm -f *.o

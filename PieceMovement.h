/**********************************************************/
/* Title: PieceMovement.h                                 */
/* Author: Daniel Guerra-Rojas                            */
/*                                                        */
/**********************************************************/

#ifndef PIECEMOVEMENT_H
#define PIECEMOVEMENT_H

//takes in piece location and location user wants to move it to
//returns 1 if valid move for selected piece, otherwise 0
int PieceMovement(char originalPos[7], char newPos[7], int liveBoard[8][8]);

//checks if valid move for Pawn specifically
int PawnMoves(char originalPos[7], char newPos[7], int liveBoard[8][8]);

//checks if valid move for Rook specifically
int RookMoves(char originalPos[7], char newPos[7], int liveBoard[8][8]);

//checks if valid move for Knight specifically
int KnightMoves(char originalPos[7], char newPos[7], int liveBoard[8][8]);

//checks if valid move for Bishop specifically
int BishopMoves(char originalPos[7], char newPos[7], int liveBoard[8][8]);

//checks if valid move for Queen specifically
int QueenMoves(char originalPos[7], char newPos[7], int liveBoard[8][8]);

// Checks if valid move for King specifically
// - originalPos: current position of black/white piece in "e1" format
// - newPos: new position of black/white piece in "e1" format
// - liveBoard[8][8]: current state of game board
//
// Returns 1 if valid, 0 otherwise
int KingMoves(char originalPos[7], char newPos[7], int liveBoard[8][8]);

// helper function that checks if a space is blank or not
// - space: input liveBoard[8][8] location of piece to determine if empty
//
// Returns 1 if empty (location is 0), otherwise 0
int is_space_blank(int space);

// helper function to determine if a piece on a space is white (postive) or black (negative).
// - space: input liveBoard[8][8] location of piece to determine color
//
// Returns 1 if white, 0 if black
int white_or_black(int space);

// returns one if a whitepawn has a black pawn either on the left or right of it (same row -- for use in en passant)
int near_opp_white(int space, int col_old, int near_left, int near_right); 

// returns one if a black pawn has a white pawn either on the left or right of it (same row -- for use in en passant)
int near_opp_black(int space, int col_old, int near_left, int near_right);
 
// checks that a pawn has a pawn of the opposite color either to the right or left of it, and that the pawn next to it just moved it's first move
int en_passant_white(int space, int diff_row, int col_old, int near_left, int near_right, int first_move);

// checks that a pawn has a pawn of the opposite color either to the right or left of it, and that the pawn next to it just moved it's first move
int en_passant_black(int space, int diff_row, int col_old, int near_left, int near_right, int first_move);

// returns en_passant_ye, the int that keeps track of whether en passant has been performed
int get_en_passant();

// helper function for PawnMoves, figures out if pawn movement without capture is valid
int pawn_helper(int space, int row_old, int diff_row);

// helper function for KnightMoves that checks if the row/col movement is correct
int knight_helper(int diff_row, int diff_col);

void has_rook_moved(int col, int rook);

// Determines if black/white King and Rook have moved from their original positions
// with respect to the direction of attempted castling.
// direction: 1 means attempt castling to the left, 2 means attempt to the right
// king: input liveBoard[8][8] location of King to determine color
//
// Returns 1 if valid, 0 if invalid
int castle_helper(int direction, int king);

// Determines if attempted move is valid with regards to castling.
// - originalPos[3]: current position of black/white King on board
// - liveBoard[8][8]: current state of the game board
// - direction: 1 means attempt castling to the left, 2 means attempt to the right
//
// Returns 1 if valid move, 2 if there are other pieces in the way, and 0 if invalid.
int castling(char originalPos[3], int liveBoard[8][8], int direction);

void castling_move(char originalPos[3], int liveBoard[8][8], int direction);

#endif /* PIECEMOVEMENT_H */

/* EOF */
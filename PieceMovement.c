/**********************************************************/
/* Title: PieceMovement.c                                 */
/* Author: Daniel Guerra-Rojas, Maxwell Blocker           */
/*                                                        */
/* Modifications:                                         */
/* 05/08/21  DGR  cleansed king and pawn functions of bugs*/
/* 05/03/21  MB   king, pawn, knight                      */
/* 05/03/21  DGR  Initial version                         */
/* 05/09/21  MB   en passant, minimizing knight/pawn      */
/**********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "PieceMovement.h"
#include "CheckWin.h"

int first_move = 0;  //--------------------------------------------------------------------------
int en_passant_ye = 0;

int king_move_w = 0;
int king_move_b = 0;

int rook_move_wl = 0;
int rook_move_wr = 0;
int rook_move_bl = 0;
int rook_move_br = 0;

//takes in piece location and location user wants to move it to
//returns 1 if valid move for selected piece, otherwise 0
int PieceMovement(char originalPos[7], char newPos[7], int liveBoard[8][8]){
    int row, col;
    row = abs( originalPos[1] - 56 );
    col = originalPos[0] - 97;

    switch( abs(liveBoard[row][col]) ){
        case 1:
            return PawnMoves(originalPos, newPos, liveBoard);
        case 2:
            return RookMoves(originalPos, newPos, liveBoard);
        case 3:
            return KnightMoves(originalPos, newPos, liveBoard);
        case 4:
            return BishopMoves(originalPos, newPos, liveBoard);
        case 5:
            return QueenMoves(originalPos, newPos, liveBoard);
        case 6:
            return KingMoves(originalPos, newPos, liveBoard);
    }
    return 0;
}

//checks if valid move for Queen specifically
int QueenMoves(char originalPos[7], char newPos[7], int liveBoard[8][8]){
    int x, rowOriginal, colOriginal, maxDistance, rowNew, colNew;
   
    //converting inputs from user to coordinate values
    rowOriginal = abs( originalPos[1] - 56 );
    colOriginal = originalPos[0] - 97;
    rowNew = abs( newPos[1] - 56 );
    colNew = newPos[0] - 97;

    //variables used to determine if path in "topleft" direction is cutoff
    //directions are in realtion to original position of piece
    int topLeft, top, topRight, left, right, bottomLeft, bottom, bottomRight;

    //all set to 1. If changed to 0, then path is obstructed by another piece
    topLeft = top = topRight = left = right = bottomLeft = bottom = bottomRight = 1;

    //finds max distance from original position to edge of board
    //(for both row and col)
    maxDistance = ( (abs(rowNew - rowOriginal)) >= (abs(colNew - colOriginal)) ) ? abs(rowNew - rowOriginal) : abs(colNew - colOriginal);

    //iterates totalMax times to determine if move for Queen is valid
    for(x = 1; x <= maxDistance; x++){

        //checks path starting from topleft expanding outwards
        if( ((rowOriginal - x) >= 0) && ((colOriginal - x) >= 0) && (topLeft == 1) ){

            //checks if path is obstructed by another piece
            if( liveBoard[rowOriginal - x][colOriginal - x] != 0){
                topLeft = 0;
            }
            //checks if new location matches adjusted original location
            //if it is, then it's a valid move. returns 1
            if( ((rowOriginal - x) == rowNew) && ((colOriginal - x) == colNew) ){
                return 1;
            }
        }

        //checks path starting from top expanding outwards
        if( ((rowOriginal - x) >= 0) && (top == 1) ){

            //checks if path is obstructed by another piece
            if( liveBoard[rowOriginal - x][colOriginal] != 0){
                top = 0;
            }
            //checks if new location matches adjusted original location
            //if it is, then it's a valid move. returns 1
            if( ((rowOriginal - x) == rowNew) && (colOriginal == colNew) ){
                return 1;
            }
        }

        //checks path starting from topRight expanding outwards
        if( ((rowOriginal - x) >= 0) && ((colOriginal + x) <= 7) && (topRight == 1) ){

            //checks if path is obstructed by another piece
            if( liveBoard[rowOriginal - x][colOriginal + x] != 0){
                topRight = 0;
            }
            //checks if new location matches adjusted original location
            //if it is, then it's a valid move. returns 1
            if( ((rowOriginal - x) == rowNew) && ((colOriginal + x) == colNew) ){
                return 1;
            }
        }

        //checks path starting from left expanding outwards
        if( ((colOriginal - x) >= 0) && (left == 1) ){

            //checks if path is obstructed by another piece
            if( liveBoard[rowOriginal][colOriginal - x] != 0){
                left = 0;
            }
            //checks if new location matches adjusted original location
            //if it is, then it's a valid move. returns 1
            if( (rowOriginal == rowNew) && ((colOriginal - x) == colNew) ){
                return 1;
            }
        }

        //checks path starting from right expanding outwards
        if( ((colOriginal + x) <= 7) && (right == 1) ){

            //checks if path is obstructed by another piece
            if( liveBoard[rowOriginal][colOriginal + x] != 0){
                right = 0;
            }
            //checks if new location matches adjusted original location
            //if it is, then it's a valid move. returns 1
            if( (rowOriginal == rowNew) && ((colOriginal + x) == colNew) ){
                return 1;
            }
        }

        //checks path starting from bottomLeft expanding outwards
        if( ((rowOriginal + x) <= 7) && ((colOriginal - x) >= 0) && (bottomLeft == 1) ){

            //checks if path is obstructed by another piece
            if( liveBoard[rowOriginal + x][colOriginal - x] != 0){
                bottomLeft = 0;
            }
            //checks if new location matches adjusted original location
            //if it is, then it's a valid move. returns 1
            if( ((rowOriginal + x) == rowNew) && ((colOriginal - x) == colNew) ){
                return 1;
            }
        }

        //checks path starting from bottom expanding outwards
        if( ((rowOriginal + x) <= 7) && (bottom == 1) ){

            //checks if path is obstructed by another piece
            if( liveBoard[rowOriginal + x][colOriginal] != 0){
                bottom = 0;
            }
            //checks if new location matches adjusted original location
            //if it is, then it's a valid move. returns 1
            if( ((rowOriginal + x) == rowNew) && (colOriginal == colNew) ){
                return 1;
            }
        }

        //checks path starting from bottomRight expanding outwards
        if( ((rowOriginal + x) <= 7) && ((colOriginal + x) <= 7) && (bottomRight == 1) ){

            if( liveBoard[rowOriginal + x][colOriginal + x] != 0){
                bottomRight = 0;
            }
            //checks if new location matches adjusted original location
            //if it is, then it's a valid move. returns 1
            if( ((rowOriginal + x) == rowNew) && ((colOriginal + x) == colNew) ){
                return 1;
            }
        }
    }
    //if all else fails, then move is invalid
    return 0;
}

void has_rook_moved(int col, int rook)
{
	switch (col)
	{
		case 0:
			if (white_or_black(rook) == 1)
			{
				rook_move_wl = 1;
			}
			else
			{
				rook_move_bl = 1;
			}
			break;
		case 7: 
			if (white_or_black(rook) == 1)
			{
				rook_move_wr = 1;
			}
			else
			{
				rook_move_br = 1;
			}
	}	
}

//checks if valid move for Rook specifically
int RookMoves(char originalPos[7], char newPos[7], int liveBoard[8][8]){
    int x, rowOriginal, colOriginal, maxDistance, rowNew, colNew;
   
    //converting inputs from user to coordinate values
    rowOriginal = abs( originalPos[1] - 56 );
    colOriginal = originalPos[0] - 97;
    rowNew = abs( newPos[1] - 56 );
    colNew = newPos[0] - 97;

    //variables used to determine if path in "top" direction is cutoff
    //directions are in realtion to original position of piece
    int top, left, right, bottom;

    //all set to 1. If changed to 0, then path is obstructed by another piece
    top = left = right = bottom = 1;

    //finds max distance from original position to edge of board
    //(for both row and col)
    maxDistance = ( (abs(rowNew - rowOriginal)) >= (abs(colNew - colOriginal)) ) ? abs(rowNew - rowOriginal) : abs(colNew - colOriginal);

    //iterates totalMax times to determine if move for Rook is valid
    for(x = 1; x <= maxDistance; x++){

        //checks path starting from top expanding outwards
        if( ((rowOriginal - x) >= 0) && (top == 1) ){

            //checks if path is obstructed by another piece
            if( liveBoard[rowOriginal - x][colOriginal] != 0){
                top = 0;
            }
            //checks if new location matches adjusted original location
            //if it is, then it's a valid move. returns 1
            if( ((rowOriginal - x) == rowNew) && (colOriginal == colNew) ){
                has_rook_moved(colOriginal, liveBoard[rowOriginal][colOriginal]);
		return 1;
            }
        }

        //checks path starting from left expanding outwards
        if( ((colOriginal - x) >= 0) && (left == 1) ){

            //checks if path is obstructed by another piece
            if( liveBoard[rowOriginal][colOriginal - x] != 0){
                left = 0;
            }
            //checks if new location matches adjusted original location
            //if it is, then it's a valid move. returns 1
            if( (rowOriginal == rowNew) && ((colOriginal - x) == colNew) ){
                has_rook_moved(colOriginal, liveBoard[rowOriginal][colOriginal]);
		return 1;
            }
        }

        //checks path starting from right expanding outwards
        if( ((colOriginal + x) <= 7) && (right == 1) ){

            //checks if path is obstructed by another piece
            if( liveBoard[rowOriginal][colOriginal + x] != 0){
                right = 0;
            }
            //checks if new location matches adjusted original location
            //if it is, then it's a valid move. returns 1
            if( (rowOriginal == rowNew) && ((colOriginal + x) == colNew) ){
                has_rook_moved(colOriginal, liveBoard[rowOriginal][colOriginal]);
		return 1;
            }
        }

        //checks path starting from bottom expanding outwards
        if( ((rowOriginal + x) <= 7) && (bottom == 1) ){

            //checks if path is obstructed by another piece
            if( liveBoard[rowOriginal + x][colOriginal] != 0){
                bottom = 0;
            }
            //checks if new location matches adjusted original location
            //if it is, then it's a valid move. returns 1
            if( ((rowOriginal + x) == rowNew) && (colOriginal == colNew) ){
                has_rook_moved(colOriginal, liveBoard[rowOriginal][colOriginal]);
		return 1;
            }
        }
    }
    //if all else fails, then move is invalid
    return 0;
}

//checks if valid move for Bishop specifically
int BishopMoves(char originalPos[7], char newPos[7], int liveBoard[8][8]){
    int x, rowOriginal, colOriginal, maxDistance, rowNew, colNew;
   
    //converting inputs from user to coordinate values
    rowOriginal = abs( originalPos[1] - 56 );
    colOriginal = originalPos[0] - 97;
    rowNew = abs( newPos[1] - 56 );
    colNew = newPos[0] - 97;

    //variables used to determine if path in "topleft" direction is cutoff
    //directions are in realtion to original position of piece
    int topLeft, topRight, bottomLeft, bottomRight;

    //all set to 1. If changed to 0, then path is obstructed by another piece
    topLeft = topRight = bottomLeft = bottomRight = 1;

    //finds max distance from original position to edge of board
    //(for both row and col)
    maxDistance = ( (abs(rowNew - rowOriginal)) >= (abs(colNew - colOriginal)) ) ? abs(rowNew - rowOriginal) : abs(colNew - colOriginal);

    //iterates totalMax times to determine if move for Bishop is valid
    for(x = 1; x <= maxDistance; x++){

        //checks path starting from topleft expanding outwards
        if( ((rowOriginal - x) >= 0) && ((colOriginal - x) >= 0) && (topLeft == 1) ){

            //checks if path is obstructed by another piece
            if( liveBoard[rowOriginal - x][colOriginal - x] != 0){
                topLeft = 0;
            }
            //checks if new location matches adjusted original location
            //if it is, then it's a valid move. returns 1
            if( ((rowOriginal - x) == rowNew) && ((colOriginal - x) == colNew) ){
                return 1;
            }
        }

        //checks path starting from topRight expanding outwards
        if( ((rowOriginal - x) >= 0) && ((colOriginal + x) <= 7) && (topRight == 1) ){

            //checks if path is obstructed by another piece
            if( liveBoard[rowOriginal - x][colOriginal + x] != 0){
                topRight = 0;
            }
            //checks if new location matches adjusted original location
            //if it is, then it's a valid move. returns 1
            if( ((rowOriginal - x) == rowNew) && ((colOriginal + x) == colNew) ){
                return 1;
            }
        }

        //checks path starting from bottomLeft expanding outwards
        if( ((rowOriginal + x) <= 7) && ((colOriginal - x) >= 0) && (bottomLeft == 1) ){

            //checks if path is obstructed by another piece
            if( liveBoard[rowOriginal + x][colOriginal - x] != 0){
                bottomLeft = 0;
            }
            //checks if new location matches adjusted original location
            //if it is, then it's a valid move. returns 1
            if( ((rowOriginal + x) == rowNew) && ((colOriginal - x) == colNew) ){
                return 1;
            }
        }

        //checks path starting from bottomRight expanding outwards
        if( ((rowOriginal + x) <= 7) && ((colOriginal + x) <= 7) && (bottomRight == 1) ){

            if( liveBoard[rowOriginal + x][colOriginal + x] != 0){
                bottomRight = 0;
            }
            //checks if new location matches adjusted original location
            //if it is, then it's a valid move. returns 1
            if( ((rowOriginal + x) == rowNew) && ((colOriginal + x) == colNew) ){
                return 1;
            }
        }
    }
    //if all else fails, then move is invalid
    return 0;
}

int is_space_blank(int space)
{
	if (space == 0){
		return 1;
	}
	return 0;
}

int white_or_black(int space)
{
	if (space > 0){
		return 1;
	}
	return 0;
}

int KingMoves(char originalPos[7], char newPos[7], int liveBoard[8][8])
{
	int row_old, col_old, row_new, col_new, diff_row, diff_col;
	
	// the int values of the old row/col
	row_old = abs(originalPos[1] - 56);
	col_old = originalPos[0] - 97;
	
	// the int values of the new row/col
	row_new = abs(newPos[1] - 56);
	col_new = newPos[0] - 97;

	// int abs difference between the old/new row & col values
	diff_row = (abs(row_old - row_new));
	diff_col = (abs(col_old - col_new));

	// a king can only move a max of 1 row 1 column away from itself
	if ((diff_row == 0 && diff_col == 1) || (diff_row == 1 && diff_col == 0) || (diff_row == 1 && diff_col == 1)){
		if(white_or_black(liveBoard[row_old][col_old]) == 1){
			king_move_w = 1;
		} else {
			king_move_b = 1;
		}
		return 1;	
	}
	return 0;
}

// returns one if a whitepawn has a black pawn either on the left or right of it (same row -- for use in en passant)
int near_opp_white(int space, int col_old, int near_left, int near_right) // near_left is space to the left of pawn, near_right is space to the right of pawn
{
	if (white_or_black(space) == 1) // if pawn that might perform en passant is white, 
	{
		switch (col_old) // have to check that the columns won't be out of bounds
		{
			case 0: // if in far left column, a black pawn could only be to the right of it
				if (near_right == -1)
				{
					return 1;
				}
				break;
			case 7: // if in the far right column, a black pawn could only be to the left of it
				if (near_left == -1)
				{
					return 1;
				}
				break;
			default: // if not on edges, then a black pawn could be on either side
				if (near_left == -1 || near_right == -1)
				{
					return 1;
				}
				break;
		}
	}

	return 0;
}

// returns one if a black pawn has a white pawn either on the left or right of it (same row -- for use in en passant)
int near_opp_black(int space, int col_old, int near_left, int near_right) // near_left is space to the left of pawn, near_right is space to the right of pawn
{
	if (white_or_black(space) == 0)// if pawn that might perform en passant is black,
	{
		switch (col_old) // have to check that the columns won't be out of bounds

		{
			case 0: // if in far left column, a white pawn could only be to the right of it

				if (near_right == 1)
				{
					return 1;
				}
				break;
			case 7: // if in far right column, a white pawn could only be to the left of it

				if (near_left == 1)
				{
					return 1;
				}
				break;
			default: // if not on edges, then a white pawn could be on either side
				if (near_left == 1 || near_right == 1)
				{
					return 1;
				}
				break;
		}
	}

	return 0;
}

// checks that a pawn has a pawn of the opposite color either to the right or left of it, and that the pawn next to it just moved it's first move
int en_passant_white(int space, int diff_row, int col_old, int near_left, int near_right, int first_move)
{
	if (diff_row == 1) // for white, the difference of its rows must be 1 (moving up the board)
	{
		if (near_opp_white(space, col_old, near_left, near_right) == 1) // if there is a black pawn near the white pawn and...
		{
			if (first_move == 2) // said pawn is on its first move of two spaces
			{
				return 1; // then en passant has been performed
			}
		}
	}	

	return 0;
}

// checks that a pawn has a pawn of the opposite color either to the right or left of it, and that the pawn next to it just moved it's first move
int en_passant_black(int space, int diff_row, int col_old, int near_left, int near_right, int first_move)
{
	if (diff_row == -1) // for black, the difference of its rows must be -1 (moving down the board)
	{
		if (near_opp_black(space, col_old, near_left, near_right) == 1) // if there is a white pawn near the black pawn and...
		{
			if (first_move == 2) // said pawn is on its first move of two spaces
			{
				return 1; // then en passant has been performed
			}
		}
	}

	return 0;
}

// returns en_passant_ye, the int that keeps track of whether en passant has been performed
int get_en_passant()
{
	return en_passant_ye;
}

// helper function for PawnMoves, figures out if pawn movement without capture is valid
int pawn_helper(int space, int row_old, int diff_row)
{ 
	if (white_or_black(space) == 1) // if the pawn is white,
	{
		if (row_old == 6) // and it's in the first pawn row for white (row 7)
		{
			switch (diff_row)
			{
				case 1: // the difference of rows is one (the white pawn moved up one row)
					return 1;
					break;
				case 2: // the difference of rows is two (the white pawn moved up two rows -- first move)
					first_move = 1; // need for en passant
					return 1;
					break;
			}
		}
		else if (diff_row == 1) // if the white pawn is not in the first pawn row for white and moved up one row, then...
		{
			return 1; // the move is valid
		}
	}
	else // if the pawn is black,
	{
		if (row_old == 1) // and it's in the first pawn row for black (row 2)
		{
			switch (diff_row)
			{
				case -1: // the difference of rows is negative one (the black pawn moved down one row)
					return 1;
					break;
				case -2: // the difference of rows is negative two (the black pawn moved down two rows -- first move)
					first_move = 1; // need for en passant
					return 1;
					break;
			}
		}
		else if (diff_row == -1) // if the black pawn is not in the first pawn row for black and moved down one row, then...
		{
			return 1; // the move is valid
		}
	}

	return 0;
}

// error handling for pawn piece movement
int PawnMoves(char originalPos[7], char newPos[7], int liveBoard[8][8])
{
	int row_old, col_old, row_new, col_new, diff_row, diff_col;
	
	// the int values of the old row/col
	row_old = abs(originalPos[1] - 56);
	col_old = originalPos[0] - 97;
	
	// the int values of the new row/col
	row_new = abs(newPos[1] - 56);
	col_new = newPos[0] - 97;

	//difference between the old/new row & col values
	diff_row = row_old - row_new;
	diff_col = col_old - col_new;

	if (diff_col == 0) // if the pawn didn't change columns
	{
		if (pawn_helper(liveBoard[row_old][col_old], row_old, diff_row) == 1) // check to make sure it moved either one space or two (and is moving the correct way up/down the board)
		{
			//first_move++; // need to update for en passant (have to be able to have a distinct value for this)------------------------------------------------------------
			return 1;
		}
	}
	else if (abs(diff_col) == 1) // if the pawn has changed columns
	{
		if (is_space_blank(liveBoard[row_new][col_new]) == 1) // and is moving to a blank space
		{
			// check to see if the piece performed en passant (white)	
			if (white_or_black(liveBoard[row_old][col_old]) == 1)
			{
				if (en_passant_white(liveBoard[row_old][col_old], diff_row, col_old, liveBoard[row_old][col_old - 1], liveBoard[row_old][col_old + 1], first_move) == 1)
				{
					if (liveBoard[row_new + 1][col_new] == -1) // making sure pawn below capturing pawn is black
					{
						en_passant_ye = 1;
						first_move = 0;
						return 1;
					}
				}
			}
			else if (white_or_black(liveBoard[row_old][col_old]) == 0) // en passant check for black
			{
				if (en_passant_black(liveBoard[row_old][col_old], diff_row, col_old, liveBoard[row_old][col_old - 1], liveBoard[row_old][col_old + 1], first_move) == 1)
				{
					if (liveBoard[row_new - 1][col_new] == 1) // making sure pawn below capturing pawn is white
					{
						en_passant_ye = 1;
						first_move = 0;
						return 1;
					}
				}
			}
		}
		else // if the piece did not move to a blank space
		{
			if ((white_or_black(liveBoard[row_old][col_old]) == 1 && white_or_black(liveBoard[row_new][col_new]) == 0) && diff_row == 1) // white has to move to a space with a black piece
			{
				return 1;
			}
			else if ((white_or_black(liveBoard[row_old][col_old]) == 0 && white_or_black(liveBoard[row_new][col_new]) == 1) && diff_row == -1) // black has to move to a space with a white piece 
			{
				return 1;
			}
		}
	}

	return 0;
}

// helper function for KnightMoves that checks if the row/col movement is correct
int knight_helper(int diff_row, int diff_col)
{
	if ((diff_row == 2 && diff_col == 1) || (diff_row == 1 && diff_col == 2)) // a knight can only move 1 col/2 rows or 2 rows/1 col at a time
	{
		return 1;
	}

	return 0;
}

// error checking for knight movement
int KnightMoves(char originalPos[7], char newPos[7], int liveBoard[8][8])
{
	// variables for old row/col values, new row/col values, and the difference between the two
	int row_old, col_old, row_new, col_new, diff_row, diff_col;
	
	// the int values of the old row/col
	row_old = abs(originalPos[1] - 56);
	col_old = originalPos[0] - 97;
	
	// the int values of the new row/col
	row_new = abs(newPos[1] - 56);
	col_new = newPos[0] - 97;

	// int abs difference between the old/new row & col values
	diff_row = (abs(row_old - row_new));
	diff_col = (abs(col_old - col_new));

	// checking to see if the move is valid first (combo of either 1 row/2 cols or 2 rows/1 col
	if (knight_helper(diff_row, diff_col) == 1)
	{
		if (is_space_blank(liveBoard[row_new][col_new]) == 1) // if the space is blank and the above is true, then the move is valid
		{
			return 1;	
		}
		else // if there is a piece on the space the knight is being moved to...
		{
			if (white_or_black(liveBoard[row_old][col_old]) == 1) // if the original piece is white, then...
			{
				if (white_or_black(liveBoard[row_new][col_new]) == 0) // the piece on the space being moved to must be black
				{
					return 1;
				}
			}
			else // if the original piece is black, then...
			{
				if (white_or_black(liveBoard[row_new][col_new]) == 1) // the piece on the space being moved to must be white
				{
					return 1;
				}
			}
		}
	}

	return 0;
}

int castle_helper(int direction, int king)
{
	switch (direction)
	{
		case 1:
			if (white_or_black(king) == 1)
			{
				if (king_move_w == 0 && rook_move_wl == 0)
				{
					return 1;
				}
			}
			else
			{
				if (king_move_b == 0 && rook_move_bl == 0)
				{
					return 1;
				}
			}
			break;
		case 2:
			if (white_or_black(king) == 1)
			{
				if (king_move_w == 0 && rook_move_wr == 0)
				{
					return 1;
				}
			}
			else
			{
				if (king_move_b == 0 && rook_move_br == 0)
				{
					return 1;
				}
			}
			break;
	}

	return 0;	
}

int castling(char originalPos[3], int liveBoard[8][8], int direction)
{
	// variables for old row/col values, new row/col values, and the difference between the two
	int row_king, col_king;

	// the int values of the old row/col
	row_king = abs(originalPos[1] - 56);
	col_king = originalPos[0] - 97;

	if (check(liveBoard) == 0)
	{
		if (castle_helper(direction, liveBoard[row_king][col_king]) == 1)
		{
			if (direction == 1)
			{
				if (is_space_blank(liveBoard[row_king][3]) == 1 && is_space_blank(liveBoard[row_king][2]) == 1 &&
					is_space_blank(liveBoard[row_king][1]) == 1){
					return 1;
				} else {
					return 2; // can't castle because pieces are in the way
				}
			}
			else if (direction == 2)
			{
				if (is_space_blank(liveBoard[row_king][5]) == 1 && is_space_blank(liveBoard[row_king][6]) == 1){
					return 1;
				} else {
					return 2;
				}
			}
		}
	}
	return 0;
}

void castling_move(char originalPos[3], int liveBoard[8][8], int direction)
{
	int row_king = abs(originalPos[1] - 56);

	switch (direction)
	{
		case 1:
			if (abs(liveBoard[row_king][0]) == 2)
			{
				liveBoard[row_king][2] = liveBoard[row_king][4];
				liveBoard[row_king][4] = 0;

				liveBoard[row_king][3] = liveBoard[row_king][0];
				liveBoard[row_king][0] = 0; 
			}

			break;
		case 2:
			if (abs(liveBoard[row_king][7]) == 2)
			{
				liveBoard[row_king][6] = liveBoard[row_king][4];
				liveBoard[row_king][4] = 0;

				liveBoard[row_king][5] = liveBoard[row_king][7];
				liveBoard[row_king][7] = 0; 
			}

			break;
	}
}

/* EOF */

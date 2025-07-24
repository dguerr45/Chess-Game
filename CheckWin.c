//ragtag file for now which will contains win conditions
#include <stdio.h>
#include "CheckWin.h"
#include "BoardEdit.h"
#include "PieceMovement.h"
   
//checks if either king is in check. Returns 1 if white king is in check
//returns 2 if black king is in check. Otherwise, returns 0

int check(int board[8][8]){
    int roww, colw, rowb, colb, x, y, z; // variables for position of kings
    roww = colw = rowb = colb = 0;
    
    //find where both kings are at
    for(x = 0; x < 8 ; x++){
        for(y = 0; y < 8; y++){
            if(board[x][y] == 6){
                roww = x;
                colw = y;
            }
            else if(board[x][y] == -6){
                rowb = x;
                colb = y;
            }
        }
    }

    //check up for white king
    for(z = 1; z<=roww ; z++){
        if( (roww-z>=0) ){
            if(board[roww-z][colw] != 0){
                if( board[roww-z][colw] == -2){
                    return 1;
                }
                else if( board[roww-z][colw] == -5){
                    return 1;
                }
                break;
            }
        }
    }
    //check down for white king
    for(z = 1; z<=(7-roww) ; z++){
        if( (roww+z<=7) ){
            if(board[roww+z][colw] != 0){
                if( board[roww+z][colw] == -2){
                    return 1;
                }
                else if( board[roww+z][colw] == -5){
                    return 1;
                }
                break;
            }
        }
    }
    //check left for white
    for(z = 1; z<=colw ; z++){
        if( (colw-z>=0) ){
            if(board[roww][colw-z] != 0){
                if( board[roww][colw-z] == -2){
                    return 1;
                }
                else if( board[roww][colw-z] == -5){
                    return 1;
                }
                break;
            }
        }
    }
    //check right for white
    for(z = 1; z<=(7-colw) ; z++){
        if( (colw+z<=7) ){
            if(board[roww][colw+z] != 0){
                if( board[roww][colw+z] == -2){
                    return 1;
                }
                else if( board[roww][colw+z] == -5){
                    return 1;
                }
                break;
            }
        }
    }
    //check bottom right diagnol for white
    for(z = 1; z<=(7-colw) ; z++){
        if( (roww+z<=7)&&(colw+z<=7) ){
            if(board[roww+z][colw+z] != 0){
                if(board[roww+z][colw+z]== -4){
                    return 1;
                }
                else if( board[roww+z][colw+z] == -5){
                    return 1;
                }
                break;
            }
        }
    }
    //check top right diagnol for white
    for(z = 1; z<=(7-colw) ; z++){
        if( (roww-z>=0)&&(colw+z<=7) ){
            if(board[roww-z][colw+z] != 0){
                if( board[roww-z][colw+z] == -4){
                    return 1;
                }
                else if( board[roww-z][colw+z] == -5){
                    return 1;
                }
                break;
            }
        }
    }
    //check top left diagnol for white
    for(z = 1; z<=colw ; z++){
        if( (roww-z>=0)&&(colw-z>=0) ){
            if(board[roww-z][colw-z] != 0){
                if( board[roww-z][colw-z] == -4){
                    return 1;
                }
                else if( board[roww-z][colw-z] == -5){
                    return 1;
                }
                break;
            }
        }
    }
    //check bottom left diagnol for white
    for(z = 1; z<=colw ; z++){
        if( (roww+z<=7)&&(colw-z>=0) ){
            if(board[roww+z][colw-z] != 0){
                if( board[roww+z][colw-z] == -4){
                    return 1;
                }
                else if( board[roww+z][colw-z] == -5){
                    return 1;
                }
                break;
            }
        }
    }

    //check up for black king
    for(z = 1; z<=roww ; z++){
        if( (rowb-z>=0) ){
            if(board[rowb-z][colb] != 0){
                if( board[rowb-z][colb] == 2){
                    return 2;
                }
                else if( board[rowb-z][colb] == 5){
                    return 2;
                }
                break;
            }
        }
    }
    //check down for black king
    for(z = 1; z<=(7-rowb) ; z++){
        if( (rowb+z<=7) ){
            if(board[rowb+z][colb] != 0){
                if( board[rowb+z][colb] == 2){
                    return 2;
                }
                else if( board[rowb+z][colb] == 5){
                    return 2;
                }
                break;
            }
        }
    }
    //check left for black king
    for(z = 1; z<=colb ; z++){
        if( (colb-z>=0) ){
            if(board[rowb][colb-z] != 0){
                if( board[rowb][colb-z] == 2){
                    return 2;
                }
                else if( board[rowb][colb-z] == 5){
                    return 2;
                }
                break;
            }
        }
    }
    //check right for black king
    for(z = 1; z<=(7-colb) ; z++){
        if( (colb+z<=7) ){
            if(board[rowb][colb+z] != 0){
                if( board[rowb][colb+z] == 2){
                    return 2;
                }
                else if( board[rowb][colb+z] == 5){
                    return 2;
                }
                break;
            }
        }
    }
    //check bottom right diagnol for black king
    for(z = 1; z<=(7-colb) ; z++){
        if( (rowb+z<=7)&&(colb+z<=7) ){
            if(board[rowb+z][colb+z] != 0){
                if( board[rowb+z][colb+z] == 4){
                    return 2;
                }
                else if( board[rowb+z][colb+z] == 5){
                    return 2;
                }
                break;
            }
        }
    }
    //check top right diagnol for black king
    for(z = 1; z<=(7-colb) ; z++){
        if( (rowb-z>=0)&&(colb+z<=7) ){
            if(board[rowb-z][colb+z] != 0){
                if( board[rowb-z][colb+z] == 4){
                    return 2;
                }
                else if( board[rowb-z][colb+z] == 5){
                    return 2;
                }
                break;
            }
        }
    }
    //check top left diagnol for black king
    for(z = 1; z<=colb ; z++){
        if( (rowb-z>=0)&&(colb-z>=0) ){
            if(board[rowb-z][colb-z] != 0){
                if( board[rowb-z][colb-z] == 4){
                    return 2;
                }
                else if( board[rowb-z][colb-z] == 5){
                    return 2;
                }
                break;
            }
        }
    }
    //check bottom left diagnol for black king
    for(z = 1; z<=colb ; z++){
        if( (rowb+z<=7)&&(colb-z>=0) ){
            if(board[rowb+z][colb-z] != 0){
                if( board[rowb+z][colb-z] == 4){
                    return 2;
                }
                else if( board[rowb+z][colb-z] == 5){
                    return 2;
                }
                break;
            }
        }
    }
 
    //check f(r white knights attacking black
    if((rowb+2 <= 7)&&(colb+1 <= 7)){
    	if(board[rowb+2][colb+1] == 3){
	    return 2;
    	}
    }
    if( (rowb+2 <= 7) && (colb-1 >= 0) ){
        if(board[rowb+2][colb-1] == 3){
            return 2;
        }
    }
    if((rowb+1 <= 7)&&(colb+2 <= 7)){
        if(board[rowb+1][colb+2] == 3){
            return 2;
        }
    }
    if((rowb+1 <= 7)&&(colb-2 >= 0)){
        if(board[rowb+2][colb+1] == 3){
            return 2;
        }
    }
    if((rowb-1 >= 0)&&(colb-2 >= 0)){
        if(board[rowb-1][colb-2] == 3){
            return 2;
        }
    }
    if((rowb-1 >= 0)&&(colb+2 >= 0)){
        if(board[rowb-1][colb+2] == 3){
            return 2;
        }
    }
    if((rowb-2 >= 0)&&(colb+1 >= 0)){
        if(board[rowb-2][colb+1] == 3){
            return 2;
        }
    }
    if((rowb-2 >= 0)&&(colb-1 >= 0)){
        if(board[rowb-2][colb-1] == 3){
            return 2;
        }
    }

    //check for black knights attacking against white
    if((roww+2 <= 7)&&(colw+1 <= 7)){
    	if(board[roww+2][colw+1] == -3){
	    return 1;
    	}
    }
    if((roww+2 <= 7)&&(colw-1 >= 0)){
        if(board[roww+2][colw-1] == -3){
            return 1;
        }
    }
    if((roww+1 <= 7)&&(colw+2 <= 7)){
        if(board[roww+1][colw+2] == -3){
            return 1;
        }
    }
    if((roww+1 <= 7)&&(colw-2 >= 0)){
        if(board[roww+2][colw+1] == -3){
            return 1;
        }
    }
    if((roww-1 >= 0)&&(colw-2 >= 0)){
        if(board[roww-1][colw-2] == -3){
            return 1;
        }
    }
    if((roww-1 >= 0)&&(colw+2 >= 0)){
        if(board[roww-1][colw+2] == -3){
            return 1;
        }
    }
    if((roww-2 >= 0)&&(colw+1 >= 0)){
        if(board[roww-2][colw+1] == -3){
            return 1;
        }
    }
    if((roww-2 >= 0)&&(colw-1 >= 0)){
        if(board[roww-2][colw-1] == -3){
            return 1;
        }
    }

    //check for white pawn against black
    if((rowb+1 <= 7)&&(colb-1 >= 0)){
        if(board[rowb+1][colb-1] == 1){
	    return 2;
        }
    }
    if((rowb+1 <= 7)&&(colb+1 <= 7)){
        if(board[rowb+1][colb+1] == 1){
            return 2;
        }
    }

    //check for black pawn against white
    if((roww-1 >= 0)&&(colw-1 >= 0)){
        if(board[roww-1][colw-1] == -1){
	    return 1;
        }
    }
    if((roww-1 >= 0)&&(colw+1 <= 7)){
        if(board[roww-1][colw+1] == -1){
            return 1;
        }
    }

    return 0;
}

//function to look at next move or move being placed to see if in check
int lookahead(int board[8][8], char input1[7], char input2[7]){
    int copy[8][8], x, y, var;
    int temp;
    //makes a copy of board 
    for(y=0;y<=7;y++){
	    for(x=0;x<=7;x++){
	        copy[y][x] = board[y][x];
	    }
    }
    //makes the move in the copy
    moves(copy, input1, input2, &var, &temp);

    //checks for check of a color
    var = check(copy);
    if(var == 1){
	return 1;
    }
    else if(var == 2){
	return 2;
    }
    else{
        return 0;
    }
}

//this willl check to see if game ends via checkmate
//returns: 0 for no checkmate, 1 for white in checkmate
//returns: 2 for black in checkmate
int checkmate(int b[8][8], int colorincheck){
    int x, y, i, j, z;
    char i1[7];
    char i2[7];
    //check to see if there is a checkmate against white
    if(colorincheck == 1){
	//while(1){
	    for( y=0;y<=7;y++ ){
		for( x=0;x<=7;x++ ){
		    if(b[y][x] <= 0){//check for nonwhite then skips
			continue;
		    }
		    else{
			//need to impliment method to look for any 
			//valid inputs that stop check
			for( i=0; i<=7; i++){
			    for( j=0; j<=7; j++){
				i1[1] = 56 - y;//translates to coordinates
				i1[0] = x + 97;
				i2[1] = 56 - j;
				i2[0] = i + 97;
				if((PieceMovement(i1, i2, b))&&(b[j][i] <= 0)){
				    z = lookahead(b,i1,i2);
				    if(z == 0){
				        return 0;
				    }
				}
			    }
			}
		    }
		}
	    }
	return 1;
    }

    //check to see if there is a checkmate against black
    if(colorincheck == 2){
	//while(1){
	    for( y=0;y<=7;y++ ){
		for( x=0;x<=7;x++ ){
		    if(b[y][x] >= 0){//check for nonblack then skips
			continue;
		    }
		    else{
			//need to impliment method to look for any 
			//valid inputs that stop check
			for( i=0; i<=7; i++){
			    for( j=0; j<=7; j++){
				i1[1] = 56 - y;//translates to coordinates
				i1[0] = x + 97;
				i2[1] = 56 - j;
				i2[0] = i + 97;
				if((PieceMovement(i1, i2, b))&&(b[j][i] >= 0)){
				    z = lookahead(b,i1,i2);
				    if(z == 0){
				        return 0;
				    }
				}
			    }
			}
		    }
		}
	    }
	return 2;
    }
    return 0;
}









  /* EOF */ 

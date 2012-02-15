
#include "othelloutils.h"


//NOTE: top corner is 0,0

typedef int (*fp)(int [][NUMSQUARES],int,int,int);
typedef void (*fp2)(int [][NUMSQUARES],int,int,int,MoveList&);
void MoveList::getRowCol(int i,int& r,int& c){
	r=legalMoves_[i].row_;
	c=legalMoves_[i].col_;
}
int owner(int board[][NUMSQUARES],int r,int c){
  int rc=0;
  if(board[r][c]>0)
    rc=1;
  else if(board[r][c] < 0)
    rc=-1;
  return rc;
}
/*the following num* functions all do the following:
  starting at the first square in direction specified by the function name
  from startrow,startcol counts the number that are owned by who.

  Function returns:
  - 0 if there are no pieces owned by who in that direction
  - a positive number representing number of pieces owned by who in the 
    given direction if it is capped by -who
  - a negative number representing number of pieces owned by who in the
    given direction but is not capped by -who.
*/


int numUp(int board[][NUMSQUARES],int startrow,int startcol,int who){
  int rc=0;
  int i;
  for(i=startrow-1;i>=0 && board[i][startcol]==who;i--){
    rc++;
  }
  //check that the cap at the other end belongs to the other player
  if(rc &&(i<0 || board[i][startcol]!=-who)){
	  rc=rc*-1;
  }
  return rc;
}

int numDown(int board[][NUMSQUARES],int startrow,int startcol,int who){
  int rc=0;
  int i;
  for(i=startrow+1;i<NUMSQUARES && board[i][startcol]==who;i++){
    rc++;
  }
  if(rc && (i>=NUMSQUARES || board[i][startcol]!=-who)){
	  rc=rc*-1;
  }

  return rc;
}

int numLeft(int board[][NUMSQUARES],int startrow,int startcol,int who){
  int rc=0;
  int i;
  for(i=startcol-1;i>=0 && board[startrow][i]==who;i--){
    rc++;
  }
  if(rc && (i<0 || board[startrow][i]!=-who)){
	  rc=rc*-1;
  }
  return rc;
}

int numRight(int board[][NUMSQUARES],int startrow,int startcol,int who){
  int rc=0;
  int i;
  for(i=startcol+1;i<NUMSQUARES && board[startrow][i]==who;i++){
    rc++;
  }
  if(rc && (i>=NUMSQUARES || board[startrow][i]!=-who)){
	  rc=rc*-1;
  }
  return rc;
}

int numUpLeft(int board[][NUMSQUARES],int startrow,int startcol,int who){
  int rc=0;
  int i,j;
  for(i=startcol-1,j=startrow-1;i>=0 && j>=0 && board[j][i]==who;i--,j--){
    rc++;
  }
  if(rc &&( i<0 || j < 0|| board[j][i]!=-who)){
	  rc=rc*-1;
  }
  return rc;
}
int numUpRight(int board[][NUMSQUARES],int startrow,int startcol,int who){
  int rc=0;
  int i,j;
  for(i=startcol+1,j=startrow-1;i<NUMSQUARES && j>=0 && board[j][i]==who;i++,j--){
    rc++;
  }
  if(rc &&(i>=NUMSQUARES|| j<0 || board[j][i]!=-who)){
	  rc=rc*-1;
  }
  return rc;
}
int numDownRight(int board[][NUMSQUARES],int startrow,int startcol,int who){
  int rc=0;
  int i,j;
  for(i=startcol+1,j=startrow+1;i<NUMSQUARES && j<NUMSQUARES && board[j][i]==who;i++,j++){
    rc++;
  }
  if(rc && (i>=NUMSQUARES|| j>=NUMSQUARES || board[j][i]!=-who)){
	  rc=rc*-1;
  }
  return rc;
}
int numDownLeft(int board[][NUMSQUARES],int startrow,int startcol,int who){
  int rc=0;
  int i,j;
  for(i=startcol-1,j=startrow+1;i>=0 && j<NUMSQUARES && board[j][i]==who;i--,j++){
    rc++;
  }
  if(rc && (i<0 || j<0 || board[j][i]!=-who)){
	  rc=rc*-1;
  }
  return rc;
}

/*this function does not do any checking.  It assumes that the pieces
   along the direction can be flipped and the board is modified to do so.
   the list of pieces flipped is stored into the flips argument*/
void flipUp(int board[][NUMSQUARES],int startrow,int startcol,int who,MoveList& flips){

  int i;
  for(i=startrow-1;i>=0 && board[i][startcol]==who;i--){
    board[i][startcol]=-who;
	flips.add(i,startcol);
  }
}

void flipDown(int board[][NUMSQUARES],int startrow,int startcol,int who,MoveList& flips){
  int i;
  for(i=startrow+1;i<NUMSQUARES && board[i][startcol]==who;i++){
   board[i][startcol]=-who;
	flips.add(i,startcol);
  }
}

void flipLeft(int board[][NUMSQUARES],int startrow,int startcol,int who,MoveList& flips){
  int i;
  for(i=startcol-1;i>=0 && board[startrow][i]==who;i--){
    board[startrow][i]=-who;
	flips.add(startrow,i);
  }
}

void flipRight(int board[][NUMSQUARES],int startrow,int startcol,int who,MoveList& flips){
  int i;
  for(i=startcol+1;i<NUMSQUARES && board[startrow][i]==who;i++){
    board[startrow][i]=-who;
	flips.add(startrow,i);
  }
}

void flipUpLeft(int board[][NUMSQUARES],int startrow,int startcol,int who,MoveList& flips){
  int i,j;
  for(i=startcol-1,j=startrow-1;i>=0 && j>=0 && board[j][i]==who;i--,j--){
    board[j][i]=-who;
	flips.add(j,i);
  }
}
void flipUpRight(int board[][NUMSQUARES],int startrow,int startcol,int who,MoveList& flips){
  int i,j;
  for(i=startcol+1,j=startrow-1;i<NUMSQUARES && j>=0 && board[j][i]==who;i++,j--){
    board[j][i]=-who;
	flips.add(j,i);
  }
}
void flipDownRight(int board[][NUMSQUARES],int startrow,int startcol,int who,MoveList& flips){
  int i,j;
  for(i=startcol+1,j=startrow+1;i<NUMSQUARES && j<NUMSQUARES && board[j][i]==who;i++,j++){
    board[j][i]=-who;
	flips.add(j,i);
  }
}
void flipDownLeft(int board[][NUMSQUARES],int startrow,int startcol,int who,MoveList& flips){
  int i,j;
  for(i=startcol-1,j=startrow+1;i>=0 && j<NUMSQUARES && board[j][i]==who;i--,j++){
    board[j][i]=-who;
	flips.add(j,i);
  }
}

/*Checks every square to see if there are still legal moves that can be made by either player. function 
  returns 0 if neither players can make a legal move and 1 if any player can move.  The two MoveList objects
  store all the legal moves for each player.*/
int canMove(int board[][NUMSQUARES], MoveList& p1moves, MoveList& p2moves){
  int foundone=0;
  int numInRow1,numInRow2;
  p1moves.clear();
  p2moves.clear();
  fp cf[8]={numUp,numDown,numLeft,numRight,
            numUpLeft,numDownRight,numUpRight, numDownLeft};

  for(int i=0;i<NUMSQUARES;i++){
    for(int j=0;j<NUMSQUARES;j++){
      if(board[i][j]==0){
		numInRow1=0;  //as soon as I find that there is at least one flip,
		             //I don't need to check the other directions.
		numInRow2=0;
        for(int k=0;(numInRow1<=0||numInRow2<=0) && k<8;k++){
          numInRow2=cf[k](board,i,j,1);
		  if(numInRow2 > 0){
			p2moves.add(i,j);
		  }
		  numInRow1=cf[k](board,i,j,-1);
		  if(numInRow1 > 0)
			p1moves.add(i,j);
        }
      }  
    }
  }
  foundone=(p1moves.numMoves()||p2moves.numMoves());
  return foundone;
}
/*
This function passes back the current "score" of the board.  This is calculated
by the number of squares owned by each player.  Note that this score is not an indication
of win or loss or even how good the board is for any player.  Furthermore, it does not have
anything to do with the evaluation function.  This is purely a count of the final score if
the game were to end.
*/
void getScores(int board[][NUMSQUARES],int& p1score, int& p2score){
  p1score=0;
  p2score=0;
  for(int i=0;i<NUMSQUARES;i++){
    for(int j=0;j<NUMSQUARES;j++){
      if(board[i][j]==1){
		  p1score++;
	  }
	  else if(board[i][j]==-1){
		  p2score++;
	  }
    }
  }
}
/*function adds a piece to the board at r,c if possible.
  All the pieces that are flipped as a result of adding the piece are added
  to the flips list and the board is altered to reflect the addition of
  the piece.  Function returns true if the move to r,c was valid, false 
  otherwise*/
bool addPiece(int board[][NUMSQUARES], int r,int c, int who,MoveList& flips){
	bool rc=false;
    fp cf[8]={numUp,numDown,numLeft,numRight,
            numUpLeft,numDownRight,numUpRight, numDownLeft};
    fp2 cf2[8]={flipUp,flipDown,flipLeft,flipRight,
            flipUpLeft,flipDownRight,flipUpRight, flipDownLeft};
	flips.clear();
	for(int k=0;k<8;k++){
       if(cf[k](board,r,c,-who)>0){
	     cf2[k](board,r,c,-who,flips);
		 rc=true;
	   }
	}
	if(rc){
		board[r][c]=who;
	}
	
    return rc;
}
bool boardFull(int board[][NUMSQUARES]){
  bool rc=true;
  for(int i=0;i<NUMSQUARES  && rc;i++){
    for(int j=0;j<NUMSQUARES && rc;j++){
      if(board[i][j]==0)
        rc=false;
    }
  }
  return rc;
}

int canMove(int board[][NUMSQUARES],int who, MoveList& moves){

  int numInRow;
  moves.clear();
  fp cf[8]={numUp,numDown,numLeft,numRight,
            numUpLeft,numDownRight,numUpRight, numDownLeft};

  for(int i=0;i<NUMSQUARES;i++){
    for(int j=0;j<NUMSQUARES;j++){
      if(board[i][j]==0){
		numInRow=0;  //as soon as I find that there is at least one flip,
		             //I don't need to check the other directions.
        for(int k=0;numInRow<=0 && k<8;k++){
          numInRow=cf[k](board,i,j,-who);
		  if(numInRow > 0){
			moves.add(i,j);
		  }
        }
      }  
    }
  }
  return moves.numMoves();
}

void copyBoard(int newboard[][NUMSQUARES], int original[][NUMSQUARES]){
	for(int i=0;i<NUMSQUARES;i++){
		for(int j=0;j<NUMSQUARES;j++){
			newboard[i][j]=original[i][j];
		}
	}
}